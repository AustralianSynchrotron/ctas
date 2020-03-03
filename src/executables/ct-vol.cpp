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
#include <queue>
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
  float arc;
  Crop crp; ///< Crop input projection image
  Dcenter center;               ///< Rotation center.
  bool beverbose;       ///< Be verbose flag
  bool SaveInt;         ///< Save image as 16-bit integer.

  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
  beverbose(false),
  arc(180.0),
  angle(0.0),
  crp(),
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
    .add(poptmx::OPTION, &contrast, 'k', "contrast",
         "Input component.",
         "Type of the contrast presented in the sinogram. " + Contrast::Desc)
    .add(poptmx::OPTION, &slicedesc, 's', "slice",
         "Slices to be processed.",
         "If the rotation angle is given the slices correspond to the rotated image."
         + SliceOptionDesc, "<all>")
    .add(poptmx::OPTION, &angle, 't', "tilt",
         "Angle of the image slicing.", "", toString(angle))
    .add(poptmx::OPTION, &crp, 0, "crop",
         CropOptionDesc, "")
    .add(poptmx::OPTION, &center, 'c', "center",
         "Variable rotation center.", DcenterOptionDesc, toString(0.0))
    .add(poptmx::OPTION, &arc, 'a', "arcan",
         "CT scan range (if > 1 deg), or step size (if < 1deg).",
         "If value is greater than 1deg then it represents the arc of the CT scan."
         " Otherwise the value is the step size.",
         toString(arc))
    .add(poptmx::OPTION, &SaveInt,'i', "int",
         "Output image(s) as integer.", IntOptionDesc)
    .add(poptmx::OPTION, &filter_type, 'f', "filter",
         "Filtering window used in the CT.", FilterOptionDesc, filter_type.name())
    .add(poptmx::OPTION, &dd, 'r', "resolution",
         "Pixel size (micron).", ResolutionOptionDesc, toString(dd))
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
    if (arc <= 0.0)
      exit_on_error(command, "CT arc (given by "+table.desc(&arc)+") must be strictly positive.");
    else if (arc < 1.0)
      arc *= table.count(&inlist);

    angle *= M_PI/180;

}



void * in_write_thread (void * _thread_args);


class slice_distributor {

public:

  const clargs & args;
  const SinoS *sins;
  const string sliceformat;

  int slice;          ///< Current projection index.
  int size;
  int pixels;

  ProgressBar bar;
  pthread_mutex_t distribution_lock;         ///< Thread mutex used in the data distribution.
  //static pthread_mutex_t ctrec_lock;

  pthread_t write_thread;

  struct ImageCue {
    Map image;
    int curslice;
    ImageCue(const Map & _image, int _curslice) :
      image(_image.copy()), curslice(_curslice) {}
  };
  queue<ImageCue*> imagecue;
  static pthread_mutex_t putake_lock;
  static pthread_cond_t new_data_cond;
  static pthread_cond_t released_memory_cond;
  bool no_more_data;

  static pthread_mutex_t createrec_lock;

  slice_distributor(const clargs & _args, const SinoS *_sins) :
    args(_args),
    sins(_sins),
    slice(0),
    sliceformat( mask2format(args.outmask, sins->imageShape()(0) ) ),
    no_more_data(false)
  {

    if (!sins)
      throw_error("slice distributor", "Non-existing sinograms.");
    if ( pthread_mutex_init(&distribution_lock, NULL) != 0 )
      throw_error("slice distributor", "Failed to initialize the mutex.");
    bar = ProgressBar(args.beverbose, "reconstruction", _sins->slices());

    if ( pthread_create(&write_thread, NULL, in_write_thread, this ) )
      throw_error("write thread", "Can't create thread.");


  }

/// \brief Destructor.
  inline ~slice_distributor() {
    // It is supposed to be onces created, never destroyed.
  }




  ImageCue * put_image(const Map & image, int curslice) {
    // will be called from one of the reconstraction threads.
    ImageCue * img ;
    pthread_mutex_lock(&putake_lock);
    while ( ! ( img = new (nothrow) ImageCue(image, curslice) ) )
      pthread_cond_wait(&released_memory_cond, &putake_lock);
    imagecue.push(img);
    pthread_mutex_unlock(&putake_lock);
    pthread_cond_signal(&new_data_cond);
    return 0;
  }


  ImageCue * take_image() {

    // will be called from writing thread.
    pthread_mutex_lock(&putake_lock);
    while ( imagecue.empty() &&  ! no_more_data )
      pthread_cond_wait(&new_data_cond, &putake_lock);

    ImageCue * img = 0;
    if ( ! imagecue.empty() ) {
      img = imagecue.front();
      imagecue.pop();
    }

    pthread_mutex_unlock(&putake_lock);
    return img;

  }


  inline void complete_writing() {
    no_more_data=true;
    pthread_cond_signal(&new_data_cond);
    pthread_join(write_thread, 0);
  }


  inline bool all_scheduled() {
    return slice >= sins->slices();
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
    pthread_mutex_lock(&distribution_lock);
    bool returned = ! all_scheduled();
    *_slice = slice++;
    pthread_mutex_unlock(&distribution_lock);
    return returned;
  }

};

pthread_mutex_t slice_distributor::putake_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t slice_distributor::new_data_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t slice_distributor::released_memory_cond = PTHREAD_COND_INITIALIZER;


void *in_write_thread (void *_thread_args) {

  slice_distributor *distributor = ( slice_distributor * ) (_thread_args);
  if ( ! distributor )
    throw_error("write thread", "Inappropriate thread function arguments.");

  slice_distributor::ImageCue *img=0;
  while ( ( img = distributor->take_image() ) ) {
    SaveImage( toString(distributor->sliceformat, img->curslice),
               img->image, distributor->args.SaveInt);
    delete img;
    distributor->bar.update();
    pthread_cond_signal(& distributor->released_memory_cond);
  }

  return 0;


}




string writefails="";
pthread_mutex_t addfail_lock = PTHREAD_MUTEX_INITIALIZER;


void *in_reconstruction_thread (void *_thread_args) {


  slice_distributor * distributor = ( slice_distributor* ) (_thread_args);
  if ( ! distributor )
    throw_error("in thread", "Inappropriate thread function arguments.");

  CTrec rec(distributor->sins->sinoShape(), distributor->args.contrast,
            distributor->args.arc, distributor->args.filter_type );

  int slice;
  Map sinogram;

  while ( distributor->distribute(&slice) ) {
    distributor->sins->sino(slice, sinogram);
    int curslice = distributor->sins->indexes()[slice]+1;
    const Map & res = rec.reconstruct(sinogram, distributor->args.center(curslice),
                               distributor->args.dd);
    try { SaveImage( toString(distributor->sliceformat, curslice), res, distributor->args.SaveInt ); }
    catch ( CtasErr err ) {
      pthread_mutex_lock(&addfail_lock);
      if ( ! writefails.empty()  )
        writefails += ",";
      writefails += toString(curslice);      
      pthread_mutex_unlock(&addfail_lock);
    }
    distributor->bar.update();
    
    // distributor->put_image( res, curslice );

  }

  return 0;

}


















/// \MAIN{ctabs}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv);
  const SinoS sins(args.inlist, args.slicedesc, args.angle, args.crp, args.beverbose);
  if ( ! sins.indexes().size() )
    throw_error(args.command, "No slices requested");

  slice_distributor dist(args, &sins);
  const int run_threads = min<int>( nof_threads() , sins.indexes().size() ) ;
  vector<pthread_t> threads(run_threads);

  for (int ith = 0 ; ith < run_threads ; ith++)
    if ( pthread_create( & threads[ith], NULL, in_reconstruction_thread, &dist ) )
      throw_error("project sino in thread", "Can't create thread.");

  for (int ith = 0 ; ith < threads.size() ; ith++)
    pthread_join( threads[ith], 0);

  dist.complete_writing();

  if ( ! writefails.empty() ) 
    cerr << "Fails: " << writefails << endl;

  exit(0);

}
