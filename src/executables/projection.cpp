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


struct PointF2D {
  float x;      ///< X coordinate
  float y;       ///< Y coordinate
  inline PointF2D(float _x=0, float _y=0)
  : x(_x), y(_y) {}
};

inline bool operator==( const PointF2D & p1, const PointF2D & p2){
  return p1.x == p2.x && p1.y == p2.y;
}

inline bool operator!=( const PointF2D & p1, const PointF2D & p2){
  return p1.x != p2.x || p1.y != p2.y;
}



std::string
type_desc (PointF2D*) {
  return "FLOAT:FLOAT";
}

int
_conversion (PointF2D* _val, const std::string & in) {
  float x, y;
  if ( sscanf( in.c_str(), "%f:%f", &x, &y) != 2  &&
       sscanf( in.c_str(), "%f,%f", &x, &y) != 2 )
    return -1;
  *_val = PointF2D(x, y);
  return 1;
}





#include "../common/poptmx.h"


using namespace std;




/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  vector<Path> images;        ///< images to combine
  vector<Path> bgs;        ///< Array of the background images.
  vector<Path> dfs;        ///< Array of the dark field images.
  Path out_name;              ///< Name of the output image.
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
  string interim_name;          ///< Prefix to save interim results
  bool beverbose;             ///< Be verbose flag
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[])
  : angle(0)
  , origin2size(0)
  , beverbose(false)
{


  poptmx::OptionTable table
    ("Combines multiple images to form the projection.",

    "Transforms and stitches portions of the projection from the complex CT experiment"
    " which may include 2D tiling and 180-deg flip."
    " Transformations are applied in the following order: rotate, crop, binning." );

  table
    .add(poptmx::NOTE, "ARGUMENTS:")
    .add(poptmx::ARGUMENT, &images, "images", "Input images.", "")

    .add(poptmx::NOTE, "OPTIONS:")
    .add(poptmx::OPTION, &out_name, 'o', "output", "Output image.", "", out_name)
    .add(poptmx::OPTION, &crp, 'c', "crop", "Crop input images: " + CropOptionDesc, "")
    .add(poptmx::OPTION, &fcrp, 'C', "crop-final", "Crops final image: " + CropOptionDesc, "")
    .add(poptmx::OPTION, &bnn, 'b', "binn", BinnOptionDesc, "")
    .add(poptmx::OPTION, &angle,'r', "rotate", "Rotation angle.", "")
    .add(poptmx::OPTION, &origin1, 'g', "origin", "Origin of the image in the first stitch.",
      "Position of the next image origin (top left corner) on the current image.")
    .add(poptmx::OPTION, &origin2, 'G', "second-origin", "Origin of the image in the second stitch.",
      "Position of the next image origin (top left corner) on the current image in the second order stitch.")
    .add(poptmx::OPTION, &origin2size, 'S', "second-size", "Number of imasges in the second stitch.",
      "Required if and only if the second stitch is requested.")
    .add(poptmx::OPTION, &originF, 'f', "flip-origin", "Origin of the flipped portion of the image.",
      "If used, makes second half of the input images to be assigned to the flipped portion."
      " Requires even number of input images.")
    .add(poptmx::OPTION, &splits, 's', "split", "Split point(s)",
         "Final image can be split into sub-images to put different portions of it apart as independent files, for example to separate samples."
         " By default splitting happens horizontally, but if the vertical split is needed, just add a 0 split point.")
    .add(poptmx::OPTION, &bgs, 'B', "bg", "Background image(s)", "")
    .add(poptmx::OPTION, &dfs, 'D', "df", "Dark field image(s)", "")
    .add(poptmx::OPTION, &interim_name, 't', "test", "Prefix to output interim images.", "")
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


  origin1Used=table.count(&origin1);
  origin2Used=table.count(&origin2);
  flipUsed=table.count(&originF);

  if ( ! table.count(&out_name)  &&  ! table.count(&interim_name) )
    exit_on_error(command, string () +
      "Neither " + table.desc(&out_name) + " nor " + table.desc(&interim_name) + " option given.");

  if ( table.count(&originF) ) {
    if ( tiledImages % 2 )
      exit_on_error(command, string () +
        "Use of the " + table.desc(&originF) + " option requires even number of input images"
        " (while " + toString(table.count(&images)) + " counted).");
    tiledImages /= 2;
  }
  if ( table.count(&origin2) > table.count(&origin1) )
    exit_on_error(command, string () +
      "Options " + table.desc(&origin2) + " requires also " + table.desc(&origin1) + " option.");
  if ( table.count(&origin2) > table.count(&origin2size) )
    exit_on_error(command, string () +
      "Options " + table.desc(&origin2) + " requires also " + table.desc(&origin2size) + " option.");
  if ( table.count(&origin2size) && ! table.count(&origin2) && ! table.count(&interim_name) )
    exit_on_error(command, string () +
      "Options " + table.desc(&origin2size) + " must be used with either "
      + table.desc(&origin2) + " or " + table.desc(&interim_name) + " options.");
  if ( table.count(&origin2size)  &&  origin2size  < 2 )
    exit_on_error(command, string () +
      "Requested second stitch size (" + toString(origin2size) + ") is less than 2.");
  if ( table.count(&origin2size)  &&  tiledImages % origin2size )
    exit_on_error(command, string () +
      "Total number of tiled images (" + toString(tiledImages) + ") is not a multiple of the requested second stitch size"
      " (" + toString(origin2size) + ") given by " + table.desc(&origin2size) + " option.");

  sort(splits.begin(), splits.end());
  unique(splits.begin(), splits.end());
  if ( splits.size() == 1 && splits.at(0) == 0 )
    exit_on_error(command, "The list of splits contains only 0 (marking vertical splits).");

  angle *= M_PI/180;

}


using namespace blitz;


void stitch( const vector<Map> & iarr, PointF2D origin, Map & oarr ) {

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

  for (blitz::MyIndexType ycur = 0 ; ycur < osh(0) ; ycur++ ) {
    for (blitz::MyIndexType xcur = 0 ; xcur < osh(1) ; xcur++ ) {

      int sweight=0;
      float svals=0.0;

      for (int acur = 0 ; acur < isz ; acur++ ) {
        const Map & curar = iarr[acur];
        const Shape cursh = curar.shape();
        const Shape coo = Shape(miny+ycur-acur*origin.y, minx+xcur-acur*origin.x);
        if ( coo(0) >= 0 && coo(0) < cursh(0) && coo(1) >= 0 && coo(1) < cursh(1) ) {
          const float varcur = curar(coo);
          if ( fisok(varcur) ) {
            const int weight = 1 + ( cursh(0) - abs( 2*coo(0) - cursh(0) + 1 ) )
                                 * ( cursh(1) - abs( 2*coo(1) - cursh(1) + 1 ) );
            sweight += weight;
            svals += varcur * weight;
          }
        }
      }

      oarr(ycur,xcur) = sweight ? svals / sweight : NAN ;

    }
  }


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

Path findCommon(const vector<Path>::const_iterator bgn, const vector<Path>::const_iterator end) {

  int len = bgn->length();
  if ( bgn>=end  ||  ! len )
    return *bgn;

  for (vector<Path>::const_iterator crnt=bgn ; crnt<end ; crnt++)
    if ( len != crnt->length() )
      len = 0;

  if (len) {
    string ret;
    for ( int idxc = 0 ; idxc < len ; idxc++ ) {
      char cchar = bgn->at(idxc);
      for (vector<Path>::const_iterator crnt=bgn+1 ; crnt<end ; crnt++)
        if ( cchar != crnt->at(idxc) )
          cchar=0;
      if (cchar)
        ret += cchar;
    }
    return ret;
  }

  bool keepGoing=true;
  int idxp=0;
  while ( keepGoing  &&  idxp < bgn->length() ) {
    char cchar = bgn->at(idxp);
    for (vector<Path>::const_iterator crnt=bgn ; crnt<end ; crnt++)
      keepGoing &= idxp < crnt->length()  &&  crnt->at(idxp) == cchar;
    if (keepGoing)
      idxp++;
  }
  const string prefix(*bgn, 0, idxp);

  keepGoing=true;
  int idxs=0;
  while ( keepGoing  &&  idxs < bgn->length() ) {
    char cchar = bgn->at( bgn->length() - 1 - idxs );
    for (vector<Path>::const_iterator crnt=bgn ; crnt<end ; crnt++)
      keepGoing &= idxs < crnt->length()  &&  crnt->length() - idxs > idxp  &&  crnt->at( crnt->length() - 1 - idxs ) == cchar;
    if (keepGoing)
      idxs++;
  }
  const string suffix(*bgn, bgn->length() - idxs, idxs);

  return prefix+suffix;


}




/// \MAIN{projection}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;

  const Shape ish(ImageSizes(args.images[0]));


  Map bgar;
  if ( ! args.bgs.empty() ) {
    bgar.resize(ish);
    bgar=0.0;
    for ( int curf = 0 ; curf < args.bgs.size() ; curf++) {
      Map iar;
      ReadImage(args.bgs[curf], iar, ish);
      bgar+=iar;
    }
    bgar /= args.bgs.size();
  }

  Map dfar;
  if ( ! args.dfs.empty() ) {
    dfar.resize(ish);
    dfar=0.0;
    for ( int curf = 0 ; curf < args.dfs.size() ; curf++) {
      Map iar;
      ReadImage(args.dfs[curf], iar, ish);
      dfar+=iar;
    }
    dfar /= args.dfs.size();
  }


  vector<Map> allIn;
  vector<Path> iimages;

  string svformat = mask2format("Sp@_", args.images.size());
  ProgressBar progBar(args.beverbose, "Prepare tiles input.", args.images.size());
  for ( int curproj = 0 ; curproj < args.images.size() ; curproj++) {
    Map iar, rar, car, bar;
    ReadImage(args.images[curproj], iar);
    if ( bgar.size() || dfar.size() ) {
      if ( iar.shape() != ish )
        throw_error("flatfielding",
                    "Size of the input image \"" + args.images[curproj] + "\""
                    " does not match that of flatfields.");
      flatfield( iar, iar, bgar, dfar );
    }
    rotate(iar, rar, args.angle);
    crop(rar, car, args.crp);
    binn(car, bar, args.bnn);
    allIn.push_back(bar);
    if ( ! args.interim_name.empty() ) {
      Path curname = args.images[curproj];
      if ( args.interim_name  ==  curname.substr(0, args.interim_name.length() ) )
        curname.erase(0, args.interim_name.length() );
      const Path svname = toString(svformat, allIn.size()) + curname.name();
      SaveDenan( args.interim_name + svname.name() , bar );
      iimages.push_back(svname.name());
    }
    progBar.update();
  }

  Path namemask;
  int nofSt;

  nofSt=args.images.size();
  if ( args.flipUsed ) nofSt /= 2;
  if ( args.origin2Used ) nofSt /= args.origin2size;

  vector<Map> o1Stitch;
  vector<Path> o1images;
  ProgressBar st1Bar( args.beverbose && 1 < allIn.size()/nofSt, "Stitching", allIn.size()/nofSt);

  for ( int inidx=0 ; inidx<allIn.size() ; inidx += nofSt ) {

    vector<Map> supply( allIn.begin() + inidx, allIn.begin() + inidx + nofSt) ;
    Map res;
    stitch(supply, args.origin1, res);
    o1Stitch.push_back(res);

    if ( nofSt == 1) {
      o1images = iimages;
    } else if ( ! args.interim_name.empty() ) {

      namemask = findCommon(args.images.begin() + inidx , args.images.begin() + inidx + nofSt ).name();
      svformat = mask2format("St1@_" + namemask, allIn.size()/nofSt );
      const Path svname = toString( svformat, o1Stitch.size() );
      o1images.push_back(svname.name());

      Map cres;
      if ( ! args.origin1.x * args.origin1.y )
          cres.reference(res);
      else if ( abs(args.origin1.x) < abs(args.origin1.y)  ) {
          int crppx = abs(args.origin1.x * (supply.size()-1));
          crop(res, cres, Crop(0, crppx, 0, crppx));
      } else {
          int crppx = abs(args.origin1.y * (supply.size()-1));
          crop(res, cres, Crop(crppx, 0, crppx, 0));
      }
      SaveDenan(args.interim_name + svname.name(), cres);

    }

    st1Bar.update();

  }

  nofSt=o1Stitch.size();
  if ( args.flipUsed ) nofSt /= 2;

  vector<Map> o2Stitch;
  vector<Path> o2images;
  ProgressBar fnBar(args.beverbose && 1 < o1Stitch.size()/nofSt, "Finilizing", 1 + o1Stitch.size()/nofSt );

  for ( int inidx=0 ; inidx<o1Stitch.size() ; inidx += nofSt ) {

    vector<Map> supply( o1Stitch.begin() + inidx , o1Stitch.begin() + inidx + nofSt ) ;
    Map res;
    stitch(supply, args.origin2, res);
    o2Stitch.push_back(res);

    if ( nofSt == 1 ) {
      o2images = o1images;
    } else if ( ! args.interim_name.empty() ) {

      namemask = findCommon(o1images.begin() + inidx , o1images.begin() + inidx + nofSt ).name();
      if ( namemask.substr(0,4) == "St1_" )
        namemask = namemask.erase(0,4);
      svformat = mask2format("St2@_" + namemask, o1Stitch.size()/nofSt );
      const Path svname = toString( svformat, o2Stitch.size() );
      o2images.push_back(svname.name());

      Map cres;
      if ( ! args.origin2.x * args.origin2.y )
          cres.reference(res);
      else if ( abs(args.origin2.x) < abs(args.origin2.y) ) {
          int crppx = abs(args.origin2.x * (supply.size()-1));
          crop(res, cres, Crop(0, crppx, 0, crppx));
      } else {
          int crppx = abs(args.origin2.y * (supply.size()-1));
          crop(res, cres, Crop(crppx, 0, crppx, 0));
      }
      SaveDenan(args.interim_name + svname.name(), cres);

    }

    fnBar.update();

  }


  nofSt=o2Stitch.size();
  Map final;

  if ( args.flipUsed ) {

    if ( nofSt != 2 ) // May it ever happen ?
      exit_on_error(args.command, "Number of images requested to flip-stitch is not equal to two.");

    o2Stitch[1].reverseSelf(secondDim);
    stitch(o2Stitch, args.originF, final);
    if ( ! args.interim_name.empty() )  {
      Map tmp;
      ReadImage(args.interim_name + o2images[1], tmp);
      tmp.reverseSelf(secondDim);
      SaveImage(args.interim_name + o2images[1], tmp);
      namemask = findCommon( o2images.begin(), o2images.end() ).name();
      if ( namemask.substr(0,4) == "St1_"  ||  namemask.substr(0,4) == "St2_")
        namemask = namemask.erase(0,4);
      SaveDenan( args.interim_name + "Sw_" + namemask , final);
    }

    fnBar.update();

  } else if  ( nofSt==1 ) {
    final.reference(o2Stitch[0]);
  } else {
    if ( args.interim_name.empty()  &&  ! args.out_name.empty() )
      warn(args.command, "Output is not a single image. Will be ignored.");
    exit(0);
  }


  if ( args.fcrp != Crop() )
    crop(final, args.fcrp);

  if ( ! args.interim_name.empty() ) {
    namemask = findCommon(args.images.begin(), args.images.end()).name();
    if ( namemask.substr(0,4) == "St1_"  ||
         namemask.substr(0,4) == "St2_"  ||
         namemask.substr(0,4) == "Sw_")
      namemask = namemask.erase(0,4);
    if (namemask.extension().empty())
      namemask += args.images[0].extension();
  }

  if ( args.splits.empty() ) {

    if ( ! args.out_name.empty() )
      SaveImage(args.out_name, final);
    if ( ! args.interim_name.empty() )
      SaveDenan( args.interim_name + "SxF_" + namemask, final );

  } else {

    Path outmask = args.out_name;
    if ( string(outmask).find('@') == string::npos )
      outmask = outmask.dtitle() + "_split@" + outmask.extension();
    const string sliceformat = mask2format(outmask, args.splits.size() );
    svformat = mask2format(args.interim_name + "Sx@_" + namemask, args.splits.size() );

    int fLine=0, lLine=0;
    const int vsplit = args.splits.at(0) ? 0 : 1;
    const int mLine = final.shape()(vsplit);
    for (int curS = vsplit ;  curS<=args.splits.size()  ;  curS++) {

      lLine = ( curS == args.splits.size()  || mLine < args.splits.at(curS) ) ?
        mLine :  args.splits.at(curS) ;
      lLine--;

      if ( lLine > fLine ) {
        Map toSave =  vsplit ?
              final( blitz::Range::all(), blitz::Range(fLine, lLine) ).copy() :
              final( blitz::Range(fLine, lLine), blitz::Range::all() ).copy();
        if ( ! args.out_name.empty() )
          SaveImage( toString(sliceformat, curS-vsplit) , toSave);
        if ( ! args.interim_name.empty() )
          SaveDenan( toString(svformat, curS-vsplit) , toSave );
      }

      fLine=lLine+1;

    }

  }


  exit(0);


}

