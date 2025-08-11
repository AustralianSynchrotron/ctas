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


#include "edei.h"
#include "poptmx.h"


using namespace std;


/// \addtogroup edeiedei
/// @{


/// \brief Prints the name of the interpretation type.
/// @return Printed name of the interpretation.
string
Interp::name() const {
  switch (mpinter) {
  case ALPHA:  return "alpha";
  case NUMBER: return "number";
  case REFLECT:    return "reflectivity";
  case INTENS:   return "intensity";
  default :
    throw_bug(__FUNCTION__);
    return "";
  }
}


/// @return The interpretation type.
Interp::inter_tp
Interp::type() const  {
  return mpinter;
}

/// @param _mpinter Interpretation type.
Interp::Interp(inter_tp _mpinter) : mpinter(_mpinter) {
}


/// @param _name The name to be converted to the type.
Interp::Interp(const string & _name) {
  string name = upper(_name);
  if ( name == "A" || name == "ALPHA" ) mpinter = ALPHA;
  else if ( name == "N" || name == "NUMBER" ) mpinter = NUMBER;
  else if ( name == "R" || name == "REFLECTIVITY" ) mpinter = REFLECT;
  else if ( name == "I" || name == "INTENSITY" ) mpinter = INTENS;
  else throw_error("point interpretation", "The string \""+ _name +"\""
           " does not describe any known point interpretation.");
}

const std::string Interp::optionDesc =
  "Can be one of the following strings (case insensitive):\n"
  "  \"A\", \"ALPHA\" -  Sets the meaning of the +/- values to the angle"
  " \\alpha. I.e. finds the given value on the \\alpha axis of the RC"
  " (first column in the input RC file) and sets the position"
  " to the found point.\n"
  "  \"N\", \"NUMBER\" - Sets the meaning of the +/- values to"
  " the RC index. Converts values to integer and considers them as the"
  " index on the RC's \\alpha axis. In other words the given values"
  " represent the string in the RC file which corresponds to the position.\n"
  "  \"I\", \"INTENSITY\" - Sets the meaning of the +/- values to the intensity."
  " I.e. finds the given value on the intensity axis of the RC (second column"
  " in the input RC file) and sets the position to the found point.\n"
  "  \"R\", \"REFLECTIVITY\" - Sets the meaning of the +/- values to"
  " the reflectivity. Similar to \"INTENSITY\", but normalizes the"
  " intensities and interprets the values as the normalized ones.";


bool operator==(const Interp & a, const Interp & b){
  return  a.type() == b.type() ;
}

bool operator!=(const Interp & a, const Interp & b){
  return  a.type() != b.type() ;
}

string
type_desc (Interp*){
  return "STRING";
}

bool
_conversion (Interp* _val, const string & in) {
  *_val=Interp(in);
  return true;
}







/// Constructs and returns the option table.
///
/// @return Option table to be used via the poptmx library.
///
poptmx::OptionTable
EDEIoptions::options() {
  poptmx::OptionTable opt
    ("EDEI processing options.",
     "Options which influence the component extraction via the EDEI method.");
  opt
    .add(poptmx::OPTION, &Gm, 'm', "minus-point",
         "Value determining the minus-point.", "", toString(Gm))
    .add(poptmx::OPTION, &Gp, 'p', "plus-point",
         "Value determining the plus-point.", "", toString(Gp));
  opt
    .add(poptmx::NOTE, "",
         "Options \"" + opt.desc(&Gm) + "\" and \"" + opt.desc(&Gp) +
         "\". The meaning of the value is defined by the -I|--meaning option.")
    .add(poptmx::OPTION, &mpinter, 'I', "meaning",
         "Meaning of " + opt.desc(&Gm) + " and " + opt.desc(&Gp) +
         " options.", Interp::optionDesc, mpinter.name())
    .add(poptmx::OPTION, &acof, 'S', "scale",
         "Data-to-angle conversion coefficient.",
         "Multiplication of this coefficient by the value in the first column of"
         " the rocking curve data file gives the actual angle in radians.",
         toString(acof))
    /*.add(poptmx::OPTION, &smooth, 'F', "filter-rc",
         "Smooth RC to avoid non-monotonous parts.",
         "The rocking curve in real experiment is never ideal and includes"
         " noise. If this option is used, the data of the rocking curve"
         " are first smoothed using very primitive algorithm. If you need"
         " some advanced smoothing algorithm, pre-process the data before"
         " using them.")*/;
  return opt;
}



const std::string EDEIoptions::fdOptionDesc =
  "The EDEI algorithm (described in \"A. Maksimenko, Nonlinear extension of the"
  " x-ray diffraction enhanced imaging, Appl. Phys. Lett. 90, 154106 (2007)\")"
  " makes use of the function derived from the RC of the analyzer (see"
  " function $F^{-1}$ defined in eq.(3)). The file assumes one virtual axis X"
  " which represents the value $x=(I_L-I_H)/(I_L-I_H)$ and can be constructed"
  " from the number of the line (n) in the file as follows: $x=2*(n-1)/(N-1)-1$,"
  " where N is the total number of lines in the file. The file itself must"
  " consist only of the strings of the form:\n"
  "    <alpha> <LH>\n"
  "where both elements are the floating-point numbers and <alpha> is the"
  " $\\alpha(x)=F^{-1}(x)$, while <LH> is the combination of the reflectivities"
  " on the low- and high-angle side of the RC $I_L+I_H$ in the corresponding"
  " $\\alpha(x)$ position.";

const std::string EDEIoptions::fdOptionShortDesc =
  "File containing processed RC used inside the EDEI.";


const std::string EDEIoptions::rcOptionDesc =
  "The absence of this option prompts to using the original"
  " DEI method.\n"
  "Text data file which contains the rocking curve of the analyzer"
  " in the EDEI experimental setup. Consists of two columns:"
  " X- and Y-axis of the RC. i.e. must contain only the lines of"
  " the form:\n"
  "    <RC X-axis> <RC Y-axis>\n"
  "where both numbers are float point representation of values"
  " which are respectively proportional to the RC scan angle and"
  " intensity. Note that the RC Y-axis does not have to be the"
  " scan angle (it can be motor pulses, encoder value, etc.), it"
  " only must be proportional to the scan angle. The same Y-azis"
  " does not have to be the reflectivity, but only must be"
  " proportional to it. However, if you want to have not only"
  " quantitative, but qualitative results (i.e. the results which"
  " represent the refraction index itself, not a value proportional"
  " to it), you should provide RC with exact data: X-axis in"
  " radians and Y-axis in reflectivity.";


const std::string EDEIoptions::rcOptionShortDesc =
  "Input file containing the RC of the analyzer.";

bool
_conversion (EDEIprocess::Component* _val, const std::string & in) {
  std::string _in = upper(in);
  if (_in == "A" || _in == "ABS" || _in == "ABSORPTION" )
    *_val= EDEIprocess::ABS;
  else if (_in == "R" || _in == "REF" || _in == "REFRACTION" )
    *_val= EDEIprocess::REF;
  else
    throw_error("EDEI component", "Unrecognized component\"" + _in + "\".");
  return true;
}






/// Determines index of the minus position.
///
/// Finds the actual index of the minus position on the RC from the general
/// value ::Gm and it's interpretation method.
///
/// @param Gm Value determining the minus position
/// @param mpinter It's interpretation.
/// @param alpha angles \f$\alpha\f$ representing X-axis of the \RC.
/// @param RC actual recorded intensities of the \RC (Y-axis).
///
/// @return Index of the minus position in the RC arrays.
///
static int
find_minus( float Gm, Interp mpinter,
            const Line & alpha, const Line & RC){

  int icur=0, maxlocRC = maxIndex(RC)(0);
  float minRC = min(RC), maxRC = max(RC);

  if ( mpinter == Interp::ALPHA ) {

  if ( Gm <= alpha(0)  ||  Gm >= alpha(maxlocRC) )
    throw_error("points",
          "Specified \\alpha of the minus point (" + toString(Gm) + ")"
          " is outside the RC minus slope"
          " [" + toString( Shape<2>(alpha(0), alpha(maxlocRC)) ) + "].");
  while ( alpha(icur) < Gm )
    icur++;
  return ( alpha(icur) - Gm > Gm - alpha(icur-1) ) ?
    icur - 1 : icur ;

  } else if ( mpinter == Interp::NUMBER ) {

  if ( Gm <= 1 || Gm >= maxlocRC )
    throw_error("points",
          "Specified position of the minus point (" + toString(Gm) + ")"
          " is outside the RC minus slope [1, " + toString(maxlocRC) + "].");
  return int( Gm-1 );

  } else if ( mpinter == Interp::REFLECT) {

  if ( Gm <= minRC/maxRC || Gm >= 1.0 )
    throw_error("points",
          "Specified reflectivity of the minus point (" + toString(Gm) + ")"
          " is outside the RC ranges [" + toString(minRC/maxRC) + ", 1].");
  return find_minus( Gm*maxRC, Interp::INTENS, alpha, RC );

  } else if ( mpinter == Interp::INTENS ) {

  if ( Gm < minRC || Gm > maxRC )
    throw_error("points",
          "Specified intensity of the minus point (" + toString(Gm) + ") is"
          " outside the RC ranges [" + toString(Shape<2>(minRC, maxRC)) + "].");
  while ( RC( icur ) < Gm )
    icur++;
  return RC(icur) - Gm > Gm - RC(icur-1) ?
    icur - 1 : icur;

  } else {

    throw_bug(__FUNCTION__);
    return 0;

  }

}


/// Determines index of the plus position.
///
/// Finds the actual index of the plus position on the RC from the general
/// value ::Gm and it's Interpretation method.
///
/// @param Gp Value determining the plus position
/// @param mpinter It's Interpretation.
/// @param alpha angles \f$\alpha\f$ representing X-axis of the \RC.
/// @param RC actual recorded intensities of the \RC (Y-axis).
///
/// @return Index of the plus position in the RC arrays.
///
static int
find_plus( float Gp, Interp mpinter,
           const Line & alpha, const Line & RC){

  int maxlocRC=maxIndex(RC)(0), icur=maxlocRC, size = alpha.size();
  float minRC = min(RC), maxRC = max(RC);

  if ( mpinter == Interp::ALPHA ) {

    if ( Gp <= alpha(maxlocRC) || Gp >= alpha(size-1) )
      throw_error("points",
          "Specified \\alpha of the plus point (" + toString(Gp) + ")"
          " is outside the RC plus slope"
          " [" + toString( Shape<2>( alpha(maxlocRC), alpha(size-1)) ) + "].");
    while ( alpha(icur) < Gp )
      icur++;
    return alpha(icur) - Gp > Gp - alpha(icur-1)  ?
      icur - 1 : icur ;

  } else if ( mpinter == Interp::NUMBER ) {

    if ( Gp <= maxlocRC || Gp >= size )
    throw_error("points",
          "Specified position of the plus point (" + toString(Gp) + ")"
          " is outside the RC plus slope"
          " [" + toString( Shape<2> (maxlocRC, size-1 ) ) + "].");
    return int( Gp-1 );

  } else if ( mpinter == Interp::REFLECT ) {

    if ( Gp <= minRC/maxRC || Gp >= 1.0 )
      throw_error("points",
          "Specified reflectivity of the plus point (" + toString(Gp) + ")"
          " is outside the RC ranges [" + toString(minRC/maxRC) + ", 1].");
    return find_plus( Gp*maxRC, Interp::INTENS, alpha, RC );

  } else if ( mpinter == Interp::INTENS ) {
    if ( Gp < minRC || Gp > maxRC )
      throw_error("points",
          "Specified intensity of the plus point (" + toString(Gp) + ") is"
          " outside the RC ranges [" + toString(Shape<2>(minRC, maxRC)) + "].");
    while ( RC( icur ) > Gp )
      icur++;
    return Gp - RC(icur) > RC(icur-1) - Gp ?
      icur -1 : icur;

  } else {

    throw_bug(__FUNCTION__);
    return 0;

  }

}


/// Tells if the function represented by the array has only one peak.
///
/// @param arr - array to find the peak in.
///
/// @return \c true if there is only one peak in the array, \c false otherwise.
///
static bool
has_one_peak(const Line &arr){
  //bool found = false;
  int icur = 0, size = arr.size();
  while ( arr(icur) <= arr(icur+1)  &&  icur < size-1 )
    icur++;
  if ( icur == size-1 )
    throw_error("find peak", "This is not RC.");
  while ( arr(icur) >= arr(icur+1)  &&  icur < size-1 )
    icur++;
  return icur == size-1;
}

/// Returns the sign of the float value.
///
/// @param val the value to determine the sign.
///
/// @return 0 if value is 0.0, -1 if negative, +1 if positive.
///
static inline int
msign(float val){
  if (val==0.0) return 0;
  if (val<0.0) return -1;
  if (val>0.0) return 1;
  throw_bug(__FUNCTION__);
  return 0;
}


/// Smooth the array.
///
/// Performs very primitive smoothing of the array values.
///
/// @param arr array to smooth.
///
static inline void
filterf(Line &arr){
  int size = arr.size();
  for ( int icur = 1 ; icur < size-1 ; icur++)
    if ( msign( arr(icur+1)-arr(icur) ) == msign( arr(icur-1)-arr(icur) ) )
      arr(icur) = ( arr(icur-1)+arr(icur)+arr(icur+1) ) / 3.0 ;
}


/// Finds the index of the value.
///
/// Finds the index of the array's value which is closest to the ::val.
/// The arrays values must be monotonously growing. Float part of the
/// returned number tells the relative position of the index between
/// two arrays values.
///
/// @param val value who's index to find.
/// @param arr array where to find the index.
///
/// @return index of the value in the array closest to ::val.
///
static float
findindex(float val, const Line & arr){
  int size = arr.size(), m = 0, M = size - 1, point;
  if ( val <= arr(0) ) {
    return 0;
  } else if ( val >= arr(size-1) ) {
    return size-1;
  } else {
    while ( M-m > 1 ) {
      point = ( M + m ) / 2;
      if ( val < arr(point) ) {
        M = point;
      } else if ( val > arr(point) ) {
        m = point;
      } else {
        return point;
      }
    }
    return m + (val-arr(m)) / (arr(M)-arr(m));
  }
}


/// Returns the value of the array.
///
/// Returns linear Interpolation of the array value.
///
/// @param index index of the value.
/// @param arr array with values.
///
/// @return array value.
///
static float
findvalue (float index, const Line &arr){
  int sizemo = arr.size()-1;
  if ( index == sizemo )
    return arr(sizemo);
  int pos = index;
  float rem = index - pos;
  return arr(pos) + rem*(arr(pos+1)-arr(pos)) ;
}


/// Returns minimum of three integers.
///
/// @param a In 1
/// @param b In 2
/// @param c In 3
///
/// @return Minimum of a, b and c.
///
static inline int
imin3(int a, int b, int c){
  int curmin = a<b ? a : b;
  return curmin<c ? curmin : c;
}


















const string EDEIprocess::modname = "RC arguments";



///
/// @param RCname Name of the file containing the \RC of the analyzer.
/// @param _Gm Value determining the minus position.
/// @param _Gp Value determining the plus position.
/// @param _mpinter Interpretation of the ::_Gm, ::_Gp.
/// @param _smooth Smooth RC to avoid non-mono regions.
/// @param _acof X-axis (angle) conversion factor.
///
EDEIprocess::EDEIprocess(const Path & RCname, float _Gm, float _Gp, Interp _mpinter,
                 bool _smooth, float _acof)
  : acof(_acof), Gp(_Gp), Gm(_Gm), mpinter(_mpinter), smooth(_smooth)
{
  AL.resize(function_size);
  PM.resize(function_size);
  if ( RCname.empty() )
  throw_error(modname, "RC is empty.");
  loadRC(RCname);
}


/// Extracts refraction component from the intensities recorded
/// on both sides of the RC.
///
/// @param minus contrast in minus position.
/// @param plus contrast in plus position.
/// @param comp component to be extracted.
///
/// @return refraction component.
///
float
EDEIprocess::extract(float minus, float plus, Component comp) const {
  float PpM = minus + plus;
  float enter =  (PpM == 0.0)  ?  0.0  :  (minus-plus)/PpM ;
  float position = 0.5 * (enter+1) * (function_size-1);
  int pos = position;
  float rem = position - pos;
  return comp == ABS ?
    PpM / ( PM(pos) + rem * ( PM(pos+1)-PM(pos) ) ) :
    AL(pos) + rem * ( AL(pos+1)-AL(pos) );
}


/// Loads \RC, processes it and makes the class ready for the data extraction.
///
/// @param filename \RC file
///
void EDEIprocess::
loadRC(const Path & filename) const {

  if (!acof)
  throw_error( modname, "Alpha axes scale factor is zero.");

  Line alpha, RC;
  LoadDataM(filename, &alpha, &RC, (Line*) 0);
  alpha *= acof ? acof : 1.0 ;

  int size = RC.size();
  if ( size <= 3 )
    throw_error("read rc", "Size of the input RC file is less than 3.");

  // temporal points to prevent changes in Gp, Gm.
  float tGm = Gm, tGp = Gp;

  // reverses the arrays if alpha axes is decreasing
  if ( alpha(0) > alpha(size-1) ){
    if ( mpinter == Interp::NUMBER ) {
      warn("read rc", "The alpha axis is decreasing and Interpretation of"
           " the +/- points set to '" + mpinter.name() + "'."
           " Make sure you know what you are doing.");
      tGm = size - 1 - tGm;
      tGp = size - 1 - tGp;
    }
    alpha.reverseSelf(blitz::firstDim);
    RC.reverseSelf(blitz::firstDim);
  }

  int
  Pminus = find_minus(tGm, mpinter, alpha, RC),
  Pplus  = find_plus (tGp,  mpinter, alpha, RC);

  // trivial check and RC normalization
  if( min(RC) < 0.0 )
    throw_error("read rc", "Input RC includes sub-zero data: no sense.");
  if( min(RC) == max(RC) )
    throw_error("read rc", "All data in the input RC are equal: no scene.");
  RC /= max(RC);

  // tries to smooth RC
  if (smooth)
    while ( ! has_one_peak(RC) )
      filterf(RC);

  // build interim arrays
  int new_size = imin3( Pplus-Pminus, Pminus, size-1-Pplus );
  int thissize = 2 * new_size - 1;
  Line _FA( thissize );
  Line _ppm( thissize );
  float rp, rm, rp0=RC(Pplus), rm0=RC(Pminus);
  for ( int icur = 0 ; icur < thissize ; icur++ ) {
    rp = RC( Pplus  - new_size + icur ) / rp0;
    rm = RC( Pminus - new_size + icur ) / rm0;
    _ppm(icur) = rp + rm ;
    _FA(icur) = (rm-rp) / _ppm(icur);
  }

  // monotonous limits
  int minloc = minIndex(_FA)(0), maxloc = maxIndex(_FA)(0);
  Line ppm =  _ppm( blitz::Range(minloc, maxloc) );
  Line FA  =   _FA( blitz::Range(minloc, maxloc) );

  float dalpha = alpha(1)-alpha(0);
  for (int icur = 0 ; icur < function_size ; icur++){
    float val = 2.0 * icur / (function_size-1) - 1;
    float index = findindex( val , FA );
    PM(icur) = findvalue (index, ppm);
    AL(icur) = dalpha * (minloc+index-new_size);
  }

}



/// \brief Saves processed \FD to file
///
/// @param filename Name for the file to save \FD to.
///
void
EDEIprocess::saveFD(const Path & filename) const {
  SaveData(filename, &AL, &PM, 0);
}

const std::string EDEIprocess::componentDesc =
  "Must be one of the following strings (case insensitive):\n"
  "A, ABS, ABSORPTION - for the absorption component\n"
  "R, REF, REFRACTION - for the refraction component";



/// @}
