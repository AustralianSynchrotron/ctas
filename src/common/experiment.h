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
/// @brief Some clases and functions common for the all-in-one tools
///        (i.e. tools which can do several operations, like background
///        removal, sinogram construction, ct serially).
///

#ifndef _H_EXPERIMENT_H_
#define _H_EXPERIMENT_H_

#include "common.h"
#include "flatfield.h"
#include <vector>

#ifdef _WIN32

//#pragma warning(disable: 4251)
#ifdef EXPERIMENT_EXPORTS
#define EXPERIMENT_API __declspec(dllexport)
#else
#define EXPERIMENT_API __declspec(dllimport)
#endif

#else

#define EXPERIMENT_API

#endif


/// \defgroup experiment Experiment.
///
/// Functions and classes responsible for the processing a series of images .
///
/// @{



/// Description of the output result mask.
extern EXPERIMENT_API const std::string MaskDesc;


/// \brief Converts the mask string to the format string.
///
/// @param mask Mask string (read ::MaskDesc).
/// @param maxslice Maximum allowed slice (total number of slices).
///
/// @return The format string which ready to be used with the fmt2s() function.
///
std::string EXPERIMENT_API
mask2format(const std::string & mask, int maxslice);


/// Description of the option giving the string of slices.
extern EXPERIMENT_API const std::string SliceOptionDesc;


/// \brief Constructs the array of slices.
///
/// Constructs the array of slices to be processed from the slice string
/// (read ::SliceOptionDesc for more details).
///
/// @param sliceS Input ::SliceOptionDesc "slice string".
/// @param hight Height of the input projection(s).
///
/// @return The vector consisting of the numbers of slices to be processed.
///         The vector is sorted and duplicates are removed.
///
std::vector<int> EXPERIMENT_API
slice_str2vec(const std::string & sliceS, int hight);



/// \brief Parsing file of the acquisition series.
///
/// This class parses and stores the list of foreground-background pairs
/// (read ::Desc for more details).
///
class EXPERIMENT_API AqSeries {

private:

  static const std::string modname;	///< Module name.
  static const int nobg = -1;	///< "Magick number" for the "no background".

  const static std::string BGPREFIX;  ///< Background prefix.
  const static std::string DCPREFIX;  ///< Dark current prefix.

  /// Element of the foregrounds array:
  /// the foreground itself and the index of the corresponding background.
  typedef std::pair< Path, int > fgelement;

  std::vector<fgelement> fgs;	///< Storage of foregrounds.
  std::vector<Path> bgs;		///< Storage of backgrounds.
  std::vector<Path> dcs;        ///< Dark current images.

  bool isbgstring(const std::string & str) const; ///< Tells if the string represents bgnd.
  bool isdcstring(const std::string & str) const; ///< Tells if the string represents dc.
  bool iscomment(const std::string & str) const; ///< Tells if the string is a comment.
  Path bgname(const std::string & str) const; ///< Extracts filename from the bgdn string.
  Path dcname(const std::string & str) const; ///< Extracts filename from the dc string.
  int index(int idx) const ;	///< Checks correctness of the index.

  Shape sh;                     ///< Shape of the input images.

  /// Constructs the dark current array.
  void darkCurrent(Map & dcc, const std::vector<int> & sliceV) const ;

public:

  AqSeries(const Path & filename); ///< Constructs from file.

  inline Shape shape() const {return sh;} ///< Shape of the projection image.
  inline long thetas() const {return fgs.size();} 			///< Total number of projections.
  inline long pixels() const {return sh(1);} 			///< Width of the image.
  inline long slices() const {return sh(0);} 			///< Number of slices (hight) in the image.
  inline long size() const {return sh(0)*sh(1)*fgs.size();}	///< Total size of the data.


  const Path fg(int idx) const ; ///< Gives name of the foreground.
  const Path bg(int idx) const ; ///< Gives name of the background.
  inline const std::vector<Path> & dc() const {return dcs;} ///< Gives the array of DC images.



  /// Constructs one cleaned projection of the experimental data.
  void projection(int idx, Map & proj,
                  const std::vector<int> & sliceV = std::vector<int>() ) const ;

  const static std::string Desc; ///< Description of the file with the AqSeries.

};









/// \brief Parent abstract class for various experimental techniques.
///
/// When you need to create a new type of experiment, just inherit from this class
/// with implementation of the constructor and projection function. See DEIexp, EDEIexp and
/// ABSexp as the examples.
class Experiment {
protected:
  long thts;					///< Number of projections (sinogram height).
  Shape sh;                     ///< Shape of the projections.
  Contrast cntr;           ///< The contrast type.

public:

  /// Constructor
  /// @param _sh Size of the projection.
  /// @param _thetas Number of projections.
  inline Experiment(const Shape & _sh, long _thetas) :
    sh(_sh),
    thts(_thetas)
    {
    if ( ! thts )
      throw_error("Experiment", "Empty list of input files.");
    if ( ! sh(0)*sh(1) )
      throw_error("Experiment", "Zero-size image requested.");
  };

  /// \brief Constructs processed projection.
  ///
  /// @param itheta the number of the projection to be returned.
  /// @param proj Array to put the projection into.
  /// @param sliceV array of slices to be extracted.
  ///
  virtual void projection(int itheta, Map & proj,
                          const std::vector<int> & sliceV = std::vector<int>()) const = 0;

  inline long thetas() const {return thts;} 			///< Total number of projections.
  inline long pixels() const {return sh(1);} 			///< Width of the image.
  inline long slices() const {return sh(0);} 			///< Number of slices (hight) in the image.
  inline Contrast contrast() const {return cntr;} ///< The contrast type of the projection.
  inline const Shape & shape() const {return sh;} ///< The contrast type of the projection.

};




/// \brief Projections storage.
///
/// This class constructs the 3D array of the input data and then can
/// form and output the sinograms.
///
/// If you want to build the sinograms from your own type of experiment,
/// Build a class inheriting from the Experiment class. See how it is
/// implemented in the DEIexp, EDEIexp constructors.
class EXPERIMENT_API SinoS {

private:

  static const std::string modname; 	///< Module name.
  long pxls;					///< Number of pixels (width) in the projection.
  long thts;					///< Number of projections (sinogram height).
  long slcs;					///< Number of slices (projection height).
  size_t allpix;				///< Number of pixels in the 3D array.
  Volume data;					///< 3D array.
  int mapfile;					///< Map file descriptor (if map memory to file)
  bool verb;					///< Show progress bar.
  std::vector<int> sliceV;
  Shape _imageShape; ///> Shape of the original projection

  int index(int idx) const;		///< Checks correctness of the index.

  void allocateArray(); ///< Allocate the storage

public:

  /// Constructor
  SinoS(const Experiment & exp, const std::vector<int> & _sliceV, bool _verb=false);

  /// Constructor
  SinoS(const std::vector<Path> & inlist, const std::string & slicedesc, bool _verb=false);

  /// Constructor
  SinoS(const std::vector<Path> & inlist, const std::string & slicedesc,
        float angle=0, const Crop & crop = Crop(), bool _verb=false);

  ~SinoS();   ///< Destructor.

  int pixels() const ;			///< Pixels.
  int slices() const ;			///< Slices.
  int thetas() const ;			///< Thetas.
  inline const std::vector<int> & indexes() const {return sliceV;}
  inline const Shape & imageShape() const {return _imageShape;}

  void sino(int idx, Map & storage) const;	///< Prepares the sinogram.

};














/// \brief Class to deal with the rotation center (read ::DcenterOptionDesc).
///
/// This class parses the string describing variable deviation
/// of the rotation axis from the center of the projection and then
/// can calculate this value for any slice.
class EXPERIMENT_API Dcenter {
private:
  float acof;		///< coefficient aconf (read ::DcenterOptionDesc).
  float bcof;		///< coefficient bconf (read ::DcenterOptionDesc).
public:

  Dcenter(float _b=0.0, float _a=0.0); ///< Constructor from coefficients.
  Dcenter(const std::string & centerdesc); ///< Constructor from description.

  float operator()(int slice=0) const ; ///< Calculates the center for the slice.

};

/// Description of the option giving the Dcenter type.
extern EXPERIMENT_API const std::string DcenterOptionDesc;

/// \brief Prints type name.
/// To be used in the CLI parsing via "poptmx" library
/// @return type name.
std::string EXPERIMENT_API
type_desc (Dcenter*);

/// \brief Converts the string "in" into the Dcenter _val.
/// To be used in the CLI parsing via "poptmx" library
/// @param _val value to be updated.
/// @param in string to be parsed.
///
/// @return \c true if success, \c false otherwise.
bool EXPERIMENT_API
_conversion (Dcenter* _val, const std::string & in);


/// @}


#endif // _H_EXPERIMENT_H_
