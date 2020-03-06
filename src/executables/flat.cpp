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
/// @date   Mon Jul 21 10:09:31 2018
///
/// @brief %Flattens background ignoring threshold.
///


#include "../common/common.h"
#include "../common/poptmx.h"
#include <math.h>

using namespace std;


/// \CLARGS
struct clargs {
  Path command;               ///< Command name as it was invoked.
  Path in_name;               ///< Name of the input file.
  Path out_name;              ///< Name of the output file.
  int rad;               ///< Radius of the mean value region.
  float mincon;         ///< Low threshold.
  float maxcon;         ///< High threshold.
  bool beverbose;       ///< Be verbose flag
  bool oavrg;           ///< Output average.
  /// \CLARGSF
  clargs(int argc, char *argv[]);
};


clargs::
clargs(int argc, char *argv[]) :
  mincon(NAN),
  maxcon(NAN),
  beverbose(false),
  oavrg(false),
  out_name("int-<input>")
{

  poptmx::OptionTable table
  ("Flattens the background.",
   "Divides intensity by the mean value of given radius and multiplies by average"
   " of the whole image, ignoring the regions out of the provided thresholds.");

  table
  .add(poptmx::NOTE, "ARGUMENTS:")
  .add(poptmx::ARGUMENT, &in_name, "input", "Input image.", "")
  .add(poptmx::ARGUMENT, &out_name, "output", "Output image.",
       "", out_name)

  .add(poptmx::NOTE, "OPTIONS:")
  .add(poptmx::OPTION, &rad, 'r', "rad",
       "Radius of the smoothing region.",
       "The pixels in the region, if within the threshold, are used to calculate the average.",
       "<radius>")
  .add(poptmx::OPTION, &mincon, 'm', "min",
       "Low threshold.",
       " All pixels below this threshold will be ignored.", "<minimum>")
  .add(poptmx::OPTION, &maxcon, 'M', "max",
       "High threshold.",
       " All pixels above this threshold will be ignored.", "<maximum>")
  .add(poptmx::OPTION, &oavrg, 'a', "average",
       "Normalizes affected region to it's average.",
       "")
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
    out_name = upgrade(in_name, "flattened-");

  if ( ! table.count(&rad) )
    exit_on_error(command, string () +
        "Missing required option: "+table.desc(&rad)+".");


}




/// \MAIN{ct}
int main(int argc, char *argv[]) {

  const clargs args(argc, argv) ;

  char ctsrc[] = {
    #include "flat.cl.includeme"  
  };
  cl_program program = initProgram( ctsrc, sizeof(ctsrc), "flat" );
  if (!program)
    exit_on_error(args.command, "Failed to initiate OpenCL program \"flat\".");

  Map arr;
  ReadImage( args.in_name, arr );
  const size_t sz = arr.size();
  cl_mem ioIm = blitz2cl(arr);


  if ( fisok(args.mincon)) {
    cl_kernel kernelMin = createKernel( program, "minThreshold");
    setArg(kernelMin, 0, ioIm);
    setArg(kernelMin, 1, (cl_float) args.mincon);
    execKernel(kernelMin, sz);
  }
  if ( fisok(args.maxcon)) {
    cl_kernel kernelMax = createKernel ( program, "maxThreshold");
    setArg(kernelMax, 0, ioIm);
    setArg(kernelMax, 1, (cl_float) args.maxcon);
    execKernel(kernelMax, sz);
  }


  cl_mem tIm = blitz2cl(arr);
  cl_kernel kernelFlat = createKernel ( program, "flat");
  setArg(kernelFlat, 0, ioIm);
  setArg(kernelFlat, 1, tIm);
  setArg(kernelFlat, 2, (cl_int) arr.shape()(1));
  setArg(kernelFlat, 3, (cl_int) arr.shape()(0));
  setArg(kernelFlat, 4, (cl_int) args.rad);
  execKernel(kernelFlat, sz);


  cl_mem avrgbf = var2cl<float>();
  cl_kernel kernelAverage = createKernel (program, "averageArr");
  setArg(kernelAverage, 0, ioIm);
  setArg(kernelAverage, 1, (cl_int) sz);
  setArg(kernelAverage, 2, avrgbf);
  execKernel(kernelAverage);
  const float avrg = cl2var<float>(avrgbf);


  if (avrg == 0.0)
    cout << 0.0 << endl;
  else if ( ! fisok(avrg) )
    cout << 1.0 << endl;
  else {
    cout << avrg << endl;
    cl_kernel kernelNorm = createKernel ( program, "normdata");
    setArg(kernelNorm, 0, ioIm);
    setArg(kernelNorm, 1, tIm);
    setArg(kernelNorm, 2, (cl_int) sz);
    setArg(kernelNorm, 3, (cl_int) args.oavrg );
    setArg(kernelNorm, 4, (cl_float) avrg);
    execKernel(kernelNorm, sz);
  }

  cl2blitz(tIm,arr);
  SaveImage(args.out_name, arr);

  exit(0);

}

