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
/// @brief %Converts float-point image into integer.
///


#include "../common/common.h"
#include "../common/poptmx.h"
#include <math.h>

using namespace std;

#ifdef _WIN32
#define NAN numeric_limits<float>::quiet_NaN();
static inline int isnan(double x){ return _isnan(x); }
#endif




/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Path in_name;               ///< Name of the input file.
  Path out_name;              ///< Name of the output file.
  float mincon;					///< Black intensity.
  float maxcon;					///< White intensity.
  bool beverbose;				///< Be verbose flag
  bool mMm;						///< Output minimum and maximum.
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
  mincon(NAN),
  maxcon(NAN),
  beverbose(false),
  mMm(false),
  out_name("int-<input>")
{

  poptmx::OptionTable table
	("Converts float-point image to the integer one.",
	 "Can also output minimum and maximum values of the image.");

  table
	.add(poptmx::NOTE, "ARGUMENTS:")
	.add(poptmx::ARGUMENT, &in_name, "input", "Input image.", "")
	.add(poptmx::ARGUMENT, &out_name, "output", "Output image.",
		 "", out_name)

	.add(poptmx::NOTE, "OPTIONS:")
	.add(poptmx::OPTION, &mincon, 'm', "min",
		 "Pixel value corresponding to black.",
		 " All values below this will turn black.", "<minimum>")
	.add(poptmx::OPTION, &maxcon, 'M', "max",
		 "Pixel value corresponding to white.",
		 " All values above this will turn white.", "<maximum>")
	.add(poptmx::OPTION, &mMm,     0, "minmax",
		 "Outputs minimum and maximum of the image.",
		 "If this option is given no float-to-int conversion is done"
		 " and no output image is written.")
	.add_standard_options(&beverbose)
	.add(poptmx::MAN, "SEE ALSO:", SeeAlsoList);

  if ( ! table.parse(argc,argv) )
	exit(0);
  if ( ! table.count() ) {
	table.usage();
	exit(0);
  }


  command = table.name();
  
  // <input> : one required argument.
  if ( ! table.count(&in_name) )
	exit_on_error(command, string () +
				  "Missing required argument: "+table.desc(&in_name)+".");
  // <output> : one more argument may or may not exist
  if ( ! table.count(&out_name) )
    out_name = upgrade(in_name, "int-");

}




/// \MAIN{ct}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;

  Map arr;
  ReadImage( args.in_name, arr );

  if (args.mMm) {
	cout << min(arr) << " " << max(arr) << endl;
	exit(0);
  }

  float mincon = args.mincon, maxcon = args.maxcon;

  if ( isnan(mincon) ) mincon = min(arr);
  if ( isnan(maxcon) ) maxcon = max(arr);

  SaveImage(args.out_name, arr, mincon, maxcon);

  exit(0);

}
