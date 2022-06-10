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
#include "../common/flatfield.h"
#include <algorithm>
#include <string.h>
#include <unordered_map>







#include "../common/poptmx.h"


using namespace std;

struct StitchRules {
  uint nofIn;
  Shape ish;
  Crop crp;                  ///< Crop input projection image
  Crop fcrp;                  ///< Crop final projection image
  Binn bnn;                  ///< binning factor
  float angle;                ///< Rotation angle.
  PointF2D origin1;            ///< Origin of the next image in the first stitch
  bool origin1Used;            ///< indicates if origin1 was given in options.
  PointF2D origin2;            ///< Origin of the next image in the second stitch
  bool origin2Used;            ///< indicates if origin2 was given in options.
  uint origin2size;           ///< Nof images in the second stitch - needed only if it is requested (origin2)
  PointF2D originF;            ///< Origin of the flipped portion
  bool flipUsed;               ///< indicates if originF was given in options.
  vector<uint> splits;          ///< Split pooints to separate samples.
  StitchRules()
  : nofIn(0)
  , angle(0)
  , origin1Used(false)
  , origin2Used(false)
  , origin2size(0)
  , flipUsed(false)
  {}
};


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  vector<Path> images;        ///< images to combine
  vector<Path> bgs;        ///< Array of the background images.
  vector<Path> dfs;        ///< Array of the dark field images.
  vector<Path> gfs;        ///< Array of the dark field images for backgrounds.
  Path out_name;              ///< Name of the output image.
  StitchRules st;
  int testMe;          ///< Prefix to save interim results
  string sliceMatch;           ///< text file with list of matching slices
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
  , testMe(-1)
  , beverbose(false)
{


  table
    .add(poptmx::NOTE, "ARGUMENTS:")
    .add(poptmx::ARGUMENT, &images, "images", "Input 2D or 3D images.",
         "All images must be of the same rank (2D or 3D). HDF5 format:\n"
         "    file:dataset[:[slice dimension][slice(s)]]\n" + DimSliceOptionDesc)

    .add(poptmx::NOTE, "OPTIONS:")
    .add(poptmx::OPTION, &out_name, 'o', "output", "Output image.", "", out_name)
    .add(poptmx::OPTION, &st.crp, 'c', "crop", "Crop input images: " + CropOptionDesc, "")
    .add(poptmx::OPTION, &st.fcrp, 'C', "crop-final", "Crops final image: " + CropOptionDesc, "")
    .add(poptmx::OPTION, &st.bnn, 'b', "binn", BinnOptionDesc, "")
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
    .add(poptmx::OPTION, &gfs, 'F', "gf", "Dark field image(s) for backgrounds", "")
    .add(poptmx::OPTION, &sliceMatch, 0, "match", "File with slice match list",
         "In the case of 3D i/o this file must contain lines with the integers,"
         " each representing the slice in the corresponding input volume to be used for the projection.")
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

  int tiledImages = table.count(&images);
  if ( ! tiledImages )
    exit_on_error(command, "No input images given.");

  st.origin1Used=table.count(&st.origin1);
  st.origin2Used=table.count(&st.origin2);
  st.flipUsed=table.count(&st.originF);

  if ( ! table.count(&out_name) )
    exit_on_error(command, "No output name provided. Use option " + table.desc(&out_name) + ".");
  if (table.count(&testMe) && testMe<0)
    exit_on_error(command, "Negative test projection given with " + table.desc(&testMe) + " option.");

  if ( table.count(&st.originF) ) {
    if ( tiledImages % 2 )
      exit_on_error(command, string () +
        "Use of the " + table.desc(&st.originF) + " option requires even number of input images"
        " (while " + toString(table.count(&images)) + " counted).");
    tiledImages /= 2;
  }
  if ( table.count(&st.origin2) > table.count(&st.origin1) )
    exit_on_error(command, string () +
      "Options " + table.desc(&st.origin2) + " requires also " + table.desc(&st.origin1) + " option.");
  if ( table.count(&st.origin2) > table.count(&st.origin2size) )
    exit_on_error(command, string () +
      "Options " + table.desc(&st.origin2) + " requires also " + table.desc(&st.origin2size) + " option.");
  if ( table.count(&st.origin2size) && ! table.count(&st.origin2) && ! table.count(&testMe) )
    exit_on_error(command, string () +
      "Options " + table.desc(&st.origin2size) + " must be used with either "
      + table.desc(&st.origin2) + " or " + table.desc(&testMe) + " options.");
  if ( table.count(&st.origin2size)  &&  st.origin2size  < 2 )
    exit_on_error(command, string () +
      "Requested second stitch size (" + toString(st.origin2size) + ") is less than 2.");
  if ( table.count(&st.origin2size)  &&  tiledImages % st.origin2size )
    exit_on_error(command, string () +
      "Total number of tiled images (" + toString(tiledImages) + ") is not a multiple of the requested second stitch size"
      " (" + toString(st.origin2size) + ") given by " + table.desc(&st.origin2size) + " option.");

  sort(st.splits.begin(), st.splits.end());
  unique(st.splits.begin(), st.splits.end());
  if ( st.splits.size() == 1 && st.splits.at(0) == 0 )
    exit_on_error(command, "The list of splits contains only 0 (marking vertical splits).");

  st.angle *= M_PI/180;
  st.nofIn = images.size();

}


namespace blitz {

/// converts NaNs into the number
///
/// Would be used only as the blitz::Array function.
///
/// @param x value to check.
///
/// @return x if it is normal number, 0.0 otherwise.
///
static inline float
denan(float x){
  return isnormal(x) ? x : 0.0 ;
}

/// \cond
BZ_DECLARE_FUNCTION(denan);
/// \endcond

}

void SaveDenan(const Path & filename, const Map & storage, bool saveint=false) {
  Map outm(storage.shape());
  outm=denan(storage);
  SaveImage(filename, outm, saveint);
}


class ProcProj {

  static const string modname;
  StitchRules st;
  const Map & bgar;
  const Map & dfar;
  const Map & gfar;

  int origin1size;
  Map iar, rar, car, final;
  vector<Map> allIn;
  vector<Map> o1Stitch, o2Stitch;

  static void stitch( const vector<Map> & iarr, PointF2D origin, Map & oarr ) {

    const int isz = iarr.size();

    if ( isz == 0 )
      return;
    if ( isz == 1 ) {
      oarr.reference(iarr[0]);
      return;
    }

    int minx=0, maxx=0, miny=0, maxy=0;
    for (int acur = 0 ; acur < isz ; acur++ ) {
      const float
        orgx = acur*origin.x,
        orgy = acur*origin.y,
        tilx = orgx + iarr[acur].shape()(1)-1,
        tily = orgy + iarr[acur].shape()(0)-1;
      if (orgx < minx) minx = orgx;
      if (tilx > maxx) maxx = tilx;
      if (orgy < miny) miny = orgy;
      if (tily > maxy) maxy = tily;
    }

    const Shape osh(maxy-miny+1, maxx-minx+1);
    oarr.resize(osh);

    for (ArrIndex ycur = 0 ; ycur < osh(0) ; ycur++ ) {
      for (ArrIndex xcur = 0 ; xcur < osh(1) ; xcur++ ) {

        int sweight=0;
        float svals=0.0;

        for (int acur = 0 ; acur < isz ; acur++ ) {
          const Map & curar = iarr[acur];
          const Shape cursh = curar.shape();
          const Shape coo = Shape(miny+ycur-acur*origin.y, minx+xcur-acur*origin.x);
          if ( coo(0) >= 0 && coo(0) < cursh(0) && coo(1) >= 0 && coo(1) < cursh(1) ) {
            const float varcur = curar(coo);
            if ( fisok(varcur) ) {
              const int weight = 1 + ( cursh(0) - abs( 2*coo(0) - cursh(0) + 1l ) )
                                   * ( cursh(1) - abs( 2*coo(1) - cursh(1) + 1l ) );
              sweight += weight;
              svals += varcur * weight;
            }
          }
        }

        oarr(ycur,xcur) = sweight ? svals / sweight : NAN ;

      }
    }

  }


public:

  ProcProj(const StitchRules & _st, const Map & _bgar, const Map & _dfar, const Map & _gfar)
    : st(_st)
    , bgar(_bgar)
    , dfar(_dfar)
    , gfar(_gfar)
    , origin1size(st.nofIn / (st.flipUsed ? 2 : 1) / (st.origin2Used ? st.origin2size : 1) )
    , allIn(st.nofIn)
    , o1Stitch(st.nofIn/origin1size)
    , o2Stitch(st.flipUsed ? 2 : 1)
  {
    if ( ! area(st.ish) )
      throw_error(modname, "Zerro area to process.");
    if ( ! st.nofIn )
      throw_error(modname, "Zerro images for input.");
    if (bgar.size() && bgar.shape() != st.ish)
      throw_error(modname, "Unexpected BG shape.");
    if (dfar.size() && dfar.shape() != st.ish)
      throw_error(modname, "Unexpected DF shape.");
    if (gfar.size() && gfar.shape() != st.ish)
      throw_error(modname, "Unexpected GF shape.");
  }


  bool process(const vector<Map> & allInR, vector<Map> & res, const Path & interim_name = Path()) {

    if (allInR.size() != st.nofIn)
      return false;
    Path lastSaved;

    for ( int curproj = 0 ; curproj < st.nofIn ; curproj++) {
      iar.resize(st.ish);
      iar = allInR[curproj];
      if ( bgar.size() || dfar.size() )
        flatfield( iar, iar, bgar, dfar, gfar, 1.0);
      rotate(iar, rar, st.angle);
      crop(rar, car, st.crp);
      binn(car, final, st.bnn);
      allIn[curproj].resize(final.shape());
      allIn[curproj] = final;

      if ( ! interim_name.empty() ) {
        string svformat = mask2format("_T@", st.nofIn);
        lastSaved = interim_name.dtitle() + toString(svformat, curproj) + ".tif";
        SaveDenan(lastSaved, allIn[curproj]);
      }
    }

    for ( int inidx=0 ; inidx<st.nofIn ; inidx += origin1size ) {
      int cidx=inidx/origin1size;
      vector<Map> supply( allIn.begin() + inidx, allIn.begin() + inidx + origin1size) ;
      stitch(supply, st.origin1, o1Stitch[cidx]);

      if ( ! interim_name.empty()  &&  origin1size != 1 ) {
        Map cres;
        if ( ! st.origin1.x * st.origin1.y ) {
            cres.reference(o1Stitch[cidx]);
        } else if ( abs(st.origin1.x) < abs(st.origin1.y)  ) {
            int crppx = abs(st.origin1.x * (supply.size()-1));
            crop(o1Stitch[cidx], cres, Crop(0, crppx, 0, crppx));
        } else {
            int crppx = abs(st.origin1.y * (supply.size()-1));
            crop(o1Stitch[cidx], cres, Crop(crppx, 0, crppx, 0));
        }
        string svformat = mask2format("_U@", o1Stitch.size() );
        lastSaved = interim_name.dtitle() + toString(svformat, cidx) + ".tif";
        SaveDenan(lastSaved, cres);
      }
    }

    for ( int inidx=0 ; inidx<o1Stitch.size() ; inidx += st.origin2size ) {
      int cidx=inidx/st.origin2size;
      vector<Map> supply( o1Stitch.begin() + inidx , o1Stitch.begin() + inidx + st.origin2size ) ;
      stitch(supply, st.origin2, o2Stitch[cidx]);

      if ( ! interim_name.empty()  &&  st.origin2size != 1 ) {
        Map cres;
        if ( ! st.origin2.x * st.origin2.y )
          cres.reference(o2Stitch[cidx]);
        else if ( abs(st.origin2.x) < abs(st.origin2.y) ) {
          int crppx = abs(st.origin2.x * (supply.size()-1));
          crop(o2Stitch[cidx], cres, Crop(0, crppx, 0, crppx));
        } else {
          int crppx = abs(st.origin2.y * (supply.size()-1));
          crop(o2Stitch[cidx], cres, Crop(crppx, 0, crppx, 0));
        }
        string svformat = mask2format("_V@", o2Stitch.size() );
        lastSaved = interim_name.dtitle() + toString( svformat, cidx)  + ".tif";
        SaveDenan(lastSaved, cres);
      }
    }

    if ( st.flipUsed ) {
      o2Stitch[1].reverseSelf(blitz::secondDim);
      stitch(o2Stitch, st.originF, final);
      if ( ! interim_name.empty() )  {
        Map tmp;
        ReadImage(lastSaved, tmp);
        tmp.reverseSelf(blitz::secondDim);
        SaveImage(lastSaved, tmp);
        SaveDenan(interim_name.dtitle() + "_W" , final);
      }
    } else {
      final.reference(o2Stitch[0]);
    }

    if ( st.fcrp != Crop() )
      crop(final, st.fcrp);
    if ( ! interim_name.empty() )
      SaveDenan( interim_name.dtitle() + "_X.tif", final );

    if ( st.splits.empty() ) {
      res.resize(1);
      res[0].resize(final.shape());
      res[0]=final;
    } else {
      const string svformat = mask2format("_Z@", st.splits.size() );
      int fLine=0, lLine=0;
      const int vsplit = st.splits.at(0) ? 0 : 1;
      const int mLine = final.shape()(vsplit);
      for (int curS = vsplit ;  curS<=st.splits.size()  ;  curS++) {
        int curI = curS - vsplit;
        lLine = ( curS == st.splits.size()  ||  mLine < st.splits.at(curS) )
          ?  mLine :  st.splits.at(curS) ;
        lLine--;
        if ( lLine > fLine ) {
          Map resp = vsplit
            ? final( all, blitz::Range(fLine, lLine) )
            : final( blitz::Range(fLine, lLine), all ) ;
          if (res.size() < curI+1)
            res.push_back(Map());
          res[curI].resize(resp.shape());
          res[curI]=resp;
          if ( ! interim_name.empty() )
            SaveDenan( interim_name.dtitle() + toString(svformat, curI) + ".tif", res[curI] );
        }
        fLine=lLine+1;
      }
    }

    return true;

  }

  static bool process(const StitchRules & _st, const Map & _bgar, const Map & _dfar, const Map & _gfar,
                      const vector<Map> & allInR, vector<Map> & res, const string & interim_name = string()) {
    ProcProj proc(_st, _bgar, _dfar, _gfar);
    return proc.process(allInR, res, interim_name);
  }


};

const string ProcProj::modname="ProcProj";



class ProjInThread : public InThread {

  const StitchRules st;
  const Map & bgar;
  const Map & dfar;
  const Map & gfar;
  const blitz::Array<int,2> & slMatch;
  unordered_map<pthread_t,ProcProj> procs;
  unordered_map<pthread_t, vector<Map> > allInMaps;
  unordered_map<pthread_t, vector<Map> > results;
  vector<ReadVolumeBySlice> & allInRd;
  vector<SaveVolumeBySlice> & allOutSv;


  bool inThread(long int idx) {

    if (idx >= allOutSv[0].slices())
      return false;

    const pthread_t me = pthread_self();
    lock();
    if ( ! procs.count(me) ) { // first call
      procs.insert({me, ProcProj(st, bgar, dfar, gfar)});
      allInMaps.insert({me, vector<Map>(allInRd.size())});
      results.insert({me, vector<Map>()});
    }
    ProcProj & myProc = procs.at(me);
    vector<Map> & myAllIn = allInMaps.at(me);
    vector<Map> & myRes = results.at(me);
    unlock();

    for (ArrIndex curI = 0  ;  curI<allInRd.size()  ;  curI++ )
      allInRd[curI].read( slMatch(idx, curI), myAllIn[curI]);
    myProc.process(myAllIn, myRes);
    for (int curO = 0  ;  curO<allOutSv.size()  ;  curO++ )
      allOutSv[curO].save(idx, myRes[curO]);

    bar.update();
    return true;

  }

public:

  ProjInThread(const StitchRules & _st, const Map & _bgar, const Map & _dfar, const Map & _gfar
              , vector<ReadVolumeBySlice> & _allInRd, vector<SaveVolumeBySlice> & _outSave
              , const blitz::Array<int,2> & _slMatch, bool verbose=false)
    : InThread(verbose, "processing projections", _outSave[0].slices() )
    , st(_st)
    , bgar(_bgar)
    , dfar(_dfar)
    , gfar(_gfar)
    , slMatch(_slMatch)
    , allInRd(_allInRd)
    , allOutSv(_outSave)
  {
  }

};


void average_stack(Map & oar, const vector<Path> & stack, const Shape & ish) {
  if (stack.empty())
    return;
  oar.resize(ish);
  oar=0.0;
  Map iar;
  for ( int curf = 0 ; curf < stack.size() ; curf++) {
    ReadImage(stack[curf], iar, ish);
    oar+=iar;
  }
  oar /= stack.size();
}



/// \MAIN{projection}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;
  const Shape ish(ImageSizes(args.images[0]));
  StitchRules st = args.st;
  st.ish = ish;

  Map bgar;
  average_stack(bgar, args.bgs, ish);
  Map dfar;
  average_stack(dfar, args.dfs, ish);
  Map gfar;
  average_stack(gfar, args.gfs, ish);

  const int nofIn = args.images.size();
  int nofProj = -1;
  vector<ReadVolumeBySlice> allInRd(nofIn);
  for ( int curI = 0 ; curI < nofIn ; curI++) {
    allInRd.at(curI).add(args.images.at(curI));
    uint cSls = allInRd.at(curI).slices();
    if (!cSls)
      exit_on_error(args.command, "No images in " + args.images.at(curI));
    if ( nofProj < 0  ||  cSls < nofProj )
      nofProj = cSls;
  }

  blitz::Array<int,2> sliceTable;
  if (nofProj==1) {
    sliceTable.resize(1,nofIn);
    sliceTable = 0;
  } else {
    Map read_sliceTable;
    if (!args.sliceMatch.empty())
      LoadData(args.sliceMatch, read_sliceTable);
    if ( min(read_sliceTable) < 0 )
      throw_error(args.command, "Negative slice requested.");
    if ( max(read_sliceTable) >= nofProj )
      throw_error(args.command, "Slice beyond volume depth requested.");
    if ( ! read_sliceTable.size() ) {
      read_sliceTable.resize(1,1);
      read_sliceTable = 0;
    }
    if ( read_sliceTable.shape()(1) == 1  &&  nofIn > 1 ) { // only one column
      read_sliceTable.resizeAndPreserve(read_sliceTable.shape()(0), nofIn);
      for (ArrIndex curP = 0 ; curP < nofProj ; curP++)
        sliceTable(curP,all) = (int) read_sliceTable(curP, (ArrIndex)0);
    }
    if ( read_sliceTable.shape()(0) == 1  &&  nofProj > 1 ) { // only one row
      nofProj -= max(read_sliceTable);
      read_sliceTable.resizeAndPreserve(nofProj, nofIn);
      for (int curP = 0 ; curP < nofProj ; curP++)
        read_sliceTable(curP,all) = read_sliceTable(0,all) + curP;
    }
    nofProj = read_sliceTable.shape()(0);
    sliceTable.resize(read_sliceTable.shape());
    for ( ArrIndex y=0 ; y < nofProj ; y++)
      for ( ArrIndex x=0 ; x < nofIn ; x++)
        sliceTable(y,x) = read_sliceTable(y,x);
  }


  if (nofProj == 1  ||  args.testMe >= 0) {
    vector<Map> allOut, allIn(allInRd.size());
    for ( ArrIndex curI = 0 ; curI < allInRd.size() ; curI++)
      allInRd[curI].read(sliceTable(ArrIndex(0), curI), allIn[curI]);
    ProcProj::process(st, bgar, dfar, gfar, allIn, allOut,
                      args.testMe >=0 ? imageFile(args.out_name).dtitle() + ".tif" : string());
    if (allOut.size()==1)
      SaveImage(args.out_name, allOut[0]);
    else {
      const string spformat = mask2format("_split@", allOut.size());
      for (int curI = 0 ; curI < allOut.size() ; curI++)
        SaveImage(args.out_name.dtitle() + toString(spformat, curI) + args.out_name.extension(),
                  allOut[curI]);
    }

  } else {
    // This is the easiest way to calculate exactly the output size shapes.
    ProcProj proc(st, Map(), Map(), Map());
    Map test(ish);
    test=0.0;
    vector<Map> allOut, allIn(allInRd.size(), Map(test));
    ProcProj::process(st, Map(), Map(), Map(), allIn, allOut);
    const string spformat = mask2format("_split@", allOut.size());

    vector<SaveVolumeBySlice> allOutSv;
    for (int curI = 0 ; curI < allOut.size() ; curI++) {
      Path filedescind  =  allOutSv.size()==1  ?  (string) args.out_name
                        :  args.out_name.dtitle() + toString(spformat, curI) + args.out_name.extension();
      allOutSv.push_back(SaveVolumeBySlice(filedescind, allOut[curI].shape(), nofProj));
    }

    ProjInThread procTh(st, bgar, dfar, gfar, allInRd, allOutSv, sliceTable, args.beverbose);
    procTh.execute();

  }


  exit(0);

}

