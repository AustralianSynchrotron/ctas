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
/// @date   Thu Sep 11 18:57:51 2008
///
/// @brief Processes the RC of the analyzer and prepares data function
/// to be used in the EDEI process.
///


#include "../common/common.h"
#include "../common/edei.h"

using namespace std;

/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Path fdname;				///< Name of the output \FD "FD" file.
  EDEIoptions edeiopt;            ///< Options for the EDEI processing.
  bool beverbose;				///< Be verbose flag

  /// \CLARGSF
  clargs(int argc, char *argv[]); 
};


clargs::
clargs(int argc, char *argv[]){

  beverbose = false;
  
  poptmx::OptionTable table
	("Converts the rocking curve of the analyzer to the function used in the EDEI.",
	 "I use this program only to develop and debug the EDEI algorithm. If you"
	 " you are not interested in it, you don't need this program.");

  table
	.add(poptmx::NOTE, "ARGUMENTS:")
    /*.add(poptmx::ARGUMENT, &edeiopt.RCname, "RC",
      EDEIoptions::rcOptionShortDesc, EDEIoptions::rcOptionDesc, "")*/
    .add(poptmx::ARGUMENT, &edeiopt.FDname, "FD",
         EDEIoptions::fdOptionShortDesc, EDEIoptions::fdOptionDesc, "processed-<RC>")

	.add(poptmx::NOTE, "OPTIONS:")
	.add(edeiopt.options())
	.add_standard_options(&beverbose)
	.add(poptmx::MAN, "SEE ALSO:", SeeAlsoList);

  if ( ! table.parse(argc,argv) )
	exit(0);
  if ( ! table.count() ) {
	table.usage();
	exit(0);
  }

  command = table.name();

  if ( ! table.count(&edeiopt.RCname) )
    exit_on_error(command, string () +
				  "Missing required argument: "+table.desc(&edeiopt.RCname)+".");
  if ( ! table.count(&edeiopt.FDname) )
	edeiopt.FDname = upgrade(edeiopt.RCname, "processed-");

}


/// \MAIN{rc2fd}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv);
  const EDEIprocess proc(args.edeiopt.RCname, args.edeiopt.Gm, args.edeiopt.Gp,
                         args.edeiopt.mpinter, args.edeiopt.smooth, args.edeiopt.acof);
  proc.saveFD(args.edeiopt.RCname);
  exit(0);

}
