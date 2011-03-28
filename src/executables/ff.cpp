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
#include "../common/flatfield.h"
#include "../common/poptmx.h"

using namespace std;


/// \CLARGS
struct clargs {
  Path command;					///< Command name as it was invoked.
  Path foreground_name;			///< Name of the foreground file.
  Path background_name;			///< Name of the background file.
  vector<Path> dc_names;        ///< Name(s) of the dark current file(s).
  Path output_name;				///< Name of the output file.
  bool idealworld;				///< Assumption of the world ideality.
  bool beverbose;				///< Be verbose flag
  bool SaveInt;					///< Save image as 16-bit integer.

  /// \CLARGSF
  clargs(int argc, char *argv[]); 
};


clargs::
clargs(int argc, char *argv[]) : 
  output_name("cleaned-<foreground>"),
  idealworld(false),
  beverbose(false),
  SaveInt(false)
{

  poptmx::OptionTable table
	("Flat field correction.",
	 "For each foreground-background pair of pixels in the corresponding images"
	 " removes the dark field, divides the foreground by the background and puts"
     " the result into the output image. If the background pixel is 0 then the"
     " result is 1. All input images must be of equal size.");
  
  table
	.add(poptmx::NOTE, "ARGUMENTS:")
	.add(poptmx::ARGUMENT, &foreground_name, "foreground",
         "Input foreground image.", "")
    .add(poptmx::ARGUMENT, &background_name, "background",
         "Input background image.", "")
	.add(poptmx::ARGUMENT, &dc_names, "dark current",
         "Input dark current image(s).", "", "NONE")

	.add(poptmx::NOTE, "OPTIONS:")
	.add(poptmx::OPTION, &output_name, 'o', "output",
         "Output image.", "", output_name)
	.add(poptmx::OPTION, &idealworld, 'w', "idealworld",
		 "Suppose we live in the ideal world.", IdealWorldOptionDesc)
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

  if ( ! table.count(&foreground_name) )
	exit_on_error(command, string () +
				  "Missing required argument: "+table.desc(&foreground_name)+".");
  if ( ! table.count(&background_name) )
	exit_on_error(command, string () +
				  "Missing required argument: "+table.desc(&background_name)+".");
  if ( output_name.empty() )
    output_name = upgrade(foreground_name, "cleaned-");

}


/// \MAIN{ff}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;
  
  Map fa, ba;
  ReadImage(args.foreground_name, fa);
  ReadImage(args.background_name, ba);
  if ( fa.shape() != ba.shape() )
    exit_on_error(args.command, "Geometry of the foreground and background"
                  " images do not match." );
  Shape sh=fa.shape();

  Map dc(sh);
  dc=0.0;
  for (int i=0; i<args.dc_names.size(); i++) {
    Map dcc(sh);
    ReadImage(args.dc_names[i], dcc, sh);
    dc += dcc;
  }
  if ( args.dc_names.size() )
    dc /= args.dc_names.size();

  Map oa(sh);
  flatfield(oa, fa, ba, dc);
  if ( args.idealworld ) oa = cutone(oa);

  SaveImage(args.output_name, oa, args.SaveInt);

  exit(0);

}
