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
/// @brief Tomosynthesis reconstruction.
///
/// Main file for the program which reads the input sinogram from image,
/// performs CT reconstruction of it and stores the result in other
/// image file
///

#define _USE_MATH_DEFINES // for M_PI

#include "../common/ctas.h"
#include "../common/ct.h"
#include "../common/poptmx.h"

using namespace std;

/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Contrast contrast;            ///< Type of the contrast.
  int plane;                    ///< Plane of the reconstruction.
  float center;                   ///< Rotation center.
  Path data_name;             ///< Name of the input data file.
  Path outmask;           ///< Name of the file to save the result to.
  bool beverbose;				///< Be verbose flag
  int bpp;					///< Save image as 16-bit integer.

  /// \CLARGSF
  clargs(int argc, char *argv[]);
};

/// Description of the TS input configuration file.
const string TSconfigDesc =
  " Must consist only of the strings containing the the following"
  " strings:\n"
  "    <float> <string>\n"
  "The first component (float value) represents the angle in degree"
  " at which the projection was recorded, while the second component"
  " gives the file name of the image with the projection. For example:\n"
  "    -25.5 ../acquire/projection_-25.5.tif\n"
  "The path to the file is relative to the path of the data file.";

clargs::
clargs(int argc, char *argv[]) :
  outmask("ts-<data file>-<plane>.tif"),
  plane(0),
  center(0),
  beverbose(false),
  bpp(0),
  contrast(Contrast::ABS)
{

  poptmx::OptionTable table
  ("Tomosynthesis reconstruction.",
   "The program reads file which describes the images containing TS"
   " projections and performs TS reconstruction of the requested plane.");

  table
  .add(poptmx::NOTE, "ARGUMENTS:")
  .add(poptmx::ARGUMENT, &data_name, "data file",
     "Input file describing the projection images.",
     TSconfigDesc)
  .add(poptmx::ARGUMENT, &outmask, "result",
     "Output reconstructed image.", "", outmask)

  .add(poptmx::NOTE, "OPTIONS:")
  .add(poptmx::OPTION, &contrast, 'C', "contrast",
     "Input component.",
     "Type of the contrast presented in the sinogram. "
     + Contrast::Desc, contrast.name())
  .add(poptmx::OPTION, &plane, 'y', "plane",
         "Plane of the reconstruction.", "Relative to the rotation center.")
  .add(poptmx::OPTION, &center, 'c', "center",
     "Rotation center.", CenterOptionDesc, toString(center))
  .add(poptmx::OPTION, &bpp,'i', "int",
         "Bits per pixel to output image(s) as integer.", IntOptionDesc)
  .add_standard_options(&beverbose)
  .add(poptmx::MAN, "SEE ALSO:", SeeAlsoList);

  if ( ! table.parse(argc,argv) )
  exit(0);
  if ( ! table.count() ) {
  table.usage();
  exit(0);
  }

  command = table.name();


  if ( ! table.count(&plane) )
  exit_on_error( command, "The reconstruction plane must be explicitly set."
           " Use option " + table.desc(&plane) + " for this.");
  if ( ! table.count(&data_name) )
  exit_on_error(command, string () +
          "Missing required argument: "+table.desc(&data_name)+".");
  if ( ! table.count(&outmask) )
    outmask = upgrade(data_name, "ts-") + "-" +  toString(plane) + ".tif";



}

/// \brief Parses input data file
///
/// This structure parses the strings from the input data file and stores
/// the result in self.
struct proj_info {

  vector< pair<float,string> > list; ///< List containing the angle-file pairs.
  string name;					///< Last file name met in the file.
  float angle;					///< Last acquisition angle in the file.
  unsigned icur;						///< Current element.

  /// \brief Constructor
  ///
  /// Parses the input file and prepares for work
  ///
  /// @param data_name The data file to be parsed. Read ::TSconfigDesc.
  inline proj_info(const string & data_name) {

  // Opens the data file.
  FILE* input_list = fopen(data_name.c_str(), "r");
  if ( ! input_list )
    exit_on_error ("scan input", "Could not open input data file"
           " '" + data_name + "' for reading.");
    if ( feof(input_list) )
    exit_on_error ("scan input", "The input file"
           "\"" + data_name + "\" is empty.");

  string fdir(data_name, 0, data_name.rfind("/")+1);
  const int MAXSTRING = 1024;
  float nangle;
  char curin[MAXSTRING];
  while ( ! feof(input_list) &&
      fscanf (input_list, "%f %s\n", &nangle, curin) == 2 )
    list.push_back( pair<float,string>
            ( nangle*M_PI/180, cdpath(fdir,curin) ) );
    if ( ! feof(input_list) )
    warn ("scan input", "Certain part of the input file"
      " \"" + data_name + "\" could not be scanned."
      " succeeded in " + toString(list.size()) + " strings.");
  fclose(input_list);

    if ( ! list.size()  )
    throw_error("scan input", "Input file \"" + data_name + "\""
          " does not provide any input info.");

  angle  = list[0].first;
  name   = list[0].second;
  icur   = 0;

  }

  /// Puts the name and angle from the current element into the
  /// structure's members and advances icur.
  ///
  /// @return \c false if the last element was parsed, \c true otherwise.
  ///
  inline bool advance() {
  if ( icur >= list.size() )
    return false;
  angle = list[icur].first;
  name = list[icur].second;
  icur++;
  return true;
  }

};



/// \MAIN{ts}
int main( int argc, char *argv[] ) { {

  const clargs args(argc, argv);

  proj_info proj_i(args.data_name);
  Shape<2> shape=ImageSizes(proj_i.name);
  if (shape(1)/2 <= abs(args.plane) )
  exit_on_error(args.command, "The reconstruction plane is outside the image:"
          " Image width: " + toString(shape(1)) + ", reconstruction"
          " plane: " + toString(args.plane) + ".");

  string first_name = proj_i.name;
  Map proj(shape), result(shape);

  ProgressBar bar(args.beverbose, "reconstruction", proj_i.list.size() );
  while ( proj_i.advance() ) {
    ReadImage(proj_i.name, proj, shape);
  ts_add(proj, result, Filter(), args.center,
       args.contrast, proj_i.angle, args.plane);
  bar.update();
  }
  bar.done();

  SaveImage(args.outmask, result, args.bpp);

} exit (0); }

