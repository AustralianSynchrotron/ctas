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

const string DimSliceOptionDesc = "[slice dimension][slice(s)]]"
" with [slice dimension] either x, y or z (default) being the perpendicular to the slicing plane"
" and [slice(s)]. " + SliceOptionDesc;


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  vector<Path> images;        ///< input image
  Path outmask;              ///< Name of the output image.
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
    ("Extract slices from 3D volume.",

    "Extract slices from 3D volume read from HDF5 images." );

  table
    .add(poptmx::NOTE, "ARGUMENTS:")
    .add(poptmx::ARGUMENT, &images,    "images", "Input combination of 2D and 3D images.",
         "Either 2D images understood by the IM or HDF5. HDF5 format as follows:\n"
         "    file:dataset[:[slice dimension][slice(s)]]\n" + DimSliceOptionDesc )

    .add(poptmx::NOTE, "OPTIONS:")
    .add(poptmx::OPTION, &outmask, 'o', "output", "Output result mask or filename.",
       "Output filename if output is a single file."
       " Output mask otherwise. " + MaskDesc, outmask)
    .add(poptmx::OPTION, &mincon, 'm', "min",
       "Pixel value corresponding to black.",
       " All values below this will turn black.", "<minimum>")
    .add(poptmx::OPTION, &maxcon, 'M', "max",
       "Pixel value corresponding to white.",
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





/// \MAIN{projection}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;

  Volume ivol;
  ReadVolume(args.images, ivol, args.beverbose);
  crop(ivol,args.crp);
  binn(ivol,args.bnn);

  const Path outmask =  ( string(args.outmask).find('@') == string::npos ) ?
                          args.outmask.dtitle() + "-@" + args.outmask.extension() :
                          string( args.outmask ) ;

  const bool toInt = fisok(args.mincon)  ||  fisok(args.maxcon) || args.SaveInt;
  const float
    mincon  =  ( fisok(args.mincon)  ||  ! toInt )  ?  args.mincon  :  min(ivol),
    maxcon  =  ( fisok(args.maxcon)  ||  ! toInt )  ?  args.maxcon  :  max(ivol);

  if (toInt)
    SaveVolume(outmask, ivol, args.beverbose, args.slicedesc, mincon, maxcon);
  else
    SaveVolume(outmask, ivol, args.beverbose, args.slicedesc);

  exit(0);



}


