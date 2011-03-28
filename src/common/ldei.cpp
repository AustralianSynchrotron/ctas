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
/// @author antonmx <antonmx@synchrotron.org.au>
/// @date   Thu Sep 11 18:43:42 2008
///
/// @brief Definitions of functions declared in dei.h
///


#include "dei.h"
#include "poptmx.h"


using namespace std;


/// \addtogroup deiedei
/// @{




const string DEIprocess::modname = "DEI process";

/// @param _Rm             Reflectivity in the minus position.
/// @param _Rp             Reflectivity in the plus position.
/// @param _Gm             Derivative of the RC in the minus position.
/// @param _Gp             Derivative of the RC in the plus position.
///
DEIprocess::
DEIprocess(float _Gm, float _Gp, float _Rm, float _Rp)
  : Gm(_Gm), Gp(_Gp), Rm(_Rm), Rp(_Rp) {
  if ( Rm <=0 || Rm >= 1 )
	throw_error( modname, "The reflectivity in minus point"
				 " is outside the range [0, 1.0].");
  if ( Rp <=0 || Rp >= 1 )
	throw_error( modname, "The reflectivity in plus point"
				 " is outside the range [0, 1.0].");
  if (Gm<=0.0)
	throw_error( modname, "The derivative of reflectivity in minus point"
				 " is less or equal to 0.");
  if (Gp>=0.0)
	throw_error( modname, "The derivative of reflectivity in plus point"
				 " is greater or equal to 0.");
}



/// Extracts absorption component from the intensities recorded
/// on both sides of the RC.
///
/// Note. ::minus and ::plus must be non-negative. No check inside.
///
/// @param minus Intensity in minus point of the RC.
/// @param plus Intensity in plus point of the RC.
/// @param comp Component to be extracted.
///
/// @return Absorption contrast (\f$ I/I_0\f$) or refraction angle
///         depending on the ::comp. 
///
float
DEIprocess::extract ( float minus, float plus, Component comp) const {
  static const float div = Rm*Gp - Rp*Gm;
  minus *= Rm;
  plus  *= Rp;
  if   ( minus == 0.0 && plus == 0.0 )
    return 0.0;
  return (comp == ABS) ?
    (minus*Gp - plus*Gm) / div :
    ( minus*Rp - plus*Rm ) / (plus*Gm - minus*Gp) ;
}

const std::string DEIprocess::componentDesc = 
  "Must be one of the following strings (case insensitive):\n"
  "A, ABS, ABSORPTION - for the absorption component\n"
  "R, REF, REFRACTION - for the refraction component";



/// @}
