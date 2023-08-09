#include "physical.world.h"
#include "external.world.h"


using namespace std;

const string
Contrast::modname="contrast";

/// \brief Constructor.
///
/// @param _cn Contrast type.
Contrast::Contrast(Cntype _cn)
  : _contrast(_cn) {}

/// \brief Constructor from name
///
/// @param _name Contrast name.
Contrast::Contrast(const string & _name) {
  string name = upper(_name);
  if ( name == "A" || name == "ABS" || name == "ABSORPTION" ) _contrast = ABS;
  else if ( name == "R" || name == "REF" || name == "REFRACTION" ) _contrast = REF;
  else if ( name == "P" || name == "PHS" || name == "PHASE" ) _contrast = PHS;
  else if ( name == "F" || name == "FLT" || name == "FILTERED" ) _contrast = FLT;
  else throw_error("contrast type", "The string \""+ _name +"\""
                                   " does not describe any known contrast.");
}

/// \brief Type of contrast
///
/// @return Type of contrast
///
Contrast::Cntype
Contrast::contrast() const {
  return _contrast;
}


/// \brief Name of the contrast.
///
/// @return Name of the contrast.
///
string
Contrast::name() const {
  switch (_contrast) {
  case REF:     return "REFRACTION";
  case ABS:     return "ABSORPTION";
  case PHS:     return "PHASE";
  case FLT:     return "FILTERED";
  default :
          throw_bug(__FUNCTION__);
          return "";
  }
}

const std::string Contrast::Desc =
            "Must be one of the following strings (case insensitive):\n"
            "A, ABS, ABSORPTION - for the absorption contrast\n"
            "R, REF, REFRACTION - for the refraction contrast\n"
            "P, PHS, PHASE      - for the phase contrast\n"
            "F, FLT, FILTERED   - for the pre-filtered contrast";



bool
operator==(const Contrast & a, const Contrast & b){
  return a.contrast() == b.contrast();
}


bool
operator!=(const Contrast & a, const Contrast & b){
  return a.contrast() != b.contrast();
}

string
type_desc (Contrast*){
  return "STRING";
}

int
_conversion (Contrast* _val, const string & in) {
  *_val=Contrast(in);
  return 1;
}


void deAbs(Map & arr) {
  if ( blitz::min(arr) <= 0.0 ) {
    //warn("unzero", "Minimum in the array is sub-zero."
    //     " This should never happen with the absorption data.");
    const float mina = blitz::max(arr)/1000000.0;
    if ( mina <= 0.0 )
      throw_error ("unzero", "Bad absorption data (maximum is sub-zero).");
    for (auto icur = arr.begin() ; icur != arr.end() ; icur++ )
      if ( *icur <= mina )
        *icur = mina;
  }
  arr = -log(arr);
}



const string CenterOptionDesc=
  "Deviation of the rotation axis from the center of the sinogram.\n"
  "Allows you to erase some minor artifacts in the reconstructed image"
  " which come from the inaccurate rotation position. In most real"
  " CT or TS experiments the rotation center of the sample"
  " is not exactly in the center of the recoded projection. Moreover,"
  " the rotation axis may be inclined from the vertical orientation."
  " If the inclination of the rotation axis is noticeable for your desired"
  " spatial resolution then you should rotate the images before the actual"
  " reconstruction. You can do it in the batch using \"mogrify\" command"
  " from the ImageMagick package with the -rotate option.";

const std::string NeedForQuant =
  "The value is required only to produce the physically correct output."
  " If not set the results are qualitative rather than quantitative.";

const std::string ResolutionOptionDesc =
  "Size of the image pixel in microns. If not present, the program attempts to"
  " to calculate it from the image dpi." + NeedForQuant;

