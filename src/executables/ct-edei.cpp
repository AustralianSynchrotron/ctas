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
/// @brief %All-in-one ct reconstruction of the refraction contrast.
///
/// Main file for the program which reads the input sinogram from image,
/// performs CT reconstruction of it and stores the result in other
/// image file.
///



#include <vector>
#include "../common/common.h"
#include "../common/kernel.h"
#include "../common/experiment.h"
#include "../common/edei.h"

using namespace std;

/// \CLARGS
struct clargs {
  Path command;                 ///< Command name as it was invoked.
  EDEIoptions edeiopt;            ///< Options for the EDEI processing.
  EDEIprocess::Component contrast; ///< Component to be extracted.
  Path Mlistname;				///< File with the list of minus input contrasts.
  Path Plistname;				///< File with the list of plus input contrasts.
  string slicedesc;				///< String describing the slices to be CT'ed.
  Path outmask;				    ///< The mask for the output file names.
  Filter filter_type;           ///< Type of the filtering function.
  Dcenter center;               ///< Rotation center.
  unsigned nof_threads;         ///< Number of threads in the reconstruction.
  bool beverbose;				///< Be verbose flag
  bool SaveInt;					///< Save image as 16-bit integer.

  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
  beverbose(false),
  nof_threads(0),
  SaveInt(false),
  outmask("reconstructed-<minus list>-@.tif"),
  contrast(EDEIprocess::REF)
{

  poptmx::OptionTable table
	("All-in-one EDEI-based CT reconstruction.",
	 "Performs all procedures for the reconstruction:\n"
	 "1) reads two text files describing the arrays of input files"
	 " (foreground-background pairs) in minus and plus position of"
	 " the analyzer crystal.\n"
	 "2) removes the backgrounds in accordance with these files.\n"
	 "3) performs the EDEI extraction of the requested contrast component.\n"
	 "4) constructs the sinograms of the slices requested in the slice string.\n"
	 "5) finally reconstructs the sinograms.\n"
	 "All these procedures can be performed on the step-by-step basis using"
	 " tools \"bg\", \"edei\", \"sino\" and \"ct\", but this approach saves all"
	 " intermediate results on the hard disk and therefore a lot of time is"
	 " spent for the I/O operations, memory allocations, etc. Also much more"
 " disk space is used.");

  table
	.add(poptmx::NOTE,     "ARGUMENTS:")
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

	.add(poptmx::NOTE,     "OPTIONS:")
	.add(poptmx::OPTION,   &contrast, 'C', "component",
		 "Type of the contrast component.",
		 "The component of the contrast to extract and reconstruct."
         " " + EDEIprocess::componentDesc, toString(contrast))
	.add(poptmx::OPTION,   &slicedesc, 's', "slice",
		 "Slices to be processed.", SliceOptionDesc, "<all>")
	.add(poptmx::OPTION,   &center, 'c', "center",
		 "Variable rotation center.", DcenterOptionDesc, toString(0.0))
	.add(poptmx::OPTION,   &filter_type, 'f', "filter",
		 "Filtering window used in the CT.", FilterOptionDesc, filter_type.name())
    .add(poptmx::OPTION, &edeiopt.RCname, 0, "rc",
         EDEIoptions::rcOptionShortDesc, EDEIoptions::rcOptionDesc)
	.add(poptmx::OPTION,   &nof_threads, 't', "threads",
		 "Number of threads used in calculations.",
		 "If the option is not used the optimal number is"
		 " calculated automatically.", "<all>")
	.add(poptmx::OPTION,   &SaveInt,'i', "int",
      "Output image(s) as integer.", IntOptionDesc)
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

  // <minus list> and <plus list> : required arguments.
  if ( ! table.count(&Mlistname) )
    exit_on_error(command, string () +
				  "Missing required argument: "+table.desc(&Mlistname)+".");
  if ( ! table.count(&Plistname) )
    exit_on_error(command, string() +
				  "Missing required argument: "+table.desc(&Plistname)+".");

  // <result mask> : one more argument may or may not exist
  if ( ! table.count(&outmask) )
	outmask = upgrade(Mlistname.dtitle(), "reconstructed-") + "-@.tif";
  if ( string(outmask).find('@') == string::npos )
	outmask = outmask.dtitle() + "-@" + outmask.extension();

}


/*

class slice_distributor {

public:

  const Filter & filter;
  const Contrast & contrast;
  const Dcenter & center;					///< Deviation of the rotation axis.
  const SinoS & sins;
  const vector<int> & slices;
  const string & sliceformat;
  const CTrec & rec;

  int SaveInt;
  int slice;					///< Current projection index.
  int size;
  int pixels;
  bool verb;
  ProgressBar bar;
  pthread_mutex_t lock;         ///< Thread mutex used in the data distribution.

  slice_distributor(const SinoS & _sins, const vector<int> & _slices,
					const string & _sliceformat, const Dcenter & _center,
					const Filter & _filter, Contrast _contrast, int _SaveInt,
					bool _verb, const CTrec & _rec )
	: sins(_sins), slices(_slices), sliceformat(_sliceformat), center(_center),
	  filter(_filter), contrast(_contrast), SaveInt(_SaveInt), verb(_verb), rec(_rec) {

	slice = 0;
	size = slices.size();
	pixels = sins.pixels();
    if ( pthread_mutex_init(&lock, NULL) != 0 )
      throw_error("slice_distributor", "Failed to initialize the mutex.");
	bar = ProgressBar(verb, "threaded reconstruction", size);

  }

  /// \brief Destructor.
  inline ~slice_distributor(){
    pthread_mutex_destroy(&lock);
  }

  /// \brief Destributor.
  ///
  /// @param Theta Rotation angle of the line.
  /// @param sinoline Line to be projected.
  ///
  /// @return \c true if the thread should process the job, \c false if the
  /// global work is finished and the thread should exit.
  ///
  inline bool distribute(int *_slice){
    pthread_mutex_lock(&lock);
	bool returned = slice < size;
	*_slice = slice++;
    pthread_mutex_unlock(&lock);
    return returned;
  }

};


void * in_thread (void * _args){

  slice_distributor * args = (slice_distributor *) _args;
  const CTrec & rec = args->rec;

  int slice;
  Map sinogram, result;

  while ( args->distribute(&slice) ) {
	args->sins.sino(slice, sinogram);
	rec.reconstruct(sinogram, result, args->center( args->slices[slice]+1 ) );
	SaveImage( fmt2s(args->sliceformat, args->slices[slice]+1), result, args->SaveInt);
	args->bar.update(slice);
  }

}

*/

/// \MAIN{ctref}
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
  CTrec rec( expr.shape() , expr.contrast(), args.filter_type);

  /*
  if ( args.nof_threads == 1 || slices.size()<=2 ) {
  */

  ProgressBar bar(args.beverbose, "reconstruction", sliceV.size());
  for (unsigned slice=0 ; slice < sliceV.size() ; slice++ ) {
    Map sinogram(thetas, pixels),
        result(pixels, pixels);
    sins.sino(slice, sinogram);
    const Map & res = rec.reconstruct(sinogram, args.center(sliceV[slice]+1) );
    SaveImage( toString(sliceformat, sliceV[slice]+1), res, args.SaveInt);
    bar.update();
  }

  /*

  } else {

  rec.threads(1);
  slice_distributor dist(sins, slices, sliceformat, args.center, args.filter_type,
               Contrast::REF,args. SaveInt, args.beverbose, rec);

  int threads = nof_threads(args.nof_threads);
  pthread_t ntid [threads];

  for (int ith = 0 ; ith < threads ; ith++)
    if ( pthread_create(ntid+ith, NULL, in_thread, &dist ) )
    throw_error("project sino in thread", "Can't create thread.");

  for (int ith = 0 ; ith < threads ; ith++)
    pthread_join( ntid[ith], 0);

  }
  */

  exit(0);

}
