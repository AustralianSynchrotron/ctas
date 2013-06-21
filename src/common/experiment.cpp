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
#include <fstream>

using namespace std;


#ifdef OPENCL_FOUND

cl::Program * AqSeries::program = AqSeries::init() ;

cl::Program * AqSeries::init() {

  if ( ! clIsInited() )
    return 0;

  cl::Program * ret_program=0;

  char csrc[] =  {
    #include "ff.cl.includeme"
  };

  try {
    ret_program = new cl::Program(*CL_context, string(csrc, sizeof(csrc)));
  } catch (cl::Error err) {
    warn(modname, (string) "Could not load OpenCL program: " + err.what() );
    return 0;
  }

  try {
    ret_program->build();
  } catch (cl::Error err) {
    warn(modname, (string) "Could not build OpenCL program: " + err.what() +
         ". More detailsd below:" );
    warn(modname, "   Build Status: " +
         toString( ret_program->getBuildInfo<CL_PROGRAM_BUILD_STATUS>(*CL_device) ) );
    warn(modname, "   Build Options: " +
         ret_program->getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(*CL_device) );
    warn(modname, "   Build Log:\n" +
         ret_program->getBuildInfo<CL_PROGRAM_BUILD_LOG>(*CL_device) +
         "   End build Log:" );
    return 0;
  }

  return ret_program;

}



#endif // OPENCL_FOUND

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

  string curstring;
  fstream input_file(filename.c_str(), ios::in);
  if ( ! input_file.is_open() )
    throw_error(modname, string() +
                "Failed to open input file \"" + filename.c_str() + "\"\n");
  const string fdir=filename.dir();

  int fgIdx=-1, prevBgIdx=-1, prevDfIdx=0, bgIdx=-1, dfIdx=-1;

  while (!input_file.eof()) {

    getline(input_file, curstring);

    if ( isbgstring(curstring) ) {

      curstring = bgname(curstring);
      if ( ! curstring.empty() ) {
        bgs.push_back( cdpath(fdir, curstring) );
        bgIdx=bgs.size()-1;
      }

      for (int idx=prevBgIdx+1 ; idx<=fgIdx ; idx++) {
        if ( curstring.empty() ) {
          fgs[idx].bg2=-1;
          fgs[idx].bgWeight=1.0;
        } else if ( fgs[idx].bg1 == -1)  {
          fgs[idx].bg1=bgIdx;
          fgs[idx].bgWeight=1.0;
        } else {
          fgs[idx].bg2=bgIdx;
          fgs[idx].bgWeight= (fgIdx-idx+0.5) / (fgIdx-prevBgIdx) ;
        }
      }

      prevBgIdx=fgIdx;

    } else if ( isdfstring(curstring) ) {

      curstring = dfname(curstring);
      if ( ! curstring.empty() ) {
        dfs.push_back( cdpath(fdir, curstring) );
        dfIdx=dfs.size()-1;
      }

      for (int idx=prevDfIdx+1 ; idx<=fgIdx ; idx++) {
        if ( curstring.empty() ) {
          fgs[idx].df2=-1;
          fgs[idx].dfWeight=1.0;
        } else if ( fgs[idx].df1 == -1)  {
          fgs[idx].df1=dfIdx;
          fgs[idx].dfWeight=1.0;
        } else {
          fgs[idx].df2=dfIdx;
          fgs[idx].dfWeight= (fgIdx-idx+0.5) / (fgIdx-prevDfIdx) ;
        }
      }

      prevDfIdx=fgIdx;

    } else if ( iscomment(curstring) ) {

      // Do nothing

    } else

    {
      fgs.push_back( FgElement( cdpath(fdir, curstring), bgIdx, dfIdx) );
      fgIdx++;
    }


  }
  input_file.close();

  if ( ! thetas() )
    throw_error (modname, "Empty input data file '" + filename + "'.");

  sh = ImageSizes(fgs[0].fg);


#ifdef OPENCL_FOUND
  if (program) {
    try {
      kernel = cl::Kernel(*program, "ff");
    } catch (cl::Error err) {
      warn(modname, (string) "Could not create OpenCL kernel \"ff\": " + err.what() );
    }
  }
#endif // OPENCL_FOUND

}


const Path & AqSeries::fg(int idx) const {
  return fgs[index(idx)].fg;
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
AqSeries::isdfstring(const string & str) const {
  return ! str.compare(0, DFPREFIX.length(), DFPREFIX);
}

/// \brief Tells if the string is a comment.
///
/// @param str The string to check
///
/// @return \c true if the string is a comment or empty, \c false otherwise.
///
bool
AqSeries::iscomment(const string & str) const {
  return
    str.empty() ||
    str.find_first_not_of(" \t") == string::npos ||
    ! str.compare(0, 1, "#");
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
AqSeries::dfname(const string & str) const {
  return isdfstring(str) ? str.substr( DFPREFIX.length() ) : string("") ;
}


/// \brief Checks if the index is within the allowed region.
///
/// @param idx The index to check.
///
/// @return The index.
///
int
AqSeries::index(int idx) const  {
  if ( idx >= thetas() || idx < 0 )
	throw_error (modname, "Bad index " + toString(idx) +
				 " is outside the list of size (" + toString(thetas()) + ")");
  return idx;
}


void readnrot(const string &img, Map &map, const Shape & sh,
              float angle, const vector<int> &slices, const Crop & crop)  {
  if (angle==0.0)
    ReadImageLine(img, map, slices, sh);
  else {
    Map temp;
    ReadImage(img, map, sh);
    rotate(map, temp, angle, crop);
    if ( ! slices.empty() ) {
      map.resize(slices.size(), temp.shape()(1));
      for ( int sls=0 ; sls < slices.size() ; sls++ )
        if (slices[sls] < temp.shape()(1))
          map(sls, blitz::Range::all()) =
            temp(slices[sls], blitz::Range::all());
        else
          map(sls, blitz::Range::all()) = 0.0;
    } else {
      map=temp.copy();
    }
  }
}


/// Constructs the flat-field subtracted projection from the experimental data
/// at given rotation position ::itheta and puts it into ::proj array.
///
/// @param idx rotation position.
/// @param proj the array to put data into.
/// @param slicedesc List of slices of the interest.
/// @param angle Rotation angle.
/// @param crop Cropping of the image (after rotation if given).
///
void AqSeries::projection(int idx, Map & proj,
                          const std::string & slicedesc,
                          float angle, const Crop &crop, float cutOff) const {
  vector<int> slicesV = slice_str2vec(slicedesc, slices());
  projection(idx, proj, slicesV, angle, crop, cutOff);
}


/// Constructs the flat-field subtracted projection from the experimental data
/// at given rotation position ::itheta and puts it into ::proj array.
///
/// @param idx rotation position.
/// @param proj the array to put data into.
/// @param sliceV array of slices to be extracted.
/// @param angle Rotation angle.
/// @param crop Cropping of the image (after rotation if given).
///
void AqSeries::projection(int idx, Map &proj,
                          const std::vector<int> &slicesV,
                          float angle, const Crop &crop, float cutOff) const {

  /*
   *  cerr << fgs[idx].fg << " "
   *       << (  fgs[idx].bg1 == -1 ?  "<NONE>" : bgs[fgs[idx].bg1] ) << " "
   *       << ( fgs[idx].bg2 == -1  ?  "<NONE>" : bgs[fgs[idx].bg2] ) << " "
   *       << fgs[idx].bgWeight << " "
   *       << ( fgs[idx].df1 == -1  ?  "<NONE>" : dfs[fgs[idx].df1] ) << " "
   *       << ( fgs[idx].df2 == -1  ?  "<NONE>" : dfs[fgs[idx].df2] ) << " "
   *       << fgs[idx].dfWeight
   *       << "\n";
   */

  Map *bg1=0, *bg2=0, *df1=0, *df2=0;

  readnrot(fgs[idx].fg, proj, sh, angle, slicesV, crop);

#ifdef OPENCL_FOUND
  const unsigned int fprsz = sizeof(float) * proj.size() ;
  if (program) {
    cl_io = cl::Buffer(*CL_context, CL_MEM_READ_WRITE, fprsz);
    CL_queue->enqueueWriteBuffer(cl_io, CL_TRUE, 0, fprsz, proj.data());
    kernel.setArg(0, cl_io);
  }
#endif // OPENCL_FOUND


  if ( prev_prsh != proj.shape() ||
       angle != prev_angle ||
       prev_slicesV != slicesV ||
       prev_crop != crop ) {

    prev_prsh = proj.shape();
    prev_angle = angle;
    prev_slicesV = slicesV;
    prev_crop = crop;

    if ( fgs[idx].bg1 != -1 ) {
      memBgA.first = fgs[idx].bg1;
      readnrot(bgs[fgs[idx].bg1], memBgA.second , sh,
               angle, slicesV, crop);
      bg1 = & memBgA.second;
#ifdef OPENCL_FOUND
      if (program) {
        cl_bgA = cl::Buffer(*CL_context, CL_MEM_READ_ONLY, fprsz );
        CL_queue->enqueueWriteBuffer(cl_bgA, CL_TRUE, 0, fprsz, bg1->data());
        kernel.setArg(1, cl_bgA);
      }
#endif // OPENCL_FOUND
    }
    if ( fgs[idx].bg2 != -1 ) {
      memBgB.first = fgs[idx].bg2;
      readnrot(bgs[fgs[idx].bg2], memBgB.second, sh,
               angle, slicesV, crop);
      bg2 = & memBgB.second;
#ifdef OPENCL_FOUND
      if (program) {
        cl_bgB = cl::Buffer(*CL_context, CL_MEM_READ_ONLY, fprsz );
        CL_queue->enqueueWriteBuffer(cl_bgB, CL_TRUE, 0, fprsz, bg2->data());
        kernel.setArg(2, cl_bgB);
      }
#endif // OPENCL_FOUND
    }
    if ( fgs[idx].df1 != -1 ) {
      memDfA.first = fgs[idx].df1;
      readnrot(dfs[fgs[idx].df1], memDfA.second, sh,
               angle, slicesV, crop);
      df1 = & memDfA.second;
#ifdef OPENCL_FOUND
      if (program) {
        cl_dfA = cl::Buffer(*CL_context, CL_MEM_READ_ONLY, fprsz );
        CL_queue->enqueueWriteBuffer(cl_dfA, CL_TRUE, 0, fprsz, df1->data());
        kernel.setArg(3, cl_dfA);
      }
#endif // OPENCL_FOUND
    }
    if ( fgs[idx].df2 != -1 ) {
      memDfB.first = fgs[idx].df2;
      readnrot(dfs[fgs[idx].df2], memDfB.second, sh,
               angle, slicesV, crop);
      df2 = & memDfB.second;
#ifdef OPENCL_FOUND
      if (program) {
        cl_dfB = cl::Buffer(*CL_context, CL_MEM_READ_ONLY, fprsz );
        CL_queue->enqueueWriteBuffer(cl_dfB, CL_TRUE, 0, fprsz, df2->data());
        kernel.setArg(4, cl_dfB);
      }
#endif // OPENCL_FOUND
    }

  } else {

    if ( memBgA.first == fgs[idx].bg1  &&  fgs[idx].bg1 >= 0) {
      bg1 = & memBgA.second;
#ifdef OPENCL_FOUND
      if (program)
        kernel.setArg(1, cl_bgA);
#endif // OPENCL_FOUND
    } else if ( memBgA.first == fgs[idx].bg2  &&  fgs[idx].bg2 >= 0) {
      bg2 = & memBgA.second;
#ifdef OPENCL_FOUND
      if (program)
        kernel.setArg(2, cl_bgA);
#endif // OPENCL_FOUND
    } else {
      memBgA.second.resize(0,0);
      memBgA.first = -1;
    }

    if ( memBgB.first == fgs[idx].bg1  &&  fgs[idx].bg1 >= 0  &&  ! bg1 ) {
      bg1 = & memBgB.second;
#ifdef OPENCL_FOUND
      if (program)
        kernel.setArg(1, cl_bgB);
#endif // OPENCL_FOUND
    } else if ( memBgB.first == fgs[idx].bg2  &&  fgs[idx].bg2 >= 0  &&  ! bg2) {
      bg2 = & memBgB.second;
#ifdef OPENCL_FOUND
      if (program)
        kernel.setArg(2, cl_bgB);
#endif // OPENCL_FOUND
    } else {
      memBgB.second.resize(0,0);
      memBgB.first = -1;
    }

    if ( ! bg1 && fgs[idx].bg1 >=0 )
      if (memBgA.first <= 1) {
        memBgA.first = fgs[idx].bg1;
        readnrot(bgs[fgs[idx].bg1], memBgA.second, sh,
                 angle, slicesV, crop);
        bg1 = & memBgA.second;
#ifdef OPENCL_FOUND
        if (program) {
          cl_bgA = cl::Buffer(*CL_context, CL_MEM_READ_ONLY, fprsz );
          CL_queue->enqueueWriteBuffer(cl_bgA, CL_TRUE, 0, fprsz, bg1->data());
          kernel.setArg(1, cl_bgA);
        }
#endif // OPENCL_FOUND
      } else if (memBgB.first <= 1) {
        memBgB.first = fgs[idx].bg1;
        readnrot(bgs[fgs[idx].bg1], memBgB.second, sh,
                 angle, slicesV, crop);
        bg1 = & memBgB.second;
#ifdef OPENCL_FOUND
        if (program) {
          cl_bgB = cl::Buffer(*CL_context, CL_MEM_READ_ONLY, fprsz );
          CL_queue->enqueueWriteBuffer(cl_bgB, CL_TRUE, 0, fprsz, bg1->data());
          kernel.setArg(1, cl_bgB);
        }
#endif // OPENCL_FOUND
      } else {
        throw_error("AqSeries projection",
                    "No expected first background. Developer's error.");
      }

    if ( ! bg2 && fgs[idx].bg2 >=0 )
      if (memBgA.first <= 1) {
        memBgA.first = fgs[idx].bg2;
        readnrot(bgs[fgs[idx].bg2], memBgA.second, sh,
                 angle, slicesV, crop);
        bg2 = & memBgA.second;
#ifdef OPENCL_FOUND
        if (program) {
          cl_bgA = cl::Buffer(*CL_context, CL_MEM_READ_ONLY, fprsz );
          CL_queue->enqueueWriteBuffer(cl_bgA, CL_TRUE, 0, fprsz, bg2->data());
          kernel.setArg(2, cl_bgA);
        }
#endif // OPENCL_FOUND
      } else if (memBgB.first <= 1) {
        memBgB.first = fgs[idx].bg2;
        readnrot(bgs[fgs[idx].bg2], memBgB.second, sh,
                 angle, slicesV, crop);
        bg2 = & memBgB.second;
#ifdef OPENCL_FOUND
        if (program) {
          cl_bgB = cl::Buffer(*CL_context, CL_MEM_READ_ONLY, fprsz );
          CL_queue->enqueueWriteBuffer(cl_bgB, CL_TRUE, 0, fprsz, bg2->data());
          kernel.setArg(2, cl_bgB);
        }
#endif // OPENCL_FOUND
      } else {
        throw_error("AqSeries projection",
                    "No expected second background. Developer's error.");
      }

    if ( memDfA.first == fgs[idx].df1  &&  fgs[idx].df1 >= 0) {
      df1 = & memDfA.second;
#ifdef OPENCL_FOUND
      if (program)
        kernel.setArg(3, cl_dfA);
#endif // OPENCL_FOUND
    } else if ( memDfA.first == fgs[idx].df2  &&  fgs[idx].df2 >= 0) {
      df2 = & memDfA.second;
#ifdef OPENCL_FOUND
      if (program)
        kernel.setArg(4, cl_dfA);
#endif // OPENCL_FOUND
    } else {
      memDfA.second.resize(0,0);
      memDfA.first = -1;
    }

    if ( memDfB.first == fgs[idx].df1  &&  fgs[idx].df1 >= 0  &&  ! df1 ) {
      df1 = & memDfB.second;
#ifdef OPENCL_FOUND
      if (program)
        kernel.setArg(3, cl_dfB);
#endif // OPENCL_FOUND
    } else if ( memDfB.first == fgs[idx].df2  &&  fgs[idx].df2 >= 0  &&  ! df2) {
      df2 = & memDfB.second;
#ifdef OPENCL_FOUND
      if (program)
        kernel.setArg(4, cl_dfB);
#endif // OPENCL_FOUND
    } else {
      memDfB.second.resize(0,0);
      memDfB.first = -1;
    }

    if ( ! df1 && fgs[idx].df1 >=0 )
      if (memDfA.first <= 1) {
        memDfA.first = fgs[idx].df1;
        readnrot(dfs[fgs[idx].df1], memDfA.second, sh,
                 angle, slicesV, crop);
        df1 = & memDfA.second;
#ifdef OPENCL_FOUND
        if (program) {
          cl_dfA = cl::Buffer(*CL_context, CL_MEM_READ_ONLY, fprsz );
          CL_queue->enqueueWriteBuffer(cl_dfA, CL_TRUE, 0, fprsz, df1->data());
          kernel.setArg(3, cl_dfA);
        }
#endif // OPENCL_FOUND
      } else if (memDfB.first <= 1) {
        memDfB.first = fgs[idx].df1;
        readnrot(dfs[fgs[idx].df1], memDfB.second, sh,
                 angle, slicesV, crop);
        df1 = & memDfB.second;
#ifdef OPENCL_FOUND
        if (program) {
          cl_dfB = cl::Buffer(*CL_context, CL_MEM_READ_ONLY, fprsz );
          CL_queue->enqueueWriteBuffer(cl_dfB, CL_TRUE, 0, fprsz, df1->data());
          kernel.setArg(3, cl_dfB);
        }
#endif // OPENCL_FOUND
      } else {
        throw_error("AqSeries projection",
                    "No expected first dark-field. Developer's error.");
      }

      if ( ! df2 && fgs[idx].df2 >=0 )
        if (memDfA.first <= 1) {
          memDfA.first = fgs[idx].df2;
          readnrot(dfs[fgs[idx].df2], memDfA.second, sh,
                   angle, slicesV, crop);
          df2 = & memDfA.second;
#ifdef OPENCL_FOUND
          if (program) {
            cl_dfA = cl::Buffer(*CL_context, CL_MEM_READ_ONLY, fprsz );
            CL_queue->enqueueWriteBuffer(cl_dfA, CL_TRUE, 0, fprsz, df1->data());
            kernel.setArg(4, cl_dfA);
          }
#endif // OPENCL_FOUND
        } else if (memDfB.first <= 1) {
          memDfB.first = fgs[idx].df2;
          readnrot(dfs[fgs[idx].df2], memDfB.second, sh,
                   angle, slicesV, crop);
          df2 = & memDfB.second;
#ifdef OPENCL_FOUND
          if (program) {
            cl_dfB = cl::Buffer(*CL_context, CL_MEM_READ_ONLY, fprsz );
            CL_queue->enqueueWriteBuffer(cl_dfB, CL_TRUE, 0, fprsz, df2->data());
            kernel.setArg(4, cl_dfB);
          }
#endif // OPENCL_FOUND
        } else {
          throw_error("AqSeries projection",
                      "No expected second dark-field. Developer's error.");
        }

  }

#ifdef OPENCL_FOUND
  if (program) {

    if (bg1 && bg2) {
      kernel.setArg(5, fgs[idx].bgWeight);
    } else if (bg1) {
      kernel.setArg(2, 0);
      kernel.setArg(5, 1);
    } else if (bg2) {
      kernel.setArg(1, 0);
      kernel.setArg(5, 0);
    } else {
      kernel.setArg(1, 0);
      kernel.setArg(2, 0);
    }

    if (df1 && df2) {
      kernel.setArg(6, fgs[idx].dfWeight);
    } else if (df1) {
      kernel.setArg(4, 0);
      kernel.setArg(6, 1);
    } else if (df2) {
      kernel.setArg(3, 0);
      kernel.setArg(6, 0);
    } else {
      kernel.setArg(3, 0);
      kernel.setArg(4, 0);
    }

    kernel.setArg(7, cutOff);

    try {
      CL_queue->enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(proj.size()), cl::NullRange);
      CL_queue->finish();
      CL_queue->enqueueReadBuffer(cl_io, CL_TRUE, 0, fprsz, proj.data());
    } catch (cl::Error err) {
      warn(modname, (string) "Failed to run OpenCL kernel \"ff\": "
           + err.what() + ". Returned value: " + toString(err.err()) + "." );
      return;
    }

    return;

  }
#endif // OPENCL_FOUND

  Map df(proj.shape());
  if (df1 && df2)
    weighted(df, *df1, *df2, fgs[idx].dfWeight);
  else if (df1)
    df=df1->copy();
  else if (df2)
    df=df2->copy();
  else
    df=0;

  Map bg(proj.shape());
  if (bg1 && bg2)
    weighted(bg, *bg1, *bg2, fgs[idx].bgWeight) - df;
  else if (bg1)
    bg = *bg1 - df;
  else if (bg2)
    bg = *bg2 - df;
  else
    bg=1+df;

  flatfield(proj, proj, bg, df);

  if ( cutOff>=0.0 )
    for (long icur=0; icur<proj.shape()(0); icur++)
      for (long jcur=0; jcur<proj.shape()(1); jcur++)
        if ( proj(icur,jcur) > cutOff )
          proj(icur,jcur) = cutOff;

}


void AqSeries::clean() const {
  prev_slicesV.clear();
  memDfA.second.resize(0,0);
  memBgA.second.resize(0,0);
  memDfB.second.resize(0,0);
  memBgB.second.resize(0,0);
  cl_io  = cl::Buffer();
  cl_bgA = cl::Buffer();
  cl_bgB = cl::Buffer();
  cl_dfA = cl::Buffer();
  cl_dfB = cl::Buffer();
}


const std::string AqSeries::modname  = "acquisition series";
const std::string AqSeries::BGPREFIX = "#BACKGROUND# ";
const std::string AqSeries::DFPREFIX = "#DARKCURRENT# ";
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
  "    " + AqSeries::DFPREFIX + "<dark current filename>\n"
  "The string prefixed with the \"" + AqSeries::DFPREFIX + "\" gives the name of"
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



/// \brief Constructs the 3D array from the absorption contrast projections.
///
/// @param exp The experiment.
/// @param sliceV List of slices of the interest.
/// @param _verb Show progress bar.
///
SinoS::SinoS(const Experiment & exp, const vector<int> & _sliceV, bool _verb) :
  verb(_verb),
  sliceV(_sliceV),
  _imageShape(exp.shape())
{

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

  allocateArray();

  // Read images to the array.
  Map proj(Shape(slcs,pxls));
  ProgressBar bar(verb, "reading projections", thts);
  for ( int curproj = 0 ; curproj < thts ; curproj++){
    exp.projection(curproj, proj, sliceV);
	data(curproj, blitz::Range::all(), blitz::Range::all()) = proj;
	bar.update();
  }

}


/// \brief Constructs the 3D array from the list of input images.
///
/// @param inlist List of input files.
/// @param slicedesc List of slices of the interest.
/// @param _verb Show progress bar.
///
SinoS::SinoS(const vector<Path> & inlist, const std::string & slicedesc, bool _verb) :
   verb(_verb)
{

  if ( ! inlist.size() )
    throw_error(modname, "Empty list of input files.");
  _imageShape = ImageSizes(inlist[0]);
  sliceV = slice_str2vec(slicedesc, _imageShape(0));
  slcs = sliceV.size();
  thts = inlist.size();
  pxls = _imageShape(1);
  allpix = long(thts) * long(slcs) *long(pxls);

  allocateArray();

  // Read images to the array.
  Map proj(Shape(slcs,pxls));
  ProgressBar bar(verb, "reading projections", thts);
  for ( int curproj = 0 ; curproj < thts ; curproj++) {
    ReadImageLine( inlist[curproj], proj, sliceV, _imageShape);
    data(curproj, blitz::Range::all(), blitz::Range::all()) = proj;
    bar.update();
  }

}




/// \brief Constructs the 3D array from the list of input images.
///
/// @param inlist List of input files.
/// @param slicedesc List of slices of the interest.
/// @param angle Rotation angle.
/// @param _verb Show progress bar.
///
SinoS::SinoS(const vector<Path> & inlist, const std::string & slicedesc,
             float angle, const Crop & crop, bool _verb) :
  verb(_verb)
{

  if ( ! inlist.size() )
    throw_error(modname, "Empty list of input files.");

  Map iar, oar;
  ReadImage(inlist[0], iar);
  const Shape sh = iar.shape();
  rotate(iar, oar, angle, crop);
  _imageShape = oar.shape();
  sliceV = slice_str2vec(slicedesc, _imageShape(0));
  slcs = sliceV.size();
  thts = inlist.size();
  pxls = _imageShape(1);
  allpix = long(thts) * long(slcs) *long(pxls);

  allocateArray();

  // Read images to the array.
  Map proj(Shape(slcs,pxls));
  ProgressBar bar(verb, "reading projections", thts);
  for ( int curproj = 0 ; curproj < thts ; curproj++) {
    ReadImage(inlist[curproj], iar, sh);
    rotate(iar, oar, angle, crop);
    for ( int sls=0 ; sls < sliceV.size() ; sls++ )
      data(curproj, sls, blitz::Range::all()) =
        oar(sliceV[sls], blitz::Range::all());
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

void SinoS::allocateArray() {
  mapfile = 0;
  blitz::TinyVector<int, 3> datashp(thts, slcs, pxls);
  try { data.resize(datashp); }
  catch(bad_alloc err) {

    warn (modname, "Failed to allocate memory."
          " Trying to play with the memory mapped to file.");

    const char * tmpenv =  "TMPDIR";
    const string tempdesc = "You can control the path to the temporary file via"
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

    mapfile = mkstemp(tmpfilename);
    if ( mapfile < 0 )
      throw_error("temp file", "Could not open temporary file"
                  " \"" + string(tmpfilename) + "\". " + tempdesc);
    if ( unlink(tmpfilename) < 0 )
    warn("temp file", "Could not unlink temporary file"
                      " \"" + string(tmpfilename) + "\". Don't forget"
                      " to delete it after the program has finished.");

    const off_t size = allpix * sizeof(float);

    if ( lseek(mapfile, size, SEEK_SET) != size ||
         write(mapfile, "END", 3) != 3 ) {
      // Should I pad the holes ???
      close (mapfile);
      throw_error("temp file",
                "Could not set size of the  temporary file"
                " \"" + string(tmpfilename) + "\". You need " + toString((long unsigned)size) +
                "bytes on the hard disk. " + tempdesc);
    }

    float * datap = (float*) mmap( 0, size, PROT_WRITE|PROT_READ, MAP_SHARED,
                                   mapfile, 0);
    if ( ! datap || datap == MAP_FAILED ) {
      close (mapfile);
      throw_error(modname, "Failed to mmap memory.");
    }

    data.reference( Volume(datap, datashp, blitz::neverDeleteData) );

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
