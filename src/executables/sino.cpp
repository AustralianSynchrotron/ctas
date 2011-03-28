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
#include <vector>
#include "../common/poptmx.h"

using namespace std;


/// \CLARGS
struct clargs {
  string command;               ///< Command name as it was invoked.
  string sinogram_name;         ///< Name of the sinogram file.
  vector<string> inlist;        ///< Array of the input images.
  unsigned index;               ///< The index of the slice.
  bool beverbose;				///< Be verbose flag
  bool SaveInt;					///< Save image as 16-bit integer.

  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
  beverbose(false),
  SaveInt(false),
  index(0)
{

  poptmx::OptionTable table
	("Prepares sinogram from the stack of files.",
	 "Reads once certain line from all input images in the order they"
	 " appear in the argument list and forms the sinogram from the lines"
	 " red. All input images must be of the same size.");

  table
	.add(poptmx::NOTE, "ARGUMENTS:")
	.add(poptmx::ARGUMENT, &sinogram_name, "sinogram",
         "Output image to store the sinogram.", "")
	.add(poptmx::ARGUMENT, &inlist, "input images",
         "List of the input images.", "")

	.add(poptmx::NOTE, "OPTIONS:")
	.add(poptmx::OPTION, &index, 's', "slice",
         "Slice index which forms the sinogram.", "")
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

  // <slice> : required option
  if ( ! table.count(&index) )
    exit_on_error(command, string () +
				  "Missing required argument: "+table.desc(&index)+".");
  // <sinogram> : required argument.
  if ( ! table.count(&sinogram_name) )
    exit_on_error(command, string () +
				  "Missing required argument: "+table.desc(&sinogram_name)+".");
  // List of input files
  if ( table.count(&inlist) < 2 )
    exit_on_error( command, "Less than 2 input images are specified.");

}





/// \MAIN{sino}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;

  Shape shape = ImageSizes(args.inlist[0]);
  int insize=args.inlist.size();
  unsigned width=shape(1), hight=shape(0);

  if (args.index >= hight)
    exit_on_error(args.command, "Index of the sinogram (" + toString(args.index) +
				  ") is outside the image size (" + toString(hight) + ").");

  Line proj(width);
  Map sinogram(insize, width);
  ProgressBar bar(args.beverbose, "making sinogram", insize);

  for (int icur = 0; icur < insize ; icur++ ) {
    ReadImageLine(args.inlist[icur], proj, (int)args.index, shape);
    sinogram(icur, blitz::Range::all() ) = proj;
	bar.update();
  }

  SaveImage(args.sinogram_name, sinogram, args.SaveInt);

  exit(0);

}
