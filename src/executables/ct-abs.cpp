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
/// @brief %All-in-one ct reconstruction of the absorption contrast.
///
/// Main file for the program which reads the input sinogram from image,
/// performs CT reconstruction of it and stores the result in other
/// image file
///



#include <vector>

#include "../common/common.h"
#include "../common/kernel.h"
#include "../common/experiment.h"
#include "../common/abs.h"
#include "../common/poptmx.h"

using namespace std;

/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Path inlist;				///< File with the list of all input contrasts.
  bool idealworld;               ///< Assumption of the world's ideality.
  string slicedesc;				///< String describing the slices to be CT'ed.
  Path outmask;				///< The mask for the output file names.
  Filter filter_type;           ///< Type of the filtering function.
  Dcenter center;               ///< Rotation center.
  float arc;
  unsigned nof_threads;              ///< Number of threads in the reconstruction.
  bool beverbose;				///< Be verbose flag
  bool SaveInt;					///< Save image as 16-bit integer.

  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
  beverbose(false),
  idealworld(false),
  nof_threads(0),
  SaveInt(false),
  outmask("reconstructed-<list>-@.tif"),
  arc(180),
  filter_type()
{

  poptmx::OptionTable table
	("All-in-one absorption-based CT reconstruction.",
	 "Performs all procedures for the reconstruction:\n"
	 "1) reads the text file describing the array of input files"
	 " (foreground-background pairs).\n"
	 "2) removes the backgrounds in accordance with this file.\n"
	 "3) constructs the sinograms of the slices requested in the slice string.\n"
	 "4) reconstructs the sinograms.\n"
	 "All these procedures can be performed on the step-by-step basis using tools"
	 " \"bg\", \"sino\" and \"ct\", but this approach saves all intermediate"
	 " results on the hard disk and therefore a lot of time is spent for the"
	 " I/O operations, memory allocations, etc. Also much more disk space is used.");

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
	.add(poptmx::OPTION, &center, 'c', "center",
		 "Variable rotation center.", DcenterOptionDesc, toString(0.0))
  .add(poptmx::OPTION, &arc, 'a', "arc",
       "CT scan range (deg).",
       "Arc of the CT scan in degrees: step size multiplied by number of projections."
       " Note: this is not where the half-object 360-degree CT is handeled.",
       toString(arc))
	.add(poptmx::OPTION, &SaveInt,'i', "int",
      "Output image(s) as integer.", IntOptionDesc)
	.add(poptmx::OPTION, &filter_type, 'f', "filter",
		 "Filtering window used in the CT.", FilterOptionDesc, filter_type.name())
	.add(poptmx::OPTION, &nof_threads, 't', "threads",
		 "Number of threads used in calculations.",
		 "If the option is not used the optimal number is"
		 " calculated automatically.", "<auto>")
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
    outmask = upgrade(inlist, "reconstructed-") + "-@.tif";
  if ( string(outmask).find('@') == string::npos )
    outmask = outmask.dtitle() + "-@" + outmask.extension();
  if (arc <= 0.0)
    exit_on_error(command, "CT arc (given by "+table.desc(&arc)+") must be strictly positive.");


}



/// \MAIN{ctabs}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv);
  const AqSeries list(args.inlist);
  const ABSexp expr(list);
  int
    thetas=expr.thetas(),
    pixels=expr.pixels(),
    slices=expr.slices();
  CTrec rec( expr.shape() , expr.contrast(), args.arc, args.filter_type);
  const string sliceformat = mask2format(args.outmask, slices);
  const vector<int> sliceV = slice_str2vec(args.slicedesc, slices);
  const SinoS sins(expr, sliceV, args.beverbose);

  Map sinogram(thetas, pixels), result(pixels, pixels);
  ProgressBar bar(args.beverbose, "reconstruction", sliceV.size());

  for (unsigned slice=0 ; slice < sliceV.size() ; slice++ ) {
    sins.sino(slice, sinogram);
    if ( args.idealworld )
      sinogram = cutone(sinogram);
    const Map & res = rec.reconstruct(sinogram, args.center(sliceV[slice]+1));
    SaveImage( toString(sliceformat, sliceV[slice]+1), res, args.SaveInt);
    bar.update();
  }

  exit(0);

}
