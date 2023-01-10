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
  deque<ImagePath> images;                 ///< contrasts at the distance.
  ImagePath oname;                ///< Output name of the phase contrast.
  float dd;                     ///< Pixel size.
  float d2b;                  ///< \f$\d2b\f$ parameter of the MBA.
  float lambda;                 ///< Wavelength.
  float dist;                   ///< Object-to-detector distance.
  bool phsExp;
  bool SaveInt;				///< Be verbose flag
  bool beverbose;             ///< Save image as 16-bit integer.

  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::clargs(int argc, char *argv[]) :
  dd(1.0),
  d2b(0.0),
  lambda(1.0),
  dist(1.0),
  phsExp(false),
  SaveInt(false),
  beverbose(false)
{


  poptmx::OptionTable table
  ("IPC contrast processing",
     "Extracts the phase and/or absorption contrast from the input image(s)."
     " Depending on the input parameters utilizes BA, MBA or BAC algorithms.");

  table
  .add(poptmx::NOTE, "ARGUMENTS:")
  .add(poptmx::ARGUMENT, &images, "input", "Raw images to be processed.", "")

  .add(poptmx::NOTE, "OPTIONS:")
  .add(poptmx::OPTION, &oname, 'o', "out", "Image name to output phase component", "", "<input>")
  .add(poptmx::OPTION, &dist, 'z', "distance", "Object-to-detector distance (mm)", NeedForQuant)
  .add(poptmx::OPTION, &dd, 'r', "resolution", "Pixel size of the detector (micron)",
       NeedForQuant, toString(dd))
  .add(poptmx::OPTION, &d2b, 'd', "d2b", d2bOptionDesc, "", toString(d2b))
  .add(poptmx::OPTION, &lambda, 'w', "wavelength", "Wavelength of the X-Ray (Angstrom)",
       "Only needed together with " + table.desc(&d2b) + ".", toString(lambda))
  .add(poptmx::OPTION, &phsExp,'e', "exp", "Outputs exponent of phase contrast.",
       "Useful for further CT processing of output as absorption contrast.")
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

  if ( ! table.count(&images) )
    exit_on_error(command, "Missing required argument: "+table.desc(&images)+".");
  if ( ! table.count(&dist) )
    exit_on_error(command, "Missing required option: " +table.desc(&dist)+ ".");

  if (dist <= 0.0)
    exit_on_error(command, "Zero or negative distance (given by "+table.desc(&dist)+").");
  dist /= 1.0E3; // convert mm -> m

  if (dd <= 0.0)
    exit_on_error(command, "Zero or negative pixel size (given by "+table.desc(&dd)+").");
  dd /= 1.0E6; // convert micron -> m

  if (lambda <= 0.0)
    exit_on_error(command, "Zero or negative wavelength (given by "+table.desc(&lambda)+").");
  if ( ! table.count(&lambda) && table.count(&d2b) )
    warn(command, "The wavelength (given by "+table.desc(&lambda)+") needed together with"
         " the d2b parameter (given by "+table.desc(&d2b)+") for the correct results.");
  lambda /= 1.0E10; // convert A -> m

  if (d2b < 0.0)
    exit_on_error(command, "Negative d2b parameter (given by "+table.desc(&d2b)+").");

}


class ProcInThread : public InThread {

  const clargs & args;
  ReadVolumeBySlice allIn;
  const Shape sh;
  const uint sz;
  const float d2bN;
  SaveVolumeBySlice * allOut;

  unordered_map<pthread_t, IPCprocess*> procs;
  unordered_map<pthread_t, Map*> iomaps;

  bool inThread(long int idx) {
    if (idx >= allIn.slices())
      return false;

    const pthread_t me = pthread_self();
    if ( ! procs.count(me) ) { // first call
      Map * iomap = new Map(sh);
      lock();
      if (procs.empty()) // need this first element to initialize the rest
        procs.emplace(me, new IPCprocess(sh, d2bN));
      else
        procs.emplace(me, new IPCprocess(*(procs.begin()->second)));
      iomaps.emplace(me, iomap);
      unlock();
    }
    lock();
    IPCprocess & myProc = *procs.at(me);
    Map & myIOmap = *iomaps.at(me);
    unlock();

    allIn.read(idx, myIOmap);
    myProc.extract(myIOmap);
    if (args.phsExp)
      myIOmap = exp(-myIOmap);
    if (allOut)
      allOut->save(idx, myIOmap);
    else
      allIn.write(idx, myIOmap);

    bar.update();
    return true;
  }

public:

  ProcInThread(const clargs & _args)
    : InThread(_args.beverbose, "IPC processing")
    , args(_args)
    , allIn(args.images, args.oname.empty())
    , sh(allIn.face())
    , sz(allIn.slices())
    , allOut(0)
    , d2bN( IPCprocess::d2bNorm(args.d2b, args.dd, args.dist, args.lambda) )
  {
    if (!args.oname.empty())
      allOut = new SaveVolumeBySlice(args.oname, sh, sz);
    bar.setSteps(sz);
  }

  ~ProcInThread() {
    #define delnul(pntr) { if (pntr) delete pntr; pntr = 0;}
    for (auto celem : procs) delnul(celem.second);
    for (auto celem : iomaps)  delnul(celem.second);
    delnul(allOut);
    #undef delnul

  }

};



/// \MAIN{ct}
int main(int argc, char *argv[]) {
  const clargs args(argc, argv) ;
  ProcInThread procall(args);
  procall.execute();
  // exit(0);  // commented to run destructors
}



