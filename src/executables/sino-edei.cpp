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
/// @date   Mon Apr  6 12:46:41 2009
///
/// @brief Makes sinograms from the EDEI input contrasts.
///


#include <vector>
#include "../common/common.h"
#include "../common/experiment.h"
#include "../common/edei.h"

using namespace std;


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  EDEIprocess::Component contrast; ///< Component to be extracted.
  Path Mlistname;				///< File with the list of minus input contrasts.
  Path Plistname;				///< File with the list of plus input contrasts.
  EDEIoptions edeiopt;            ///< Options for the DEI processing.
  string slicedesc;				///< String describing the slices to be CT'ed.
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
  outmask("sino-<minus list>-@.tif"),
  contrast(EDEIprocess::REF)
{

  poptmx::OptionTable table
	("Forms EDEI-based sinogram(s).",
	 "Performs following procedures:\n"
	 "1) reads two text files describing the arrays of input files"
	 " (foreground-background pairs) in minus and plus position of"
	 " the analyzer crystal.\n"
	 "2) removes the backgrounds in accordance with these files.\n"
	 "3) performs the EDEI extraction of the requested contrast component.\n"
	 "4) constructs the sinograms of the slices requested in the slice string.");

  table
	.add(poptmx::NOTE, "ARGUMENTS:")
	.add(poptmx::ARGUMENT, &Mlistname, "minus list",
		 "List of the input images in the minus position.", "")
	.add(poptmx::ARGUMENT, &Plistname, "plus list",
         "List of the input images in the plus position.", "")
	.add(poptmx::NOTE,     "",
		 "Arguments \"" + table.desc(&Mlistname) + "\""
		 " and \"" + table.desc(&Plistname) + "\". "
		 + AqSeries::Desc)
	.add(poptmx::ARGUMENT, &outmask, "result mask",
		 "Output result mask.", MaskDesc, outmask)

	.add(poptmx::NOTE, "OPTIONS:")
	.add(poptmx::OPTION, &contrast, 'C', "contrast",
		 "Type of the contrast component.",
		 "The component of the contrast to extract via the (E)DEI method"
		 " and then reconstruct. " + EDEIprocess::componentDesc, toString(contrast))
    .add(poptmx::OPTION, &edeiopt.RCname, 0, "rc",
         EDEIoptions::rcOptionShortDesc, EDEIoptions::rcOptionDesc)
	.add(edeiopt.options())
	.add(poptmx::OPTION, &slicedesc, 's', "slice",
		 "Slices to be processed.", SliceOptionDesc, "<all>")
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

  // <minus list> and <plus list> : required arguments.
  if ( ! table.count(&Mlistname) )
    exit_on_error(command, string () +
				  "Missing required argument: "+table.desc(&Mlistname)+".");
  if ( ! table.count(&Plistname) )
    exit_on_error(command, string() +
				  "Missing required argument: "+table.desc(&Plistname)+".");


  // <result mask> : one more argument may or may not exist
  if ( ! table.count(&outmask) )
	outmask = upgrade(Mlistname, "reconstructed-") + "-@.tif";
  if ( string(outmask).find('@') == string::npos )
	outmask = outmask.dtitle() + "-@" + outmask.extension();

}



/// \MAIN{sinr}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv);
  const AqSeries Mlist(args.Mlistname);
  const AqSeries Plist(args.Plistname);
  const EDEIprocess proc(args.edeiopt.RCname, args.edeiopt.Gm, args.edeiopt.Gp,
                         args.edeiopt.mpinter, args.edeiopt.smooth, args.edeiopt.acof);
  const EDEIexp expr(proc, args.contrast, Mlist, Plist);
  int
    thetas=expr.thetas(),
    pixels=expr.pixels(),
    slices=expr.slices();
  const string sliceformat = mask2format(args.outmask, slices);
  const vector<int> sliceV = slice_str2vec(args.slicedesc, slices);
  const SinoS sins(expr, sliceV, args.beverbose);

  Map sinogram(thetas, pixels);
  ProgressBar bar(args.beverbose, "sinogram formation", sliceV.size());

  for (unsigned slice=0 ; slice < sliceV.size() ; slice++ ) {
	sins.sino(slice, sinogram);
	SaveImage( toString(sliceformat, sliceV[slice]+1), sinogram, args.SaveInt);
	bar.update();
  }

  exit(0);

}
