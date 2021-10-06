/******************************************************************************
 *   Copyright (C) 2007 by Anton Maksimenko                                   *
 *   antonmx@post.kek.jp                                                      *
 *                                                                            *
 *   This program is free software; you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by     *
 *   the Free Software Foundation; either version 2 of the License, or        *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   This program is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU General Public License for more details.                             *
 *                                                                            *
 *   You should have received a copy of the GNU General Public License        *
 *   along with this program; if not, write to the                            *
 *   Free Software Foundation, Inc.,                                          *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                *
 ******************************************************************************/

///
/// @file
/// @author antonmx <antonmx@gmail.com>
/// @date   Mon Jul 21 10:09:31 2008
///
/// @brief Make the sinogram.
///
/// Main file for the program which reads the list of files forming the
/// array of CT projections, picks one specified line from each of the
/// and prepares the sinogram from these lines.
///
/// \todo Add possibility to make vertical sinograms.




#include "../common/common.h"
#include "../common/experiment.h"
#include "../common/flatfield.h"
#include <vector>
#include <unistd.h>
#include <fstream>
#include "../common/poptmx.h"

using namespace std;
using namespace blitz;


/// \CLARGS
struct clargs {
  string command;               ///< Command name as it was invoked.
  Path outmask;       ///< The mask for the output file names.
  Path fileinlist;
  vector<Path> inlist;        ///< Array of the input images.
  vector<Path> bgs;        ///< Array of the background images.
  vector<Path> dfs;        ///< Array of the dark field images.
  vector<Path> dbs;        ///< Array of the dark field images for the backgrounds.
  string slicedesc;       ///< String describing the slices to be sino'ed.
  float angle;           ///< Angle of the sino slicing.
  Crop crp; //< Crop input projection image
  bool beverbose;       ///< Be verbose flag
  int width;
  float centre;
  float amplitude;
  float period;
  float phase;
  bool SaveInt;         ///< Save image as 16-bit integer.

  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
  angle(0.0),
  beverbose(false),
  SaveInt(false),
  outmask("sino_@.tif"),
  width(0),
  centre(0),
  amplitude(0),
  period(0),
  phase(0)
{

  const string sino_eq("image_X = C + image_W / 2 + A * cos( pi * ( F + image_Y / T )");

  poptmx::OptionTable table
  ("Prepares sinogram from the stack of files.",
   "Reads line(s) requested in the slice string from all input images"
   " or file with the list and forms the sinograms."
   " All input images must be of the same size.");

  table
  .add(poptmx::NOTE, "ARGUMENTS:")
  .add(poptmx::ARGUMENT, &inlist, "input images",
       "List of the input images.", "")

  .add(poptmx::NOTE, "OPTIONS:")
  .add(poptmx::OPTION, &bgs, 'b', "bg", "Background image(s)", "")
  .add(poptmx::OPTION, &dfs, 'd', "df", "Dark field image(s)", "")
  .add(poptmx::OPTION, &dbs, 'f', "db", "Dark field image(s) for backgrounds", "")
  .add(poptmx::OPTION, &outmask, 'o', "output",
       "Output result mask or filename.",
       "Output filename if only one sinogram is requested."
       " Output mask otherwise. " + MaskDesc, outmask)
  .add(poptmx::OPTION, &slicedesc, 's', "slice",
       "Slices to be processed.",
       "If the rotation angle is given the slices correspond to the rotated image." +
       SliceOptionDesc, "<all>")
  .add(poptmx::OPTION, &angle, 'a', "angle",
       "Angle (deg) of the image slicing.", "", toString(angle))
  .add(poptmx::OPTION, &crp, 'c', "crop",
       CropOptionDesc, "")
  .add(poptmx::OPTION, &width, 'w', "width",
       "Width of the output sinogram", "", "<image width>")
  .add(poptmx::OPTION, &centre, 'C', "centre",
      "Centre of the sinogram", DcenterOptionDesc + " C in " + sino_eq + "."
        " Width may get recalculated to fit into the image.", toString(centre))
  .add(poptmx::OPTION, &width, 'A', "amplitude",
       "Amplitude of the rotating sino", "A in " + sino_eq, toString(amplitude))
  .add(poptmx::OPTION, &width, 'P', "period",
       "Half period of the rotating sino", "T in " + sino_eq, "<image height>")
  .add(poptmx::OPTION, &width, 'F', "phase",
       "Phase (in pi) of the rotating sino", "F in " + sino_eq, toString(phase))
  .add(poptmx::OPTION, &SaveInt,'i', "int",
       "Output image(s) as integer.", IntOptionDesc)
  .add_standard_options(&beverbose)
  .add(poptmx::MAN, "SEE ALSO:", SeeAlsoList);
  if ( ! table.parse(argc,argv) )
    exit(0);
  if ( ! table.count() ) {
    table.usage();
    exit(0);
  }

  command = table.name();

  if ( inlist.size() < 2  )
    exit_on_error( command, "Less than 2 input images are specified.");

  angle *= M_PI/180;

}





/// \MAIN{sino}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;

  const Shape sh=ImageSizes(args.inlist[0]);

  const SinoS sins( args.inlist, args.slicedesc, args.angle, args.crp, args.beverbose );
  if ( ! sins.indexes().size() )
    throw_error(args.command, "No sinograms requested");

  Map bgar, bgsin;
  if ( ! args.bgs.empty() ) {
    bgar.resize(sins.imageShape());
    bgsin.resize(sins.sinoShape());
    bgar=0.0;
    Map iar(sh), rar, car;
    for ( int curf = 0 ; curf < args.bgs.size() ; curf++) {
      ReadImage(args.bgs[curf], iar, sh);
      rotate(iar, rar, args.angle);
      crop(rar, car, args.crp);
      for ( int sls=0 ; sls < sins.indexes().size() ; sls++ )
        bgar(sls, Range::all()) += car(sins.indexes()[sls], Range::all());
    }
    bgar /= args.bgs.size();
  }

  Map dfar, dfsin;
  if ( ! args.dfs.empty() ) {
    dfar.resize(sins.imageShape());
    dfsin.resize(sins.sinoShape());
    dfar=0.0;
    Map iar(sh), rar, car;
    for ( int curf = 0 ; curf < args.dfs.size() ; curf++) {
      ReadImage(args.dfs[curf], iar, sh);
      rotate(iar, rar, args.angle);
      crop(rar, car, args.crp);
      for ( int sls=0 ; sls < sins.indexes().size() ; sls++ )
        dfar(sls, Range::all()) += car(sins.indexes()[sls], Range::all());
    }
    dfar /= args.dfs.size();
  }


  Map dbar, dbsin;
  if ( args.dbs.empty() ) {
    dbar.reference(dfar);
    dbsin.reference(dfsin);
  } else {
    dbar.resize(sins.imageShape());
    dbsin.resize(sins.sinoShape());
    dbar=0.0;
    Map iar(sh), rar, car;
    for ( int curf = 0 ; curf < args.dbs.size() ; curf++) {
      ReadImage(args.dbs[curf], iar, sh);
      rotate(iar, rar, args.angle);
      crop(rar, car, args.crp);
      for ( int sls=0 ; sls < sins.indexes().size() ; sls++ )
        dbar(sls, Range::all()) += car(sins.indexes()[sls], Range::all());
    }
    dbar /= args.dbs.size();
  }


  Map sinogram;

  const Path outmask =  ( string(args.outmask).find('@') == string::npos ) ?
                        args.outmask.dtitle() + "-@" + args.outmask.extension() :
                        string( args.outmask ) ;
  const string sliceformat = mask2format(outmask, sins.imageShape()(0) );

  ProgressBar bar(args.beverbose, "sinograms formation", sins.indexes().size());
  for (unsigned slice=0 ; slice < sins.indexes().size() ; slice++ ) {
    sins.sino(slice, sinogram);
    if ( bgsin.size() )
      for (int scur=0; scur<sinogram.shape()(0); scur++)
        bgsin(scur,Range::all()) = bgar(slice, Range::all());
    if ( dfsin.size() )
      for (int scur=0; scur<sinogram.shape()(0); scur++)
        dfsin(scur,Range::all()) = dfar(slice, Range::all());
    flatfield(sinogram, sinogram, bgsin, dfsin, dbsin);
    const Path fileName = sins.indexes().size() == 1  ?  args.outmask :
                          Path(toString(sliceformat, sins.indexes()[slice]+1));
    SaveImage(fileName, sinogram, args.SaveInt);
    bar.update();
  }

  exit(0);

}

