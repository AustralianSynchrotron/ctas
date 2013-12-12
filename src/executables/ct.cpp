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
/// @brief %Reconstruction of one sinogram.
///
/// Main file for the program which reads the input sinogram from image,
/// performs CT reconstruction of it and stores the result in other
/// image file
///



#include "../common/common.h"
#include "../common/kernel.h"
#include "../common/poptmx.h"
#include "FreeImage.h"

using namespace std;


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Contrast contrast;            ///< Type of the contrast.
  Filter filter_type;           ///< Type of the filtering function.
  float center;                   ///< Rotation center.
  Path sinogram_name;         ///< Name of the sinogram file.
  Path result_name;           ///< Name of the file to save the result to.
  float arc;
  float dd;             ///< Pixel size.
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
  result_name("reconstructed-<sinogram>"),
  arc(180),
  dd(1.0)
{

  poptmx::OptionTable table
  ("CT reconstruction of one sinogram.",
   "The program reads pre-formed sinogram and reconstructs it.");

  table
  .add(poptmx::NOTE, "ARGUMENTS:")
  .add(poptmx::ARGUMENT, &sinogram_name, "sinogram",
       "Input image containing the sinogram.", "")
  .add(poptmx::ARGUMENT, &result_name, "result",
       "Output reconstructed image.", "", result_name)

  .add(poptmx::NOTE, "OPTIONS:")
  .add(poptmx::OPTION, &contrast, 'k', "contrast",
       "Input component.",
       "Type of the contrast presented in the sinogram. " + Contrast::Desc)
  .add(poptmx::OPTION, &center, 'c', "center",
       "Rotation center.", CenterOptionDesc, toString(center))
  .add(poptmx::OPTION, &arc, 'a', "arc",
       "CT scan range (deg).",
       "Arc of the CT scan in degrees: step size multiplied by number of projections."
       " Note: this is not where the half-object 360-degree CT is handeled.",
       toString(arc))
  .add(poptmx::OPTION, &filter_type, 'f', "filter",
       "Filtering window used in the CT.", FilterOptionDesc, filter_type.name())
  .add(poptmx::OPTION, &dd, 'r', "resolution",
       "Pixel size (micron).", ResolutionOptionDesc, toString(dd))
  //  .add(poptmx::OPTION, &lambda, 'w', "wavelength",
  //  "Wave length (Angstrom).", "Wavelength.")
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

  if ( ! table.count(&sinogram_name) )
    exit_on_error(command, string () +
                  "Missing required argument: "+table.desc(&sinogram_name)+".");
  if ( ! table.count(&result_name) )
    result_name = upgrade(sinogram_name, "reconstructed-");
  if ( table.count(&dd) ) {
    if ( dd <= 0.0 )
      exit_on_error(command, "Negative pixel size (given by "+table.desc(&dd)+").");
    dd /= 1.0E6;
  }
  if (arc <= 0.0)
    exit_on_error(command, "CT arc (given by "+table.desc(&arc)+") must be strictly positive.");


}



/// \MAIN{ct}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;

  Map sino;
  ReadImage( args.sinogram_name, sino);
  const Map rec =
    CTrec::reconstruct(sino, args.contrast, args.arc, args.filter_type, args.center, args.dd);
  SaveImage(args.result_name, rec, args.SaveInt);

  exit(0);

}
