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
/// @author antonmx <antonmx@synchrotron.org.au>
/// @date   Thu Sep 11 18:55:53 2008
///
/// @brief Extracts refraction and/or absorption components of two contrasts
/// taken on the either sides of the analyzers RC. Uses DEI or EDEI methods.
///



#include "../common/ctas.h"
#include "../common/dei.h"

using namespace std;

/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  DEIoptions deiopt;          ///< Options for the DEI processing.
  Path minus_name;            ///< Input contrast in the minus position.
  Path plus_name;             ///< Input contrast in the plus position.
  Path ref_name;              ///< Output refraction image.
  Path abs_name;              ///< Output absorption image.
  bool beverbose;				///< Be verbose flag
  int bpp;					///< Save image as 16-bit integer.

  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
  beverbose(false),
  bpp(0)
{

  poptmx::OptionTable table
  ("DEI contrast processing.",
   "Takes two images recorded in different positions of the analyzing"
   " crystal and process them with the DEI method separating the"
   " apparent absorption and clean refraction components and saving"
   " the requested one(s).");

  table
  .add(poptmx::NOTE, "ARGUMENTS:")
  .add(poptmx::ARGUMENT, &minus_name, "minus",
     "Contrast in the minus position.", "")
  .add(poptmx::ARGUMENT, &plus_name, "plus",
         "Contrast in the plus position.", "")
  .add(poptmx::NOTE, "",
     table.desc(&minus_name) + ", " + table.desc(&plus_name) +
     ". " + "Two images recorded in different position"
     " of the analyzing crystal. Must be of equal sizes.")

  .add(poptmx::NOTE, "OPTIONS:")
  .add(poptmx::OPTION, &ref_name,'r', "refraction",
     "Image name to output refraction component.", "", "NONE")
  .add(poptmx::OPTION, &abs_name,'a', "absorption",
     "Image name to output absorption component.", "", "NONE")
  .add(poptmx::OPTION, &bpp,'i', "int",
      "Bits per pixel to output image(s) as integer.", IntOptionDesc)
  .add(deiopt.options())
  .add_standard_options(&beverbose)
  .add(poptmx::MAN, "SEE ALSO:", SeeAlsoList);

  if ( ! table.parse(argc,argv) )
  exit(0);
  if ( ! table.count() ) {
  table.usage();
  exit(0);
  }

  command = table.name();

  // <minus> and <plus> : required arguments.
  if ( ! table.count(&minus_name) )
    exit_on_error(command, string () +
          "Missing required argument: "+table.desc(&minus_name)+".");
  if ( ! table.count(&plus_name) )
    exit_on_error(command, string () +
          "Missing required argument: "+table.desc(&plus_name)+".");
  // At least one of two options required
  if ( ! table.count(&abs_name) &&  ! table.count(&ref_name) )
    exit_on_error(command, string() +
          "At least one of two options must be set: " +
          table.desc(&ref_name) + table.desc(&abs_name) + ".");


}


/// \MAIN{dei}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv);
  const DEIprocess proc(args.deiopt.Gm, args.deiopt.Gp, args.deiopt.Rm, args.deiopt.Rp);

  Map ma, pa;
  ReadImage(args.minus_name, ma);
  ReadImage(args.plus_name, pa);
  if ( ma.shape() != pa.shape() )
    exit_on_error(args.command, "Geometry of the minus and plus"
                  " images do not match." );

  Map compa(ma.shape());
  if ( ! args.ref_name.empty() ) {
    compa = proc.extract(ma,pa,DEIprocess::REF);
    SaveImage(args.ref_name, compa, args.bpp);
  }
  if ( ! args.abs_name.empty() ) {
    compa = proc.extract(ma,pa,DEIprocess::ABS);
    SaveImage(args.abs_name, compa, args.bpp);
  }

  exit(0);

}
