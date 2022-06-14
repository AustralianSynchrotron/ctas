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
/// @date   Mon Apr  6 12:45:15 2009
///
/// @brief Makes sinograms from the list of foreground-background pairs.
///



#include <vector>
#include "../common/poptmx.h"
#include "../common/common.h"
#include "../common/flatfield.h"
#include "../common/experiment.h"
#include "../common/abs.h"

using namespace std;

/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Path inlist;				///< File with the list of all input contrasts.
  bool idealworld;               ///< Assumption of the world's ideality.
  string slicedesc;				///< String describing the slices to be sino'ed.
  Path outmask;				///< The mask for the output file names.
  bool beverbose;				///< Be verbose flag
  bool SaveInt;					///< Save image as 16-bit integer.

  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
  beverbose(false),
  SaveInt(false),
  idealworld(false),
  outmask("sino-<list>-@.tif")
{

  poptmx::OptionTable table
	("Forms absorption-based sinogram(s).",
	 "Performs following procedures:\n"
	 "1) reads the text file describing the array of input files"
	 " (foreground-background pairs).\n"
	 "2) removes the backgrounds in accordance with this file.\n"
	 "3) constructs the sinograms of the slices requested in the slice string.");

  table
	.add(poptmx::NOTE, "ARGUMENTS:")
	.add(poptmx::ARGUMENT, &inlist, "list",
      "List of the input images.", AqSeries::Desc)
	.add(poptmx::ARGUMENT, &outmask, "result mask",
		 "Output result mask.", MaskDesc, outmask)

	.add(poptmx::NOTE, "OPTIONS:")
	.add(poptmx::OPTION, &slicedesc, 's', "slice",
		 "Slices to be processed.", SliceOptionDesc, "<all>")
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


  // <list> : one required argument.
  if ( ! table.count(&inlist) )
    exit_on_error(command, "Missing required argument: <list>.");

  // <result mask> : one more argument may or may not exist
  if ( ! table.count(&outmask) )
	outmask = upgrade(inlist, "sino-") + "-@.tif";
  if ( string(outmask).find('@') == string::npos )
	outmask = outmask.dtitle() + "-@" + outmask.ext();

}



/// \MAIN{sina}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv);
  const AqSeries list(args.inlist);
  const ABSexp expr(list);
  int
    thetas=expr.thetas(),
    pixels=expr.pixels(),
    slices=expr.slices();
  const string sliceformat = mask2format(args.outmask, slices);
  const vector<int> sliceV = slice_str2vec(args.slicedesc, slices);
  const SinoS sins(expr, sliceV, args.beverbose);

  Map sinogram(thetas, pixels);
  ProgressBar bar( args.beverbose, "sinograms formation", sliceV.size());

  for (unsigned slice=0 ; slice < sliceV.size() ; slice++ ) {
    sins.sino(slice, sinogram);
    if ( args.idealworld )  sinogram = cutone(sinogram);
    SaveImage( toString(sliceformat, sliceV[slice]+1), sinogram, args.SaveInt);
    bar.update();
  }

  exit(0);

}
