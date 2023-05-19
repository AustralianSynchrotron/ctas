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



const string IPCprocess::modname = "IPC process";	///< Module name.



IPCprocess::ForCLdev::ForCLdev(CLenv & _cl, const Shape & _sh, float _d2b)
  : sh(_sh)
  , msh(closest_factorable(sh(0), {2,3,5,7}),
        closest_factorable(sh(1), {2,3,5,7}))
  , d2b(_d2b)
  , cl(_cl)
  , clmid(0)
  , oclProgram(0)
  , clfft_plan(0)
  , clfftTmpBuff(0)
  , locker(PTHREAD_MUTEX_INITIALIZER)
{
  if (d2b<=0)
    return;
  if (!CL_isReady())
    warn(modname, "OpenCL is not functional.");
}



IPCprocess::ForCLdev::~ForCLdev() {
  if (clfft_plan)
    clfftDestroyPlan(&clfft_plan);
  pthread_mutex_destroy(&locker);
}



bool
IPCprocess::ForCLdev::extract(Map & in) {

  if (d2b<=0)
    return true;
  if (in.shape() != msh)
    throw_error(modname, "Unexpected array size on OCL extract.");
  if ( ! CL_isReady() || ( cin.size() && ! clfft_plan ) ||  pthread_mutex_trylock(&locker) )
    return false;

  bool toRet = false;
  try {

    // allocate on first execution
    if ( !clfft_plan ) {

      cin.resize(msh);
      const string oclSource = {
        #include "ipc.cl.includeme"
      };
      oclProgram = initProgram(oclSource, oclProgram, "IPC on OCL", cl.cont);
      if (!oclProgram) {
        warn(modname, "Failed to compile OCL program for IPC processing.");
        return false;
      }

      clmid(clAllocArray<float>(area(msh)*2, cl.cont));
      kernelApplyPhsFilter(oclProgram, "applyPhsFilter");
      kernelApplyPhsFilter.setArg(0, clmid());
      kernelApplyPhsFilter.setArg(1, (cl_int) msh(1));
      kernelApplyPhsFilter.setArg(2, (cl_int) msh(0));
      kernelApplyPhsFilter.setArg(3, (cl_float) d2b );

      cl_int err=10;
      size_t clfftTmpBufSize = 0;
      clfftSetupData fftSetup;
      const size_t mshs[2] = {(size_t) msh(1), (size_t) msh(0)};
      if ( CL_SUCCESS != (err = clfftInitSetupData(&fftSetup) ) ||
           CL_SUCCESS != (err = clfftSetup(&fftSetup) ) ||
           CL_SUCCESS != (err = clfftCreateDefaultPlan(&clfft_plan, cl.cont, CLFFT_2D, mshs)) ||
           //CL_SUCCESS != (err = clfftSetPlanPrecision(clfft_plan, CLFFT_SINGLE_FAST)) ||
           //CL_SUCCESS != (err = clfftSetLayout(clfft_plan, CLFFT_REAL, CLFFT_HERMITIAN_INTERLEAVED )) ||
           CL_SUCCESS != (err = clfftBakePlan(clfft_plan, 1, &cl.que, NULL, NULL)) ||
           CL_SUCCESS != (err = clfftGetTmpBufSize(clfft_plan, &clfftTmpBufSize) ) )
      {
        if (clfft_plan) {
          clfftDestroyPlan(&clfft_plan);
          clfft_plan=0;
        }
        throw_error(modname,  "Failed to prepare the clFFT: " + toString(err) );
      }
      if (clfftTmpBufSize)
        clfftTmpBuff(clAllocArray<float>(clfftTmpBufSize, cl.cont));

    }

    // execute
    cin = blitz::cast<complex<float>>(in);
    blitz2cl(cin, clmid(), cl.que);
    clfftExec(CLFFT_FORWARD);
    kernelApplyPhsFilter.exec(area(msh), cl.que);
    clfftExec(CLFFT_BACKWARD);
    cl2blitz(clmid(), cin, cl.que);
    in = real(cin);
    toRet = true;

  } catch (...) {}

  pthread_mutex_unlock(&locker);
  return toRet;

}


cl_int IPCprocess::ForCLdev::clfftExec(clfftDirection dir) const {
  if (d2b<0)
    return CL_SUCCESS;
  cl_int err;
  err = clfftEnqueueTransform(clfft_plan, dir, 1, &cl.que, 0, NULL, NULL, &clmid(), NULL, clfftTmpBuff());
  if ( CL_SUCCESS != err )
    throw_error(modname, "Failed to execute clFFT plan: " + toString(err) + ".");
  err = clFinish(cl.que);
  if ( CL_SUCCESS != err )
    throw_error(modname, "Failed to complete clFFT plan: " + toString(err) + ".");
  return err;
}




IPCprocess::IPCprocess(const Shape & _sh, float _d2b)
  : sh(_sh)
  , msh(closest_factorable(sh(0), {2,3,5,7}),
        closest_factorable(sh(1), {2,3,5,7}))
  , d2b(_d2b)
  , phsFilter(msh)
  , mid(msh)
  , envs(_envs)
  , fft_f(0)
  , fft_b(0)
{

  if (d2b <= 0.0) // no IPC processing
    return;
  if (sh(0) < 3 || sh(1) < 3)
    throw_error(modname, "Insufficient size requested: (" + toString(sh) + ")."
                " Must be at least (" + toString(Shape(3,3)) + ").");

  if (!CL_isReady())
    warn(modname, "OpenCL is not functional.");
  for (CLenv & env : clenvs)
    try{ envs.emplace_back(env, sh, d2b); } catch(...) {}

  float* midd = mid.data();
  fft_f = fftwf_plan_r2r_2d ( msh(0), msh(1), midd, midd, FFTW_R2HC, FFTW_R2HC, FFTW_ESTIMATE);
  fft_b = fftwf_plan_r2r_2d ( msh(0), msh(1), midd, midd, FFTW_HC2R, FFTW_HC2R, FFTW_ESTIMATE);

  // prepare filters
  for (long i = 0; i < msh(0); i++) {
    float ei = i/float(msh(0));
    if (ei>0.5)
      ei = 1.0 - ei;
    ei *= ei;
    for (long j = 0; j < msh(1); j++) {
      float ej;
      ej = j/float(msh(1));
      if (ej>0.5)
        ej = 1.0 - ej;
      ej *= ej;
      phsFilter(i,j) = ei + ej;
    }
  }
  phsFilter = 1/(mid.size()*(d2b*phsFilter+1));

}


IPCprocess::IPCprocess( const IPCprocess & other )
  : sh(other.sh)
  , msh(other.msh)
  , d2b(other.d2b)
  , phsFilter(other.phsFilter)
  , mid(other.mid.shape())
  , envs(other.envs)
  , fft_f(0)
  , fft_b(0)
{
  if (d2b <= 0.0) // no IPC processing
    return;
  float* midd = mid.data();
  fft_f = fftwf_plan_r2r_2d ( msh(0), msh(1), midd, midd, FFTW_R2HC, FFTW_R2HC, FFTW_ESTIMATE);
  fft_b = fftwf_plan_r2r_2d ( msh(0), msh(1), midd, midd, FFTW_HC2R, FFTW_HC2R, FFTW_ESTIMATE);
}


IPCprocess::~IPCprocess() {
  if (fft_f)
    fftwf_destroy_plan(fft_f);
  if (fft_b)
    fftwf_destroy_plan(fft_b);
}





void
IPCprocess::extract(const Map & in, Map & out) {
  if ( in.shape() != sh )
    throw_error(modname, "Size of the input array (" +toString(in.shape())+ ")"
                " does not match the expected one (" +toString(sh)+ ")." );

  if (d2b<=0 && !out.size()) {
    out.reference(in);
    return;
  }
  if (!areSame(in, out)) {
    out.resize(sh);
    out = in;
  }
  if (d2b<=0)
    return;

  const blitz::Range r0_1(0, sh(0)-1), r1_1(0, sh(1)-1);
  deAbs(out);
  mid = 0.0;
  mid(r0_1, r1_1) = out;
  bool doneOnGPU = false;
  for (ForCLdev & env : envs)
    if ((doneOnGPU = env.extract(mid)))
      break;
  if (!doneOnGPU) {
    fftwf_execute(fft_f);
    mid *= phsFilter;
    fftwf_execute(fft_b);
  }
  out = mid(r0_1, r1_1);


  //const Shape opnt( (msh(0)-sh(0))/2 , (msh(1)-sh(1))/2 );
  //const Shape cpnt( opnt(0)+sh(0) , opnt(1)+sh(1) );
  //const blitz::Range r0_0(0, opnt(0)-1);
  //const blitz::Range r0_1(opnt(0), cpnt(0)-1);
  //const blitz::Range r0_2(cpnt(0), msh(0)-1);
  //const blitz::Range r1_0(0, opnt(1)-1);
  //const blitz::Range r1_1(opnt(1), cpnt(1)-1);
  //const blitz::Range r1_2(cpnt(1), msh(1)-1);
  //mid(r0_1, r1_1) = out;
  //for (int cur0=0 ; cur0 < opnt(0) ; cur0++ )
  //  mid(cur0, r1_1) = mid(opnt(0), r1_1);
  //for (int cur0=cpnt(0) ; cur0 < msh(0) ; cur0++ )
  //  mid(cur0, r1_1) = mid(cpnt(0)-1, r1_1);
  //for (int cur1=0 ; cur1 < opnt(1) ; cur1++ )
  //  mid(r0_1, cur1) = mid(r0_1, opnt(1));
  //for (int cur1=cpnt(1) ; cur1 < msh(1) ; cur1++ )
  //  mid(r0_1, cur1) = mid(r0_1, cpnt(1)-1);
  //mid(r0_0, r1_0) = mid(opnt(0), opnt(1));
  //mid(r0_0, r1_2) = mid(opnt(0), cpnt(1)-1);
  //mid(r0_2, r1_0) = mid(cpnt(0)-1, opnt(1));
  //mid(r0_2, r1_2) = mid(cpnt(0)-1, cpnt(1)-1);


}


float IPCprocess::d2bNorm (float _d2b, float _dd, float _dist, float _lambda) {
  if (_dd == 0.0) {
    warn(modname, "Impossible zero pixel size. Will use 1mum instead.");
    _dd = 1.0;
  }
  if (_lambda == 0.0) {
    warn(modname, "Impossible zero wawelength. Will use 1A instead.");
    _lambda = 1.0;
  }
  if (_dist == 0.0) {
    warn(modname, "Trivial case of contact plane with 0 distance.");
    return 1.0;
  }
  if (_d2b == 0.0) {
    warn(modname, "Trivial case of no phase with 0 delta/beta.");
    return 1.0;
  }
  return M_PI * _d2b * _dist * _lambda / ( _dd * _dd );
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
  for (ArrIndex i = 0 ; i<ish(0) ; i++)
    for (ArrIndex j = 0 ; j<ish(1) ; j++) {
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

  for (ArrIndex cur=0 ; cur<3 ; cur++) {

    for (ArrIndex i = 0 ; i<sh(0) ; i++)
      for (ArrIndex j = 0 ; j<sh(1); j++) {
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


