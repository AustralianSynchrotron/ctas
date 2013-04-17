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
#include <vector>
#include "../common/poptmx.h"

using namespace std;


/// \CLARGS
struct clargs {
  string command;               ///< Command name as it was invoked.
  Path outmask;       ///< The mask for the output file names.
  vector<Path> inlist;        ///< Array of the input images.
  string slicedesc;       ///< String describing the slices to be sino'ed.
  float angle;           ///< Angle of the sino slicing.
  Crop crop; //< Crop input projection image
  bool beverbose;       ///< Be verbose flag
  bool SaveInt;         ///< Save image as 16-bit integer.

  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
angle(0.0),
beverbose(false),
SaveInt(false),
outmask("sino_@.tif")
{

  poptmx::OptionTable table
  ("Prepares sinogram from the stack of files.",
   "Reads line(s) requested in the slice string from all input images"
   " in the order they appear in the argument list and forms the sinograms."
   " All input images must be of the same size.");

  table
  .add(poptmx::NOTE, "ARGUMENTS:")
  .add(poptmx::ARGUMENT, &inlist, "input images",
       "List of the input images.", "")

  .add(poptmx::NOTE, "OPTIONS:")
  .add(poptmx::OPTION, &outmask, 'o', "output",
       "Output result mask or filename.",
       "Output filename if only one sinogram is requested."
       " Output mask otherwise. " + MaskDesc, outmask)
  .add(poptmx::OPTION, &slicedesc, 's', "slice",
       "Slices to be processed.",
       "If the rotation angle is given the slices correspond to the rotated image." +
       SliceOptionDesc, "<all>")
  .add(poptmx::OPTION, &angle, 'a', "angle",
       "Angle of the image slicing.", "", toString(angle))
  .add(poptmx::OPTION, &crop, 'c', "crop",
       CropOptionDesc, "")
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

  // List of input files
  if ( table.count(&inlist) < 2 )
    exit_on_error( command, "Less than 2 input images are specified.");

  angle *= M_PI/180;

}





/// \MAIN{sino}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;
  const SinoS *sins = (args.angle==0.0) ?
  new SinoS(args.inlist, args.slicedesc, args.beverbose) :
  new SinoS(args.inlist, args.slicedesc, args.angle, args.crop, args.beverbose);
  if ( ! sins->indexes().size() )
    throw_error(args.command, "No sinograms requested");

  Map sinogram;

  if ( sins->indexes().size() == 1 ) {

    sins->sino(0, sinogram);
    SaveImage( args.outmask, sinogram, args.SaveInt );

  } else {

    const Path outmask =  ( string(args.outmask).find('@') == string::npos ) ?
    args.outmask.dtitle() + "-@" + args.outmask.extension() :
    string( args.outmask ) ;
    const string sliceformat = mask2format(outmask, sins->imageShape()(0) );
    ProgressBar bar(args.beverbose, "sinograms formation", sins->indexes().size());

    for (unsigned slice=0 ; slice < sins->indexes().size() ; slice++ ) {
      sins->sino(slice, sinogram);
      SaveImage( toString(sliceformat, sins->indexes()[slice]+1), sinogram, args.SaveInt);
      bar.update();
    }

  }

  delete sins;

  exit(0);

}

