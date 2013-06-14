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
  Contrast contrast;            ///< Type of the contrast.
  vector<Path> inlist;        ///< List of all input contrasts.
  bool idealworld;               ///< Assumption of the world's ideality.
  string slicedesc;       ///< String describing the slices to be CT'ed.
  Path outmask;       ///< The mask for the output file names.
  Filter filter_type;           ///< Type of the filtering function.
  float dd;             ///< Pixel size.
  float angle;           ///< Angle of the sino slicing.
  Crop crop; ///< Crop input projection image
  Dcenter center;               ///< Rotation center.
  unsigned nof_threads;              ///< Number of threads in the reconstruction.
  bool beverbose;       ///< Be verbose flag
  bool SaveInt;         ///< Save image as 16-bit integer.

  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
  beverbose(false),
  angle(0.0),
  crop(),
  nof_threads(0),
  SaveInt(false),
  outmask("reconstructed-@.tif"),
  filter_type(),
  dd(1.0)
{

  poptmx::OptionTable table
    ("Volume CT reconstruction.",
     "Reconstructs multiple slices from the list of preprocessed projections."
     "The same processing can be performed on the step-by-step basis using tools"
     " \"sino\" and \"ct\", but this approach saves all intermediate"
     " results on the hard disk and therefore a lot of time is spent for the"
     " I/O operations, memory allocations, etc. Also much more disk space is used.");

  table
    .add(poptmx::NOTE, "ARGUMENTS:")
    .add(poptmx::ARGUMENT, &inlist, "list",
         "List of the input images.", AqSeries::Desc)

    .add(poptmx::NOTE, "OPTIONS:")
    .add(poptmx::OPTION, &outmask, 'o', "output",
         "Output result mask.", MaskDesc, outmask)
    .add(poptmx::OPTION, &contrast, 'C', "contrast",
         "Input component.",
         "Type of the contrast presented in the sinogram. " + Contrast::Desc)
    .add(poptmx::OPTION, &slicedesc, 's', "slice",
         "Slices to be processed.",
         "If the rotation angle is given the slices correspond to the rotated image."
         + SliceOptionDesc, "<all>")
    .add(poptmx::OPTION, &angle, 'a', "angle",
         "Angle of the image slicing.", "", toString(angle))
    .add(poptmx::OPTION, &crop, 0, "crop",
         CropOptionDesc, "")
    .add(poptmx::OPTION, &center, 'c', "center",
         "Variable rotation center.", DcenterOptionDesc, toString(0.0))
    .add(poptmx::OPTION, &SaveInt,'i', "int",
         "Output image(s) as integer.", IntOptionDesc)
    .add(poptmx::OPTION, &filter_type, 'f', "filter",
         "Filtering window used in the CT.", FilterOptionDesc, filter_type.name())
    .add(poptmx::OPTION, &dd, 'r', "resolution",
         "Pixel size (micron).", ResolutionOptionDesc, toString(dd))
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

    if ( string(outmask).find('@') == string::npos )
      outmask = outmask.dtitle() + "-@" + outmask.extension();

    angle *= M_PI/180;

}






class slice_distributor {

public:

  const clargs & args;
  const SinoS *sins;
  const string sliceformat;
  const CTrec & rec;

  int slice;          ///< Current projection index.
  int size;
  int pixels;

  ProgressBar bar;
  pthread_mutex_t lock;         ///< Thread mutex used in the data distribution.

  slice_distributor(const clargs & _args, const SinoS *_sins, const CTrec & _rec ) :
    args(_args),
    sins(_sins),
    rec(_rec),
    slice(0),
    sliceformat( mask2format(args.outmask, sins->imageShape()(0) ) )
  {
    if (!sins)
      throw_error("slice distributor", "Non-existing sinograms.");
    if ( pthread_mutex_init(&lock, NULL) != 0 )
      throw_error("slice distributor", "Failed to initialize the mutex.");
    bar = ProgressBar(args.beverbose, "reconstruction", _sins->slices());
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
 inline bool distribute(int *_slice) {
   pthread_mutex_lock(&lock);
   bool returned = slice < sins->slices();
   *_slice = slice++;
   pthread_mutex_unlock(&lock);
   return returned;
 }

};


void * in_thread (void * _thread_args){

  slice_distributor * thread_args = ( slice_distributor* ) (_thread_args);
  if ( ! thread_args )
    throw_error("in thread", "Inappropriate thread function arguments.");
  const CTrec & rec = thread_args->rec;

  int slice;
  Map sinogram, result;

  while ( thread_args->distribute(&slice) ) {
    thread_args->sins->sino(slice, sinogram);
    int curslice = thread_args->sins->indexes()[slice]+1;
    rec.reconstruct(sinogram, result, thread_args->args.center(curslice) );
    SaveImage( toString(thread_args->sliceformat, curslice),
               result, thread_args->args.SaveInt);
    thread_args->bar.update();
  }


}


















/// \MAIN{ctabs}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv);
  const SinoS *sins = (args.angle==0.0) ?
    new SinoS(args.inlist, args.slicedesc, args.beverbose) :
    new SinoS(args.inlist, args.slicedesc, args.angle, args.crop, args.beverbose);
  if ( ! sins || ! sins->indexes().size() )
    throw_error(args.command, "No slices requested");

  CTrec rec(sins->pixels(), args.contrast, args.nof_threads, args.filter_type);

  if ( args.nof_threads == 1 || sins->indexes().size()<=2 ) {

    Map sinogram, result;
    const Path outmask =  ( string(args.outmask).find('@') == string::npos ) ?
    args.outmask.dtitle() + "-@" + args.outmask.extension() :
    string( args.outmask ) ;
    const string sliceformat = mask2format(outmask, sins->imageShape()(0) );
    ProgressBar bar(args.beverbose, "reconstruction", sins->indexes().size());

    for (unsigned slice=0 ; slice < sins->indexes().size() ; slice++ ) {
      sins->sino(slice, sinogram);
      rec.reconstruct(sinogram, result, args.center(slice+1));
      SaveImage( toString(sliceformat, sins->indexes()[slice]+1), result, args.SaveInt);
      bar.update();
    }

  }  else {

    rec.threads(1);
    slice_distributor dist(args, sins, rec);
    const int threads = nof_threads(args.nof_threads);

    pthread_t ntid [threads];
    for (int ith = 0 ; ith < threads ; ith++)
      if ( pthread_create(ntid+ith, NULL, in_thread, &dist ) )
        throw_error("project sino in thread", "Can't create thread.");

      for (int ith = 0 ; ith < threads ; ith++)
        pthread_join( ntid[ith], 0);

  }


  delete sins;

  exit(0);

}
