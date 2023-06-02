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
#include "../common/projection.h"
#include <algorithm>
#include <string.h>
#include <math.h>
#include <unordered_map>


using namespace std;



/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  ImagePath outmask;
  ImagePath gaps;
  deque<ImagePath> dfs0;
  deque<ImagePath> dfs1;
  deque<ImagePath> bgs0;
  deque<ImagePath> bgs1;
  deque<ImagePath> ims0;
  deque<ImagePath> ims1;
  PointF2D shift;
  float ashift;
  float arc;
  Crop crop;
  Crop cropF;
  float angle;
  Binn bnn;
  float center;
  uint trans;
  bool SaveInt;
  bool beverbose;
  int testme;
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[])
  : outmask("_.tif")
  , shift(0,0)
  , ashift(0.0)
  , arc(180)
  , angle(0)
  , center(0)
  , trans(0)
  , SaveInt(false)
  , beverbose(false)
  , testme(-1)
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
      .add(poptmx::OPTION, &bnn, 0, "binn", BinnOptionDesc, "")
      .add(poptmx::OPTION, &center, 'c', "center", "Rotation center after cropping.", CenterOptionDesc)
      .add(poptmx::OPTION, &trans, 'T', "trans", "Transition area around gaps.",
           "The area of this thickness around the gaps is used to smoothly"
           " interpolate between single (gap) and double signal areas." )
      .add(poptmx::OPTION, &testme, 't', "test", "Tests single projection given.",
           "Outputs tif with the projection as well as processed masks." , "")
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

  auto chck_inSet = [&](deque<ImagePath> & set, const string & role) {
    if ( ! table.count(&set) ) \
      exit_on_error(command, "Missing required input for the " + string(role) + " set.");
  };
  chck_inSet(ims0, "first image");
  chck_inSet(ims1, "second image");
  chck_inSet(bgs0, "first background");
  chck_inSet(bgs0, "second background");

  if ( arc <= 0.0 )
    exit_on_error(command, "CT arc (given by "+table.desc(&arc)+") must be strictly positive.");
  if ( ashift < 0.0 )
    exit_on_error(command, "Angular shift between two sets (given by "+table.desc(&shift)+")"
                           " must be strictly positive.");
  auto restrictTo360 = [](float & ang) { // restricts to [0..360) range
    ang = remainder(ang, 360.0f);
    if (ang<0)
      ang+=360;
  };
  restrictTo360(ashift);
  restrictTo360(angle);

}




class FrameFormInThread : public InThread {

private:

  static const string modname;
  static cl_program formframeProgram;

  Volume & res;
  ReadVolumeBySlice * imrd0;
  ReadVolumeBySlice * imrd1;
  SaveVolumeBySlice * imsv;
  FlatFieldProc canon0;
  FlatFieldProc canon1;
  const clargs & args;

  Shape ish;
  Shape osh;
  int nofProj;
  float step;
  int nshift;
  int cshift;

  CLmem clgaps0;
  CLmem clgaps1;
  CLmem clgapsF;

  struct PerThread {
    CLkernel kernelFormFrame;
    CLkernel kernelEqNoise;
    CLkernel kernelFill;
    CLmem clwA;
    CLmem clwB;
    CLmem clim1;
    CLmem clim0;
    Map rdim;
    Map tim;
    Map crim;

    void prepareme(const Shape & _osh, CLmem & _clgaps) {

      static const string oclsrc = {
        #include "formframe.cl.includeme"
      };
      formframeProgram = initProgram(oclsrc, formframeProgram, modname);
      if (!formframeProgram)
        throw_error(modname, "Failed to compile CL program.");

      kernelFormFrame(formframeProgram , "formframe");
      kernelEqNoise(formframeProgram , "eqnoise");
      kernelFill(formframeProgram , "gapfill");

      clwA(clAllocArray<float>(area(_osh)));
      clwB(clAllocArray<float>(area(_osh)));
      clim1(clAllocArray<float>(area(_osh),CL_MEM_READ_ONLY));
      clim0(clAllocArray<float>(area(_osh),CL_MEM_READ_ONLY));

      kernelFormFrame.setArg(0, clwA() );
      kernelFormFrame.setArg(1, clim0() );
      kernelFormFrame.setArg(2, clim1() );
      kernelFormFrame.setArg(3, _clgaps() );

      kernelEqNoise.setArg(0, (int) _osh(1) );
      kernelEqNoise.setArg(1, (int) _osh(0) );
      kernelEqNoise.setArg(2, clwA() );
      kernelEqNoise.setArg(3, clwB() );
      kernelEqNoise.setArg(4, _clgaps() );

      kernelFill.setArg(0, (int) _osh(1) );
      kernelFill.setArg(1, (int) _osh(0) );
      kernelFill.setArg(2, clwB());
    }
  } ;
  unordered_map<pthread_t,PerThread> perThread;
  unordered_map<pthread_t, FlatFieldProc> ffproc0;
  unordered_map<pthread_t, FlatFieldProc> ffproc1;


  ~FrameFormInThread() {
    if (imsv)
      delete imsv;
    if (imrd0)
      delete imrd0;
    if (imrd1)
      delete imrd1;
  }


  bool inThread(const long int idx) {

    if ( idx >= nofProj )
      return false;
    if (args.testme >= 0  &&  idx != args.testme)
      return true;

    pthread_t me(pthread_self());
    lock();
    if ( ! perThread.count(me) ) { // first run
      perThread[me];
      ffproc0.emplace(me,canon0);
      ffproc1.emplace(me,canon1);
    }
    PerThread & my = perThread.at(me);
    FlatFieldProc & myff0 = ffproc0.at(me);
    FlatFieldProc & myff1 = ffproc1.at(me);
    unlock();
    my.prepareme(osh, clgaps0);

    int id1 = idx;
    bool flp = false;
    if (nshift) {
      id1 += cshift - nshift;
      if ( id1 >= nofProj) {
        id1 -= cshift/2;
        flp=true;
      }
      if ( id1 >= nofProj) { // NOT a mistake
        id1 -= cshift/2;
        flp=false;
      }
    }

    #define getSlice(inp, tidx, dosft, doflp) \
      imrd##inp->read(tidx, my.rdim); \
      if (my.rdim.shape() != ish) \
        throw_error("Frame formation", "Unexpected shape of source " #inp " at index " + toString(tidx) + "." ); \
      myff##inp.process(my.rdim); \
      procImg(my.rdim, my.tim, dosft, doflp); \
      blitz2cl(my.tim, my.clim##inp()); \
      if (args.testme >= 0) \
        SaveImage(args.outmask.dtitle() + "_src_" + string(inp?"sft":"org") + ".tif", my.tim);

    getSlice(0,idx,false,false);
    getSlice(1,id1,true,flp);
    #undef getSlice

    cl_mem & gaps1 = flp ? clgapsF() : clgaps1();
    my.kernelFormFrame.setArg(4, gaps1);
    my.kernelEqNoise.setArg(5, gaps1);

    my.kernelFormFrame.exec(area(osh));
    my.kernelEqNoise.exec(osh);
    my.kernelFill.exec(osh);
    my.kernelFill.exec(osh); // second call to make sure no 0-crosses were in the image

    cl2blitz(my.clwB(), my.tim);
    crop(my.tim, my.crim, args.cropF);
    if (args.testme >= 0) {
      SaveImage(args.outmask.dtitle() + ".tif", my.tim);
      return false;
    } else {
      res(idx, all, all) = my.crim;
      imsv->save(idx, my.crim);
      bar.update();
      return true;
    }

  }


  void procImg(const Map & im, Map & om, bool shft, bool flp){
    om.resize(im.shape());
    om=im;
    Crop tcrop = shft
      ? Crop( -min(0.0f, args.shift.y), -min(0.0f, args.shift.x)
            ,  max(0.0f, args.shift.y),  max(0.0f, args.shift.x))
      : Crop(  max(0.0f, args.shift.y),  max(0.0f, args.shift.x)
            , -min(0.0f, args.shift.y), -min(0.0f, args.shift.x));
    crop(om,tcrop);
    rotate(om, args.angle,0.0);
    crop(om, args.crop);
    const float ccent = 2 * args.center - args.shift.x
                  + args.crop.right - args.crop.left;
    crop(om, Crop(0,  max(0.0f, ccent), 0, -min(0.0f, ccent)));
    binn(om, args.bnn);
    if (flp)
      om.reverseSelf(blitz::secondDim);
  }


  void prepareGaps(Map & _gaps) {

    const float mm = min(_gaps);
    const float MM = max(_gaps);
    if (MM <= 0)
      throw_error("GapsMask", "Mask covers whole image.");
    if (mm==MM) // no _gaps
      return;
    for (ArrIndex i = 0 ; i<_gaps.shape()(0) ; i++)
      for (ArrIndex j = 0 ; j<_gaps.shape()(1) ; j++)
        if (_gaps(i,j)<1.0)
          _gaps(i,j)=0.0 ;

    const Shape ish = _gaps.shape();
    const float step = 1.0 / (args.trans+1);
    Map tmp(_gaps.shape());
    tmp = _gaps;

    for ( int st = 1 ; st <= args.trans ; st++ ) {
      const float fill = step*st;
      for (ArrIndex i = 0 ; i<ish(0) ; i++)
        for (ArrIndex j = 0 ; j<ish(1) ; j++)

          if ( _gaps(i,j) != 1.0 )
            for (ArrIndex ii = i-1 ; ii <= i+1 ; ii++)
              for (ArrIndex jj = j-1 ; jj <= j+1 ; jj++)

                if (     ii >= 0 && ii < ish(0) && jj >= 0 && jj < ish(1)
                  &&  _gaps(ii,jj) == 1.0 )
                  tmp(ii,jj) = fill;
      _gaps = tmp;
    }

  }


  size_t countZeroes(const Map & im) {
    size_t cnt=0;
    for (ArrIndex i = 0 ; i<im.shape()(0) ; i++)
      for (ArrIndex j = 0 ; j<im.shape()(1) ; j++)
        if (im(i,j)==0.0)
          cnt++;
    return cnt;
  }

public:

  FrameFormInThread( const Map & bgar0, const Map & bgar1, const Map & dfar0, const Map & dfar1
                   , const Map & gaps, const clargs & _args, Volume & _res)
    : InThread( _args.beverbose && _args.testme < 0, "performing frame formation")
    , res(_res)
    , imrd0(0)
    , imrd1(0)
    , imsv(0)
    , canon0(bgar0, dfar0, Map(), gaps)
    , canon1(bgar1, dfar1, Map(), gaps)
    , args(_args)
  {

    imrd0 = new ReadVolumeBySlice(args.ims0);
    imrd1 = new ReadVolumeBySlice(args.ims1);
    nofProj = imrd0->slices();
    if (nofProj<2)
      throw_error("InputSets", "Less than 2 images in the input volume(s).");
    if (imrd0->slices() != imrd1->slices())
      throw_error("InputSets", "Input volumes are of different sizes: "
                  + toString(imrd0->slices()) + " and " + toString(imrd1->slices()) + ".");
    bar.setSteps(nofProj);
    bar.start();

    Map gapst;
    procImg(gaps, gapst, false, false);
    prepareGaps(gapst);
    Map test_map;
    const ImagePath test_file = args.outmask.dtitle();
    if (args.testme >= 0) {
      SaveImage(test_file + "_mask_org.tif", gapst);
      test_map.resize(gapst.shape());
      test_map = gapst;
    }
    clgaps0(blitz2cl(gapst, CL_MEM_READ_ONLY));
    procImg(gaps, gapst, true, false);
    prepareGaps(gapst);
    if (args.testme >= 0) {
      SaveImage(test_file + "_mask_sft.tif", gapst);
      test_map += gapst;
    }
    clgaps1(blitz2cl(gapst, CL_MEM_READ_ONLY));
    procImg(gaps, gapst, true, true);
    prepareGaps(gapst);
    if (args.testme >= 0) {
      SaveImage(test_file + "_mask_flp.tif", gapst);
      test_map += gapst;
      SaveImage(test_file + "_mask_all.tif", test_map);
    }
    clgapsF(blitz2cl(gapst, CL_MEM_READ_ONLY));


    ish = gaps.shape();
    osh = Shape( gapst.shape()(0) - args.cropF.top  - args.cropF.bottom
               , gapst.shape()(1) - args.cropF.left - args.cropF.right) ;
    step = args.arc < 1.0  ?  args.arc  :  args.arc / (nofProj-1);
    nshift = args.ashift/step;
    cshift = 360/step;

    if (args.testme >= nofProj)
      throw_error("Test", toString( "Requested test %i is beyond number of input slices %i."
                                  , args.testme, nofProj));
    if (args.testme >= 0)
      return;

    res.resize(nofProj, osh(0), osh(1));
    imsv = new SaveVolumeBySlice(args.outmask, osh, nofProj);

  }


  static void execute( const Map & _bgar0, const Map & _bgar1
                     , const Map & _dfar0, const Map & _dfar1
                     , const Map & _gaps, const clargs & _args, Volume & _res) {
    FrameFormInThread( _bgar0, _bgar1, _dfar0, _dfar1, _gaps,  _args, _res)
        .InThread::execute();
  }

};
const string FrameFormInThread::modname = "form frame";
cl_program FrameFormInThread::formframeProgram = 0;







/// \MAIN{projection}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;

  const Shape ish = ImageSizes(args.ims0.at(0));

  // Read DFs, BGs, GapMask

  #define ReadSumSet( set, res ) { \
    if ( ! set.empty() ) {\
      Volume tmp; \
      ReadVolume(set, tmp, args.beverbose); \
      if ( faceShape(tmp.shape()) != ish ) \
        exit_on_error("ReadAux", "Wrong image sizes."); \
      binn(tmp, Binn3(1,1,0)); \
      res = tmp(0, all, all); \
    } \
  }
  Map dfs0(ish); ReadSumSet(args.dfs0, dfs0);
  Map dfs1(ish); ReadSumSet(args.dfs1, dfs1);
  Map bgs0(ish); ReadSumSet(args.bgs0, bgs0);
  Map bgs1(ish); ReadSumSet(args.bgs1, bgs1);
  #undef ReadSumSet

  Map gaps(ish);
  if ( ! args.gaps.empty() ) {
    ReadImage(args.gaps, gaps, ish);
    for (ArrIndex i = 0 ; i<gaps.shape()(0) ; i++)
      for (ArrIndex j = 0 ; j<gaps.shape()(1) ; j++)
        if (gaps(i,j)!=0.0)
          gaps(i,j) = 1.0;
  } else
    gaps = 1.0;

  Volume frames;
  FrameFormInThread::execute(bgs0, bgs1, dfs0, dfs1, gaps, args, frames);


}
