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



#include "../common/ctas.h"
#include "../common/ct.h"
#include "../common/poptmx.h"

using namespace std;


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Contrast contrast;            ///< Type of the contrast.
  Filter filter_type;           ///< Type of the filtering function.
  float center;                   ///< Rotation center.
  deque<ImagePath> sinograms;         ///< Name of the sinogram file.
  ImagePath outmask;           ///< Name of the file to save the result to.
  float arc;
  float dd;             ///< Pixel size.
  float lambda;             ///< wavelength
  uint ringBox;
  bool beverbose;				///< Be verbose flag
  float mincon;         ///< Black intensity.
  float maxcon;         ///< White intensity.
  bool allowCPU;

  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[])
  : contrast(Contrast::ABS)
  , center(0)
  , beverbose(false)
  , mincon(0)
  , maxcon(0)
  , outmask("reconstructed-<sinogram>")
  , arc(180)
  , dd(0.0)
  , lambda(0.0)
  , ringBox(0)
  , allowCPU(false)
{

  poptmx::OptionTable table
  ("CT reconstruction of sinogram(s).",
   "The program reads pre-formed sinograms and reconstructs them.");

  table
  .add(poptmx::NOTE, "ARGUMENTS:")
  .add(poptmx::ARGUMENT, &sinograms, "sinogram",
       "Input image containing the sinogram.", "")

  .add(poptmx::NOTE, "OPTIONS:")
  .add(poptmx::OPTION, &outmask, 'o', "output",
       "Mask to output reconstructed image(s).", "", outmask)
  .add(poptmx::OPTION, &contrast, 'k', "contrast",
       "Input component.",
       "Type of the contrast presented in the sinogram. " + Contrast::Desc)
  .add(poptmx::OPTION, &center, 'c', "center",
       "Rotation center.", CenterOptionDesc, toString(center))
  .add(poptmx::OPTION, &arc, 'a', "arcan",
       "CT scan range (if > 1 deg), or step size (if < 1deg).",
       "If value is greater than 1deg then it represents the arc of the CT scan."
       " Otherwise the value is the step size.",
       toString(arc))
  .add(poptmx::OPTION, &filter_type, 'f', "filter",
       "Filtering window used in the CT.", FilterOptionDesc, filter_type.name())
  .add(poptmx::OPTION, &dd, 'r', "resolution",
       "Pixel size (micron).", ResolutionOptionDesc, toString(dd))
  .add(poptmx::OPTION, &lambda, 'w', "wavelength",
       "Wave length (Angstrom).", "If this option is used then outputs refraction index."
                                  " Otherwise outputs linnear attenuation coefficient (1/m).")
  .add(poptmx::OPTION, &ringBox, 'R', "ring",
           "Half size of the ring filter.", "", toString(ringBox))
  .add(poptmx::OPTION, &mincon, 'm', "min", "Pixel value corresponding to black.",
       " All values below this will turn black.", "<minimum>")
  .add(poptmx::OPTION, &maxcon, 'M', "max", "Pixel value corresponding to white.",
       " All values above this will turn white.", "<maximum>")
  .add(poptmx::OPTION, &allowCPU, 0, "cpu", "Allow CT'ing on CPU (very slow).",
       "By default CPU processing is allowed only if no OpenCL devices were found or failing."
       " Using CPU can be benificial on large slices, but detrimental on small ones.", toString(allowCPU))
  .add_standard_options(&beverbose)
  .add(poptmx::MAN, "SEE ALSO:", SeeAlsoList);

  if ( ! table.parse(argc,argv) )
    exit(0);
  if ( ! table.count() ) {
    table.usage();
    exit(0);
  }
  command = table.name();

  if ( ! table.count(&sinograms) )
    exit_on_error(command, string () +
                  "Missing required argument: "+table.desc(&sinograms)+".");
  if ( ! table.count(&outmask) )
    outmask = upgrade(sinograms[0], "reconstructed-");
  if ( table.count(&dd) ) {
    if ( dd <= 0.0 )
      exit_on_error(command, "Negative pixel size (given by "+table.desc(&dd)+").");
    dd /= 1.0E6; // mum -> m
  }
  if ( table.count(&lambda) ) {
    if ( lambda <= 0.0 )
      exit_on_error(command, "Negative wawelwngth (given by "+table.desc(&lambda)+").");
    lambda /= 1.0E10; // Angstrom -> m
  }
  if (arc <= 0.0)
    exit_on_error(command, "CT arc (given by "+table.desc(&arc)+") must be strictly positive.");


}


class RecInThread : public InThread {

  const clargs & ctrl;
  ReadVolumeBySlice ivolRd;
  const Shape<2> ish;
  const Shape<2> osh;
  SaveVolumeBySlice ovolSv;
  CTrec canonRec;

  unordered_map<pthread_t, RingFilter*> rings;
  unordered_map<pthread_t, CTrec*> recs;
  unordered_map<pthread_t, Map*> imaps;
  unordered_map<pthread_t, Map*> omaps;

  bool inThread(long int idx) {

    if (idx >= ivolRd.slices()) {
      for (auto & inrecs : recs)
        inrecs.second->doOnGPU();
      return false;
    }

    const pthread_t me = pthread_self();
    if ( ! recs.count(me) ) {
      CTrec * erec = ivolRd.slices() == 1 ? &canonRec : new CTrec(canonRec);
      erec->setPhysics(ctrl.dd, ctrl.lambda);
      RingFilter * ering = new RingFilter(ish(1), ctrl.ringBox);
      Map * eimap = new Map(ish);
      Map * eomap = new Map(osh);
      lock();
      recs.emplace(me, erec);
      rings.emplace(me, ering);
      imaps.emplace(me, eimap);
      omaps.emplace(me, eomap);
      unlock();
    }
    lock();
    RingFilter & myRing = *rings.at(me);
    CTrec & myRec = *recs.at(me);
    Map & myImap = *imaps.at(me);
    Map & myOmap = *omaps.at(me);
    unlock();

    ivolRd.read(idx, myImap);
    myRing.apply(myImap);
    myRec.reconstruct(myImap, myOmap, ctrl.center);
    ovolSv.save(idx, myOmap);
    bar.update();
    return true;

  }

public:

  RecInThread(const clargs & _args)
    : InThread(_args.beverbose, "CT'ing")
    , ctrl(_args)
    , ivolRd(ctrl.sinograms)
    , ish(ivolRd.face())
    , osh(ish(1),ish(1))
    , ovolSv(ctrl.outmask, osh, ivolRd.slices(), ctrl.mincon, ctrl.maxcon)
    , canonRec(ish, ctrl.contrast, ctrl.arc, ctrl.filter_type)
  {
    bar.setSteps(ivolRd.slices());
    if (ctrl.allowCPU)
      canonRec.allowCPU();
  }

  ~RecInThread() {
    auto dlnl = [&](auto pntr) { if (pntr) { delete pntr; pntr=0; } } ;
    for (auto celem : rings) dlnl(celem.second);
    for (auto celem : recs)  dlnl(celem.second);
    for (auto celem : imaps) dlnl(celem.second);
    for (auto celem : omaps) dlnl(celem.second);
  }

};



/// \MAIN{ct}
int main(int argc, char *argv[]) {
  const clargs args(argc, argv) ;
  RecInThread factory(args);
  factory.execute();
  exit(0);
}
