/******************************************************************************
 *   Copyright (C) 2017 by Anton Maksimenko                                   *
 *   antonmx@gmail.com                                                        *
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
/// @date   Thu Jun 06 10:09:31 2017
///
/// @brief %Forms projection from multiple images.
///

#define _USE_MATH_DEFINES // for M_PI

#include "../common/common.h"
#include "../common/poptmx.h"
#include "../common/flatfield.h"
#include "../common/kernel.h"
#include "../common/ipc.h"
#include <algorithm>
#include <string.h>
#include <math.h>
#include <unordered_map>


using namespace std;



/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Path outmask;
  Path gaps;
  vector<Path> dfs0;
  vector<Path> dfs1;
  vector<Path> bgs0;
  vector<Path> bgs1;
  vector<Path> ims0;
  vector<Path> ims1;
  PointF2D shift;  // for frame formation
  float ashift;  // for frame formation
  Filter filter_type;  // for CT
  float dd;  // for Phase and CT
  float arc; // for frame formation and CT
  Crop crop; // for frame formation
  Crop cropF; // for frame formation
  float angle;
  float center; // for frame formation and CT
  uint trans; // for frame formation
  uint radFill; // for frame formation: fill gap intersections
  float d2b; // for phase
  float lambda; // for phase
  float dist;  // for phase
  bool SaveInt;
  bool beverbose;
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[])
  : outmask("_.tif")
  , shift(0,0)
  , ashift(0.0)
  , dd(1.0)
  , arc(180)
  , angle(0)
  , center(0)
  , trans(0)
  , radFill(0)
  , d2b(0.0)
  , lambda(1.0)
  , dist(1.0)
  , SaveInt(false)
  , beverbose(false)
{

  poptmx::OptionTable table
    ("IMBL BCT pipeline.",
     "Proccesses the IMBL BCT experiment input data to produce CT-reconstructed slices.");

  table
      .add(poptmx::NOTE, "ARGUMENTS:")
      .add(poptmx::ARGUMENT, &outmask, "output", "Output result mask or filename.",
           "Output filename if output is a single file. Output mask otherwise. " + MaskDesc, outmask)

      .add(poptmx::NOTE, "OPTIONS:")
      .add(poptmx::OPTION, &ims0, 'f', "sample0", "First input set.", "")
      .add(poptmx::OPTION, &ims1, 'F', "sample1", "Second input set.", "")
      .add(poptmx::OPTION, &dfs0, 'd', "df0", "Dark current image(s) for the first input set.", "")
      .add(poptmx::OPTION, &dfs1, 'D', "df1", "Dark current image(s) for the second input set.", "")
      .add(poptmx::OPTION, &bgs0, 'b', "bg0", "Background image(s) for the first input set.", "")
      .add(poptmx::OPTION, &bgs1, 'B', "bg1", "Background image(s) for the second input set.", "")
      .add(poptmx::OPTION, &gaps, 'g', "gaps", "Map of the gaps",
           "Image file containing the mask of the detector gaps.")
      .add(poptmx::OPTION, &shift, 's', "shift", "Spatial shift of the second data set.", "")
      .add(poptmx::OPTION, &ashift, 'S', "start", "Starting angle of the second data set.",
           "The starting angular position relative to the starting position of the first data set.")
      .add(poptmx::OPTION, &arc, 'a', "arcan", "CT scan range (if > 1 deg), or step size (if < 1 deg).",
           "If value is greater than 1deg then it represents the arc of the CT scan."
           " Otherwise the value is the step size.", toString(arc))
      .add(poptmx::OPTION, &crop, 0, "crop", "Crop input images: " + CropOptionDesc, "")
      .add(poptmx::OPTION, &cropF, 0, "crop-final", "Crops final image: " + CropOptionDesc, "")
      .add(poptmx::OPTION, &angle, 0, "rotate", "Rotate input images by given angle.", "")
      .add(poptmx::OPTION, &center, 'c', "center", "Rotation center after cropping.", CenterOptionDesc)
      .add(poptmx::OPTION, &trans, 'T', "trans", "Transition area around gaps.",
           "The area of this thickness around the gaps is used to smoothly"
           " interpolate between single (gap) and double signal areas." )
      .add(poptmx::OPTION, &radFill, 'R', "fill", "Radius of the area used for filling gaps.", "")
      .add(poptmx::OPTION, &dist, 'z', "distance", "Object-to-detector distance (mm)",
             "More correctly the distance from the contact print plane and the detector plane"
             " where the image was acquired. " + NeedForQuant)
      .add(poptmx::OPTION, &d2b, 0, "d2b", d2bOptionDesc, "", toString(d2b))
      .add(poptmx::OPTION, &lambda, 'w', "wavelength", "Wavelength of the X-Ray (Angstrom)",
             "Only needed together with " + table.desc(&d2b) + ".", toString(lambda))
      .add(poptmx::OPTION, &filter_type, 0, "filter",
           "Filtering window used in the CT.", FilterOptionDesc, filter_type.name())
      .add(poptmx::OPTION, &dd, 'r', "resolution",
           "Pixel size (micron).", ResolutionOptionDesc, toString(dd))

      .add(poptmx::OPTION, &SaveInt,'i', "int", "Output image(s) as integer.", IntOptionDesc)
      .add_standard_options(&beverbose)
      .add(poptmx::MAN, "SEE ALSO:", SeeAlsoList);

  if ( ! table.parse(argc,argv) )
    exit(0);
  if ( ! table.count() ) {
    table.usage();
    exit(0);
  }
  command = table.name();

  #define chck_inSet(set, role) \
    if ( ! table.count(&set) ) \
      exit_on_error(command, "Missing required input for the" + string(role) + "set.");

  chck_inSet(ims0, "first image");
  chck_inSet(ims1, "second image");
  chck_inSet(bgs0, "first background");
  chck_inSet(bgs0, "second background");

  #undef chck_inSet

  if ( arc <= 0.0 )
    exit_on_error(command, "CT arc (given by "+table.desc(&arc)+") must be strictly positive.");
  if ( ashift < 0.0 )
    exit_on_error(command, "Angular shift between two sets (given by "+table.desc(&shift)+")"
                           " must be strictly positive.");
  if ( ashift >= 360.0 ) {
    float dummy;
    ashift -= 360 * modf( ashift/360.0, &dummy);
  }

  if ( ! table.count(&dist) )
    exit_on_error(command, "Missing required option: "+table.desc(&dist)+".");
  if (dist <= 0.0)
    exit_on_error(command, "Zero or negative distance (given by "+table.desc(&dist)+").");
  dist /= 1.0E3; // convert mm -> m

  if (dd <= 0.0)
    exit_on_error(command, "Zero or negative pixel size (given by "+table.desc(&dd)+").");
  dd /= 1.0E6; // convert micron -> m

  if (d2b < 0.0)
    exit_on_error(command, "Negative d2b parameter (given by "+table.desc(&d2b)+").");

  if (lambda <= 0.0)
    exit_on_error(command, "Zero or negative wavelength (given by "+table.desc(&lambda)+").");
  if ( table.count(&lambda) && ! table.count(&d2b) )
    warn(command, "The wavelength (given by "+table.desc(&lambda)+") has influence only together"
         " with the d2b parameter (given by "+table.desc(&d2b)+").");
  if ( ! table.count(&lambda) && table.count(&d2b) )
    warn(command, "The wavelength (given by "+table.desc(&lambda)+") needed together with"
         " the d2b parameter (given by "+table.desc(&d2b)+") for the correct results.");
  lambda /= 1.0E10; // convert A -> m

}




class FlatInThread : public InThread {

  Volume & vol;
  const FlatFieldProc canon;
  unordered_map<pthread_t,FlatFieldProc> ffProcs;

  bool inThread (long int idx) {

    if ( idx >= vol.shape()(0) )
      return false;

    pthread_t me(pthread_self());
    lock();
    if ( ! ffProcs.count(me) )
      ffProcs.insert({me, FlatFieldProc(canon)});
    FlatFieldProc & myProc = ffProcs.at(me);
    unlock();

    Map frame(vol(idx,all,all));
    myProc.process(frame);
    bar.update();
    return true;

  }

public:

  FlatInThread(Volume & v, const Map & bg, const Map & df, const Map & mask, bool verbose=false)
    : vol(v)
    , canon(bg, df, mask)
    , InThread(verbose , "performing flat field", v.shape()(0))
  {}

  static void execute(Volume & v, const Map & bg, const Map & df, const Map & mask, bool verbose=false) {
    FlatInThread(v, bg, df, mask, verbose).InThread::execute();
  }

};






// Prepare gaps for frame formation
void prepareGaps(Map & gaps, uint trans) {

  const float mm = min(gaps);
  const float MM = max(gaps);
  if (MM <= 0)
    throw_error("GapsMask", "Mask covers whole image.");
  if (mm==MM) // no gaps
    return;
  gaps = (gaps-mm)/(MM-mm);

  const Shape ish = gaps.shape();
  const float step = 1.0 / (trans+1);

  Map tmp(ish);
  tmp = gaps;

  for ( int st = 1 ; st <= trans ; st++ ) {
    const float fill = step*st;

    for (ArrIndex i = 0 ; i<ish(0) ; i++)
      for (ArrIndex j = 0 ; j<ish(1) ; j++)
        if ( gaps(i,j) != 1.0 )

          for (ArrIndex ii = i-1 ; ii <= i+1 ; ii++)
            for (ArrIndex jj = j-1 ; jj <= j+1 ; jj++)

              if ( ii >= 0 && ii < ish(0) && jj >= 0 && jj < ish(1)
                   &&  gaps(ii,jj) == 1.0 )
                tmp(ii,jj) = fill;

    gaps = tmp;
  }

}






class FrameFormInThread : public InThread {

private:

  Volume & res;
  const Volume & ims0;
  const Volume & ims1;
  const Map & gaps;
  const float arc;
  const PointF2D & shift;
  const float ashift;
  const float cent;

  const Shape ish;
  const Shape osh;
  const Crop crop0;
  const Crop crop1;
  const float step;
  const float oz;
  const int nshift;
  const int rFill;

  static char formframe_src[];
  static const cl_program formframeProgram;

  struct PerThread {
    cl_kernel kernelFormFrame;
    cl_kernel kernelFill;
    cl_mem clim0;
    cl_mem clim1;
    cl_mem clout;
    PerThread(cl_kernel kFF, cl_kernel kF, cl_mem _clim0, cl_mem _clim1, cl_mem _clout)
      : kernelFormFrame(kFF)
      , kernelFill(kF)
      , clout(_clout)
      , clim1(_clim1)
      , clim0(_clim0)
    {}
  } ;
  unordered_map<pthread_t,PerThread> perThread;

  cl_mem clgaps0;
  cl_mem clgaps1;
  cl_mem clgapsF;

  ~FrameFormInThread() {
    clReleaseMemObject(clgaps0);
    clReleaseMemObject(clgaps1);
    clReleaseMemObject(clgapsF);
    for (auto& it: perThread) {
      clReleaseKernel(it.second.kernelFormFrame);
      clReleaseKernel(it.second.kernelFill);
      clReleaseMemObject(it.second.clim0);
      clReleaseMemObject(it.second.clim1);
      clReleaseMemObject(it.second.clout);
    }
  }


  bool inThread(const long int idx) {

    if ( idx >= oz )
      return false;


    pthread_t me(pthread_self());
    lock();
    if ( ! perThread.count(me) ) { // first run
      unlock();

      cl_kernel kernelFormFrame = createKernel(formframeProgram , "formframe");
      cl_mem _clout = clAllocArray<float>(area(osh));
      cl_mem _clim0 = clAllocArray<float>(area(osh),CL_MEM_READ_ONLY);
      cl_mem _clim1 = clAllocArray<float>(area(osh),CL_MEM_READ_ONLY);
      setArg(kernelFormFrame, 0, _clout );
      setArg(kernelFormFrame, 1, _clim0 );
      setArg(kernelFormFrame, 2, _clim1 );
      setArg(kernelFormFrame, 3, clgaps0 );

      cl_kernel kernelFill = rFill ? createKernel(formframeProgram , "gapfill") : 0;
      if (rFill) {
        setArg( kernelFill, 0, (int) osh(1) );
        setArg( kernelFill, 1, (int) osh(0) );
        setArg( kernelFill, 2, rFill );
        setArg( kernelFill, 3, _clout );
        setArg( kernelFill, 4, clgaps0 );
      }

      lock();
      perThread.emplace( me, PerThread(kernelFormFrame, kernelFill, _clim0, _clim1, _clout) );
    }
    PerThread & my = perThread.at(me);
    unlock();

    Map im0(osh);
    crop( ims0(idx, all, all), im0, crop0 );
    blitz2cl(im0, my.clim0);

    int id1 = idx-nshift;
    if (id1 < 0)
      id1 += oz-1;
    if (id1 < 0) // yes, two times needed
      id1 += oz-1;

    Map im1(osh);
    crop( ims1(id1, all, all), im1, crop1 );
    if ( idx < nshift  &&  idx > nshift - oz ) {
      im1.reverseSelf(blitz::secondDim);
      setArg( my.kernelFormFrame, 4, clgapsF );
      if (rFill)
        setArg( my.kernelFill, 5, clgapsF );
    } else {
      setArg( my.kernelFormFrame, 4, clgaps1 );
      if (rFill)
        setArg( my.kernelFill, 5, clgaps1 );
    }
    blitz2cl(im1, my.clim1);

    /*
    lock();
    SaveImage("io0.tif",  ims0(idx, all, all), true);
    SaveImage("io1.tif",  ims1(id1, all, all), true);
    SaveImage("im0.tif",  im0, true);
    SaveImage("im1.tif",  im1, true);
    unlock();
    */

    execKernel(my.kernelFormFrame, area(osh));
    if (rFill)
      execKernel(my.kernelFill, osh);

    cl2blitz(my.clout, im0);
    res(idx, all, all) = im0;
    bar.update();
    return true;

  }


  Crop crops(const PointF2D & _shift, float _cent, bool org) {
    const float Ms = max(0.0f, _shift.x);
    const float ms = -min(0.0f, _shift.x);
    const float M2c = 2*max(0.0f, _cent);
    const float m2c = -2*min(0.0f, _cent);
    const float My = max(0.0f, shift.y);
    const float my = -min(0.0f, shift.y);
    if (org)
      return Crop(My , Ms + M2c, my, ms + m2c);
    else
      return Crop(my , ms + M2c, My, Ms + m2c);
  }


public:

  FrameFormInThread(Volume & _res, const Volume & _ims0, const Volume & _ims1, const Map & _gaps,
                    float _arc, const PointF2D & _shift, float _ashift, float _cent, uint _rFill,
                    bool verbose=false)

    : res(_res)
    , ims0(_ims0)
    , ims1(_ims1)
    , gaps(_gaps)
    , arc(_arc)
    , shift(_shift)
    , ashift(_ashift)
    , cent(_cent)
    , rFill(_rFill)
    , crop0(crops(_shift, _cent, true))
    , crop1(crops(_shift, _cent, false))
    , ish(faceShape(ims0.shape()))
    , osh( ish(0) - abs(_shift.y), ish(1) - abs(_shift.x) - 2*abs(_cent) )
    , step( arc / (ims0.shape()(0)-1) )
    , oz(1+(int)(180/step))
    , nshift((int)(_ashift/step))

    , InThread(verbose , "performing frame formation", _ims0.shape()(0))

  {
    res.resize(oz , osh(0), osh(1));
    Map gapst(osh);
    crop(gaps, gapst, crop0);
    clgaps0 = blitz2cl(gapst, CL_MEM_READ_ONLY);
    crop(gaps, gapst, crop1);
    clgaps1 = blitz2cl(gapst, CL_MEM_READ_ONLY);
    gapst.reverseSelf(blitz::secondDim);
    clgapsF = blitz2cl(gapst, CL_MEM_READ_ONLY);
  }


  static void execute(Volume & _res, const Volume & _ims0, const Volume & _ims1, const Map & _gaps,
                      float _arc, const PointF2D & _shift, float _ashift, float _cent, uint _rFill,
                      bool verbose=false) {
    FrameFormInThread(_res, _ims0, _ims1, _gaps, _arc, _shift, _ashift, _cent, _rFill, verbose)
        .InThread::execute();
  }

};

char FrameFormInThread::formframe_src[] = {
  #include "formframe.cl.includeme"
};

const cl_program FrameFormInThread::formframeProgram =
      initProgram( formframe_src, sizeof(formframe_src), "Frame formation on OCL" );







class PhaseInThread : public InThread {

private:

  unordered_map<pthread_t,IPCprocess> procs;
  Volume & frames;
  float ind2b;

  bool inThread(long int idx) {

    if ( idx >= frames.shape()(0) )
      return false;

    const pthread_t me = pthread_self();
    lock();
    if ( ! procs.count(me) ) // first call
      procs.insert({me, IPCprocess(faceShape(frames.shape()), ind2b)});
    IPCprocess & myProc = procs.at(me);
    unlock();

    Map io(frames(idx,all,all));
    myProc.extract(io, IPCprocess::PHS);
    return true;

  }

public:

  PhaseInThread(Volume & fms, float _ind2b, bool verbose=false)
    : frames(fms)
    , ind2b(_ind2b)
    , InThread(verbose , "Performing phase retrieval.", fms.shape()(0))
  {}


  static void execute(Volume & fms, float _ind2b, bool verbose=false) {
    PhaseInThread(fms, _ind2b, verbose).InThread::execute();
  }


};









class CTinThread : public InThread {

private:

  unordered_map<pthread_t,CTrec> recs;
  const Volume & frames;
  Volume & result;
  const Contrast contrast;
  const Filter filter;
  const float pixelSize;
  const Shape ssh;
  const int slices;

  bool inThread(long int idx) {

    if ( idx >= slices )
      return false;

    const pthread_t me = pthread_self();
    lock();
    if ( ! recs.count(me) ) // first call
      recs.insert({me, CTrec(ssh, contrast, 180, filter)}); // arc is 180 after frames formation
    CTrec & rec = recs.at(me);
    unlock();

    Map sino(frames(all, idx, all));
    result(idx, all, all)
        = rec.reconstruct(sino , 0, pixelSize); // centre is 0 after frames formation
    bar.update();
    return true;

  }

public:

  CTinThread(const Volume & fms, Volume & res, Contrast cn, Filter ft, float pp, bool verbose=false)
    : frames(fms)
    , result(res)
    , contrast(cn)
    , filter(ft)
    , pixelSize(pp)
    , ssh(fms.shape()(2), fms.shape()(0))
    , slices(fms.shape()(1))
    , InThread(verbose , "Performing flat field.", fms.shape()(1))
  {
    result.resize(slices, ssh(0), ssh(0));
  }


  static void execute(const Volume & fms, Volume & res, Contrast cn, Filter ft, float pp, bool verbose=false) {
    CTinThread(fms, res, cn, ft, pp, verbose).InThread::execute();
  }


};








/// \MAIN{projection}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;

  const Shape ish = ImageSizes(args.ims0.at(0));
  const Shape sh = Shape( ish(0) - args.crop.top  - args.crop.bottom,
                          ish(1) - args.crop.left - args.crop.right );





  // Read DFs, BGs, GapMask

  #define ReadSumSet( set, res ) { \
    if ( ! set.empty() ) { \
      Volume tmp; \
      ReadVolume(set, tmp, args.beverbose); \
      if ( faceShape(tmp.shape()) != ish ) \
        exit_on_error("ReadAux", "Wrong image sizes."); \
      binn(tmp, Binn3(1,1,0)); \
      Map img = tmp(0, all, all); \
      crop(img, res, args.crop); \
    } \
  }

  Map dfs0(sh); ReadSumSet(args.dfs0, dfs0);
  Map dfs1(sh); ReadSumSet(args.dfs1, dfs1);
  Map bgs0(sh); ReadSumSet(args.bgs0, bgs0);
  Map bgs1(sh); ReadSumSet(args.bgs1, bgs1);

  #undef ReadSumSet

  Map gaps(sh);
  if ( ! args.gaps.empty() ) {
    Map tmp;
    ReadImage(args.gaps, tmp, ish);
    crop(tmp, gaps, args.crop);
    prepareGaps(gaps, args.trans);
  } else
    gaps = 1;





  // Read sample sets

  #define ReadSet(set, bmap, dmap, vol) { \
    ReadVolume(set, vol, args.beverbose); \
    crop(vol, args.crop); \
    if ( faceShape(vol.shape()) != sh ) \
      exit_on_error("ReadSet", "Wrong image sizes."); \
    FlatInThread::execute(vol, bmap, dmap, gaps, args.beverbose); \
  }

  Volume ims0; ReadSet(args.ims0, bgs0, dfs0, ims0);
  Volume ims1; ReadSet(args.ims1, bgs1, dfs1, ims1);



  #undef ReadSet

  if (ims0.shape()(0)<2)
    exit_on_error("InputSets", "Less than 2 images in the input volume(s).");
  if (ims0.shape() != ims1.shape())
    exit_on_error("InputSets", "Volumes are of different size.");
  dfs0.free();
  dfs1.free();
  bgs0.free();
  bgs1.free();




  // Form final frames

  const float arc =  args.arc > 1.0  ?  args.arc  :  args.arc * ims0.shape()(0);
  Volume frames;
  FrameFormInThread::execute(frames, ims0, ims1, gaps, arc,
                             args.shift, args.ashift, args.center, args.radFill, args.beverbose);
  crop(frames, args.cropF);
  const int oz = frames.shape()(0);
  const Shape fsh = faceShape(frames.shape());

  SaveImage("fill.tif", frames(0,all,all), true);

  ims0.free();
  ims1.free();

  exit(0);



  // TODO Rings

  // TODO Zinger





  // Phase
  const float ind2b = M_PI * args.d2b * args.dist * args.lambda / ( args.dd * args.dd );
  PhaseInThread::execute(frames, ind2b, args.beverbose);


  // CT
  Volume recs;
  CTinThread::execute(frames, recs, Contrast::PHS, args.filter_type, args.dd);


  // Output

  const float mincon = args.SaveInt ? min(frames) : 0;
  const float maxcon = args.SaveInt ? max(frames) : 0;
  const string sliceformat = mask2format(args.outmask, oz);
  Map cur(faceShape(frames.shape()));
  ProgressBar bar(args.beverbose, "Saving slices", oz );

  for (unsigned slice=0 ; slice < oz ; slice++ ) {
    const Path fileName =  oz == 1  ?  args.outmask : Path(toString(sliceformat, slice));
    cur = frames(slice, all, all);
    if (args.SaveInt)
      SaveImage(fileName, cur, mincon, maxcon);
    else
      SaveImage(fileName, cur);
    bar.update();
  }

  exit(0);


}
