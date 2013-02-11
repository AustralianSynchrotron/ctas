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
/// @brief Definitions of the functions declared in experiment.h.
///

#include "experiment.h"
#include "flatfield.h"
#include "poptmx.h"
#include <algorithm>
#include <unistd.h>

using namespace std;





/// \brief Parses input file.
///
/// Parses input file as it is described in \ref AqSeries and constructs the
/// foreground and background arrays.
///
/// @param filename File with the foreground-background pairs.
/// 
AqSeries::AqSeries(const Path & filename){

  if (filename.empty()) // empty list
    return;

  FILE* input_list = fopen(filename.c_str(), "r");
  if ( ! input_list )
    throw_error (modname, "Could not open input data file"
				 " \"" + filename + "\" for reading.");
  const int MAXSTRING = FILENAME_MAX + 1 + 16 + 1;
  char curin[MAXSTRING];
  int bgindex = nobg;
  string fdir=filename.dir(), curstring;
  
  while ( fgets (curin, MAXSTRING, input_list) ) {

    curstring = string(curin);
	if ( ! curstring.empty() && *(curstring.end()-1) == '\n')
      curstring.erase(curstring.end()-1);

	if ( curstring.find_first_not_of(" \t") != string::npos ) {
	  if ( isbgstring(curstring) ) {
		curstring = bgname(curstring);
		if ( curstring.empty() ) {
		  bgindex = nobg;
		} else {
            // Windows version is likely to fail here. See cdpath in common.cpp
		  bgs.push_back( cdpath(fdir, curstring) );
		  bgindex = bgs.size()-1 ;
		}
      } else if ( isdcstring(curstring) ) {
        curstring = dcname(curstring);
		if ( ! curstring.empty() )
		  dcs.push_back( cdpath(fdir, curstring) );
      } else if ( iscomment(curstring) ) {
        // Do nothing
	  } else {
		fgs.push_back( fgelement( cdpath(fdir, curstring), bgindex ) );
	  }
	}

  }

  fclose(input_list);

  if ( ! thetas() )
	throw_error (modname, "Empty input data file '" + filename + "'.");

  sh = ImageSizes(fg(0));

}



/// \brief Tells if the string describes the background.
///
/// @param str The string to check
///
/// @return \c true if the string describes the background, \c false otherwise.
///
bool
AqSeries::isbgstring(const string & str) const {
  return ! str.compare(0, BGPREFIX.length(), BGPREFIX);
}

/// \brief Tells if the string describes the dark current.
///
/// @param str The string to check
///
/// @return \c true if the string describes the dark current, \c false otherwise.
///
bool
AqSeries::isdcstring(const string & str) const {
  return ! str.compare(0, DCPREFIX.length(), DCPREFIX);
}

/// \brief Tells if the string is a comment.
///
/// @param str The string to check
///
/// @return \c true if the string is a comment or empty, \c false otherwise.
///
bool
AqSeries::iscomment(const string & str) const {
  return str.empty() || ! str.compare(0, 1, "#");
}


/// \brief extracts the background name from the background string.
///
/// @param str The string to extract from.
///
/// @return The background filename.
///
Path
AqSeries::bgname(const string & str) const {
  return isbgstring(str) ? str.substr( BGPREFIX.length() ) : string("") ;
}

/// \brief extracts the dark current name from the dc string.
///
/// @param str The string to extract from.
///
/// @return The dark current filename.
///
Path
AqSeries::dcname(const string & str) const {
  return isdcstring(str) ? str.substr( DCPREFIX.length() ) : string("") ;
}


/// \brief Checks if the index is within the allowed region.
///
/// @param idx The index to check.
///
/// @return The index.
///
int
AqSeries::index(int idx) const  {
  if ( idx >= thetas() )
	throw_error (modname, "Bad index " + toString(idx) +
				 " is outside the list of size (" + toString(thetas()) + ")");
  return idx;
}

/// \brief Foreground name.
///
/// @param idx Projection index.
///
/// @return The name of the (image) file containing the foreground.
///
const Path
AqSeries::fg(int idx) const {
  return fgs[index(idx)].first;
}


/// \brief Background name.
///
/// @param idx Projection index.
///
/// @return The name of the (image) file containing the background
///         corresponding to the projection index.
///
const Path
AqSeries::bg(int idx) const {
  index(idx);
  return ( fgs[idx].second == nobg ) ? Path() : bgs[ fgs[idx].second ];
}


/// Constructs the dark current map from all dark-current images in the input file
/// and puts it into the ::dcc array.
///
/// @param dcc resulting array.
/// @param sliceV
///
void
AqSeries::darkCurrent(Map & dcc, const std::vector<int> & sliceV) const {

  Shape dsh = sliceV.size() ? Shape(sliceV.size(), sh(1)) : sh ;
  dcc.resize(dsh);
  dcc=0.0;

  for (int icur=0 ; icur < dcs.size() ; icur++) {
    Map dct(sh);
    ReadImageLine(dcs[icur], dct, sliceV, sh);
    dcc += dct;
  }
  if (dcs.size())
    dcc /= dcs.size();
}


/// Constructs the flat-field subtracted projection from the experimental data
/// at given rotation position ::itheta and puts it into ::proj array.
///
/// @param idx rotation position.
/// @param proj the array to put data into.
/// @param sliceV array of slices to be extracted.
///
void AqSeries::projection(int idx, Map & proj, const std::vector<int> & sliceV)  const {

  index(idx);
  static vector<int> slices;
  static Shape dsh;
  static Map dcc, bgc;
  static string bgname;

  if ( slices != sliceV ) {
    if ( sliceV.back() >= sh(0) )
      throw_error(modname, "The maximum slice in the requested list of slices"
                  " (" + toString(sliceV.back()+1) + ") is outside the image height"
                  " (" + toString(sh(0)) + ").");
    slices=sliceV;
    dsh = sliceV.size() ? Shape(sliceV.size(), sh(1)) : sh ;
    bgc.resize(dsh);
    bgc = 1.0;
    darkCurrent(dcc, slices);
    bgname = "";
  }

  if ( bg(idx) != bgname ) {
    bgname = bg(idx);
    if (bgname.empty())
      bgc = 1.0;
    else
      ReadImageLine(bgname, bgc, sliceV, sh);
  }

  Map fgc(sh);
  ReadImageLine(fg(idx), fgc, sliceV, sh);

  flatfield(proj, fgc, bgc, dcc);

  return;

}

const std::string AqSeries::modname  = "acquisition series";
const std::string AqSeries::BGPREFIX = "#BACKGROUND# ";
const std::string AqSeries::DCPREFIX = "#DARKCURRENT# ";
const std::string AqSeries::Desc =
  "Text file which describes the stack of input images: foregrounds and (optionally)"
  " background(s). The strings in the file can be one of the following forms:\n"
  "    " + AqSeries::BGPREFIX + "<background filename>\n"
  "The string prefixed with the \"" + AqSeries::BGPREFIX + "\" gives the name of the"
  " background file right after the prefix. This background file contains the background"
  " contrast for all foregrounds below it until the next background string is met."
  " No background removal is performed for all foregrounds given in the strings"
  " above the first background. If the string starting with the prefix does not"
  " contain any background name then all following foregrounds are considered as"
  " is ( i.e. no background removal is performed).\n"
  "    " + AqSeries::DCPREFIX + "<dark current filename>\n"
  "The string prefixed with the \"" + AqSeries::DCPREFIX + "\" gives the name of"
  " the dark current file right after the prefix. If more than one DC file is"
  " given, then the average of all are used.\n"
  "    <foreground filename>\n"
  "The foreground string gives the name of the actual contrast of the sample.\n"
  "    # <comment>\n"
  "The string starting with '#' sign, is treated as a comment.\n"
  "    <empty string>\n"
  "Empty strings can be used for grouping and have no influence.";






const string MaskDesc =
  "Used when there is a need to describe a stack of output files. In this case"
  " the special character represents the number of the output file in the stack."
  " The result mask is a string which forms the names of the processed slices."
  " The mask should (but not must) contain the '@' character which denotes the"
  " position where the slice number will be inserted:\n"
  "    If there is no '@' in the mask, it is inserted together with the '-'"
  " prefix right before the file extension (if any) or at the end of the mask"
  " (if no extension).\n"
  "    If there are more than one '@' character in the mask, only the last of"
  " them is replaced by the slice number.\n"
  "For example the mask\n"
  "    ../path/to/@/result/rec-@-@@.png\n"
  "produces file names similar to this (for the 13th slice):\n"
  "    ../path/to/@/result/rec-@-@013.png\n"
  "Note that the path(s) to the files must exist: the program will not create"
  " any directory.";

string
mask2format(const string & mask, int maxslice){
  string format(mask);
  // replace all '%' by "%%"
  string::size_type pos = format.find('%');
  while ( pos != string::npos ) {
	format.insert(pos, "%");
	pos = format.find('%', pos+2);
  }
  //replace last '@' with the format expression.
  format.replace( format.rfind('@'), 1,
				  "%0" + toString( toString(maxslice).length() ) + "u");
  return format;
}





const string SliceOptionDesc=
  "The string describing the slices in the image stack which are to be"
  " processed. Must be a string consisting only of numbers, commas ','"
  " minus signs '-' and the character 'n'. First the string is divided"
  " into the substrings separated by comas and each substring is"
  " processed on it's own:\n"
  "    The string consisting only of numbers is read as the integer and"
  " added to the list of the reconstructed slices.\n"
  "    The string with the minus sign surrounded by the numbers adds all"
  " slices in the range into the list.\n"
  "    If the string starts with the minus then the range start is"
  " assumed to be 1.\n"
  "    If minus is the last character in the string then the range"
  " finishes at the maximum slice number.\n"
  "    If the string has negation prefix 'n' then the slice(s) are"
  " excluded from the previously formed list.\n"
  "    If all substrings have 'n' prefix or the first substring contains"
  " only it, then the meaning of the whole string is \"all except ...\".\n"
  "    Two and more negations are interpreted as a single one.\n"
  "    If no slice string is given then all slices are reconstructed.\n"
  "For example the following string:\n"
  "    9,-4,6,20-400,n3,500-440,n450-470,800-,n920-910,915\n"
  "requests processing of the slices with numbers 1, 2, 4, 6, 9, 20 to 400,"
  " 440 to 449, 471 to 500, 800 to 909, 915 and 921 to the end.";


/// \brief Adds or removes the element into the array.
///
/// Adds or removes the element numb to/from the array depending on negation.
///
/// @param arr The array to be updated.
/// @param numb The element to be added/removed to/from the array.
/// @param negate If \c true then removes, if \c false then adds to the array.
///
/// @return Updated array.
///
static inline vector<int> &
rmadd(vector<int> & arr, int numb, bool negate){
  if (negate)
	arr.erase( remove( arr.begin(), arr.end(), numb ), arr.end() );
  else
	arr.push_back(numb);
  return arr;
}

/// \brief Parses the string to non-negative integer.
///
/// @param str The string to parse.
///
/// @return The result of parsing.
///
static inline int str2n(const string & str){
  long idx = strtol ( str.c_str() , 0, 0);
  if ( ! idx )
	warn("string to int", "Zero slice. Must be positive. Set to 1.");
  return idx ? idx-1 : 1;
}

vector<int>
slice_str2vec(const string & sliceS, int hight){

  const char negatec = 'n';
  const string permitted_chars = string("0123456789,-") + negatec ;

  vector<int> sliceV; // the array to be returned as the result.
 
  // empty string
  if ( sliceS.empty() ) {
	for (int slice = 0 ; slice < hight ; slice++)
	  sliceV.push_back(slice);
	return sliceV;
  }


  // construct the vector of substrings and checks for the global negation.
  vector<string> subSV;
  bool negateall = true; // turns to false if an unnegated substring found.
  string::size_type startidx=0, endidx;
  do{

	// extract substring
	endidx = sliceS.find(',', startidx);
	string subS = sliceS.substr(startidx, ( endidx == string::npos ?
											sliceS.length() :
											endidx-startidx) );
	startidx=endidx+1;

	// checks for the non permitted characters
	if ( subS.find_first_not_of(permitted_chars) != string::npos ) {
	  warn("slice string", "Substring \""+ subS +"\" in the "
		   " string describing set of slices to be reconstructed:\n"
		   + sliceS +"\n"
		   "has character(s) not from the permitted set"
		   " \""+ permitted_chars +"\". Skipping the substring.");
	  subS.erase();
	}

	// check it and add to the vector of substrings.
	if ( ! subS.empty() ) {

	  const string initS = subS;

	  // modifies in regards to the negatec
	  string::size_type lastneg = subS.rfind(negatec);
	  if ( lastneg != string::npos ) {
		if ( lastneg != 0 ) {
		  warn("slice string", "Suspicious substring \""+ initS +"\" in the"
			   " string describing set of slices to be reconstructed:\n"
			   + sliceS +"\n"
			   "it has '" + negatec + "' character not in the first position."
			   " Moving it to the begining. Is it what you meant?");
		  // moves all negatec's to the beginning and erases it's duplicates.
		  subS.erase
			( subS.begin(),
			  stable_partition( subS.begin(), subS.end(),
								bind2nd(equal_to<char>(),negatec) ) - 1 );
		}
	  } else {
		negateall = false;
	  }

	  // modifies in regards to '-'
	  if ( count(subS.begin(), subS.end(), '-') > 1 ) {
		warn("slice string", "Substring \""+ initS +"\" in the "
			 " string describing set of slices to be reconstructed:\n"
			 + sliceS +"\n"
			 "has more than one minus sign '-'. Everything between first"
			 " and last minuses is ignored. Is it what you meant?");
		subS = subS.substr(0,subS.find('-')) + subS.substr(subS.rfind('-') );
	  }
	  // make sure minus is surrounded by numbers.
	  if ( subS[0] == '-' )
		subS = "1" + subS;
	  if ( subS.substr(0,2) == "n-" )
		subS.insert(1,"1");
	  if ( subS[subS.length()-1] == '-' )
		subS = subS + toString(hight);

	  subSV.push_back(subS);

	}

  } while ( string::npos != endidx );


  // Check for global negation.
  if ( negateall |= ( subSV[0] == string(1,negatec) ) )
	for ( int icur = 0 ; icur < hight ; icur++)
	  sliceV.push_back(icur);
  subSV.erase( remove( subSV.begin(), subSV.end(), string(1,negatec) ),
			   subSV.end() ); // no "negate" strings

  // adds/removes substrings into the array of slices
  vector<string>::iterator subSVi = subSV.begin();
  const vector<string>::iterator subSVe=subSV.end();


  while ( subSVi != subSVe ) {

	bool negatethis = (*subSVi)[0]==negatec;
	if (negatethis) (*subSVi).erase(0,1);
	negatethis |= negateall;

	string::size_type minuspos = (*subSVi).find('-');
	if ( minuspos != string::npos ) {
	  int
		rangeB = str2n( (*subSVi).substr(0,minuspos) ),
		rangeE = str2n( (*subSVi).substr(minuspos+1) );
	  if ( rangeB > rangeE ) swap(rangeB,rangeE);
	  if ( rangeB == rangeE )
		warn("slice string", "One of the substrings with ranges"
			 " in the string describing set of slices to be reconstructed:\n"
			 + sliceS +"\n"
			 "has equal ends of the ranges. Is it what you meant?");
	  for (int curS = rangeB ; curS <= rangeE ; curS++ )
		rmadd(sliceV, curS, negatethis);
	} else {
	  rmadd(sliceV, str2n( *subSVi ), negatethis );
	}

	subSVi++;

  }

  // sort and remove duplicates, too large numbers
  sort(sliceV.begin(), sliceV.end());
  sliceV.erase( unique( sliceV.begin(), sliceV.end() ), sliceV.end() );
  if ( sliceV.back() > hight )
	warn("slice string",
		 "The string describing set of slices to be reconstructed:\n"
		 + sliceS +"\n"
		 "Includes slices more than the height of the input image"
		 " (" + toString(hight) + "). These slices are ignored." );
  sliceV.erase( find_if( sliceV.begin(), sliceV.end(),
						 bind2nd( greater<int>(), hight ) ),
				sliceV.end() );

  // last check
  if ( sliceV.empty() )
	warn("slice string",
		 "The string describing set of slices to be reconstructed:\n"
		 + sliceS +"\n"
		 "leads to the empty range of slices." );

  return sliceV;

}















const string SinoS::modname = "sinogram array";

#ifdef _WIN32
#include "experiment.win32.cpp"
#else
#include <sys/mman.h>
#endif

#include <string.h>


/// \brief Prepares the temporary file to map the memory to.
///
/// @param size Size of the memory region to be mapped.
///
/// @return The file descriptor
///
inline static int
tempfile(const off_t size){

  const char * tmpenv =  "TMPDIR";
  const string tempdesc =  "You can control the path to the temporary file via"
	" the environment variable \"" + (string) tmpenv + "\".";
  const char * TMPDIR = getenv (tmpenv);
  if ( ! TMPDIR ) TMPDIR = "./" ;
  if ( strlen(TMPDIR) >= 248 ) { // little under the maximum length (256)
	warn("temp file", (string)
		 "The environment variable \"" + tmpenv + "\" is too long:\n"
		 + TMPDIR + "\n"
		 "Switching to the default one \"./\". " + tempdesc);
	TMPDIR = "./";
  }
  char tmpfilename[256]; // maximum file name length
  strcpy(tmpfilename, TMPDIR);
  if ( tmpfilename[ strlen(tmpfilename)-1 ] != '/' )
	strcpy(tmpfilename+strlen(tmpfilename), "/");
  strcpy(tmpfilename+strlen(tmpfilename), "XXXXXX");

  int filedesc = mkstemp(tmpfilename);
  if ( filedesc < 0 )
	throw_error("temp file", "Could not open temporary file"
				" \"" + string(tmpfilename) + "\". " + tempdesc);
  if ( unlink(tmpfilename) < 0 )
	warn("temp file", "Could not unlink temporary file"
		 " \"" + string(tmpfilename) + "\". Don't forget"
		 " to delete it after the program has finished.");
  if ( lseek(filedesc, size, SEEK_SET) != size ||
	   write(filedesc, "END", 3) != 3 ) {
	// Should I pad the holes ???
	close (filedesc);
	throw_error("temp file", "Could not set size of the  temporary file"
		    " \"" + string(tmpfilename) + "\". You need " + toString((long unsigned)size) +
                "bytes on the hard disk. " + tempdesc);
  }

  return filedesc;

}

/// \brief Constructs the 3D array from the absorption contrast projections.
///
/// @param exp The experiment.
/// @param sliceV List of slices of the interest.
/// @param _verb Show progress bar.
///
SinoS::SinoS(const Experiment & exp, const vector<int> & sliceV, bool _verb)
  : verb(_verb) {

  if ( ! exp.thetas() )
	throw_error(modname, "Empty experiment.");

  slcs = sliceV.size() ? sliceV.size() : exp.slices();
  thts = exp.thetas();
  pxls = exp.pixels();

  if ( sliceV.back() >= exp.slices() )
	throw_error(modname, "The maximum slice in the requested list of slices"
				" (" + toString(sliceV.back()+1) + ") is outside the image height"
				" (" + toString(exp.slices()) + ").");
  allpix = long(thts) * long(slcs) *long(pxls);

  // Create this huge array of input data
  mapfile = 0;
  blitz::TinyVector<long, 3> datashp(thts, slcs, pxls);
  try { data.resize(datashp); }
  catch(bad_alloc err) {
	warn (modname, "Failed to allocate memory."
		  " Trying to play with the memory mapped to file.");
	mapfile = tempfile( allpix*sizeof(float) );
	float * datap = (float*)
	  mmap( 0, allpix*sizeof(float),
			PROT_WRITE|PROT_READ, MAP_SHARED, mapfile, 0);
	data.reference( Volume(datap, datashp, blitz::neverDeleteData) );
  }


  // Read images to the array.
  Map proj(Shape(slcs,pxls));
  ProgressBar bar(verb, "reading projections", thts);
  for ( int curproj = 0 ; curproj < thts ; curproj++){
    exp.projection(curproj, proj, sliceV);
	data(curproj, blitz::Range::all(), blitz::Range::all()) = proj;
	bar.update();
  }

}


SinoS::~SinoS(){
  if (mapfile) {
	munmap(data.data(), allpix*sizeof(float) );
	close(mapfile);
  } else {
	data.free();
  }
}

/// \brief Forms the sinogram.
///
/// @param idx Index of the sinogram to form.
/// @param storage Storage for the sinogram.
///
void
SinoS::sino(int idx, Map & storage) const {
  index(idx);
  storage.resize(thts, pxls);
  storage = data
	(blitz::Range::all(), idx, blitz::Range::all()).copy();
}


/// \brief Checks if the index is within the allowed region.
///
/// @param idx The index to check.
///
/// @return The index.
///
int
SinoS::index(int idx) const  {
  if ( idx<0 || idx >= slcs )
	throw_error (modname, "Bad index " + toString(idx) +
				 " is outside the slices (" + toString(slcs) + ")");
  return idx;
}


int
SinoS::pixels() const {
  return pxls;
}

int
SinoS::slices() const {
  return slcs;
}

int
SinoS::thetas() const {
  return thts;
}
















/// \brief Constructs the class from the string (read ::DcenterOptionDesc).
///
/// @param centerdesc String describing the rotation axis deviation.
///
Dcenter::Dcenter(const string & centerdesc) {
  acof = 0.0;
  bcof = 0.0;

  const char *start = centerdesc.c_str();
  char *tail;
  bcof = strtod (start, &tail);
  if (tail == start)
	throw_error("dcenter", "String \""+ centerdesc +"\""
				" could not be decomposed to any float number.");
  if ( *tail ) {
	if ( ! *(start=tail+1) ) {
	  warn("dcenter",
		   "Second part of the string \""+ centerdesc +"\" is empty.");
	  return;
	}
	acof = strtod (start, &tail);
	if (tail == start)
	  throw_error("dcenter", "Second part of the string \""+ centerdesc +"\""
				  " : \"" + start + "\" could not be decomposed to any"
				  " float number.");
  }
  if ( *tail )
	warn("dcenter",
		 "String \""+ centerdesc +"\", after decomposed into float(s)"
		 " has remaining tail \""+tail+"\".");

};


/// \brief Default constructor.
///
/// @param _b Coefficient bconf.
/// @param _a Coefficient aconf.
///
Dcenter::Dcenter(float _b, float _a)
  : acof(_a), bcof(_b) { }

/// \brief Calculates the axis deviation.
///
/// @param slice The index of the slice.
///
/// @return Calculated deviation of the rotation axis.
///
float
Dcenter::operator()(int slice) const {
  return bcof + acof*slice ;
}


const string DcenterOptionDesc=
  "Deviation of the rotation axis from the center of the sinogram.\n"
  "Allows you to erase some minor artifacts in the reconstructed image"
  " which come from the inaccurate rotation position. The two float"
  " values are the coefficient \"bconf\" and \"aconf\" respectively for the"
  " calculation of the deviation of the rotation center as\n"
  "    center = bconf + aconf * slice\n"
  "with the \"slice\" being the index in the projection image. If only"
  " first of two floats is given then aconf=0.0 (i.e. the rotation axis"
  " is strictly vertical).\n"
  "Note: in most real CT or TS experiments the rotation center of the sample"
  " is not exactly in the center of the recoded projection. Moreover,"
  " the rotation axis may be inclined from the vertical orientation."
  " If the inclination of the rotation axis is noticeable for your desired"
  " spatial resolution then you should rotate the images before the actual"
  " reconstruction. You can do it in the batch using \"mogrify\" command"
  " from the ImageMagick package with the -rotate option.";


string
type_desc (Dcenter*){
  return "FLOAT[:FLOAT]";
}

bool
_conversion (Dcenter* _val, const string & in) {
  *_val=Dcenter(in);
  return true;
}
