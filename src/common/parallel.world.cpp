#include "parallel.world.h"
#include "matrix.world.h"
#include <unistd.h>
#include <list>

using namespace std;

long nof_threads(long _threads) {
  if (_threads)
    return _threads;

#ifdef _WIN32
#ifndef _SC_NPROCESSORS_ONLN
  SYSTEM_INFO info;
  GetSystemInfo(&info);
#define sysconf(a) info.dwNumberOfProcessors
#define _SC_NPROCESSORS_ONLN
#endif
#endif

  long nProcessorsOnline = sysconf(_SC_NPROCESSORS_ONLN);
  if (nProcessorsOnline == -1) {
    warn ("thread number",
          "Unable to read online processor count.");
    return 1;
  } else {
    return nProcessorsOnline;
  }

}

static const int run_threads = nof_threads();




class ThreadDistributor {

private :

  pthread_mutex_t idxLock; // to protect index incriment
  pthread_mutex_t startLock; // to protect start signal.
  pthread_cond_t startCond; // to be signalled after all threads has started

  long int currentidx;
  std::list<pthread_t> threads;
  bool somefinished;

  void * arg;
  function<bool(void*)> before_thread;
  function<bool()> sub_routine0;
  function<bool(long int)> sub_routine1;
  function<bool(void*, long int)> sub_routine2;
  bool sub_routine (long int idx) {
    if (sub_routine0) return sub_routine0();
    if (sub_routine1) return sub_routine1(idx);
    if (sub_routine2) return sub_routine2(arg, idx);
    return false;
  }

  inline ThreadDistributor( function<bool()> _sub_routine0,
                            function<bool(long int)> _sub_routine1,
                            function<bool(void*, long int)> _sub_routine2,
                            void * _arg = 0,
                            function<bool(void*)> _before_thread = 0 )
    : idxLock(PTHREAD_MUTEX_INITIALIZER)
    , startLock(PTHREAD_MUTEX_INITIALIZER)
    , startCond(PTHREAD_COND_INITIALIZER)
    , currentidx(0)
    , arg(_arg)
    , before_thread(_before_thread)
    , sub_routine0(_sub_routine0)
    , sub_routine1(_sub_routine1)
    , sub_routine2(_sub_routine2)
  {}

  ThreadDistributor( function<bool()> _sub_routine )
    : ThreadDistributor(_sub_routine, 0, 0)
  {}

  ThreadDistributor( function<bool(long int)> _sub_routine )
    : ThreadDistributor(0, _sub_routine, 0)
  {}

  ThreadDistributor( function<bool(void *, long int)> _sub_routine, void * _arg, function<bool(void*)> _before_thread = 0)
    : ThreadDistributor(0, 0, _sub_routine, _arg, _before_thread)
  {}

  ~ThreadDistributor() {
    pthread_mutex_destroy(&idxLock);
    pthread_mutex_destroy(&startLock);
    pthread_cond_destroy(&startCond);
  }


  long int distribute() {
    long int idx;
    pthread_mutex_lock(&idxLock);
    idx = currentidx++;
    pthread_mutex_unlock(&idxLock);
    return idx;
  }


  static bool in_threade (ThreadDistributor * dist) {
    bool ret;
    try {
      ret = dist->sub_routine(dist->distribute());
    } catch (...) {
      return false;
    }
    return ret;
  }


  static void * in_thread (void * vdist) {
    ThreadDistributor * dist = (ThreadDistributor*) vdist;
    bool keepgoing=true;
    if (dist->before_thread) {
      try {
        keepgoing = dist->before_thread(dist->arg);
      } catch (...) {
        keepgoing=false;
      }
    }
    pthread_mutex_unlock(&dist->startLock);
    if (!keepgoing)
      return vdist;
    while ( in_threade(dist) ) {}
    dist->somefinished=true;
    return vdist;
  }

  void start(uint nThreads=0) {
    if (!nThreads)
      nThreads=run_threads;
    somefinished=false;
    pthread_mutex_lock(&startLock);
    pthread_t thread;
    for (int ith = 0 ; ith < nThreads ; ith++) {
      if (somefinished)
        break;
      if ( pthread_create( & thread, NULL, in_thread, this ) ) {
        warn("Thread operation", "Can't create thread.");
        pthread_mutex_unlock(&startLock);
      } else
        threads.emplace_back(thread);
      pthread_mutex_lock(&startLock);
    }
    pthread_mutex_unlock(&startLock);
    pthread_cond_signal(&startCond);
  }

  void finish() {
    for (auto curTh = threads.begin() ; curTh != threads.end() ; curTh++)
      pthread_join(*curTh, NULL);
  }


public:

  static void execute( function<bool()> _thread_routine, int nThreads=0) {
    ThreadDistributor dist(_thread_routine);
    dist.start(nThreads);
    dist.finish();
  }

  static void execute( function<bool(long int)> _thread_routine, int nThreads=0 ) {
    ThreadDistributor dist(_thread_routine);
    dist.start(nThreads);
    dist.finish();
  }

  static void execute( function<bool(void*, long int)> _thread_routine, void * _arg, int nThreads=0 ) {
    ThreadDistributor dist(_thread_routine, _arg);
    dist.start(nThreads);
    dist.finish();
  }

  static void execute( function<bool(void*, long int)> _thread_routine, function<bool(void*)> _before_routine,
                       void * _arg, int nThreads=0 ) {
    ThreadDistributor dist(_thread_routine, _arg, _before_routine);
    dist.start(nThreads);
    dist.finish();
  }

};




const string InThread::modname="InThread";

void InThread::execute(int nThreads) {
  bar.start();
  ThreadDistributor::execute([&](void* arg, long int n){return inThread(arg, n);},
                             [&](void* arg){return beforeThread(arg);},
                             this, nThreads);
}

void InThread::execute(function<bool()> _thread_routine, int nThreads) {
  ThreadDistributor::execute(_thread_routine, nThreads);
}

void InThread::execute( function<bool(long int)> _thread_routine, int nThreads) {
  ThreadDistributor::execute(_thread_routine, nThreads);
}

void InThread::execute( int from, int to, function<void(long int)> _thread_routine, int nThreads) {
  auto toExec = [&](long int idx) {
    if (to<=from) return false;
    if (idx>=to) return false;
    if (idx>=from)
      _thread_routine(idx);
    return true;
  };
  ThreadDistributor::execute(toExec, nThreads);
}


void InThread::needMutexes(uint nof_mut) {
  if (nof_mut==locks.size())
    return;
  if (nof_mut<locks.size()) {
    warn(modname, "Request to reduce number of available mutexes will be ignored.");
    return;
  }
  while (locks.size() != nof_mut)
    locks.push_back(PTHREAD_MUTEX_INITIALIZER);
}

void InThread::lock(int idx) {
  if (idx>=locks.size())
    throw_error(modname, "Trying to lock non-existing mutex " +toString(idx)+ ".");
  pthread_mutex_lock(&locks.at(idx));
}

void InThread::unlock(int idx) {
  if (idx>=locks.size())
    throw_error(modname, "Trying to unlock non-existing mutex " +toString(idx)+ ".");
  pthread_mutex_unlock(&locks.at(idx));
}



#ifdef OPENCL_FOUND

std::vector<CLenv> clenvs(0);
int cl_defidx=-1;
static bool CL_intialize();
cl_device_id CL_device() { CL_intialize(); return cl_defidx<0 ? 0 : clenvs[cl_defidx].dev; }
cl_context CL_context() { CL_intialize(); return cl_defidx<0 ? 0 : clenvs[cl_defidx].cont; }
cl_command_queue CL_queue() { CL_intialize(); return cl_defidx<0 ? 0 : clenvs[cl_defidx].que; }


bool _CL_intialize() {

  if (cl_defidx>=0)
    return true;

  cl_int err;

  cl_uint nof_platforms;
  err = ::clGetPlatformIDs(0, 0, &nof_platforms);
  if ( err != CL_SUCCESS  ||  0 >= nof_platforms ) {
    warn("OpenCLinit", "Could not get number of OpenCL platforms: " + toString(err) );
    return false;
  }
  vector<cl_platform_id> platforms(nof_platforms);
  err = clGetPlatformIDs(nof_platforms, platforms.data(), 0);
  if (err != CL_SUCCESS) {
    warn("OpenCLinit", "Could not get OpenCL platforms: " + toString(err) );
    return false;
  }

  for (int plidx=0; plidx < platforms.size() ; plidx++ ) {

    cl_uint nof_devices = 0;
    err = clGetDeviceIDs( platforms[plidx], CL_DEVICE_TYPE_GPU, 0, 0, &nof_devices);
    if ( err != CL_SUCCESS || 0 >= nof_devices ) {
      warn("OpenCLinit", "Could not get OpenCL number of GPU devices of a platform: "
           + toString(err) );
    } else {

      vector<cl_device_id> platform_devices(nof_devices);
      err = clGetDeviceIDs( platforms[plidx], CL_DEVICE_TYPE_GPU,
                            nof_devices, platform_devices.data(), 0);
      if (err != CL_SUCCESS) {
        warn("OpenCLinit", "Could not get OpenCL GPU devices of a platform: "
             + toString(err) );
      } else {

        for (int devidx=0; devidx<platform_devices.size(); devidx++) {

          bool errHappened=false;
          cl_device_id dev = platform_devices[devidx];

          cl_bool devIsAvailable;
          err = clGetDeviceInfo(dev, CL_DEVICE_AVAILABLE,
                                sizeof(cl_bool), &devIsAvailable, 0);
          if (err != CL_SUCCESS) {
            warn("OpenCLinit", "Could not get OpenCL device info \"CL_DEVICE_AVAILABLE\": "
                 + toString(err) );
            errHappened=true;
          }

          cl_bool devCompilerIsAvailable;
          err = clGetDeviceInfo(dev, CL_DEVICE_COMPILER_AVAILABLE, sizeof(cl_bool),
                                &devCompilerIsAvailable, 0);
          if (err != CL_SUCCESS) {
            warn("OpenCLinit", "Could not get OpenCL device info \"CL_DEVICE_COMPILER_AVAILABLE\": "
                 + toString(err) );
            errHappened=true;
          }

          cl_device_exec_capabilities devExecCapabilities;
          err = clGetDeviceInfo(dev, CL_DEVICE_EXECUTION_CAPABILITIES, sizeof(cl_device_exec_capabilities),
                                &devExecCapabilities, 0);
          if (err != CL_SUCCESS) {
            warn("OpenCLinit", "Could not get OpenCL device info \"CL_DEVICE_EXECUTION_CAPABILITIES\": "
                 + toString(err) );
            errHappened=true;
          }

          cl_context cont = clCreateContext(0, 1, &dev, 0, 0, &err);
          if (err != CL_SUCCESS || ! cont) {
            warn("OpenCLinit", "Could not create OpenCL context: " + toString(err) );
            errHappened=true;
          }

          cl_command_queue que = clCreateCommandQueueWithProperties(cont, dev, NULL, &err);
          if (err != CL_SUCCESS || ! que) {
            warn("OpenCLinit", "Could not create OpenCL queue: " + toString(err) );
            errHappened=true;
          }

          if ( ! errHappened &&
               devIsAvailable &&
               devCompilerIsAvailable &&
               ( devExecCapabilities & CL_EXEC_KERNEL ) )
            clenvs.push_back({dev, cont, que});

        }

      }

    }

  }
  if (clenvs.empty()) {
    warn("OpenCLinit", "No OpenCL devices found.");
    return false;
  }

  cl_ulong devmem, devmaxmem=0;
  for (int devidx=0; devidx < clenvs.size(); devidx++) {
    err = clGetDeviceInfo(clenvs[devidx].dev, CL_DEVICE_GLOBAL_MEM_SIZE,
                          sizeof(cl_ulong),  &devmem, 0);
    if (err == CL_SUCCESS  &&  devmem > devmaxmem) {
      devmaxmem = devmem;
      cl_defidx=devidx;
    }
  }
  if (cl_defidx<0) {
    warn("OpenCLinit", "Could not select default OpenCL device.");
    return false;
  }
  return true;

}

static pthread_mutex_t lockCLinit(PTHREAD_MUTEX_INITIALIZER);
bool CL_intialize() {
  if (cl_defidx>=0)
    return true;
  bool toRet=false;
  pthread_mutex_lock(&lockCLinit);
  try { toRet = _CL_intialize(); } catch (...) { }
  pthread_mutex_unlock(&lockCLinit);
  return toRet;
}


CLenv & clenv(const cl_context cont) {
  for (CLenv & env : clenvs)
    if (env.cont == cont)
      return env;
  throw_error("CLenv", "Found no OpenCL context "+toString((void*)cont)+".");
}



CLprogram & CLprogram::operator()(const string & source, cl_context context) {

  static const string modname = "OpenCL program";
  static pthread_mutex_t protectProgramCompilation = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_lock(&protectProgramCompilation);

  prog = [&source,context] () -> cl_program {

    if ( ! CL_intialize() )
      return 0;

    cl_int err = CL_SUCCESS;
    const size_t length = source.size();
    const char * csrc = source.data();

    cl_uint numdev;
    err = clGetContextInfo(context, CL_CONTEXT_NUM_DEVICES, sizeof(cl_uint), &numdev, 0);
    if (err != CL_SUCCESS) {
      warn(modname, "Could not get number of OpenCL devices from context: " + toString(err) );
      return 0;
    }
    if (numdev != 1) {
      warn(modname, "Context with non-single device.");
      return 0;
    }
    cl_device_id cldev;
    err = clGetContextInfo(context, CL_CONTEXT_DEVICES, sizeof(cl_device_id), &cldev, 0);
    if (err != CL_SUCCESS) {
      warn(modname, "Could not get OpenCL device from context: " + toString(err) );
      return 0;
    }


    cl_program program = clCreateProgramWithSource(context, 1, &csrc, &length, &err);
    if (err != CL_SUCCESS) {
      warn(modname, "Could not load OpenCL program: " + toString(err) );
      return 0;
    }

    err = clBuildProgram( program, 0, 0, "", 0, 0);
    if (err != CL_SUCCESS) {

      warn(modname, (string) "Could not build OpenCL program: " + toString(err) +
      ". More detailsd below:" );

      cl_build_status stat;
      err = clGetProgramBuildInfo(program, cldev, CL_PROGRAM_BUILD_STATUS,
                                  sizeof(cl_build_status), &stat, 0);
      if (err != CL_SUCCESS)
        warn(modname, "Could not get OpenCL program build status: " + toString(err) );
      else
        warn(modname, "   Build status: " + toString(stat));

      size_t len=0;
      err=clGetProgramBuildInfo(program, cldev, CL_PROGRAM_BUILD_OPTIONS, 0, 0, &len);
      char * buildOptions = (char*) calloc(len, sizeof(char));
      if (buildOptions)
        err=clGetProgramBuildInfo(program, cldev, CL_PROGRAM_BUILD_OPTIONS, len, buildOptions, 0);
      if (err != CL_SUCCESS)
        warn(modname, "Could not get OpenCL program build options: " + toString(err) );
      else
        warn(modname, "   Build options: \"" + string(buildOptions, len) + "\"");
      if (buildOptions)
        ::free(buildOptions);

      err = clGetProgramBuildInfo(program, cldev, CL_PROGRAM_BUILD_LOG, 0, 0, &len);
      char * buildLog = (char*) calloc(len, sizeof(char));
      if (buildLog)
        err = clGetProgramBuildInfo(program, cldev, CL_PROGRAM_BUILD_LOG, len, buildLog, 0);
      if (err != CL_SUCCESS)
        warn(modname, "Could not get OpenCL program build log: " + toString(err) );
      else
        warn(modname, "   Build log:\n" + string(buildLog, len));
      if (buildLog)
        ::free(buildLog);

      warn(modname, "\n");

      return 0;

    }

    return program;

  }();

  pthread_mutex_unlock(&protectProgramCompilation);
  if (!prog)
    throw_error(modname, "Failed to compile.");
  return *this;

}


cl_context CLprogram::context() const {
  if(!prog)
    return 0;
  cl_context toRet=0;
  cl_int clerr = clGetProgramInfo(prog, CL_PROGRAM_CONTEXT, sizeof(cl_context), &toRet, NULL);
  if ( clerr != CL_SUCCESS )
    throw_error("CLprogram", "Failed to get context of OpenCL program: " + toString(clerr));
  return toRet;
}


void CLprogram::free() {
  cl_program toDel = prog;
  prog=0;
  if (toDel)
    clReleaseProgram(toDel);
}



const string CLkernel::modname = "CLkernel";


CLkernel & CLkernel::operator()(const CLprogram & program, const std::string & name) {
  if ( ! program || name.empty() ) {
    if (kern)
      clReleaseKernel(kern) ;
    kern = 0;
    return *this;
  }
  cl_int clerr;
  kern = clCreateKernel ( program(), name.c_str(), &clerr);
  if (clerr != CL_SUCCESS) {
    kern = 0;
    throw_error(modname, "Could not create OpenCL kernel \"" + name + "\": " + toString(clerr));
  }
  return *this;
}


string CLkernel::name() const {
  if (!kern)
    return "";
  size_t len=0;
  cl_int clerr = clGetKernelInfo ( kern, CL_KERNEL_FUNCTION_NAME, 0, 0, &len);
  if ( clerr != CL_SUCCESS )
    throw_error(modname, "Could not get OpenCL kernel name size: " + toString(clerr));
  string kernel_function(len-1, ' '); // -1 to exclude NL
  if (kernel_function.data()) {
    clerr = clGetKernelInfo ( kern, CL_KERNEL_FUNCTION_NAME, len, kernel_function.data(), 0);
    if (clerr != CL_SUCCESS)
      throw_error(modname, "Could not get OpenCL kernel name: " + toString(clerr));
  }
  return kernel_function;
}

cl_program CLkernel::program() const {
  if (!kern)
    return 0;
  cl_program prog=0;
  cl_int clerr = clGetKernelInfo ( kern, CL_KERNEL_PROGRAM, sizeof(cl_program), &prog, 0);
  if ( clerr != CL_SUCCESS )
    throw_error(modname, "Could not get OpenCL kernel's program: " + toString(clerr));
  return prog;
}

cl_context CLkernel::context() const {
  if (!kern)
    return 0;
  cl_context cont=0;
  cl_int clerr = clGetKernelInfo ( kern, CL_KERNEL_CONTEXT, sizeof(cl_context), &cont, 0);
  if ( clerr != CL_SUCCESS )
    throw_error(modname, "Could not get OpenCL kernel's context: " + toString(clerr));
  return cont;
}


cl_int CLkernel::exec(size_t dims, size_t * sizes, cl_command_queue clque) const {
  if (!kern)
    return CL_SUCCESS;
  cl_int clerr = clEnqueueNDRangeKernel( clque, kern, dims, 0, sizes, 0, 0, 0, 0);
  if (clerr != CL_SUCCESS)
    throw_error(modname, "Failed to execute OpenCL kernel \"" + name() + "\": " + toString(clerr));
  clerr = clFinish(clque);
  if (clerr != CL_SUCCESS)
    throw_error(modname, "Failed to finish OpenCL kernel \"" + name() + "\": " + toString(clerr));
  return clerr;
}



/*

cl_mem clAllocFImage(const Shape sh, cl_mem_flags flag) {
  cl_int err;
  const cl_image_desc ifdesc(
    { CL_MEM_OBJECT_IMAGE2D, size_t(sh(1)), size_t(sh(0)), 1, 1, 0, 0, 0, 0, {0} } ) ;
  cl_mem clImage = clCreateImage( CL_context, flag, &clfimage_format, &ifdesc, 0, &err);
  if (err != CL_SUCCESS)
    throw_error("OpenCL", "Could not create OpenCL image: " + toString(err) );
  return clImage;
}

cl_mem blitz2clfi(const Map & storage, cl_mem clStorage, cl_mem_flags flag) {
  const size_t origin[3] = {0,0,0};
  const size_t region[3] = {size_t(storage.shape()(1)), size_t(storage.shape()(0)),1};
  cl_int err = clEnqueueWriteImage( CL_queue, clStorage, CL_TRUE, origin, region
                                  , 0, 0, storage.data(), 0, 0, 0);
  if (err != CL_SUCCESS)
    throw_error("OpenCL", "Could not write OpenCL image: " + toString(err) );
  return clStorage;
}

cl_mem blitz2clfi(const Map & storage, cl_mem_flags flag) {
  cl_mem clStorage = clAllocFImage(storage.shape(), flag);
  return blitz2clfi(storage, clStorage, flag);
}


cl_mem clAllocFImage(const Shape3 sh, cl_mem_flags flag) {
  cl_int err;
  const cl_image_desc ifdesc(
    { CL_MEM_OBJECT_IMAGE3D, size_t(sh(2)), size_t(sh(1)), size_t(sh(0)), 1, 0, 0, 0, 0, {0} } ) ;
  cl_mem clImage = clCreateImage( CL_context, flag, &clfimage_format, &ifdesc, 0, &err);
  if (err != CL_SUCCESS)
    throw_error("OpenCL", "Could not create OpenCL image: " + toString(err) );
  return clImage;
}

cl_mem blitz2clfi(const Volume & storage, cl_mem clStorage, cl_mem_flags flag) {
  const size_t origin[3] = {0,0,0};
  const size_t region[3] = {size_t(storage.shape()(2)),
                            size_t(storage.shape()(1)),
                            size_t(storage.shape()(0))};
  cl_int err = clEnqueueWriteImage( CL_queue, clStorage, CL_TRUE, origin, region
                                  , 0, 0, storage.data(), 0, 0, 0);
  if (err != CL_SUCCESS)
    throw_error("OpenCL", "Could not write OpenCL image: " + toString(err) );
  return clStorage;
}

cl_mem blitz2clfi(const Volume & storage, cl_mem_flags flag) {
  cl_mem clStorage = clAllocFImage(storage.shape(), flag);
  return blitz2clfi(storage, clStorage, flag);
}


static void clImageInfo(cl_mem image, size_t region[3]) {
  cl_int clerr;
  if (  CL_SUCCESS != (clerr = clGetImageInfo(image, CL_IMAGE_WIDTH,   sizeof(size_t), region,   0) )
     || CL_SUCCESS != (clerr = clGetImageInfo(image, CL_IMAGE_HEIGHT,  sizeof(size_t), region+1, 0) )
     || CL_SUCCESS != (clerr = clGetImageInfo(image, CL_IMAGE_DEPTH,   sizeof(size_t), region+2, 0) )  )
    throw_error("execFill", "Failed to get image parameter: " + toString(clerr));
}


cl_int fillClImage(cl_mem image, float val) {
  const size_t origin[3] = {0,0,0};
  size_t region[3];
  const cl_float4 valc = {{val,0,0,0}};
  clImageInfo(image, region);
  int clerr = clEnqueueFillImage(CL_queue, image, &valc, origin, region, 0, 0, 0);
  if (clerr != CL_SUCCESS)
    throw_error("execFill", "Failed to execute OpenCL image fill: " + toString(clerr));
  clerr = clFinish(CL_queue);
  if (clerr != CL_SUCCESS)
    throw_error("execFill", "Failed to finish OpenCL image fill: " + toString(clerr));
  return clerr;
}


void clfi2blitz(cl_mem image, Map & storage) {
  const size_t origin[3] = {0,0,0};
  size_t region[3];
  clImageInfo(image, region);
  if ( region[2] > 1 )
    throw_error("Read CLimage", "Can't read 3D image into Map array.");
  storage.resize(region[1], region[0]);
  if ( ! storage.size() )
    throw_error("Read CLimage", "Empty image.");
  cl_int clerr = clEnqueueReadImage(CL_queue, image, CL_TRUE, origin, region
                                    , 0, 0, storage.data(), 0, 0, 0);
  if (clerr != CL_SUCCESS)
    throw_error("Read CLimage", "Failed to read image.");
}

void clfi2blitz(cl_mem image, Volume & storage) {
  const size_t origin[3] = {0,0,0};
  size_t region[3];
  clImageInfo(image, region);
  storage.resize(region[2], region[1], region[0]);
  if ( ! storage.size() )
    throw_error("Read CLimage", "Empty image.");
  cl_int clerr = clEnqueueReadImage(CL_queue, image, CL_TRUE, origin, region
                                    , 0, 0, storage.data(), 0, 0, 0);
  if (clerr != CL_SUCCESS)
    throw_error("Read CLimage", "Failed to read image.");
}


*/

#endif // OPENCL_FOUND


