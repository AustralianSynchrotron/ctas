/******************************************************************************
 *   Copyright (C) 2007 by Anton Maksimenko                                   *
 *   antonmx@post.kek.jp                                                      *
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
/// @date   Mon Jul 21 10:09:31 2008
///
/// @brief %Reconstruction of one sinogram.
///
/// Main file for the program which reads the input sinogram from image,
/// performs CT reconstruction of it and stores the result in other
/// image file
///



#include "../common/common.h"
#include "../common/ct.h"
#include "../common/projection.h"
#include "../common/poptmx.h"

using namespace std;


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Contrast contrast;            ///< Type of the contrast.
  deque<ImagePath> imgs;         ///< input images.
  ImagePath outmask;           ///< Name of the file to save the result to.
  float arc;
  uint denoiseRad;
  float denoiseThr;
  float maxShift;
  bool beverbose;				///< Be verbose flag
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[])
  : contrast(Contrast::ABS)
  , arc(180)
  , maxShift(0.25)
  , denoiseRad(0)
  , denoiseThr(0.0)
  , beverbose(false)
{

  poptmx::OptionTable table
  ("Estimates rotation centre.",
   "Prints the result(s) as the deviation from the image centre.");

  table
  .add(poptmx::NOTE, "ARGUMENTS:")
  .add(poptmx::ARGUMENT, &imgs, "images", "Input images.",
       "Depending on the selected technique(s), regards them as the pair of 180deg projection(s) or sinogram(s).")
  .add(poptmx::NOTE, "OPTIONS:")
  .add(poptmx::OPTION, &contrast, 'k', "contrast", "Input component.",
       "Type of the contrast presented in the sinogram. " + Contrast::Desc)
  .add(poptmx::OPTION, &arc, 'a', "arcan",
       "CT scan range (if > 1 deg), or step size (if < 1deg).",
       "Only makes sense if input represents sinogram(s).",
       toString(arc))
  .add(poptmx::OPTION, &denoiseRad, 'n', "noise-rad", "Radius for noise reducing algorithm.",
       "Radius of the local region used to calculate average to replace the pixel value." )
  .add(poptmx::OPTION, &denoiseThr, 'N', "noise-thr", "Noise threshold.",
       "If positive, then gives maximum absolute deviation from the average;"
       " if negative, then negated value is maximum relative deviation.")
  .add(poptmx::OPTION, &outmask, 'o', "output",
       "Mask to output reconstructed image(s).", "", outmask)
  .add(poptmx::OPTION, &maxShift, 'm', "max", "Maximum relative shift.", "", toString(maxShift))
  .add_standard_options(&beverbose)
  .add(poptmx::MAN, "SEE ALSO:", SeeAlsoList);

  if ( ! table.parse(argc,argv) )
    exit(0);
  if ( ! table.count() ) {
    table.usage();
    exit(0);
  }
  command = table.name();

  if ( ! table.count(&imgs) )
    exit_on_error(command, string () +
                  "Missing required argument: "+table.desc(&imgs)+".");
  if (arc <= 0.0)
    exit_on_error(command, "CT arc (given by "+table.desc(&arc)+"): must be strictly positive.");
  if (maxShift <= 0.0 )
    exit_on_error(command, "Impossible maximum shift "+toString(maxShift)+" (given by "+table.desc(&maxShift)+"): must be strictly positive.");

}




/// \MAIN{ct}
int main(int argc, char *argv[]) {
  const clargs args(argc, argv) ;
  const Shape ish = ImageSizes(args.imgs[0]);
  if (ish(1) < 2 || ish(0) < 2)
    throw_error (args.command, "Size of input image ("+toString(ish)+") is less than 2x2.");

  float cent;
  #define rotax(f1, ...) \
    cent = 0.0; \
    try { \
      cent = raxis(f1, ##__VA_ARGS__) - 0.5*(ish(1)-1); \
      cout << cent << "\n"; \
      fflush(stdout);\
    } catch (...) {\
      cerr << "Failed to calculate COR.\n";\
      fflush(stderr);\
    }

  if (args.imgs.size() == 1) { // sinogram
    const float ang = args.arc / ( abs(args.arc)<=1.0 ? 1.0 : ish(0)-1 );
    Map sino;
    ReadImage(args.imgs[0], sino);
    if (args.denoiseRad) {
      Denoiser dnz(sino.shape(), args.denoiseRad, args.denoiseThr);
      dnz.proc(sino);
    }
    CTrec * rec = 0;
    Map recr;
    if (args.outmask.size()) {
      rec = new CTrec(sino.shape(), Contrast::ABS, args.arc);
      Map _sino(sino.copy());
      rec->sino(_sino);
      recr.resize(rec->recShape());
    }
    #define saveres(num) \
      if (rec) { \
        rec->repeat(recr,cent); \
        Path ofile = toString(mask2format(args.outmask,3),num); \
        SaveImage( ofile.dtitle() + "_" + toString(cent) + ofile.ext() , recr); \
      }
    rotax(sino, ang, args.maxShift);
    saveres(0);
    rotax(sino, ang, args.maxShift, 0);
    saveres(1);
    rotax(sino, ang, args.maxShift, 1);
    saveres(2);
    rotax(sino, ang, args.maxShift, 2);
    saveres(3);
    #undef saveres
    if (rec)
      delete rec;
  } else if(args.imgs.size() == 2) { // projections
    Map pr0, pr180;
    ReadImage(args.imgs[1], pr180, ish);
    ReadImage(args.imgs[0], pr0);
    if (args.denoiseRad) {
      Denoiser dnz(pr0.shape(), args.denoiseRad, args.denoiseThr);
      dnz.proc(pr0);
      dnz.proc(pr180);
    }

    #define saveres(num) \
      if (args.outmask.size()) { \
        StitchRules rl; \
        rl.nofIn = 2; \
        rl.flip = true; \
        rl.originF = PointF2D(2*cent,0); \
        std::deque<Map> emp; \
        ProcProj canonPP(rl, pr0.shape(), emp, emp, emp, emp); \
        std::deque<Map> allInR = {pr0.copy(), pr180.copy()}; \
        const std::deque<Map> & allInSv = canonPP.process(allInR); \
        if (allInSv.size()==1) { \
          Path ofile = toString(mask2format(args.outmask,1),num); \
          SaveImage( ofile.dtitle() + "_" + toString("%+f", cent) + "." + ofile.ext() , allInSv[0]); \
        } \
      }
    rotax(pr0, pr180, args.maxShift);
    saveres(0);
    //rotax(pr0, pr180);
    //saveres(1);
    #undef saveres
  } else {
    throw_error(args.command, "Can accept only one (sinogram) or two (opposit projections) images.");
  }
  #undef rotax

  exit(0);


}
