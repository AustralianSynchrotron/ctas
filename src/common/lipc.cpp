///
/// @file
/// @author antonmx <antonmx@gmail.com>
/// @date   Mon Jul 12 10:46:57 2010
///
/// @brief
///
///
///


#define _USE_MATH_DEFINES // for M_PI

#include "ipc.h"
#include <unistd.h>


long process_size_in_pages(void)
{
  long s = -1;
  FILE *f = fopen("/proc/self/statm", "r");
  if (!f) return -1;
  // if for any reason the fscanf fails, s is still -1,
  //      with errno appropriately set.
  fscanf(f, "%ld", &s);
  fclose (f);
  return s * ( getpagesize() / 1024.0 / 1024.0 );
}

using namespace std;


bool
_conversion (IPCprocess::Component* _val, const std::string & in) {
  std::string _in = upper(in);
  if (_in == "A" || _in == "ABS" || _in == "ABSORPTION" )
    *_val= IPCprocess::ABS;
  else if (_in == "P" || _in == "PHS" || _in == "PHASE" )
    *_val= IPCprocess::PHS;
  else
    throw_error("IPC component", "Unrecognized component\"" + _in + "\".");
  return true;
}



const string IPCprocess::modname = "IPC process";	///< Module name.

const string IPCprocess::componentDesc =
  "Must be one of the following strings (case insensitive):\n"
  "A, ABS, ABSORPTION - for the absorption contrast\n"
  "P, PHS, PHASE      - for the phase contrast";

#ifdef OPENCL_FOUND

const char IPCprocess::oclSource[] = {
  #include "ipc.cl.includeme"
};

const cl_program IPCprocess::oclProgram =
  initProgram( IPCprocess::oclSource, sizeof(IPCprocess::oclSource), "IPC on OCL" );

#endif // OPENCL_FOUND


IPCprocess::IPCprocess( const Shape & _sh, float alpha,
                        float dist, float dd, float lambda) :
  sh(_sh)
{

  if (sh(0) < 3 || sh(1) < 3)
    throw_error(modname, "Insufficient size requested: (" + toString(sh) + ")."
                " Must be at least (" + toString(Shape(3,3)) + ").");
  if (dist <= 0.0)
    throw_error(modname, "Distance from object is less or equal to 0." );
  if (dd <= 0.0)
    throw_error(modname, "Pixel size is less or equal to 0." );
  if (lambda <= 0.0)
    throw_error(modname, "Wavelength is less or equal to 0." );
  if (alpha < 0.0)
    throw_error(modname, "Alpha parameter of the MBA is less than 0." );

  const int isz = sh[0]*sh[1];
  alpha *= dd*dd/(M_PI*dist*lambda);

  #ifdef OPENCL_FOUND

  mid = clAllocArray<float>(2*isz);
  kernelApplyAbsFilter = createKernel(oclProgram, "applyAbsFilter");
  setArg(kernelApplyAbsFilter, 0, mid);
  setArg(kernelApplyAbsFilter, 1, (cl_int) sh[1]);
  setArg(kernelApplyAbsFilter, 2, (cl_int) sh[0]);
  setArg(kernelApplyAbsFilter, 3, (cl_float) alpha );

  kernelApplyPhsFilter = createKernel(oclProgram, "applyPhsFilter");
  setArg(kernelApplyPhsFilter, 0, mid);
  setArg(kernelApplyPhsFilter, 1, (cl_int) sh[1]);
  setArg(kernelApplyPhsFilter, 2, (cl_int) sh[0]);
  setArg(kernelApplyPhsFilter, 3, (cl_float) alpha );
  setArg(kernelApplyPhsFilter, 4, (cl_float) ( dd * dd / (4.0*M_PI*M_PI*dist) ) ) ;

  clfftSetupData fftSetup;
  if ( CL_SUCCESS != clfftInitSetupData(&fftSetup) ||
       CL_SUCCESS != clfftSetup(&fftSetup) )
    throw_error(modname,  "Failed to initialize clFFT.");

  const size_t isht[2] = {sh(0),sh(1)};
  if ( CL_SUCCESS != clfftCreateDefaultPlan(&clfft_plan, CL_context, CLFFT_2D, isht ) ||
       // CL_SUCCESS != clfftSetPlanPrecision(clfft_plan, CLFFT_SINGLE) ||
       // CL_SUCCESS != clfftSetLayout(clfft_plan, CLFFT_COMPLEX_INTERLEAVED, CLFFT_COMPLEX_INTERLEAVED) ||
       // CL_SUCCESS != clfftSetResultLocation(clfft_plan, CLFFT_INPLACE) ||
       CL_SUCCESS != clfftBakePlan(clfft_plan, 1, &CL_queue, NULL, NULL) )
    throw_error(modname, "Failed to create clFFT plans.");


  #else // OPENCL_FOUND


  mid.resize(sh);
  phsFilter.resize(sh);
  absFilter.resize(sh);

  fftwf_complex* midd = (fftwf_complex*) (void*) mid.data(); // Bad trick!
  fft_f = fftwf_plan_dft_2d ( sh(0), sh(1), midd, midd, FFTW_FORWARD,  FFTW_ESTIMATE);
  fft_b = fftwf_plan_dft_2d ( sh(0), sh(1), midd, midd, FFTW_BACKWARD, FFTW_ESTIMATE);

  // prepare the filters
  for (long i = 0; i < sh(0); i++)
    for (long j = 0; j < sh(1); j++) {
      float ei, ej;
      ei = i/float(sh(0));
      ej = j/float(sh(1));
      if (ei>0.5) ei = 1.0 - ei;
      if (ej>0.5) ej = 1.0 - ej;
      absFilter(i,j) = ei*ei + ej*ej;
    }
  if (alpha == 0.0) // to avoid 0-division
    absFilter(0l, 0l) = 1.0;
  phsFilter = 1.0/(absFilter+alpha);
  absFilter *= phsFilter;
  phsFilter *= dd * dd / (4.0*M_PI*M_PI*dist);
  if (alpha == 0.0)
    phsFilter(0l, 0l) = 0.0;


  #endif // OPENCL_FOUND

}


IPCprocess::~IPCprocess() {
    #ifdef OPENCL_FOUND
    clReleaseMemObject(mid);
    if ( CL_SUCCESS !=  clfftDestroyPlan( &clfft_plan ) )
      throw_error(modname, "Failed to destroy clFFT plans.");  
    clfftTeardown( );
    #else // OPENCL_FOUND
    fftwf_destroy_plan(fft_f);
    fftwf_destroy_plan(fft_b);
    #endif // OPENCL_FOUND
}


#ifdef OPENCL_FOUND
cl_int IPCprocess::clfftExec(clfftDirection dir) const {
  cl_int err;
  err = clfftEnqueueTransform(clfft_plan, dir, 1, &CL_queue, 0, NULL, NULL, &mid, NULL, NULL);
  if ( CL_SUCCESS != err )
    throw_error(modname, "Failed to execute clFFT plan: " + toString(err) + ".");  
  err = clFinish(CL_queue);
  if ( CL_SUCCESS != err )
    throw_error(modname, "Failed to complete clFFT plan: " + toString(err) + "."); 
  return err; 
}
#endif // OPENCL_FOUND


void
IPCprocess::extract(const Map & in, Map & out, Component comp, const float param) const {

  if ( sh != out.shape() )
    out.resize(sh);
  if ( in.shape() != sh )
    throw_error(modname, "Size of the input array (" +toString(in.shape())+ ")"
                " does not match the expected one (" +toString(sh)+ ")." );

  const int isz = sh[0]*sh[1];


  #ifdef OPENCL_FOUND

  Map io;
  if ( out.isStorageContiguous()  &&  out.stride() == Shape(sh(0),1) )
    io.reference(out);
  else {
    io.resize(sh);
  }
  io = 1-in;

  blitz2cl(io, mid);
  clfftExec(CLFFT_FORWARD);
  execKernel( comp == PHS ? kernelApplyPhsFilter : kernelApplyAbsFilter, isz);
  clfftExec(CLFFT_BACKWARD);
  cl2blitz(mid, io);
  if (out.data() != io.data())
    out = io;


  #else // OPENCL_FOUND


  mid = 1.0 - in;
  fftwf_execute(fft_f);
  mid *= (comp == PHS) ? phsFilter : absFilter ;
  fftwf_execute(fft_b);
  out = real(mid)/isz;


  #endif // OPENCL_FOUND


  const float bmean = mean(out(blitz::Range::all(), 0)) + mean(out(blitz::Range::all(), sh(1)-1))
                    + mean(out(0, blitz::Range::all())) + mean(out(sh(0)-1, blitz::Range::all()));
  out -= bmean/4.0;
  if (comp == ABS) out = in / (1 - param*out);
  //else              out += param / (4*M_PI); 
    
}



static const complex<float> I_C = complex<float>(0.0f,-1.0f); ///< Complex unity: sqrt(-1)

void
propagate(const CMap & tif, Map & out, float dd, float lambda,  float dist) {

  const int zPad=3.0;

  const Shape sh = tif.shape();
  if (sh(0) < 3 || sh(1) < 3)
    throw_error("IPC simulation", "Insufficient size of the input data: (" + toString(sh) + ")."
                " Must be at least (" + toString(Shape(3,3)) + ").");
  if (out.shape() != sh)
    out.resize(sh);
  if (lambda<=0)
    throw_error("IPC simulation", "The wavelength is less or equal to 0.");
  if (dist<0)
    throw_error("IPC simulation", "The sample-to-detector distance is less than 0.");
  if (dd<=0)
    warn("IPC simulation", "The pixel size is less or equal to 0. Will use 1.0.");

  const Shape ish(zPad*sh(0), zPad*sh(1));
  CMap mid(ish);
  int mst0 = sh(0)*(zPad-1)/2, mst1 = sh(1)*(zPad-1)/2;
  blitz::Range r0(mst0, mst0+sh(0)-1), r1(mst1, mst1+sh(1)-1);
  mid = 1.0;
  mid(r0,r1) = tif;

  fftwf_complex* midd = (fftwf_complex*) (void*) mid.data(); // Bad trick!
  fftwf_plan fft_f = fftwf_plan_dft_2d ( ish(0), ish(1), midd, midd, FFTW_FORWARD,  FFTW_ESTIMATE);
  fftwf_plan fft_b = fftwf_plan_dft_2d ( ish(0), ish(1), midd, midd, FFTW_BACKWARD, FFTW_ESTIMATE);

  const float sizeX = dd*ish(0), sizeY=dd*ish(1);

  fftwf_execute(fft_f);
  for (blitz::MyIndexType i = 0 ; i<ish(0) ; i++)
    for (blitz::MyIndexType j = 0 ; j<ish(1) ; j++) {
      float ui = ( i<ish(0)/2  ?  i  :  ish(0)-i ) / sizeX;
      float vj = ( j<ish(1)/2  ?  j  :  ish(1)-j ) / sizeY;
      mid(i,j) *= exp( - (float)M_PI * I_C * lambda * dist * (ui*ui + vj*vj) );
    }
  fftwf_execute(fft_b);

  mid *= exp( 2.0f * (float)M_PI * I_C * dist / lambda ) / float(ish(0)*ish(1));
  out = pow2(abs(mid(r0,r1)));

  fftwf_destroy_plan(fft_f);
  fftwf_destroy_plan(fft_b);

}


void
simulateTif( CMap & tif, const Shape & sh, float bd,
             float theta, float dd, float lambda) {

  // Prepare spherical phantoms.

  Line beta(3), delta(3);
  delta = 2.9e-7, 6.0e-7, 0.9e-7;
  beta = bd*delta;
  //bd = 0 - no absorption
  //bd = 0.01 - weak absorption
  //bd = 0.1 - strong absorption


  //const float r[3] = {0.1, 0.04, 0.01};
  const float r[3] = {0.225, 0.1, 0.05};
  const float xr[3] = {0.015, 0.075, -0.1};
  const float yr[3] = {0.015, -0.05, 0.05};

  const int D = min(sh(0),sh(1));

  tif.resize(sh);
  tif = 1.0;

  for (blitz::MyIndexType cur=0 ; cur<3 ; cur++) {

    for (blitz::MyIndexType i = 0 ; i<sh(0) ; i++)
      for (blitz::MyIndexType j = 0 ; j<sh(1); j++) {
        float y = i/(float(D)-1) - 0.5*sh(0)/float(D);
        float x = j/(float(D)-1) - 0.5*sh(1)/float(D);
        float xxr = xr[cur] * cos(theta);
        float yyr = yr[cur];
        float rr  = r[cur]*r[cur];
        float hh = (x-xxr)*(x-xxr) + (y-yyr)*(y-yyr);
        float travel = (hh >= rr) ? 0.0 : 2 * sqrt(rr-hh) * D * dd;
        tif(i,j) *= exp( - 2.0f * (float)M_PI * travel * (beta(cur) + I_C * delta(cur)) / lambda );
        if (cur)
          tif(i,j) /= exp( - 2.0f * (float)M_PI * travel * (beta(0) + I_C * delta(0)) / lambda );
      }
  }

}
