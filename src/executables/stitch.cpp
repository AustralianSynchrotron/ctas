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



/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  vector<Path> images;       ///< images to stitch
  Path image1;               ///< Name of the first input image.
  Path image2;               ///< Name of the second input image.
  Path out_name;              ///< Name of the output image.
  bool hOrientation;           ///< Mutual orientation of images: vertical or horizontal.
  unsigned int width;                  ///< Width of the stiching region.
  int shift;                         ///< Shift along the stitch.
  unsigned int cut1;               ///< Cut of the first image.
  unsigned int cut2;               ///< Cut of the second image.zzd
  bool beverbose;				///< Be verbose flag
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
  width(0),
  shift(0),
  cut1(0),
  cut2(0),
  out_name("stitched-<input1>"),
  beverbose(false)
{

  bool horizontal(false), vertical(false);


  poptmx::OptionTable table
	("Stitches two images together.",
	 "The stitching is done gradually in the transition stripe.");

  table
	.add(poptmx::NOTE, "ARGUMENTS:")
	.add(poptmx::ARGUMENT, &images, "images", "Input images.", "")

	.add(poptmx::NOTE, "OPTIONS:")
	.add(poptmx::OPTION, &out_name, 'o', "output", "Output image.", "", out_name)
	.add(poptmx::OPTION, &horizontal, 'r', "row",
		 "Images are placed side by side.", "")
	.add(poptmx::OPTION, &vertical, 'c', "column",
		 "Images are placed top to bottom.", "")
	.add(poptmx::OPTION, &width, 'w', "width",
		 "Width of the transition stripe.", "", "0")
	.add(poptmx::OPTION, &shift, 's', "shift",
		 "Shift along the stitch.", "", "0")
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
   if ( table.count(&images) < 2 )
	exit_on_error(command, string () +
				  "At least two input images must be given as argument: "+table.desc(&images)+".");
  // <output> : one more argument may or may not exist
  if ( ! table.count(&out_name) )
    out_name = images[0].dir() + "stitched-" + images[0].title() + images[0].extension();

}


using namespace blitz;

/// \MAIN{stitch}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;

  Map imio;
  ReadImage( args.images[0], imio );
  const int eRows=imio.rows(),
            eCols=imio.columns(),
            nofims = args.images.size(),
            & shift = args.shift;


  if ( args.cut1 + args.width >= ( args.hOrientation ? eCols : eRows ) )
    exit_on_error(args.command,
      "Crop and stitch width (" + toString(args.cut1) + "+"  + toString(args.width) + "=" + toString(args.cut1+args.width) + ") "
      + "is greater that the size of \"" + args.images[0]+ "\" image "
      + toString( args.hOrientation ? eCols : eRows ) + ".");

  if ( nofims * args.shift >= ( args.hOrientation ? eRows : eCols ) )
    exit_on_error(args.command,
      "Total shift (" + toString(shift) + "*" + toString(nofims) + "=" + toString(nofims*shift) + ") "
      + "is greater that the size of \"" + args.images[0]+ "\" image "
      + toString( args.hOrientation ? eRows : eCols ) + ".");


  if (shift<0) {
    Map tmp;
    if (args.hOrientation)
      imio.reference(imio( Range( -shift*(nofims-1), eRows-1), Range::all() ).copy());
    else
      imio.reference(imio( Range::all(), Range( -shift*(nofims-1), eCols-1) ).copy());
  } else if (shift>0) {
    if (args.hOrientation)
      imio.reference(imio( Range( 0, eRows-shift*(nofims-1)-1 ) , Range::all() ).copy());
    else
      imio.reference(imio( Range::all(), Range( 0, eCols-shift*(nofims-1)-1 ) ).copy());
  }

  for ( int curim=1 ; curim < nofims; curim++ ) {

    Map imadd;
    Path imname = args.images[curim];
    ReadImage(imname, imadd);

    if ( args.cut2+args.width >= ( args.hOrientation ? imadd.columns() : imadd.rows() ) )
      exit_on_error(args.command,
         "Crop and stitch width (" + toString(args.cut2) + "+"
         + toString(args.width) + "=" + toString(args.cut2+args.width) + ") "
         + "is greater thain the size of \"" + imname + "\"image "
         + toString( args.hOrientation ? imadd.columns() : imadd.rows() ) + ".");

    Map imnext;

    if (args.hOrientation) {

      if ( imadd.rows() != eRows )
        exit_on_error(args.command,
          "Rows in image \"" + imname + "\" (" + toString(imadd.rows()) + ")"
          " is not as in the first image (" + toString(eRows) + ").");

      if (shift<0)
        imadd.reference(imadd( Range( -shift*(nofims-1-curim), eRows+shift*curim-1 ) , Range::all() ).copy());
      else if (shift>0)
        imadd.reference(imadd( Range( shift*curim, eRows-shift*(nofims-1-curim)-1 ) , Range::all() ).copy());

      imnext.resize( imio.rows(),
        imio.columns() - args.cut1 + imadd.columns() - args.cut2 - args.width );
      imnext( Range::all(), Range(0, imio.columns() - args.cut1 - args.width - 1) ) =
        imio( Range::all(), Range(0, imio.columns() - args.cut1 - args.width - 1 ) );
      imnext( Range::all(), Range( imio.columns() - args.cut1, imnext.columns() - 1  ) ) =
        imadd( Range::all(), Range( args.width + args.cut2, imadd.columns() - 1 ) );

      for (blitz::MyIndexType iidx=0; iidx < args.width; iidx++) {
        float ratio = (iidx+1.0)/(args.width+1.0);
        for (blitz::MyIndexType jidx=0; jidx < imnext.rows(); jidx++ )
          imnext ( jidx, (blitz::MyIndexType) (imio.columns() - args.cut1 - args.width + iidx) ) =
            (1.0-ratio) * imio( jidx, (blitz::MyIndexType) (imio.columns() - args.cut1 - args.width + iidx ) ) +
            ratio * imadd( jidx, (blitz::MyIndexType) (args.cut2 + iidx) );
      }

    } else {

      if ( imadd.columns() != eCols )
        exit_on_error(args.command,
          "Columns in image \"" + imname + "\" (" + toString(imadd.columns()) + ")"
          " is not as in the first image (" + toString(eCols) + ").");

      if (shift<0)
	imadd.reference(imadd( Range::all(), Range( -shift*(nofims-1-curim), eCols+shift*curim-1) ).copy());
      else if (shift>0)
	imadd.reference(imadd( Range::all(), Range( shift*curim, eCols-shift*(nofims-1-curim)-1 ) ).copy());

      imnext.resize( imio.rows() - args.cut1 + imadd.rows() - args.cut2 - args.width,
        imio.columns() );

      imnext( Range(0, imio.rows() - args.cut1 - args.width - 1), Range::all() ) =
        imio( Range(0, imio.rows() - args.cut1 - args.width - 1 ), Range::all() );
      imnext( Range( imio.rows() - args.cut1, imnext.rows() - 1  ), Range::all() ) =
        imadd( Range( args.width + args.cut2, imadd.rows() - 1 ), Range::all() );

      for (blitz::MyIndexType iidx=0; iidx < args.width; iidx++) {
        float ratio = (iidx+1.0)/(args.width+1.0);
        for (blitz::MyIndexType jidx=0; jidx < imnext.columns(); jidx++ )
          imnext ( (blitz::MyIndexType) ( imio.rows() - args.cut1 - args.width + iidx) , jidx ) =
            (1.0-ratio) * imio( (blitz::MyIndexType) ( imio.rows() - args.cut1 - args.width + iidx), jidx ) +
            ratio * imadd( (blitz::MyIndexType) (args.cut2 + iidx), jidx );
      }

    }

    imio.reference(imnext);

  }

  SaveImage(args.out_name, imio);

  exit(0);

}
