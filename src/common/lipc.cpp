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
#include <vector>
#include <unistd.h>
#include <utility>

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

static int ipow(int base, int exp) {
  int result = 1;
  for (;;)  {
    if (exp & 1)
      result *= base;
    exp >>= 1;
    if (!exp)
      break;
    base *= base;
  }
  return result;
}

static vector<int> factor(int n, const vector<int> & primes) {
  vector<int> f(primes.size(), 0);
  for (int i = 0; i < primes.size(); ++i) {
    while (0< n && !(n % primes[i])) {
      n /= primes[i];
      ++f[i]; } }
  // append the "remainder"
  f.push_back(n);
  return f;
}

static int closest_factorable(int n, const vector<int> & primes) {
  int d = 0;
  vector<int> r;
  while (true) {
    r = factor(n + d, primes);
    if (r[r.size() - 1] == 1) { break; }
    ++d;
  }
  r.pop_back();
  int fnl = 1;
  for (int cprm=0 ; cprm<primes.size() ; cprm++)
    fnl *= ipow(primes[cprm],r[cprm]);
  return fnl;
}

#endif // OPENCL_FOUND


IPCprocess::IPCprocess( const Shape & _sh, float d2b) :
  sh(_sh)
{

  if (sh(0) < 3 || sh(1) < 3)
    throw_error(modname, "Insufficient size requested: (" + toString(sh) + ")."
                " Must be at least (" + toString(Shape(3,3)) + ").");
  if (d2b < 0.0)
    throw_error(modname, "Delata to Beta ratio is less than 0." );


  #ifdef OPENCL_FOUND


  const size_t msh[2] = {(size_t) closest_factorable(2*sh(1)-1, {2,3,5,7}),
                         (size_t) closest_factorable(2*sh(0)-1, {2,3,5,7})};
  mid.resize(msh[1], msh[0]);

  clmid = clAllocArray<float>(2*mid.size());
  kernelApplyAbsFilter = createKernel(oclProgram, "applyAbsFilter");
  setArg(kernelApplyAbsFilter, 0, clmid);
  setArg(kernelApplyAbsFilter, 1, (cl_int) msh[0]);
  setArg(kernelApplyAbsFilter, 2, (cl_int) msh[1]);
  setArg(kernelApplyAbsFilter, 3, (cl_float) d2b );

  kernelApplyPhsFilter = createKernel(oclProgram, "applyPhsFilter");
  setArg(kernelApplyPhsFilter, 0, clmid);
  setArg(kernelApplyPhsFilter, 1, (cl_int) msh[0]);
  setArg(kernelApplyPhsFilter, 2, (cl_int) msh[1]);
  setArg(kernelApplyPhsFilter, 3, (cl_float) d2b );

  kernelApply00 = createKernel(oclProgram, "apply00");
  setArg(kernelApply00, 0, clmid);

  cl_int err;
  clfftSetupData fftSetup;
  if ( CL_SUCCESS != (err = clfftInitSetupData(&fftSetup) ) ||
       CL_SUCCESS != (err = clfftSetup(&fftSetup) ) ||
       CL_SUCCESS != (err = clfftCreateDefaultPlan(&clfft_plan, CL_context, CLFFT_2D, msh)) ||
       CL_SUCCESS != (err = clfftBakePlan(clfft_plan, 1, &CL_queue, NULL, NULL)) )
    throw_error(modname,  "Failed to prepare the clFFT: " + toString(err) );


  #else // OPENCL_FOUND


  mid.resize(sh);
  phsFilter.resize(sh);
  absFilter.resize(sh);

  fftwf_complex* midd = (fftwf_complex*) (void*) mid.data(); // Bad trick!
  fft_f = fftwf_plan_dft_2d ( sh(1), sh(0), midd, midd, FFTW_FORWARD,  FFTW_ESTIMATE);
  fft_b = fftwf_plan_dft_2d ( sh(1), sh(0), midd, midd, FFTW_BACKWARD, FFTW_ESTIMATE);

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
  if ( d2b == 0.0 ) {
    phsFilter = 1/absFilter;
    absFilter = 0.0;
  } else {
    phsFilter = d2b/(d2b*absFilter+1);
    absFilter *= phsFilter;
  }

  #endif // OPENCL_FOUND

}


IPCprocess::~IPCprocess() {
    #ifdef OPENCL_FOUND
    clReleaseMemObject(clmid);
    clfftDestroyPlan(&clfft_plan);
    clfftTeardown( );
    #else // OPENCL_FOUND
    fftwf_destroy_plan(fft_f);
    fftwf_destroy_plan(fft_b);
    #endif // OPENCL_FOUND
}


#ifdef OPENCL_FOUND
cl_int IPCprocess::clfftExec(clfftDirection dir) const {
  cl_int err;
  err = clfftEnqueueTransform(clfft_plan, dir, 1, &CL_queue, 0, NULL, NULL, &clmid, NULL, NULL);
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

  if ( in.shape() != sh )
    throw_error(modname, "Size of the input array (" +toString(in.shape())+ ")"
                " does not match the expected one (" +toString(sh)+ ")." );
  if ( sh != out.shape() )
    out.resize(sh);

  mid = 1.0;
  mid(blitz::Range(0,sh[0]-1), blitz::Range(0,sh[1]-1)) = 1 - in;

  #ifdef OPENCL_FOUND

  blitz2cl(mid, clmid);
  clfftExec(CLFFT_FORWARD);
  // -1 in the next string is here to exclude 0,0 which is done in apply00
  execKernel( comp == PHS ? kernelApplyPhsFilter : kernelApplyAbsFilter, mid.size() - 1);
  execKernel( kernelApply00, 1);
  clfftExec(CLFFT_BACKWARD);
  cl2blitz(clmid, mid);
  out = real(mid(blitz::Range(0,sh[0]-1), blitz::Range(0,sh[1]-1)));

  #else // OPENCL_FOUND

  fftwf_execute(fft_f);
  mid *= (comp == PHS) ? phsFilter : absFilter ;
  fftwf_execute(fft_b);
  out = real(mid)/mid.size();


  #endif // OPENCL_FOUND


  //const float bmean = mean(out(all, 0)) + mean(out(all, sh(1)-1))
  //                  + mean(out(0, all)) + mean(out(sh(0)-1, all));
  //out -= bmean/4.0;
  out *= param;
  if (comp == ABS)   out = in / (1 - out);
  else if (param<0)  out = exp(out) ;
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
simulateTif( CMap & tif, const Shape & sh, float d2b,
             float theta, float dd, float lambda) {

  // Prepare spherical phantoms.

  Line beta(3), delta(3);
  delta = 2.9e-7, 6.0e-7, 0.9e-7;
  if (d2b == 0.0)
    beta = 0.0;
  else
    beta = delta/d2b;

  //const float r[3] = {0.1, 0.04, 0.01};
  const float r[3] = {0.225, 0.1, 0.05};
  const float xr[3] = {0.015, 0.075, -0.1};
  const float yr[3] = {0.015, -0.05, 0.05};
  const float zr[3] = {0.00, 0.015, 0.2};


  const int D = min(sh(0),sh(1));

  tif.resize(sh);
  tif = 1.0;

  for (blitz::MyIndexType cur=0 ; cur<3 ; cur++) {

    for (blitz::MyIndexType i = 0 ; i<sh(0) ; i++)
      for (blitz::MyIndexType j = 0 ; j<sh(1); j++) {
        float y = i/(float(D)-1) - 0.5*sh(0)/float(D);
        float x = j/(float(D)-1) - 0.5*sh(1)/float(D);
        float xxr = xr[cur] * cos(theta) + zr[cur] * sin(theta);
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


