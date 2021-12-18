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
/// @brief %Normalizes an image per using selected stripe.
///


#include "../common/common.h"
#include "../common/poptmx.h"
#include "../common/experiment.h"
#include <math.h>

using namespace std;


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Path in_name;               ///< Name of the input file.
  Path out_name;              ///< Name of the output file.
  string columndesc;
  float norm;
  bool beverbose;
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
  beverbose(false),
  out_name("<input>"),
  norm(0)
{

  poptmx::OptionTable table
  ("Normalizes the stripe of the image.",
   "Normalizes the rows of the input image in accordance with the columns mean value.");

  table
  .add(poptmx::NOTE, "ARGUMENTS:")
  .add(poptmx::ARGUMENT, &in_name, "input", "Input image.", "")
  .add(poptmx::ARGUMENT, &out_name, "output", "Output image.",
       "", out_name)

  .add(poptmx::NOTE, "OPTIONS:")
  .add(poptmx::OPTION, &columndesc, 'c', "column",
       "Reference columns.",
       "Columns which are used as the reference in the normalization." +
       SliceOptionDesc, "<all>")
  .add(poptmx::OPTION, &norm, 'n', "norm",
       "The normal level.", "" , "<auto>")
  .add_standard_options(&beverbose)
  .add(poptmx::MAN, "SEE ALSO:", SeeAlsoList);

  if ( ! table.parse(argc,argv) )
    exit(0);
  if ( ! table.count() ) {
    table.usage();
    exit(0);
  }


  command = table.name();

  // <input> : one required argument.
  if ( ! table.count(&in_name) )
    exit_on_error(command, string () +
    "Missing required argument: "+table.desc(&in_name)+".");
  // <output> : one more argument may or may not exist
  if ( ! table.count(&out_name) )
    out_name = in_name;
  if ( table.count(&norm)  &&  norm == 0.0 )
    exit_on_error(command, "Zero norm.");

}




/// \MAIN{ct}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;

  Map arr;
  ReadImage( args.in_name, arr );
  const Shape sh = arr.shape();
  vector<int> columns = slice_str2vec( args.columndesc, sh(1) );

  Line norm(sh(0));
  for (ArrIndex rcur=0; rcur<sh(0); rcur++) {
    float sum=0;
    for (int icur=0; icur<columns.size(); icur++ )
      sum += arr(rcur, (ArrIndex) columns[icur]);
    norm(rcur) = (sum==0.0) ? 1.0 : sum / columns.size() ;
  }
  const float averagenorm = (args.norm==0.0)  ?
                            sum(norm)/sh(0)  :  args.norm;

  for (ArrIndex rcur=0; rcur<sh(0); rcur++)
    arr(rcur, all) *= averagenorm / norm(rcur) ;

  SaveImage(args.out_name, arr);

  exit(0);

}
