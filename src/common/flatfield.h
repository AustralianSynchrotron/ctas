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
///
/// @return intensity of the pixel with the subtracted background.
///
static inline float
flatfield(float fg, float bg, float dc=0.0){
  if ( dc >= bg ) return 1.0;
  else if (dc >= fg ) return 0.0;
  else return (fg-dc)/(bg-dc);
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
  if ( in1.shape() != in2.shape() )
    throw_error("Weighted array addition", "Input arrays are of different size.");
  result = in1*ratio + in2*(1-ratio) ;
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
    result = fg;
    return;
  }
  Shape sh=fg.shape();
  if( sh != bg.shape() )
    throw_error("flat field", "Different shapes of the input arrays.");
  int sz=0;
  for (blitz::MyIndexType icur=0; icur<sh(0); icur++)
    for (blitz::MyIndexType jcur=0; jcur<sh(1); jcur++)
      result(icur,jcur) = flatfield( fg(icur,jcur), bg(icur,jcur) );
}


/// Performs the flat field subtraction on arrays.
///
/// @param result resulting array.
/// @param fg foreground intensity.
/// @param bg background intensity.
/// @param dc dark current intensity.
///
static inline void
flatfield(Map & result, const Map & fg, const Map & bg, const Map & dc){
  if ( ! dc.size() ) {
    flatfield(result, fg, bg);
    return;
  }
  const Shape sh=fg.shape();
  if( sh != bg.shape() || sh != dc.shape() )
    throw_error("flat field", "Different shapes of the input arrays.");
  if( sh != result.shape() )
    result.resize(sh);
  for (blitz::MyIndexType icur=0; icur<sh(0); icur++)
    for (blitz::MyIndexType jcur=0; jcur<sh(1); jcur++)
      result(icur,jcur) = flatfield( fg(icur,jcur), bg(icur,jcur), dc(icur,jcur) );
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
