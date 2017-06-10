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
#include <math.h>
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
  PointF2D origin2;            ///< Origin of the next image in the second stitch
  int origin2shift;           ///< Nof images in the first shift (needed only if the second shift is requested)
  PointF2D originF;            ///< Origin of the flipped portion  
  string interim_name;          ///< Prefix to save interim results  
  bool beverbose;             ///< Be verbose flag
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[])
  : angle(0)
  , origin2shift(0)  
  , out_name("combined-<input1>")
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
    .add(poptmx::OPTION, &crp, 'C', "crop-final", "Crops final image: " + CropOptionDesc, "")
    .add(poptmx::OPTION, &bnn, 'b', "binn", BinnOptionDesc, "")
    .add(poptmx::OPTION, &angle,'r', "rotate", "Rotation angle.", "")
    .add(poptmx::OPTION, &origin1, 'g', "origin", "Origin of the image in the first stitch.",
      "Position of the next image origin (top left corner) on the current image.")
    .add(poptmx::OPTION, &origin2, 'G', "second-origin", "Origin of the image in the second stitch.",
      "Position of the next image origin (top left corner) on the current image in the second order stitch.")
    .add(poptmx::OPTION, &origin2shift, 's', "origin-size", "Number of imasges in the first stitch.",
      "Required if and only if the second stitch is requested.")
    .add(poptmx::OPTION, &originF, 'f', "flip-origin", "Origin of the flipped portion of the image.",
      "If used, makes second half of the input images to be assigned to the flipped portion."
      " Requires even number of input images.")
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
  
  if ( tiledImages < 2 )
    exit_on_error(command, string () +
      "At least two input images must be given as argument: " + table.desc(&images) + ".");
  // <output> : one more argument may or may not exist
  if ( ! table.count(&out_name) )
    out_name = "combined-" + images[0].name();

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
  if ( table.count(&origin2) > table.count(&origin2shift) )
    exit_on_error(command, string () +
      "Options " + table.desc(&origin2) + " requires also " + table.desc(&origin2shift) + " option.");
  if ( table.count(&origin2shift) && ! table.count(&origin2) && ! table.count(&interim_name) )
    exit_on_error(command, string () +
      "Options " + table.desc(&origin2shift) + " must be used with either "
      + table.desc(&origin2) + " or " + table.desc(&interim_name) + " options.");
  if ( table.count(&origin2shift)  &&  origin2shift  < 2 )
    exit_on_error(command, string () +
      "Requested first stitch size (" + toString(origin2shift) + ") is less than 2.");
  if ( table.count(&origin2shift)  &&  tiledImages % origin2shift )
    exit_on_error(command, string () +
      "Total number of tiled images (" + toString(tiledImages) + ") is not a multiple of the requested first stitch size"
      " (" + toString(origin2shift) + ") given by " + table.desc(&origin2shift) + " option.");
    
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
  
  const Shape
    ish( iarr[0].shape() ),
    osh( ish(0) + abs(origin.y) * (isz-1), ish(1) + abs(origin.x) * (isz-1)),
    ssh( origin.y >= 0 ? 0 : osh(0) - ish(0), origin.x >= 0 ? 0 : osh(1) - ish(1) );

  oarr.resize(osh);
  vector< Shape > cssh (isz);
  for (int acur = 0 ; acur < isz ; acur++ )
    cssh[acur] = Shape( ssh(0) + origin.y * acur, ssh(1) + origin.x * acur );
  
  for (blitz::MyIndexType ycur = 0 ; ycur < osh(0) ; ycur++ ) {
    for (blitz::MyIndexType xcur = 0 ; xcur < osh(1) ; xcur++ ) {
      
      int sweight=0;
      float svals=0.0;
      
      for (int acur = 0 ; acur < isz ; acur++ ) {
        const Shape coo = Shape(ycur - cssh[acur](0), xcur - cssh[acur](1) );
        if ( coo(0) >= 0 && coo(0) < ish(0) && coo(1) >= 0 && coo(1) < ish(1) 
             && ! isnan( iarr[acur](coo) ) ) {
          const int weight = ( ish(0) - abs( 2.0*coo(0) - ish(0) + 1 ) )
                           * ( ish(1) - abs( 2.0*coo(1) - ish(1) + 1 ) );
          sweight += weight;
          svals += iarr[acur](coo) * weight;
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
/// @param x value to cut.
///
/// @return x if it is less than 1.0, 1.0 otherwise.
///
static inline float
denan(float x){
  return isnan(x) ? 0.0 : x ;
}

/// \cond
BZ_DECLARE_FUNCTION(denan);
/// \endcond

}

void SaveDenan(const Path & filename, const Map & storage, bool saveint=false) {
  Map outm(storage.shape());
  outm=storage;
  denan(outm); 
  SaveImage(filename, outm, saveint);
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
    rotate(bgar, args.angle);
    crop(bgar, args.crp);
    binn(bgar, args.bnn);
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
    rotate(dfar, args.angle);
    crop(dfar, args.crp);
    binn(dfar, args.bnn);
    dfar /= args.dfs.size();
  }
  
  
  vector<Map> allIn;
  
  ProgressBar progBar(args.beverbose, "Prepare tiles input.", args.images.size());
  for ( int curproj = 0 ; curproj < args.images.size() ; curproj++) {
    Map iar, rar, car, bar;
    ReadImage(args.images[curproj], iar, ish);
    rotate(iar, rar, args.angle);
    crop(rar, car, args.crp);
    binn(car, bar, args.bnn);
    flatfield( bar, bar, bgar, dfar );
    allIn.push_back(bar);
    if ( ! args.interim_name.empty()
         && ( bgar.size() || dfar.size() 
              || args.angle != 0.0 || args.crp != Crop() || args.bnn != Binn() )  )
      SaveDenan( args.interim_name + "w-" + args.images[curproj].name(), bar );
    progBar.update();
  }
  
  if ( args.origin1 == PointF2D()  &&  args.originF == PointF2D() ) {
    if ( args.interim_name.empty() )      
      warn(args.command, "Exit with no output - nothing requested.");
    exit(0);
  }
  
  int nofSt=0;
  string svformat;
  
  if      ( args.origin2shift )          nofSt=args.origin2shift;
  else if ( args.origin1 == PointF2D() ) nofSt=1;
  else if ( args.originF == PointF2D() ) nofSt=allIn.size();
  else                                   nofSt=allIn.size()/2;  
  svformat = mask2format(args.interim_name + "x-@" + args.images[0].extension(), allIn.size()/nofSt );
  vector<Map> o1Stitch;
  ProgressBar st1Bar( args.beverbose && 1 < allIn.size()/nofSt, "Stitching", allIn.size()/nofSt); 
  for ( vector<Map>::iterator st=allIn.begin() ; st < allIn.end() ; st += nofSt ) {
    vector<Map> supply(st, st+nofSt) ;
    Map res;
    stitch(supply, args.origin1, res);
    o1Stitch.push_back(res);
    if ( ! args.interim_name.empty() && nofSt!=1 )
      SaveDenan( toString( svformat, o1Stitch.size() ), res );
    st1Bar.update();
  }
  
  if ( o1Stitch.size() > 1  &&  args.origin2 == PointF2D()  &&  args.originF == PointF2D() ) {
    if ( args.interim_name.empty() )      
      warn(args.command, "Exit with no output - nothing requested.");
    exit(0);
  }
  
  if      ( ! args.origin2shift )        nofSt=1;
  else if ( args.originF == PointF2D() ) nofSt=o1Stitch.size();
  else                                   nofSt=o1Stitch.size()/2;
  svformat = mask2format(args.interim_name + "y-@" + args.images[0].extension(), o1Stitch.size()/nofSt );
  vector<Map> o2Stitch;
  ProgressBar fnBar(args.beverbose && 1 < o1Stitch.size()/nofSt, "Finilizing", 1 + o1Stitch.size()/nofSt ); 
  for ( vector<Map>::iterator st=o1Stitch.begin() ; st < o1Stitch.end() ; st += nofSt ) {
    vector<Map> supply(st, st+nofSt) ;
    Map res;
    stitch(supply, args.origin2, res);
    o2Stitch.push_back(res);
    if ( ! args.interim_name.empty() && nofSt!=1 )
      SaveDenan( toString( svformat, o2Stitch.size() ), res );
    fnBar.update();
  }
  
  Map final;
  if        ( o2Stitch.size()==1 ) {
    final.reference(o2Stitch[0]);
  } else if ( o2Stitch.size()==2 ) {
    o2Stitch[1].reverseSelf(secondDim);
    if ( ! args.interim_name.empty() )
      SaveDenan(args.interim_name + "z" + args.images[0].extension() , o2Stitch[1]);
    stitch(o2Stitch, args.originF, final);  
    fnBar.update();
  } else {
    if ( args.interim_name.empty() )
      warn(args.command, "Request to flip-stitch more than two images. Will be ignored.");
    exit(1);
  }
  
  if ( args.fcrp != Crop() )
    crop(final, args.fcrp);
  
  SaveImage(args.out_name, final);    

  exit(0);

}

