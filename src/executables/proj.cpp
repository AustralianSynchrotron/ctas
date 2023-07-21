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
  ImagePath out_name;              ///< Name of the output image.
  string out_range;
  StitchRules st;
  int zbinn;
  int testMe;          ///< Prefix to save interim results
  bool beverbose;             ///< Be verbose flag
  /// \CLARGSF
  clargs(int argc, char *argv[]);
  poptmx::OptionTable table;
};


clargs::
clargs(int argc, char *argv[])
  : table("Combines multiple images to form the projection.",
          "Transforms and stitches portions of the projection from the complex CT experiment"
          " which may include 2D tiling and 180-deg flip."
          " Transformations are applied in the following order: rotate, crop, binning." )
  , denoiseRad(0)
  , denoiseThr(0.0)
  , zbinn(1)
  , testMe(-1)
  , beverbose(false)
{

  deque<ImagePath> iimages;

  table
    .add(poptmx::NOTE, "ARGUMENTS:")
    .add(poptmx::ARGUMENT, &iimages, "images", "Input 2D or 3D images.",
         "All images must be of the same rank (2D or 3D). HDF5 format:\n"
         "    file:dataset[:[slice dimension][slice(s)]]\n" + DimSliceOptionDesc)

    .add(poptmx::NOTE, "OPTIONS:")
    .add(poptmx::OPTION, &out_name, 'o', "output", "Output image.", "", out_name)
    .add(poptmx::OPTION, &out_range, 'O', "select", "Slices to process. All if not used.",
         SliceOptionDesc + ". Only makes sense in multiple projections.", "all")
    .add(poptmx::OPTION, &st.crp, 'c', "crop", "Crop input images.", CropOptionDesc)
    .add(poptmx::OPTION, &st.fcrp, 'C', "crop-final", "Crop final image.", CropOptionDesc)
    .add(poptmx::OPTION, &st.bnn, 'b', "binn", "Binn factor(s).", BinnOptionDesc)
    .add(poptmx::OPTION, &zbinn, 'z', "zinn", "Binning over multiple input prrojections.",
         "If zero binn binns all outputs. Ignored for tests.")
    .add(poptmx::OPTION, &st.angle,'r', "rotate", "Rotation angle.", "")
    .add(poptmx::OPTION, &st.origin1, 'g', "origin", "Origin of the image in the first stitch.",
         "Position of the next image origin (top left corner) on the current image.")
    .add(poptmx::OPTION, &st.origin2, 'G', "second-origin", "Origin of the image in the second stitch.",
         "Position of the next image origin (top left corner) on the current image in the second order stitch.")
    .add(poptmx::OPTION, &st.origin2size, 'S', "second-size", "Number of imasges in the second stitch.",
         "Required if and only if the second stitch is requested.")
    .add(poptmx::OPTION, &st.originF, 'f', "flip-origin", "Origin of the flipped portion of the image.",
         "If used, makes second half of the input images to be assigned to the flipped portion."
         " Requires even number of input images.")
    .add(poptmx::OPTION, &st.splits, 's', "split", "Split point(s)",
         "Final image can be split into sub-images to put different portions of it apart as independent"
         " files, for example to separate samples. By default splitting happens horizontally,"
         " but if the vertical split is needed, just add a 0 split point.")
    .add(poptmx::OPTION, &bgs, 'B', "bg", "Background image(s)", "")
    .add(poptmx::OPTION, &dfs, 'D', "df", "Dark field image(s)", "")
    .add(poptmx::OPTION, &dgs, 'F', "dg", "Dark field image(s) for backgrounds", "")
    .add(poptmx::OPTION, &mss, 'M', "mask", "Mask image",
         "Image where values are weights of corresponding pixels in superimposition operations."
         " F.e. 0 values exclude corresponding or affected pixels from further use.")
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

  int tiledImages = table.count(&iimages);
  if (tiledImages) {
    images.resize(tiledImages);
    for ( int curI = 0 ; curI < tiledImages ; curI++ )
      images.at(curI).push_back(iimages.at(curI));
  }
  if (!isatty(fileno(stdin))) { // read from pipe
    string inputline;
    while ( ! cin.eof() && getline(cin, inputline) ) {
      trim(inputline);
      if (inputline.size() && inputline.at(0) != '#' ) {
        deque<string> inputdeque = split(inputline, " ");
        if (!tiledImages) {
          tiledImages = inputdeque.size();
          images.resize(tiledImages);
        } else if (inputdeque.size() != tiledImages) {
          exit_on_error(command, "Inconsistent number of input images given in stdin.");
        }
        for ( int curI = 0 ; curI < tiledImages ; curI++ )
          images.at(curI).push_back(inputdeque.at(curI));
      }
    }
  }
  if ( ! tiledImages )
    exit_on_error(command, "No input images given.");
  st.nofIn = tiledImages;

  auto chkNofIns = [&](const deque<ImagePath> & ims, const string & lbl) {
    if (ims.size() && ims.size() != 1 && ims.size() != st.nofIn) \
      exit_on_error(command, "Number of " +lbl+ " images given by " + table.desc(&ims) + " option " \
                             " is neither 0, 1, nor the number of inputs (" + toString(st.nofIn) +  ").");
  };
  chkNofIns(bgs, "background");
  chkNofIns(dfs, "darkfield");
  chkNofIns(dgs, "darkground");
  chkNofIns(mss, "mask");

  if (dgs.size() && ! bgs.size())
    exit_on_error(command, "No background images (" + table.desc(&bgs) + ") for provided darkgrounds (" + table.desc(&dgs) + ").");
  if ( ! mss.size()  &&  ! bgs.size()  &&  st.edge )
    throw_error(command, "No background (" + table.desc(&bgs) + ") or mask (" + table.desc(&mss) + ") images provided while"
                         " mask operation (" + table.desc(&st.edge) + ") is requested.");

  if ( ! table.count(&out_name) )
    exit_on_error(command, "No output name provided. Use option " + table.desc(&out_name) + ".");
  if (table.count(&testMe) && testMe<0)
    exit_on_error(command, "Negative test projection given with " + table.desc(&testMe) + " option.");

  if ( table.count(&st.originF) ) {
    if ( tiledImages % 2 )
      exit_on_error(command, string () +
        "Use of the " + table.desc(&st.originF) + " option requires even number of input images"
        " (while " + toString(tiledImages) + " counted).");
    tiledImages /= 2;
  }
  if ( table.count(&st.origin2) > table.count(&st.origin1) )
    exit_on_error(command, string () +
      "Options " + table.desc(&st.origin2) + " requires also " + table.desc(&st.origin1) + " option.");
  if ( table.count(&st.origin2) > table.count(&st.origin2size) )
    exit_on_error(command, string () +
      "Options " + table.desc(&st.origin2) + " requires also " + table.desc(&st.origin2size) + " option.");
  if ( table.count(&st.origin2size) ) {
    if ( ! table.count(&st.origin2) && ! table.count(&testMe) )
      exit_on_error(command, string () +
        "Options " + table.desc(&st.origin2size) + " must be used with either "
        + table.desc(&st.origin2) + " or " + table.desc(&testMe) + " options.");
    if ( st.origin2size  < 2 )
      exit_on_error(command, string () +
        "Requested second stitch size (" + toString(st.origin2size) + ") is less than 2.");
    if ( tiledImages % st.origin2size )
      exit_on_error(command, string () +
        "Total number of tiled images (" + toString(tiledImages) + ")"
        " is not a multiple of the second stitch size (" + toString(st.origin2size) + ")"
        " given by " + table.desc(&st.origin2size) + " option.");
  }

  if ( table.count(&denoiseThr) && ! table.count(&denoiseRad) )
    warn(command, "Setting noise threshold via " + table.desc(&denoiseThr) + " option "
         " without noise rad (option " + table.desc(&denoiseRad) + ") makes no sense.");

  sort(st.splits.begin(), st.splits.end());
  unique(st.splits.begin(), st.splits.end());
  if ( st.splits.size() == 1 && st.splits.at(0) == 0 )
    exit_on_error(command, "The list of splits contains only 0 (marking vertical splits).");

  st.angle *= M_PI/180;
  st.flip=table.count(&st.originF);
  st.origin1size = st.nofIn / st.origin2size / (st.flip ? 2 : 1) ;
  if (st.origin1size<1)
    exit_on_error(command, string () +
      "Number of input images (" + toString(st.nofIn) + ") is not sufficient for given "
      + table.desc(&st.origin2size) + "(" + toString(st.origin2size) + ")" +
      (st.flip ? " and/or " + table.desc(&st.flip) : "") + "." );

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

  // Class to accumulate projections in binning Z axis.
  struct Accumulator {

    Map storage;
    const int bn;
    int odx;
    int cnt;
    pthread_mutex_t * locker;

    Accumulator(Shape<2> sh, int _bn)
      : bn(_bn)
      , odx(-1)
      , cnt(0)
      , locker(0)
    {
      if (!bn)
        throw_bug("Z-binning. Binning can't be 0.");
      if (bn==1)
        return;
      storage.resize(sh);
      locker = new pthread_mutex_t(PTHREAD_MUTEX_INITIALIZER);
    }

    ~Accumulator() {
      if (!locker)
        return;
      if (cnt)
        warn("Z-binning", "Unfinished accumulation on slice "+toString(odx)+".");
      pthread_mutex_destroy(locker);
      delete locker;
      locker = 0;
    }

    // returns true if accamulation has filled
    bool addme (Map & nmap) {
      if (bn==1)
        return true;
      if (nmap.shape() != storage.shape())
        throw_error("Z-binning", "Wrong input image shape.");
      pthread_mutex_lock(locker);
      cnt++;
      if (cnt==1) // first map
        storage = nmap;
      else if (cnt == bn) { // last map
        nmap = (nmap + storage) / bn;
        cnt = 0;
        odx = -1;
      } else
        storage += nmap;
      const bool toRet = (cnt==0);
      pthread_mutex_unlock(locker);
      return toRet;
    }

  };
  deque< pair< pthread_mutex_t, list<Accumulator> > > accs;


  bool inThread(long int idx) {

    const long int odx = idx/zbinn;
    if (odx >= allOutSv[0].slices() || idx >= allInRd[0].slices() )
      return false;
    if ( find(projes.begin(), projes.end(), odx) == projes.end() )
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
        allInRd[curI].read(idx, myAllIn[curI]);
        curDnz(myDnsr, curI).proc(myAllIn[curI]);
      }
      deque<Map> & myRes = myProc.process(myAllIn);
      if (zbinn == 1)
        for (int curO = 0  ;  curO<allOutSv.size()  ;  curO++ )
          allOutSv[curO].save(odx, myRes[curO]);
      else {
        for (int curO = 0  ;  curO<allOutSv.size()  ;  curO++ ) {
          pthread_mutex_lock(&accs[curO].first);
          list<Accumulator> & curAccs = accs[curO].second;
          Map & curMyRes = myRes[curO];
          if (!curAccs.size())
            curAccs.emplace_back(curMyRes.shape(), zbinn);
          auto useacc = curAccs.end();
          for ( auto accsp  = curAccs.begin() ; accsp != curAccs.end() ; accsp++) {
            if (useacc == curAccs.end() && accsp->odx<0 ) { // found free
              useacc = accsp;
              useacc->odx = odx; // reserve this acc, but keep searching
            } else if (accsp->odx == odx) {  // found existing
              if ( useacc != curAccs.end() ) // if was reserved
                useacc->odx = -1; // release reservation
              useacc = accsp;
              break;
            }
          }
          if (useacc == curAccs.end()) { // nothing found: create new
            curAccs.emplace_back(curMyRes.shape(), zbinn);
            useacc = --curAccs.end();
          }
          Accumulator & myacc = *useacc;
          myacc.odx = odx;
          pthread_mutex_unlock(&accs[curO].first);
          if (myacc.addme(curMyRes))
            allOutSv[curO].save(odx, curMyRes);
        }
      }
    } catch (...) {
      warn("form projection", toString("Failed processing input index %i, for output slice %i.", idx, odx));
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
        accs.emplace_back( pthread_mutex_t(PTHREAD_MUTEX_INITIALIZER), list<Accumulator>() );
  }

  ~ProjInThread() {
    auto dlnl = [&](auto pntr) { if (pntr) { delete pntr; pntr=0; } } ;
    for (auto celem : dnsrs)       dlnl(celem.second) ;
    for (auto celem : procs)       dlnl(celem.second) ;
    for (auto celem : allInMaps)   dlnl(celem.second) ;
    for (auto & accPair : accs)
      pthread_mutex_destroy(&accPair.first);
  }


};




/// \MAIN{projection}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;
  const Shape<2> ish(ImageSizes(args.images.at(0).at(0).repr()));
  const int nofIn = args.images.size();

  // Read auxiliary images
  #define rdAux(pfx) \
    deque<Map> pfx##as(args.pfx##s.size()); \
    for ( int curf = 0 ; curf < args. pfx##s.size() ; curf++) \
      ReadImage(args. pfx##s[curf].repr(), pfx##as[curf], ish);
  rdAux(bg);
  rdAux(df);
  rdAux(dg);
  rdAux(ms);
  #undef rdAux

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
  const int nofOuts = args.zbinn ? nofProj/args.zbinn : 1;
  if ( args.testMe >= nofOuts )
    exit_on_error(args.command, "Requested test is beyond number of projections.");
  const deque<int> projes = slice_str2vec(args.out_range, nofOuts);
  if (!projes.size())
    exit_on_error(args.command, "Given range \"" + args.out_range + "\""
                                " is beyond output slices "+toString(nofOuts)+".");

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
    allOutSv.emplace_back(filedescind, Shape<3>(nofOuts, outShapes[curSplt](1), outShapes[curSplt](0)) );
  }

  // Test or process and save
  if ( args.testMe >= 0 ) { // test
    deque<Map> allIn;
    for ( ssize_t curI = 0 ; curI < nofIn ; curI++) {
      allIn.emplace_back(ish);
      const int trd = args.zbinn * ( args.testMe >= 0  ?  args.testMe  :  projes[0] );
      allInRd[curI].read(trd, allIn[curI]);
      curDnz(canonDZs, curI).proc(allIn.back());
    }
    const deque<Map> & allOut = canonPP.process(allIn, toString(testFormat, ".tif"));
    for (int curSplt = 0 ; curSplt < nofSplts ; curSplt++) {
      const Map & cOut = allOut[curSplt];
      const int svIdx = args.zbinn ? args.testMe / args.zbinn : 0;
      ImagePath svPath = allOutSv[curSplt].save(svIdx, cOut);
      cout << nofOuts << " " << cOut.shape()(0) << " " << cOut.shape()(1) << " " << svPath << "\n";
    }
  } else // finally process
    ProjInThread(allInRd, allOutSv, canonDZs, canonPP, projes, args.zbinn, args.beverbose)
        .execute();

  exit(0);

}

