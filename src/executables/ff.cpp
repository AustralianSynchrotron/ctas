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
/// @author antonmx <antonmx@asw062.synchrotron.org.au>
/// @date   Thu Jul 24 13:55:58 2008
///
/// @brief Remove the background.
///
/// Takes "foreground" and "background" image files as input, divides
/// one by other and writes the result to the "output" image.
///


#include "../common/common.h"
#include "../common/experiment.h"
#include "../common/flatfield.h"
#include "../common/poptmx.h"

using namespace std;


/// \CLARGS
struct clargs {
  Path command;         ///< Command name as it was invoked.
  Path inlist;        ///< List of the acquisition series.
  Path foreground_name;     ///< Name of the foreground file.
  Path background_name;   ///< Name of the background file.
  Path background_2_name;   ///< Name of the second background file.
  float backgroundWeight;   ///< Weight of the first background file.
  Path dark_name;        ///< Name of the dark current file.
  Path dark_2_name;        ///< Name of the second dark current file.
  float darkWeight;   ///< Weight of the first dark current file.
  Path output_name;       ///< Name of the output file.
  float angle;           ///< Angle of the sino slicing.
  Crop crop; ///< Crop input projection image
  bool idealworld;        ///< Assumption of the world ideality.
  bool beverbose;       ///< Be verbose flag
  bool SaveInt;         ///< Save image as 16-bit integer.

  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
  output_name("cleaned-<foreground>"),
  idealworld(false),
  beverbose(false),
  SaveInt(false),
  backgroundWeight(0.5),
  darkWeight(0.5),
  angle(0.0),
  crop()
{

  Path FgL;

  poptmx::OptionTable table
  ("Flat field correction.",
   "For each foreground-background pair of pixels in the corresponding images"
   " removes the dark field, divides the foreground by the background and puts"
   " the result into the output image. If the background pixel is 0 then the"
   " result is 1. All input images must be of equal size.");

  table
  .add(poptmx::NOTE, "ARGUMENTS:")
  .add(poptmx::ARGUMENT, &FgL, "foreground/list",
       "Input foreground image or list of the acquisition series.",
       "If background is given then treated as foreground image"
       " and single-image operation is done."
       " If no background is given then treated as list"
       " and all files in the list are processed."
       " In this case represents the output mask.\n" + AqSeries::Desc)
  .add(poptmx::ARGUMENT, &background_name, "background",
       "Input background image.", "")
  .add(poptmx::ARGUMENT, &dark_name, "dark current",
       "Input dark current image.", "", "NONE")

  .add(poptmx::NOTE, "OPTIONS:")
  .add(poptmx::OPTION, &output_name, 'o', "output",
       "Output image.", "", output_name)
  .add(poptmx::OPTION, &background_2_name, 'b', "back",
       "Second background image.", "")
  .add(poptmx::OPTION, &backgroundWeight, 'B', "bw",
       "Weight of the first background image.", "", toString(backgroundWeight))
  .add(poptmx::OPTION, &dark_2_name, 'd', "dark",
       "Second dark current image.", "")
  .add(poptmx::OPTION, &darkWeight, 'D', "dw",
       "Weight of the first dark current image.", "", toString(darkWeight))
  .add(poptmx::OPTION, &angle, 'a', "angle",
       "Angle of the image slicing.", "", toString(angle))
  .add(poptmx::OPTION, &crop, 'c', "crop",
       CropOptionDesc, "")
  .add(poptmx::OPTION, &idealworld, 'w', "idealworld",
       "Suppose we live in the ideal world.", IdealWorldOptionDesc)
  .add(poptmx::OPTION, &SaveInt, 'i', "int",
       "Output image(s) as integer.", IntOptionDesc)
  .add_standard_options(&beverbose)
  .add(poptmx::MAN, "SEE ALSO:", SeeAlsoList);

  if ( ! table.parse(argc, argv) )
    exit(0);
  if ( ! table.count() ) {
    table.usage();
    exit(0);
  }

  command = table.name();

  angle *= M_PI/180;

  if ( ! table.count(&FgL) )
    exit_on_error(command, string () +
    "Missing required argument: " + table.desc(&FgL) + ".");


  if ( ! table.count(&background_name) ) { // list

    inlist = FgL;

    if ( ! table.count(&output_name) )
      output_name = "cleaned-@.tif";
    else if ( string(output_name).find('@') == string::npos )
      output_name = output_name.dtitle() + "-@" + output_name.extension();

  } else {

    foreground_name = FgL;

    if ( table.count(&backgroundWeight) && ! table.count(&background_2_name) )
      exit_on_error(command, string () +
                    "Option " + table.desc(&backgroundWeight) +
                    " requires option " + table.desc(&background_2_name) + ".");

    if ( table.count(&darkWeight) &&
         ( ! table.count(&dark_2_name) || ! table.count(&dark_name) ) )
      exit_on_error(command, string () +
                    "Option " + table.desc(&darkWeight) +
                    " requires argument " + table.desc(&dark_name) +
                    " and option " + table.desc(&dark_2_name) + ".");
    if ( table.count(&dark_2_name) && ! table.count(&dark_name) )
      exit_on_error(command, string () +
                    "Option " + table.desc(&dark_2_name) +
                    " requires argument " + table.desc(&dark_name) + ".");

    if ( ! table.count(&output_name) )
      output_name = upgrade(foreground_name, "cleaned-");

  }

}


/// \MAIN{ff}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;

  Map oa;

  if ( ! args.inlist.empty() ) {

    AqSeries series(args.inlist);
    ProgressBar bar(args.beverbose, "flat field correction", series.thetas());
    for (int idx=0 ; idx<series.thetas() ; idx++) {
      series.projection(idx, oa, vector<int>(), args.angle, args.crop,
                        args.idealworld ? 1.0 : 0.0 );
      string toSave=args.output_name;
      toSave.replace( toSave.rfind('@'), 1, series.fg(idx).name() );
      SaveImage(toSave, oa, args.SaveInt);
      bar.update();
    }

  } else {

    Map fa;

    ReadImage(args.foreground_name, fa);
    const Shape sh = fa.shape();
    Map ba(sh), dc(sh);
    ReadImage(args.background_name, ba, sh);

    if ( ! args.dark_name.empty() )
      ReadImage(args.dark_name, dc, sh);
    else
      dc=0;

    if ( ! args.background_2_name.empty() ) {
      Map tmp(sh);
      ReadImage(args.background_2_name, tmp, sh);
      weighted(ba, ba, tmp, args.backgroundWeight);
    }
    if ( ! args.dark_2_name.empty() ) {
      Map tmp(sh);
      ReadImage(args.dark_2_name, tmp, sh);
      weighted(dc, dc, tmp, args.darkWeight);
    }

    flatfield(oa, fa, ba, dc);
    if ( args.idealworld ) oa = cutone(oa);

    SaveImage(args.output_name, oa, args.SaveInt);

  }

  exit(0);

}
