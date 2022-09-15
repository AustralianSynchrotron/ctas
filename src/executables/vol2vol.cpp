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


#include "../common/common.h"
#include "../common/poptmx.h"
#include <algorithm>
#include <string.h>


using namespace std;



/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  deque<ImagePath> images;        ///< input image
  ImagePath outmask;              ///< Name of the output image.
  Crop3 crp;                  ///< Crop input projection image
  Binn3 bnn;                  ///< binning factor
  string slicedesc;       ///< String describing the slices to be sino'ed.
  float mincon;         ///< Black intensity.
  float maxcon;         ///< White intensity.
  bool SaveInt;         ///< Save image as 16-bit integer.
  bool beverbose;             ///< Be verbose flag
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[])
  : outmask ("slice.tif")
  , mincon(NAN)
  , maxcon(NAN)
  , SaveInt(false)
  , beverbose(false)
{


  poptmx::OptionTable table
    ("Reslices 3D volume.",
     "Reads 3D volume from input file(s) and, after applying manipulations saves"
     " the result as the HDF volume or set of 2D images as requested by the output format.");

  table
    .add(poptmx::NOTE, "ARGUMENTS:")
    .add(poptmx::ARGUMENT, &images,    "images", "Input combination of 2D and 3D images.",
         "Either 2D images understood by the IM or HDF5. HDF5 format as follows:\n"
         "    file:dataset[:[slice dimension][slice(s)]]\n" + DimSliceOptionDesc )

    .add(poptmx::NOTE, "OPTIONS:")
    .add(poptmx::OPTION, &outmask, 'o', "output", "Output result mask or filename.",
         "Output filename if output is a single file. Output mask otherwise. " + MaskDesc, outmask)
    .add(poptmx::OPTION, &mincon, 'm', "min", "Pixel value corresponding to black.",
         " All values below this will turn black.", "<minimum>")
    .add(poptmx::OPTION, &maxcon, 'M', "max", "Pixel value corresponding to white.",
         " All values above this will turn white.", "<maximum>")
    .add(poptmx::OPTION, &crp, 'c', "crop", "Crop input volume: " + Crop3OptionDesc, "")
    .add(poptmx::OPTION, &bnn, 'b', "binn", Binn3OptionDesc, "")
    .add(poptmx::OPTION, &slicedesc, 's', "slice", "Slices to be processed.", DimSliceOptionDesc, "<all>")
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

  if ( ! table.count(&images) )
    exit_on_error(command, "No input image given.");

}






class SliceInThread : public InThread {

  const clargs & args;
  ReadVolumeBySlice * ivolRd;
  SaveVolumeBySlice * ovolSv;
  vector<int> indices;

  unsigned bnz;
  Shape ish, osh;
  unsigned isz, osz;
  Crop crp;
  Binn bnn;


  struct CLacc {

    const Shape mish;
    const unsigned bn;
    int odx;
    unsigned cnt;
    CLmem resmem;
    CLmem addmem;
    pthread_mutex_t locker;
    static cl_program binnProgram;
    cl_kernel addKernel;
    cl_kernel divKernel;

    CLacc(Shape _mish, unsigned _bn)
      : mish(_mish)
      , bn(_bn)
      , odx(-1)
      , cnt(0)
      , locker(PTHREAD_MUTEX_INITIALIZER)
    {
      if (!bn)
        throw_bug("CLacc: zero binning. Here must be size.");
      if (bn==1)
        return;
      if (!binnProgram)  {
        const char binnSource[] = {
          #include "../common/binn.cl.includeme"
        };
        binnProgram =
          initProgram( binnSource, sizeof(binnSource), "Binn on OCL" );
        if (!binnProgram)
          throw_error("Summ on CL", "Could not initiate summing program");
      }
      if (bn>1) {
        resmem(clAllocArray<float>(area(mish)));
        addmem(clAllocArray<float>(area(mish), CL_MEM_READ_ONLY) );
      }
      addKernel = createKernel(binnProgram, "addToSecond");
      setArg(addKernel, 0, addmem());
      setArg(addKernel, 1, resmem());
      fillClArray<float>(resmem(), area(mish), 0);
      divKernel = createKernel(binnProgram, "multiplyArray");
      setArg(divKernel, 0, resmem());
      setArg(divKernel, 1, (float)1.0/bn);
    }

    bool addme (Map & nmap) {
      if (bn==1)
        return false;
      if (nmap.shape() != mish)
        throw_error("Sum on CL", "Wrong input image shape.");
      pthread_mutex_lock(&locker);
      blitz2cl(nmap, addmem());
      execKernel(addKernel, area(mish));
      cnt++;
      if (cnt==bn) {
        execKernel(divKernel, area(mish));
        cl2blitz(resmem(), nmap);
        fillClArray<float>(resmem(), area(mish), 0);
        cnt=0;
        odx=-1;
      }
      pthread_mutex_unlock(&locker);
      return cnt;
    }

  };

  unordered_map<pthread_t, ImageProc> rdprocs;
  unordered_map<pthread_t, Map> rdmaps;
  deque<CLacc> accs;


  bool inThread(long int idx) {

    const long int sodx = idx/bnz;
    if ( sodx >= ovolSv->slices() )
      return false;
    if ( find(indices.begin(), indices.end(), sodx) == indices.end() )
      return true;

    const pthread_t me = pthread_self();
    lock();

    if ( ! rdprocs.count(me) ) {
      rdprocs.emplace(piecewise_construct,
                      forward_as_tuple(me),
                      forward_as_tuple(0, crp, bnn, ish));
      //rdprocs.try_emplace(me, 0, crp, bnn, ish);
      rdmaps.emplace(me, osh);
    }
    ImageProc & myrdproc = rdprocs.at(me);
    Map & myrdmap = rdmaps.at(me);

    if (!accs.size())
      accs.emplace_back(osh, bnz);
    auto useacc = accs.begin();
    if (bnz!=1) {
      useacc = accs.end();
      for ( auto accsp  = accs.begin() ; accsp != accs.end() ; accsp++) {
        if (accsp->odx<0)
          useacc = accsp;
        else if (accsp->odx == sodx) {
          useacc = accsp;
          break;
        }
      }
      if (useacc == accs.end()) {
        accs.emplace_back(osh, bnz);
        useacc = accs.end() - 1;
      }
    }
    CLacc & myacc = *useacc;
    myacc.odx = sodx;

    unlock();

    myrdproc.read(*ivolRd, indices[idx], myrdmap);
    if ( ! myacc.addme(myrdmap) )
      ovolSv->save(sodx, myrdmap);

    bar.update();
    return true;

  }

public:

  SliceInThread(const clargs & _args)
    : InThread(_args.beverbose, "processing slices")
    , args(_args)
    , ivolRd(0)
    , ovolSv(0)
  {

    ivolRd = new ReadVolumeBySlice(args.images);
    ish = ivolRd->face();
    isz = ivolRd->slices();
    crp = args.crp;
    bnn = args.bnn;
    bnz = args.bnn.z ? args.bnn.z : isz;
    osh = binn(crop(ish, crp), bnn);
    osz = binn(isz, bnz);
    if ( osh(0) <= 0 || osh(1) <= 0 || osz <=0 )
      throw_error(args.command, "Cropping or binning is larger than the shape of input volume.");
    string sindex = args.slicedesc;
    if (sindex.size() && string("zZ").find(sindex.at(0)) != string::npos)
      sindex.erase(0,1);
    indices = slice_str2vec(sindex, osz);
    ovolSv = new SaveVolumeBySlice(args.outmask, osh, osz
                                   , fisok(args.mincon) ?  args.mincon : 0
                                   , fisok(args.maxcon) ?  args.maxcon : 0);

    bar.setSteps(indices.size()*bnz);

  }

  ~SliceInThread() {
    if (ivolRd)
      delete ivolRd;
    ivolRd = 0;
    if (ovolSv)
      delete ovolSv;
    ovolSv = 0;
  }


};


cl_program SliceInThread::CLacc::binnProgram = 0;




/// \MAIN{projection}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;

  const bool toInt = fisok(args.mincon)  ||  fisok(args.maxcon) || args.SaveInt;
  if (  ( ! args.slicedesc.empty()  &&  string("xXyY").find(args.slicedesc.at(0)) != string::npos )
     || ( toInt  &&  ( ! fisok(args.mincon)  ||  ! fisok(args.maxcon) ) ) ) {
    // Requires whole volume in memory to produce cross-sections or calculate min/max

    Volume ivol;
    ReadVolume(args.images, ivol, args.beverbose);
    crop(ivol,args.crp);
    binn(ivol,args.bnn);

    if (toInt) {
      const float
        mincon  =  fisok(args.mincon) ?  args.mincon  :  min(ivol),
        maxcon  =  fisok(args.maxcon) ?  args.maxcon  :  max(ivol);
      SaveVolume(args.outmask, ivol, args.beverbose, args.slicedesc, mincon, maxcon);
    } else {
      SaveVolume(args.outmask, ivol, args.beverbose, args.slicedesc);
    }

  } else {
    // Can work slice-by-slice.
    SliceInThread factory(args);
    factory.execute();

  }

  exit(0);



}


