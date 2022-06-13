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
#include <blitz/rand-tt800.h>
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

/// \brief Parsing file of the acquisition series.
///
/// This class parses and stores the list of foreground-background pairs
/// (read ::Desc for more details).
///
class EXPERIMENT_API AqSeries {

private:

  const static std::string BGPREFIX;  ///< Background prefix.
  const static std::string DFPREFIX;  ///< Dark current prefix.

  struct FgElement {

    Path fg;
    int bg1;
    int df1;
    int bg2;
    float bgWeight;
    int df2;
    float dfWeight;

    FgElement( Path _fg, int _bg1=-1, int _dc1=-1,
               int _bg2=-1, float _bgWeight=1.0,
               int _dc2=-1, float _dfWeight=1.0 ) :
      fg(_fg), bg1(_bg1), df1(_dc1),
      bg2(_bg2), bgWeight(_bgWeight),
      df2(_dc2), dfWeight(_dfWeight)
    {}

  };

  std::vector<FgElement> fgs;	///< Storage of foregrounds.
  std::vector<Path> bgs;		///< Storage of backgrounds.
  std::vector<Path> dfs;        ///< Dark current images.

  bool isbgstring(const std::string & str) const; ///< Tells if the string represents bgnd.
  bool isdfstring(const std::string & str) const; ///< Tells if the string represents dc.
  bool iscomment(const std::string & str) const; ///< Tells if the string is a comment.
  Path bgname(const std::string & str) const; ///< Extracts filename from the bgdn string.
  Path dfname(const std::string & str) const; ///< Extracts filename from the dc string.
  int index(int idx) const ;	///< Checks correctness of the index.

  Shape sh;                     ///< Shape of the input images.


  // needed to remember the parameters of the previous ::projection()
  mutable std::vector<int> prev_slicesV;
  mutable float prev_angle;
  mutable Crop prev_crop;
  mutable Shape prev_prsh;

  mutable std::pair<int, Map> memDfA;
  mutable std::pair<int, Map> memBgA;
  mutable std::pair<int, Map> memDfB;
  mutable std::pair<int, Map> memBgB;

#ifdef OPENCL_FOUND

  static cl_int err;
  static cl_program program;

  mutable cl_kernel kernel;
  mutable cl_mem cl_io;
  mutable cl_mem cl_bgA;
  mutable cl_mem cl_bgB;
  mutable cl_mem cl_dfA;
  mutable cl_mem cl_dfB;

  void cleanCLmem() const;

#endif // OPENCL_FOUND


public:

  AqSeries(const Path & filename); ///< Constructs from file.

  static const std::string modname; ///< Module name.

  inline Shape shape() const {return sh;} ///< Shape of the projection image.
  inline long thetas() const {return fgs.size();} 			///< Total number of projections.
  inline long pixels() const {return sh(1);} 			///< Width of the image.
  inline long slices() const {return sh(0);} 			///< Number of slices (hight) in the image.
  inline long size() const {return sh(0)*sh(1)*fgs.size();}	///< Total size of the data.

  void projection(int idx, Map & proj,
                  const std::string & slicedesc = std::string() ,
                  float angle=0, const Crop &crop = Crop(), float cutOff=0.0) const ;

  void projection(int idx, Map &proj,
                  const std::vector<int> &sliceV,
                  float angle=0, const Crop &crop = Crop(), float cutOff=0.0) const ;

  void clean() const;

  const Path & fg(int idx) const ; ///< Gives name of the foreground.

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
  virtual void projection( int itheta, Map & proj,
                           const std::vector<int> & sliceV,
                           float angle=0, const Crop &crop = Crop() ) const = 0;

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
  SinoS(const std::deque<Path> & inlist, const std::string & slicedesc,
        float angle=0, const Crop & crop = Crop(), bool _verb=false);

  ~SinoS();   ///< Destructor.

  int pixels() const ;			///< Pixels.
  int slices() const ;			///< Slices.
  int thetas() const ;			///< Thetas.
  inline const std::vector<int> & indexes() const {return sliceV;}
  inline const Shape & imageShape() const {return _imageShape;}
  inline Shape sinoShape() const {return Shape(thetas(), pixels());}

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
