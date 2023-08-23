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

#include <unistd.h>
#include "../common/projection.h"
#include "../common/poptmx.h"


using namespace std;


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  deque< deque<ImagePath> > images;        ///< images to combine
  deque<ImagePath> bgs;        ///< Array of the background images.
  deque<ImagePath> dfs;        ///< Array of the dark field images.
  deque<ImagePath> dgs;        ///< Array of the dark field images for backgrounds.
  deque<ImagePath> mss;        ///< Mask Array.
  uint denoiseRad;
  float denoiseThr;
  deque<ImagePath> out_names;              ///< Name of the output image.
  string out_range;
  N_StitchRules st;
  int zbinn;
  int testMe;          ///< Prefix to save interim results
  bool doTest;
  bool beverbose;             ///< Be verbose flag
  /// \CLARGSF
  clargs(int argc, char *argv[]);
  poptmx::OptionTable table;
};


clargs::
clargs(int argc, char *argv[])
  : table("Combines multiple images to form single projection.",
          "Transforms and stitches components of the composite image(s)."
          " Transformations are applied in the following order: rotate, crop, binning." )
  , denoiseRad(0)
  , denoiseThr(0.0)
  , zbinn(1)
  , testMe(-1)
  , doTest(false)
  , beverbose(false)
{

  auto chkNofIns = [&](auto & ims, const string & lbl) {
    if (ims.size() && ims.size() != 1 && ims.size() != st.nofIn)
      exit_on_error(command, "Number of " +lbl+ " given by " + table.desc(&ims) + " option "
                             " is neither 0, 1, nor the number of inputs (" + toString(st.nofIn) +  ").");
  };
  const string multiIN = "If only one provided, then it will be used for all images."
                         " Otherwise must be equal to the number of input images.";
  deque<ImagePath> iimages;

  table
    .add(poptmx::NOTE, "ARGUMENTS:")
    .add(poptmx::ARGUMENT, &iimages, "images"
         , "Input 2D or 3D images. If '-', then reads image from the stdin."
         , "All images must be of the same rank (2D or 3D). HDF5 format:\n"
           "    file:dataset[:[slice dimension][slice(s)]]\n" + DimSliceOptionDesc)

    .add(poptmx::OPTION, &bgs, 'B', "bg", "Background image(s)", "")
    .add(poptmx::OPTION, &dfs, 'D', "df", "Dark field image(s)", "")
    .add(poptmx::OPTION, &dgs, 'F', "dg", "Dark field image(s) for backgrounds", "")
    .add(poptmx::OPTION, &mss, 'M', "mask", "Mask image",
         "Image where values are weights of corresponding pixels in superimposition operations."
         " F.e. 0 values exclude corresponding or affected pixels from further use.")
    .add(poptmx::NOTE, "OPTIONS:")
    .add(poptmx::OPTION, &out_names, 'o', "output", "Output image(s) mask."
         , "If more than one is provided, then equal number of final crops must be given.", "")
    .add(poptmx::OPTION, &out_range, 'O', "select", "Slices to process. All if not used.",
         SliceOptionDesc + ". Only makes sense in multiple projections.", "all")
    .add(poptmx::OPTION, &st.angles,'r', "rotate", "Rotation angle(s).", multiIN)
    .add(poptmx::OPTION, &st.binns, 'b', "binn", "Binn factor(s)."
         , multiIN + " " + BinnOptionDesc)
    .add(poptmx::OPTION, &st.crops, 'c', "crop", "Crop input image(s)."
         , multiIN + " " + CropOptionDesc)
    .add(poptmx::OPTION, &st.origins, 'g', "origin", "Origin of the image relative to the first one.",
         "Number of origins must be one less than the number of input images.")
    .add(poptmx::OPTION, &st.outCrops, 'C', "crop-final", "Crop final image(s)."
         , "If more than one output image is set (via "+table.desc(&out_names)+" options),"
           " then the number of these crops must be equal to the number of output image masks."
           " Otherwise can be ommited (no crop). "+ CropOptionDesc)
    .add(poptmx::OPTION, &zbinn, 'z', "zinn", "Binning over multiple input prrojections.",
         "If zero, binns all outputs down to a single image. Ignored for tests.")
    .add(poptmx::OPTION, &st.edge, 'e', "edge", "Thickness in pixels of edge transition.",
         "Smoothly reduces the weight of pixels around the mask edges (0 values in mask)"
         " to produce seamless image stitching." )
    .add(poptmx::OPTION, &denoiseRad, 'n', "noise-rad", "Radius for noise reducing algorithm.",
         "Radius of the local region used to calculate average to replace the pixel value." )
    .add(poptmx::OPTION, &denoiseThr, 'N', "noise-thr", "Noise threshold.",
         "If positive, then gives maximum absolute deviation from the average;"
         " if negative, then negated value is maximum relative deviation.")
    .add(poptmx::OPTION, &testMe, 't', "test", "Produces interim images.",
         "Uses output name with suffixes to store results. In case of multiple projections provides"
         " slice index to test; ignored for single image mode.")
    .add_standard_options(&beverbose)
    .add(poptmx::MAN, "SEE ALSO:", SeeAlsoList);


  if ( ! table.parse(argc,argv) )
    exit(0);
  if ( ! table.count() ) {
    table.usage();
    exit(0);

  }
  command = table.name();

  st.nofIn = table.count(&iimages);
  if (!st.nofIn)
    exit_on_error(command, "No input images provided.");
  images.resize(st.nofIn);
  deque<uint> toRead;
  for ( int curI = 0 ; curI < st.nofIn ; curI++ )
    if ( iimages.at(curI) != "-" )
      images.at(curI).push_back(iimages.at(curI));
    else
      toRead.push_back(curI);
  if (toRead.size()) {
    if (isatty(fileno(stdin)))
      exit_on_error(command, "Could not read from stdin " + toString(toRead.size()) +
                             " input image(s) as requested by '-' name." );
    uint incounter=0;
    string inputline;
    while ( ! cin.eof() && getline(cin, inputline) ) {
      trim(inputline);
      if (inputline.size() && inputline.at(0) != '#' ) {
        deque<string> inputdeque = split(inputline, " ");
        if (inputdeque.size() != toRead.size())
          exit_on_error(command, "Line "+toString(incounter)+" of stdin input contains "
                                 +toString(inputdeque.size())+" images, while "
                                 +toString(toRead.size())+" are expected.");
        for (int curJ = 0 ; curJ < toRead.size() ; curJ++ )
          images.at(toRead[curJ]).push_back(inputdeque.at(curJ));
      }
    }
  }

  chkNofIns(bgs, "background images");
  chkNofIns(dfs, "darkfield images");
  chkNofIns(dgs, "darkground images");
  chkNofIns(mss, "mask images");
  if (dgs.size() && ! bgs.size())
    exit_on_error(command, "No background images (" + table.desc(&bgs) + ") for"
                           " provided darkgrounds (" + table.desc(&dgs) + ").");
  if ( ! mss.size()  &&  ! bgs.size()  &&  st.edge )
    throw_error(command, "No background (" + table.desc(&bgs) + ") or mask"
                         " (" + table.desc(&mss) + ") images provided while"
                         " mask operation (" + table.desc(&st.edge) + ") is requested.");

  const uint nofOuts = table.count(&out_names);
  const uint nofCrops = table.count(&st.outCrops);
  if ( ! nofOuts )
    exit_on_error(command, "No output name provided. Use option " + table.desc(&out_names) + ".");
  else if ( nofOuts == 1  &&  nofCrops > 1  )
    exit_on_error(command, "With single output, only a single " + table.desc(&st.outCrops) +
                           " option is allowed, while "+toString(nofCrops)+" are provided.");
  else if ( nofOuts != 1  &&  nofCrops != nofOuts )
    exit_on_error(command, "With multiple ("+toString(nofOuts)+") outputs, equal number of " +
                           table.desc(&st.outCrops) + " must be provided instead of "+toString(nofCrops)+".");
  doTest = table.count(&testMe);

  for (float & ang : st.angles)
    ang *= M_PI/180;
  chkNofIns(st.angles, "rotations");
  chkNofIns(st.crops, "crops");
  chkNofIns(st.binns, "binns");
  if ( st.origins.size() != st.nofIn - 1 )
    exit_on_error(command, "Number of image origins given by " + table.desc(&st.origins) + " option "
                           " must be one less than the number of inputs (" + toString(st.nofIn) + ")."
                           " Instead " + toString(st.origins.size()) + " are provided.");
  st.origins.push_front(PointF<2>()); // first image is always at 0,0.

  if ( table.count(&denoiseThr) && ! table.count(&denoiseRad) )
    warn(command, "Setting noise threshold via " + table.desc(&denoiseThr) + " option "
         " without noise rad (option " + table.desc(&denoiseRad) + ") makes no sense.");

}


namespace blitz {

static inline float denan(float x){ return isnormal(x) ? x : 0.0 ;}
BZ_DECLARE_FUNCTION(denan);

static inline float invert(float x){ return x==0.0 ? 0.0 : 1.0/x ;}
BZ_DECLARE_FUNCTION(invert);

}




static const Denoiser & curDnz(const deque<Denoiser> & dnzs, int cur) {
  if (dnzs.size()==1)
    return dnzs[0];
  else if (dnzs.size() > cur)
    return dnzs[cur];
  throw_error("select denoiser", "Empty or incomplete list of denoisers.");
  return dnzs[0]; // just to get rid of no-return warning
}


class ProjInThread : public InThread {

  deque<ReadVolumeBySlice> & allInRd;
  deque<SaveVolumeBySlice> & allOutSv;
  const deque<Denoiser> & dnsr;
  const ProcProj & proc;
  const deque<int> & projes;
  const int zbinn;

  unordered_map<pthread_t, deque<Denoiser>* > dnsrs;
  unordered_map<pthread_t, ProcProj*> procs;
  unordered_map<pthread_t, deque<Map>* > allInMaps;
  deque< pair< pthread_mutex_t, list<BinnProc::Accumulator> > > accs;


  bool inThread(long int idx) {

    const long int sodx =  idx/zbinn + ( zbinn > 0 ? 0 : allOutSv[0].slices() - 1 ) ;
    if (sodx < 0 || sodx >= allOutSv[0].slices() || idx >= allInRd[0].slices() )
      return false;
    if ( find(projes.begin(), projes.end(), sodx) == projes.end() )
      return true;

    const pthread_t me = pthread_self();
    if ( ! procs.count(me) ) { // first call
      deque<Denoiser> * ednsr = new deque<Denoiser>(dnsr);
      ProcProj* eproc = new ProcProj(proc);
      deque<Map> * eAllInMaps = new deque<Map>(allInRd.size());
      lock();
      dnsrs.emplace(me, ednsr);
      procs.emplace(me, eproc);
      allInMaps.emplace(me, eAllInMaps);
      unlock();
    }
    lock();
    deque<Denoiser> & myDnsr = *dnsrs.at(me);
    ProcProj & myProc = *procs.at(me);
    deque<Map> & myAllIn = *allInMaps.at(me);
    unlock();

    try {
      for (ssize_t curI = 0  ;  curI<allInRd.size()  ;  curI++ ) {
        allInRd[curI].readTo(idx, myAllIn[curI]);
        curDnz(myDnsr, curI).proc(myAllIn[curI]);
      }
      deque<Map> & myRes = myProc.process(myAllIn);
      if (zbinn == 1)
        for (int curO = 0  ;  curO<allOutSv.size()  ;  curO++ )
          allOutSv[curO].save(sodx, myRes[curO]);
      else {
        for (int curO = 0  ;  curO<allOutSv.size()  ;  curO++ ) {
          pthread_mutex_lock(&accs[curO].first);
          list<BinnProc::Accumulator> & curAccs = accs[curO].second;
          Map & curMyRes = myRes[curO];
          if (!curAccs.size())
            curAccs.emplace_back(curMyRes.shape(), zbinn);
          auto useacc = curAccs.end();
          for ( auto accsp  = curAccs.begin() ; accsp != curAccs.end() ; accsp++) {
            if (useacc == curAccs.end() && accsp->index()<0 ) { // found free
              useacc = accsp;
              useacc->index(sodx); // reserve this acc, but keep searching
            } else if (accsp->index() == sodx) {  // found existing
              if ( useacc != curAccs.end() ) // if was reserved
                useacc->index(-1); // release reservation
              useacc = accsp;
              break;
            }
          }
          if (useacc == curAccs.end()) { // nothing found: create new
            curAccs.emplace_back(curMyRes.shape(), zbinn);
            useacc = --curAccs.end();
          }
          BinnProc::Accumulator & myacc = *useacc;
          myacc.index(sodx);
          pthread_mutex_unlock(&accs[curO].first);
          if (myacc.addme(curMyRes))
            allOutSv[curO].save(sodx, curMyRes);
        }
      }
    } catch (...) {
      warn("form projection", toString("Failed processing input index %i, for output slice %i.", idx, sodx));
    }

    bar.update();
    return true;

  }

public:

  ProjInThread(deque<ReadVolumeBySlice> & _allInRd, deque<SaveVolumeBySlice> & _outSave
              , const deque<Denoiser> & _dnsr, const ProcProj & _proc
              , const deque<int> & _projes, const int _zbinn, bool verbose)
    : InThread(verbose, "processing projections")
    , dnsr(_dnsr)
    , proc(_proc)
    , projes(_projes)
    , allInRd(_allInRd)
    , allOutSv(_outSave)
    , zbinn(_zbinn ? _zbinn : allInRd.at(0).slices())
  {
    bar.setSteps(zbinn*projes.size());
    if (zbinn != 1)
      for (int curO = 0  ;  curO<allOutSv.size()  ;  curO++ )
        accs.emplace_back( pthread_mutex_t(PTHREAD_MUTEX_INITIALIZER), list<BinnProc::Accumulator>() );
  }

  ~ProjInThread() {
    auto dlnl = [](auto pntr) { if (pntr) { delete pntr; pntr=0; } } ;
    for (auto celem : dnsrs)       dlnl(celem.second) ;
    for (auto celem : procs)       dlnl(celem.second) ;
    for (auto celem : allInMaps)   dlnl(celem.second) ;
    for (auto & accPair : accs)
      pthread_mutex_destroy(&accPair.first);
  }

  void execute(int nThreads=0) { // To deals with possible last incomplete slice
    InThread::execute(nThreads);
    if (abs(zbinn)==1)
      return;
    Map res;
    for (int curO = 0  ;  curO<allOutSv.size()  ;  curO++ ) {
      list<BinnProc::Accumulator> & curAccs = accs[curO].second;
      for ( auto & acc : curAccs ) {
        const int odx = acc.getme(res);
        if ( odx >= 0 )
          allOutSv[curO].save(odx, res);
      }
    }
    accs.clear();
    bar.done();
  }



};




/// \MAIN{projection}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;
  const int nofIn = args.images.size();
  const deque< Shape<2> > ishs ( [&args](){
    deque< Shape<2> > toRet;
    for (auto & imagePath : args.images.at(0) )
      toRet.push_back(ImageSizes(imagePath.repr()));
    return toRet;
  }() );

  // Prepare read factories
  deque<ReadVolumeBySlice> allInRd(nofIn);
  for ( int curI = 0 ; curI < nofIn ; curI++) {
    allInRd.at(curI).add(args.images.at(curI));
    int cSls = allInRd.at(curI).slices();
    if (!cSls)
      exit_on_error(args.command, "No images in input "+ toString(curI) +".");
    if (curI && allInRd.at(0).slices() != cSls)
      exit_on_error(args.command, "Not matching slices in input "+ toString(curI) +".");
  }
  const int nofProj = allInRd.at(0).slices();
  const int nofOuts = binnOne(nofProj, args.zbinn);
  if ( args.testMe >= nofOuts )
    exit_on_error(args.command, "Requested test is beyond number of projections.");
  const deque<int> projes = slice_str2vec(args.out_range, nofOuts);
  if (!projes.size())
    exit_on_error(args.command, "Given range \"" + args.out_range + "\""
                                " is beyond output slices "+toString(nofOuts)+".");

  // only print output sizes and exit
  if (args.doTest && args.testMe < 0) {
    vector<Shape<2>> oshs(N_ProcProj::shapes(args.st, ishs));
    if (oshs.empty())
      exit_on_error(args.command, "No output. Check your input and stitching parameters.");
    for(auto osh : oshs)
      cout << nofOuts << " " << osh(0) << " " << osh(1) << "\n";
    exit(0);
  }

  // Read auxiliary images
  auto rdAux = [&ishs](const deque<ImagePath> & comp) {
    deque<Map> toRet(comp.size());
    for ( int curf = 0 ; curf < comp.size() ; curf++) \
      ReadImage(comp[curf].repr(), toRet[curf], ishs[curf]);
    return toRet;
  };
  const deque<Map> bgas(rdAux(args.bgs));
  const deque<Map> dfas(rdAux(args.dfs));
  const deque<Map> dgas(rdAux(args.dgs));
  const deque<Map> msas(rdAux(args.mss));

  /*
  // normalize masks
  for (int curm=0 ; curm<msas.size() ; curm++) {
    Map & msk = msas[curm];
    const float vmin = min(msk);
    const float delta = max(msk) - vmin;
    if (delta==0.0)
      msk = 1.0;
    else
      msk = (msk-vmin)/delta;
  }

  // Construct masks from bgs and df/gs if not provided but required by other parameters.
  if ( ! msas.size()  &&  args.st.edge ) {
    deque<Map> & bgdf = dgas.size() ? dgas : dfas;
    const int mssz = max(bgas.size(), bgdf.size());
    msas.resize(mssz);
    for (int curms=0 ; curms<mssz ; curms++) {
      Map & mscur = msas[curms];
      mscur.resize(ish);
      Map bgcur = bgas[bgas.size() > curms ? curms : 0];
      mscur = bgcur;
      if (bgdf.size())
        mscur -= bgdf[bgdf.size() > curms ? curms : 0];
      for (ssize_t ycur = 0 ; ycur < ish(0) ; ycur++ ) {
        for (ssize_t xcur = 0 ; xcur < ish(1) ; xcur++ ) {
          float val = mscur(ycur,xcur);
          mscur(ycur,xcur) = ( val <= 0.0 || ! fisok(val) ) ? 0 : 1.0;
        }
      }
    }
  }

  // create canonical denoiser(s)
  deque<Denoiser> canonDZs;
  if (!msas.size())
    canonDZs.emplace_back(ish, (int) args.denoiseRad, args.denoiseThr);
  else if (msas.size()==1)
    canonDZs.emplace_back(ish, (int) args.denoiseRad, args.denoiseThr, msas[0]);
  else
    for ( int curI = 0 ; curI < nofIn ; curI++)
      canonDZs.emplace_back(ish, (int) args.denoiseRad, args.denoiseThr, msas[curI]);

  // denoise bg and dg
  for (int curg=0 ; curg<bgas.size() ; curg++)
    curDnz(canonDZs, curg).proc(bgas[curg]);
  for (int curg=0 ; curg<dgas.size() ; curg++)
    curDnz(canonDZs, curg).proc(dgas[curg]);

  // Prepare canonical projection processor
  const string testFormat = [&](){
    if (args.testMe < 0)
      return string();
    string toRet = args.out_name.dtitle();
    if (toRet.find('@') != string::npos)
      toRet = toString(mask2format(toRet, nofProj), args.testMe);
    toRet += "%s";
    return toRet;
  }();
  ProcProj canonPP(args.st, ish, bgas, dfas, dgas, msas, toString(testFormat, "_mask.tif"));

  // Prepare saving factories
  const std::vector<Shape<2>> & outShapes = canonPP.outputShapes();
  const size_t nofSplts = outShapes.size();
  const string spformat = mask2format("_split@", nofSplts);
  deque<SaveVolumeBySlice> allOutSv;
  for (int curSplt = 0 ; curSplt < nofSplts ; curSplt++) {
    ImagePath filedescind = args.out_name;
    if (args.testMe >= 0)
      filedescind = args.out_name.dtitle() + ".tif";
    if (nofSplts > 1)
      filedescind =   filedescind.dtitle() + toString(spformat, curSplt)
                    + filedescind.ext() + filedescind.desc();
    allOutSv.emplace_back(filedescind, Shape<3>(nofOuts, outShapes[curSplt](0), outShapes[curSplt](1)) );
  }

  // Test or process and save
  if ( args.testMe >= 0 ) { // test
    deque<Map> allIn;
    for ( ssize_t curI = 0 ; curI < nofIn ; curI++) {
      allIn.emplace_back(ish);
      const int trd = args.zbinn * ( args.testMe >= 0  ?  args.testMe  :  projes[0] );
      allInRd[curI].readTo(trd, allIn[curI]);
      curDnz(canonDZs, curI).proc(allIn.back());
    }
    const deque<Map> & allOut = canonPP.process(allIn, toString(testFormat, ".tif"));
    for (int curSplt = 0 ; curSplt < nofSplts ; curSplt++) {
      const Map & cOut = allOut[curSplt];
      const int svIdx = args.zbinn ? args.testMe / args.zbinn : 0;
      ImagePath svPath = allOutSv[curSplt].save(svIdx, cOut);
      if (args.beverbose)
        cout << nofOuts << " " << cOut.shape()(0) << " " << cOut.shape()(1) << " " << svPath << "\n";
    }
  } else // finally process
    ProjInThread(allInRd, allOutSv, canonDZs, canonPP, projes, args.zbinn, args.beverbose)
        .execute();

  /**/

  exit(0);

}

