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
/// @file   ipc.cpp
/// @author antonmx <antonmx@gmail.com>
/// @date   Fri Jul  2 12:20:00 2010
///
/// @brief
///
///
///


#include "../common/common.h"
#include "../common/ipc.h"
#include "../common/poptmx.h"

#define _USE_MATH_DEFINES // for M_PI

using namespace std;


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Path z0_name;                 ///< contrasts in the contact print plane.
  Path zD_name;                 ///< contrasts at the distance.
  Path phs_name;                ///< Output name of the phase contrast.
  Path abs_name;                ///< Output name of the absorption contrast.
  float dd;                     ///< Pixel size.
  float alpha;                  ///< \f$\alpha\f$ parameter of the MBA.
  float lambda;                 ///< Wavelength.
  float dist;                   ///< Object-to-detector distance.
  float dgamma;                 ///< \f$\gamma\f$ parameter of the BAC method
  bool SaveInt;				///< Be verbose flag
  bool beverbose;             ///< Save image as 16-bit integer.

  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::clargs(int argc, char *argv[]) :
  z0_name(""),
  dd(1.0),
  alpha(0.0),
  lambda(1.0),
  dgamma(1.0),
  SaveInt(false),
  beverbose(false)
{


  poptmx::OptionTable table
	("IPC contrast processing",
     "Extracts the phase and/or absorption contrast from the input image(s)."
     " Depending on the input parameters utilizes BA, MBA or BAC algorithms.");

  table
	.add(poptmx::NOTE, "ARGUMENTS:")
	.add(poptmx::ARGUMENT, &zD_name, "far-intensity", "Contrast taken at the distance.", "")
	.add(poptmx::ARGUMENT, &z0_name, "0-intensity", "Contrast taken in the contact print plane"
         " (clean absorption contrast).", "", "<NONE>")

    .add(poptmx::NOTE, "OPTIONS:")
	.add(poptmx::OPTION, &phs_name, 'p', "phase", "Image name to output the phase component", "", "<NONE>")
	.add(poptmx::OPTION, &abs_name, 'a', "absorption",
         "Image name to output the absorption component", "", "<NONE>")
	.add(poptmx::OPTION, &dist, 'z', "distance", "Object-to-detector distance (mm)",
         "More correctly the distance from the contact print plane and the detector plane where the image"
         " given by the argument " + table.desc(&zD_name) + " was taken. " + NeedForQuant)
	.add(poptmx::OPTION, &dd, 'r', "resolution", "Pixel size of the detector (micron)",
         NeedForQuant, toString(dd))
	.add(poptmx::OPTION, &alpha, 'l', "alpha", "The alpha-parameter of the MBA.", "", toString(alpha))
    .add(poptmx::OPTION, &lambda, 'w', "wavelength", "Wavelength of the X-Ray (Angstrom)",
         "Only needed together with " + table.desc(&alpha) + ".", toString(lambda))
	.add(poptmx::OPTION, &dgamma, 'g', "gamma", "Gamma coefficient of the BAC.",
         "Must be a value around 1.0 (theoretical).", toString(dgamma))
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


  if ( ! table.count(&zD_name) )
	exit_on_error(command, "Missing required argument: "+table.desc(&zD_name)+".");

  if ( ! table.count(&phs_name) && ! table.count(&abs_name) )
	exit_on_error(command, "At least one of the two following arguments is required: "
                  +table.desc(&phs_name)+ ", " +table.desc(&phs_name)+ ".");

  if ( ! table.count(&dist) )
	exit_on_error(command, "Missing required option: "+table.desc(&dist)+".");
  if (dist <= 0.0)
	exit_on_error(command, "Zero or negative distance (given by "+table.desc(&dist)+").");
  dist /= 1.0E3; // convert mm -> m

  if (dd <= 0.0)
	exit_on_error(command, "Zero or negative pixel size (given by "+table.desc(&dd)+").");
  dd /= 1.0E6; // convert micron -> m

  if ( abs(dgamma)>1.0 ) // should set even smaller limit
	exit_on_error(command, "Absolute value of gamma (given by "+table.desc(&dgamma)+")"
                  " is greater than 1.0.");

  if (lambda <= 0.0)
	exit_on_error(command, "Zero or negative wavelength (given by "+table.desc(&lambda)+").");
  if ( table.count(&lambda) && ! table.count(&alpha) )
    warn(command, "The wavelength (given by "+table.desc(&lambda)+") has influence only together"
         " with the alpha parameter (given by "+table.desc(&alpha)+").");
  if ( ! table.count(&lambda) && table.count(&alpha) )
    warn(command, "The wavelength (given by "+table.desc(&lambda)+") needed together with"
         " the alpha parameter (given by "+table.desc(&alpha)+") for the correct results.");
  lambda /= 1.0E10; // convert A -> m

  if (alpha < 0.0)
	exit_on_error(command, "Negative alpha parameter (given by "+table.desc(&alpha)+").");

}




/// \MAIN{ct}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;

  Map id;
  ReadImage(args.zD_name, id);
  const Shape sh=id.shape();
  Map out(sh);
  IPCprocess proc(sh, args.alpha, args.dist, args.dd, args.lambda);

  if ( ! args.z0_name.empty() ) {
    if (args.alpha != 0.0)
      warn (args.command, "Both the contrast in the contact print plane (pure absorption) and"
            " \\alpha parameter of the MBA are given: two overlapping methods to correct the"
            " absorption.");
    Map i0;
    ReadImage(args.z0_name, i0);
    id /= unzero(i0);
  }

  if( ! args.phs_name.empty() ) { // MBA
    proc.extract(id, out, IPCprocess::PHS);
    SaveImage(args.phs_name, out, args.SaveInt);
  }
  if( ! args.abs_name.empty() ) { // BAC
    if (args.alpha == 0.0)
      warn (args.command, "The \\alpha parameter of the BAC is zero."
            " Always produces (almost) flat absorption.");
    proc.extract (id, out, IPCprocess::ABS, args.dgamma);
    SaveImage (args.abs_name, out, args.SaveInt);
  }

  proc.~IPCprocess(); // why it does not happen automatically?
  exit(0);

}



