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
/// @date   Thu Jul 17 12:58:59 2008
///
/// @brief Functions needed for flatfield correction.
///


#ifndef _H_BACKGROUND_H_
#define _H_BACKGROUND_H_

#include "../common/common.h"


#ifdef OPENCL_FOUND




class FlatFieldProc {


private:


  const CLmem bgR;
  const CLmem dfR;
  const CLmem maskR;

  CLmem   io;
  const CLmem & bg;
  const CLmem & df;
  const CLmem & mask;

  const Shape sh;
  cl_kernel kernel;

public:

  FlatFieldProc(const Map & _bg, const Map & _df,
                const Map & _mask);

  FlatFieldProc(const FlatFieldProc & other);



  ~FlatFieldProc() {
    clReleaseKernel(kernel);
  }

  void execute(const Map & _io);

  Map & process(Map & _io) {
    execute(_io);
    return cl2blitz(io(), _io);
  }

};


#endif // OPENCL_FOUND



/// \defgroup ff Flat field correction.
///
/// Functions which perform the flat field correction.
///
/// @{

/// Performs the flat image subtraction.
///
/// Would be used only as the blitz::Array function.
///
/// @param fg foreground intensity.
/// @param bg background intensity.
/// @param dc dark current intensity.
/// @param zer value to output on dc >= bg
///
/// @return intensity of the pixel with the subtracted background.
///
static inline float
flatfield(float fg, float bg, float dc, float gf=-1, float zer=0.0){
  if ( gf < 0 ) gf = dc;
  if ( gf >= bg ) return zer;
  else if (dc >= fg ) return 0.0;
  else return (fg-dc)/(bg-gf);
}


/// Adds two arrays with weights.
///
/// @param result resulting array.
/// @param in1 first array.
/// @param in2 second array.
/// @param ratio weight of the first array.
///
static inline Map &
weighted(Map & result, const Map & in1, const Map & in2, float ratio=0.5) {
  if      ( ratio <= 0.0 ) result.reference(in2);
  else if ( ratio >= 1.0 ) result.reference(in1);
  else if ( in1.shape() != in2.shape() )
    throw_error("Weighted array addition", "Input arrays are of different size.");
  else {
    result.resize(in1.shape());
    result = in1*ratio + in2*(1-ratio) ;
  }
  return result;
}



/// Performs the background subtraction on arrays.
///
/// @param result resulting array.
/// @param fg foreground intensity.
/// @param bg background intensity.
///
static inline void
flatfield(Map & result, const Map & fg, const Map & bg){
  if ( ! bg.size() ) {
    if ( &fg != &result )  // arrays are not the same
      result.reference(fg);
    return;
  }
  const Shape sh=fg.shape();
  if( sh != bg.shape() )
    throw_error("flat field", "Different shapes of the input arrays.");
  if( sh != result.shape() )
    result.resize(sh);
  int sz=0;
  for (blitz::MyIndexType icur=0; icur<sh(0); icur++)
    for (blitz::MyIndexType jcur=0; jcur<sh(1); jcur++)
      result(icur,jcur) = flatfield( fg(icur,jcur), bg(icur,jcur), 0.0, 0.0, 0.0);
}


/// Performs the background subtraction on arrays.
///
/// @param result resulting array.
/// @param fg foreground intensity.
/// @param bg background intensity.
/// @param mask do nothing if 0.0.
///
static inline void
flatfield(Map & result, const Map & fg, const Map & bg, const Map & mask){
  if ( ! bg.size() ) {
    if ( &fg != &result )  // arrays are not the same
      result.reference(fg);
    return;
  }
  const Shape sh=fg.shape();
  if( sh != bg.shape() )
    throw_error("flat field", "Different shapes of the input arrays.");
  if( sh != result.shape() )
    result.resize(sh);
  int sz=0;
  for (blitz::MyIndexType icur=0; icur<sh(0); icur++)
    for (blitz::MyIndexType jcur=0; jcur<sh(1); jcur++)
      result(icur,jcur) = flatfield( fg(icur,jcur), bg(icur,jcur), 0.0, 0.0, 0.0);
}





/// Performs the flat field subtraction on arrays.
///
/// @param result resulting array.
/// @param fg foreground intensity.
/// @param bg background intensity.
/// @param dc dark current intensity.
/// @param zer value to output on dc >= bg
///
static inline void
flatfield(Map & result, const Map & fg, const Map & bg, const Map & dc, const Map & gf, const float zer=1.0){
  if ( ! dc.size() ) {
    flatfield(result, fg, bg);
    return;
  }
  if ( ! gf.size() ) {
    flatfield(result, fg, bg, dc, dc, zer);
    return;
  }

  const Shape sh=fg.shape();
  if( sh != bg.shape() || sh != dc.shape() || sh != gf.shape() )
    throw_error("flat field", "Different shapes of the input arrays.");
  if( sh != result.shape() )
    result.resize(sh);
  for (blitz::MyIndexType icur=0; icur<sh(0); icur++)
    for (blitz::MyIndexType jcur=0; jcur<sh(1); jcur++)
      result(icur,jcur) = flatfield( fg(icur,jcur), bg(icur,jcur), dc(icur,jcur), gf(icur,jcur), zer);
}





namespace blitz {

/// Cut out values greater that 1.0.
///
/// Would be used only as the blitz::Array function.
///
/// @param x value to cut.
///
/// @return x if it is less than 1.0, 1.0 otherwise.
///
static inline float
cutone(float x){
  return x>1.0 ? 1.0 : x ;
}

/// \cond
BZ_DECLARE_FUNCTION(cutone);
/// \endcond

}


/// Description of the option giving the --idealworld option.
const std::string IdealWorldOptionDesc =
  "In the ideal absorption contrast the value of the background always"
  " exceeds the respective value of the foreground"
  " (i.e. foreground/background <= 1). If this option is used, the"
  " points where the ratio exceeds 1, are set to 1.";

/// @}

#endif // _H_BACKGROUND_H_
