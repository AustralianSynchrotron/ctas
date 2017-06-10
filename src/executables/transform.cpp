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
/// @brief %Transforms an image.
///


#include "../common/common.h"
#include "../common/poptmx.h"
#include <math.h>

using namespace std;
using namespace blitz;

#ifdef _WIN32
#define NAN numeric_limits<float>::quiet_NaN();
static inline int isnan(double x){ return _isnan(x); }
#endif



/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Path in_name;               ///< Name of the input file.
  Path out_name;              ///< Name of the output file.
  float angle;                ///< Rotation angle.
  float background;           ///< value for the points out of the FOV.
  Crop crp;                   ///< Crop resulting image
  Binn bnn;                   ///< Apply binning. 
  bool beverbose;
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
  beverbose(false),
  angle(0.0),
  background(NAN),
  crp(),
  bnn(), 
  out_name("<input>")
{

  poptmx::OptionTable table
  ("Performs basic transformations on the input image.",
   "Currently can rotate, crop and apply binning.");

  table
  .add(poptmx::NOTE, "ARGUMENTS:")
  .add(poptmx::ARGUMENT, &in_name, "input", "Input image.", "")
  .add(poptmx::ARGUMENT, &out_name, "output", "Output image.",
       "", out_name)

  .add(poptmx::NOTE, "OPTIONS:")
  .add(poptmx::OPTION, &angle, 'a', "angle",
       "Rotation angle (deg).", "")
  .add(poptmx::OPTION, &crp, 'c', "crop",
       CropOptionDesc, "")
  .add(poptmx::OPTION, &bnn, 'b', "binn",
       CropOptionDesc, "")
  .add(poptmx::OPTION, &background, 'g', "background",
       "Value for the points out of the image.", "", "<average>")
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
    out_name = in_name;

  angle *= M_PI/180;

}




/// \MAIN{ct}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;

  Map arr, out;
  ReadImage( args.in_name, arr );
  const Shape sh = arr.shape();

  rotate(arr, out, args.angle, args.background);
  crop(arr, args.crp);
  binn(arr, args.bnn);

  SaveImage(args.out_name, out);

  exit(0);

}
