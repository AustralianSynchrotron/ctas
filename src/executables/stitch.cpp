/******************************************************************************
 *   Copyright (C) 2013 by Anton Maksimenko                                   *
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
/// @date   Thu Feb 07 10:09:31 2013
///
/// @brief %Stitches two images together.
///


#include "../common/common.h"
#include "../common/poptmx.h"
#include <math.h>
#include <string.h>

using namespace std;

#ifdef _WIN32
#define NAN numeric_limits<float>::quiet_NaN();
static inline int isnan(double x){ return _isnan(x); }
#endif




/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Path image1;               ///< Name of the first input image.
  Path image2;               ///< Name of the second input image.
  Path out_name;              ///< Name of the output image.
  bool hOrientation;           ///< Mutual orientation of images: vertical or horizontal.
  unsigned int width;                  ///< Width of the stiching region.
  unsigned int cut1;               ///< Cut of the first image.
  unsigned int cut2;               ///< Cut of the second image.
  bool beverbose;				///< Be verbose flag
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
  width(0),
  cut1(0),
  cut2(0),
  out_name("stitched-<input1>-<input2>"),
  beverbose(false)
{

  bool horizontal(false), vertical(false);


  poptmx::OptionTable table
	("Stitches two images together.",
	 "The stitching is done gradually in the transition stripe.");

  table
	.add(poptmx::NOTE, "ARGUMENTS:")
	.add(poptmx::ARGUMENT, &image1, "image1", "First input image (left/top).", "")
	.add(poptmx::ARGUMENT, &image2, "image2", "Second input image (right/bottom).", "")
	.add(poptmx::ARGUMENT, &out_name, "output", "Output image.", "", out_name)

	.add(poptmx::NOTE, "OPTIONS:")
	.add(poptmx::OPTION, &horizontal, 'r', "row",
		 "Images are placed side by side.", "")
	.add(poptmx::OPTION, &vertical, 'c', "column",
		 "Images are placed top to bottom.", "")
	.add(poptmx::OPTION, &width, 'w', "width",
		 "Width of the transition stripe.", "", "0")
	.add(poptmx::OPTION, &cut1, '1', "cut1",
		 "Crop first image edge.",
		 "Width of the crop region which is cut from the right/bottom edge"
		 " of the first image before stitching.", "0")
	.add(poptmx::OPTION, &cut2, '2', "cut2",
		 "Crop second image edge.",
		 "Width of the crop region which is cut from the left/top edge"
		 " of the second image before stitching.", "0")
	.add_standard_options(&beverbose)
	.add(poptmx::MAN, "SEE ALSO:", SeeAlsoList);

 
  if ( ! table.parse(argc,argv) )
	exit(0);
  if ( ! table.count() ) {
	table.usage();
	exit(0);
  }

  command = table.name();

  if (horizontal == vertical)
    exit_on_error(command, string() +
                                  "One and only one of two options must be set: " +
                                  table.desc(&horizontal) + ", " + table.desc(&vertical) + ".");
  else
    hOrientation = horizontal;
  // <image1,2> : two required arguments.
   if ( ! table.count(&image1) )
	exit_on_error(command, string () +
				  "Missing required argument: "+table.desc(&image1)+".");
  if ( ! table.count(&image2) )
	exit_on_error(command, string () +
				  "Missing required argument: "+table.desc(&image2)+".");
  // <output> : one more argument may or may not exist
  if ( ! table.count(&out_name) )
    out_name = image1.dir() + "stitched-" + image1.title() + "-" + image2.title() + image1.extension();

}


using namespace blitz;

/// \MAIN{stitch}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;

  Map im1, im2;
  ReadImage( args.image1, im1 );
  ReadImage( args.image2, im2 );

  if ( args.hOrientation && im1.rows() != im2.rows() )
    exit_on_error(args.command, "Different heights of row-stiched images.");
  if ( ! args.hOrientation && im1.columns() != im2.columns() )
    exit_on_error(args.command, "Different widths of column-stiched images.");

  if ( args.cut1+args.width >= ( args.hOrientation ? im1.columns() : im1.rows() ) )
    exit_on_error(args.command,
	 "Crop and stitch width (" + toString(args.cut1) + "+"
	 + toString(args.width) + "=" + toString(args.cut1+args.width) + ") "
	 + "is greater that the image size "
	 + toString( args.hOrientation ? im1.columns() : im1.rows() ) + ".");
  if ( args.cut2+args.width >= ( args.hOrientation ? im2.columns() : im2.rows() ) )
    exit_on_error(args.command, 
         "Crop and stitch width (" + toString(args.cut2) + "+"
         + toString(args.width) + "=" + toString(args.cut2+args.width) + ") "
         + "is greater that the image size " 
         + toString( args.hOrientation ? im2.columns() : im2.rows() ) + ".");

  Map out;
  if (args.hOrientation) {

    out.resize( im1.rows(),
      im1.columns() - args.cut1 + im2.columns() - args.cut2 - args.width );

    out( Range::all(), Range(0, im1.columns() - args.cut1 - args.width - 1) ) =
      im1( Range::all(), Range(0, im1.columns() - args.cut1 - args.width - 1 ) );
    out( Range::all(), Range( im1.columns() - args.cut1, out.columns() - 1  ) ) =
      im2( Range::all(), Range( args.width + args.cut2, im2.columns() - 1 ) );

    for (int iidx=0; iidx < args.width; iidx++) {
      float ratio = (iidx+1.0)/(args.width+1.0);
      for (int jidx=0; jidx < out.rows(); jidx++ )
        out ( jidx, im1.columns() - (int)args.cut1 - (int)args.width + iidx ) =
          (1.0-ratio) * im1( jidx, im1.columns() - (int)args.cut1 - (int)args.width + iidx ) +
          ratio * im2( jidx, (int)args.cut2 + iidx );
    }

  } else {

    out.resize( im1.rows() - args.cut1 + im2.rows() - args.cut2 - args.width,
      im1.columns() );

    out( Range(0, im1.rows() - args.cut1 - args.width - 1), Range::all() ) =
      im1( Range(0, im1.rows() - args.cut1 - args.width - 1 ), Range::all() );
    out( Range( im1.rows() - args.cut1, out.rows() - 1  ), Range::all() ) =
      im2( Range( args.width + args.cut2, im2.rows() - 1 ), Range::all() );

    for (int iidx=0; iidx < args.width; iidx++) {
      float ratio = (iidx+1.0)/(args.width+1.0);
      for (int jidx=0; jidx < out.columns(); jidx++ )
        out ( im1.rows() - (int)args.cut1 - (int)args.width + iidx, jidx ) =
          (1.0-ratio) * im1( im1.rows() - (int)args.cut1 - (int)args.width + iidx, jidx ) +
          ratio * im2( (int)args.cut2 + iidx, jidx );
    }


  }


 

  cout << out.columns() << " "  << out.rows() << "\n";
  
  SaveImage(args.out_name, out);

/*

  Map arr;
  ReadImage( args.in_name, arr );

  if (args.mMm) {
	cout << min(arr) << " " << max(arr) << endl;
	exit(0);
  }

  float mincon = args.mincon, maxcon = args.maxcon;

  if ( isnan(mincon) ) mincon = min(arr);
  if ( isnan(maxcon) ) maxcon = max(arr);

  SaveImage(args.out_name, arr, mincon, maxcon);

*/

  exit(0);

}
