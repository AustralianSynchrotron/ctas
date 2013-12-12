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
/// @brief %Reconstruction of one slice on the reconstruct-as-acquired approach.
///
/// Main file for the program which reconstructs one slice from the set of
/// images as they are ready to read.
///


#define _USE_MATH_DEFINES // for M_PI

#include "../common/common.h"
#include "../common/kernel.h"
#include "../common/poptmx.h"


using namespace std;


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Contrast contrast;            ///< Type of the contrast.
  int slice;                  ///< Slice number to pick up in the projections.
  Filter filter_type;           ///< Type of the filtering function.
  float center;                   ///< Rotation center.z
  Path result_name;           ///< Name of the file to save the result to.
  bool beverbose;				///< Be verbose flag
  bool SaveInt;					///< Save image as 16-bit integer.

  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
  center(0),
  beverbose(false),
  SaveInt(false),
  result_name("reconstructed.tiff")
{


  poptmx::OptionTable table
	("CT reconstruction of one sinogram picked from the series of images.",
	 "This program is the prototype of the reconstruct-as-acquired approach."
     " In this approach you don't need to wait until all projections are"
     " acquired, then form sinogram and reconstruct it. The program picks up"
     " the projections (input filenames from the stdin) as they arrive,"
     " picks the specified slice and adds to the reconstructed image."
     " The end of input data is triggered with the empty line."
     );

  table
	.add(poptmx::NOTE, "ARGUMENTS:")
	.add(poptmx::ARGUMENT, &result_name, "result",
		 "Output reconstructed image.", "", result_name)

	.add(poptmx::NOTE, "OPTIONS:")
	.add(poptmx::OPTION, &contrast, 'C', "contrast",
		 "Input component.",
         "Type of the contrast presented in the sinogram. " + Contrast::Desc)
	.add(poptmx::OPTION, &slice, 's', "slice",
		 "Slice to be reconstructed.", "", "<all>")
	.add(poptmx::OPTION, &center, 'c', "center",
		 "Rotation center.", CenterOptionDesc, toString(center))
	.add(poptmx::OPTION, &filter_type, 'f', "filter",
		 "Filtering window used in the CT.", FilterOptionDesc, filter_type.name())
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

  if ( ! table.count(&slice) )
	exit_on_error(command, string () +
				  "Missing required argument: "+table.desc(&slice)+".");

}



int imageSlice;   ///< Slice number to pick up in the projections.



/// Parses the input string.
///
/// Reads the projection angle and the name of the image file
/// from the stdin, then puts the line from the projection read
/// from image into the ::sinoline and the angle of the
/// projection into ::Theta.
///
/// @param sinoline line storage
/// @param Theta projection angle
///
/// @return 0 if this is the end of input, non-zero otherwise
///
static int
getLine(Line & sinoline, float & Theta) {

  string input;
  getline(cin, input);

  if ( input.empty() )
    return 0; // end of input

  Path projname;
  istringstream ist(input);
  ist >> Theta >> projname;

  static Shape expectedShape(0,0);

  if ( ! blitz::product(expectedShape) ) { // first run only
    expectedShape = Shape(ImageSizes(projname));
    if ( expectedShape(0) < imageSlice )
      throw_error("ct-line", "The slice to be reconstructed"
                  " (" + toString(imageSlice) + ") is out of the"
                  " image height ("+ toString(expectedShape(0)) + ").");
    sinoline.resize(expectedShape(1));
  }

  ReadImageLine(projname, sinoline, imageSlice, expectedShape);

  return 1;

}



/// \MAIN{ct}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;
  imageSlice = args.slice;

  float Theta;
  Line sinoline;

  if ( ! getLine(sinoline, Theta) )
    throw_error("ct-line", "Could not read from input.");

  int pixels = sinoline.size();
  CTrec rec( Shape(1,pixels), args.contrast, 180.0, args.filter_type);

  ProgressBar pbar(args.beverbose, "Reconstructing");

  do {
    rec.addLine(sinoline, Theta * M_PI / 180.0, args.center);
    pbar.update();
  } while (  getLine(sinoline, Theta) );

  pbar.done();

  SaveImage(args.result_name, rec.result(), args.SaveInt);

  exit(0);

}
