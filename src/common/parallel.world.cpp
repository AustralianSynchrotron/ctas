#include "common.h"
#include <unistd.h>

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
  std::vector<pthread_t> threads;

  void * arg;
  bool (*sub_routine0) ();
  bool (*sub_routine1) (long int);
  bool (*sub_routine2) (void *, long int);
  bool sub_routine (long int idx) {
    if (sub_routine0) return sub_routine0();
    if (sub_routine1) return sub_routine1(idx);
    if (sub_routine2) return sub_routine2(arg, idx);
    return false;
  }

  inline ThreadDistributor( bool (*_sub_routine0)(),
                     bool (*_sub_routine1)(long int),
                     bool (*_sub_routine2) (void *, long int),
                     void * _arg)
    : idxLock(PTHREAD_MUTEX_INITIALIZER)
    , startLock(PTHREAD_MUTEX_INITIALIZER)
    , startCond(PTHREAD_COND_INITIALIZER)
    , currentidx(0)
    , arg(_arg)
    , sub_routine0(_sub_routine0)
    , sub_routine1(_sub_routine1)
    , sub_routine2(_sub_routine2)
  {}

  ThreadDistributor( bool (*_sub_routine)() )
    : ThreadDistributor(_sub_routine, 0, 0, 0)
  {}

  ThreadDistributor( bool (*_sub_routine)(long int) )
    : ThreadDistributor(0, _sub_routine, 0, 0)
  {}

  ThreadDistributor( bool (*_sub_routine) (void *, long int), void * _arg )
    : ThreadDistributor(0, 0, _sub_routine, _arg)
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
    while ( in_threade(dist) ) {}
    return 0;
  }

  void start(uint nThreads=0) {
    pthread_t thread;
    if (!nThreads)
      nThreads=run_threads;
    for (int ith = 0 ; ith < nThreads ; ith++)
      if ( pthread_create( & thread, NULL, in_thread, this ) )
        warn("Thread operation", "Can't create thread.");
      else
        threads.push_back(thread);
    pthread_cond_signal(&startCond);
  }

  void finish() {
    for (int ith = 0 ; ith < threads.size() ; ith++)
      pthread_join( threads[ith], 0);
  }


public:

  static void execute( bool (*_thread_routine) (void *, long int), void * _arg, int nThreads=0 ) {
    ThreadDistributor dist(_thread_routine, _arg);
    dist.start(nThreads);
    dist.finish();
  }

  static void execute( bool (*_thread_routine) (long int), int nThreads=0) {
    ThreadDistributor dist(_thread_routine);
    dist.start(nThreads);
    dist.finish();
  }

  static void execute( bool (*_thread_routine)(), int nThreads=0) {
    ThreadDistributor dist(_thread_routine);
    dist.start(nThreads);
    dist.finish();
  }

};





void InThread::execute(int nThreads) {
  bar.start();
  ThreadDistributor::execute(inThread, this, nThreads);
}

void InThread::execute( bool (*_thread_routine) (long int), int nThreads) {
  ThreadDistributor::execute(_thread_routine, nThreads);
}

void InThread::execute( bool (*_thread_routine)(), int nThreads) {
  ThreadDistributor::execute(_thread_routine, nThreads);
}

void InThread::lock(int idx) {
  if (!locks.count(idx)) {
    pthread_mutex_lock(&proglock);
    // check again just for the case if was added before locking above
    if (!locks.count(idx))
      locks.emplace(idx, new pthread_mutex_t(PTHREAD_MUTEX_INITIALIZER));
    pthread_mutex_unlock(&proglock);
  }
  pthread_mutex_lock(locks.at(idx));
}

void InThread::unlock(int idx) {
  if (!locks.count(idx))
    throw_error("InThread", "Trying to unlock non-existing mutex " +toString(idx)+ ".");
  pthread_mutex_unlock(locks.at(idx));
}



#ifdef OPENCL_FOUND

//#include<libconfig.h++>
//using namespace libconfig;

cl_device_id _CL_device = 0;
cl_context _CL_context = 0;
cl_command_queue _CL_queue = 0;


static bool clIsInited();
cl_device_id CL_device() { return clIsInited() ? _CL_device : 0; }
cl_context CL_context() { return clIsInited() ? _CL_context : 0; }
cl_command_queue CL_queue() { return clIsInited() ? _CL_queue : 0; }




static bool clInited = false;

bool clIsInited() {

  if (clInited)
    return true;

  cl_int err;

  cl_uint nof_platforms;
  err = ::clGetPlatformIDs(0, 0, &nof_platforms);
  if (err != CL_SUCCESS) {
    warn("OpenCLinit", "Could not get number of OpenCL platforms: " + toString(err) );
    return false;
  }

  vector<cl_platform_id> platforms(nof_platforms);
  vector<cl_device_id> devices;

  err = clGetPlatformIDs(nof_platforms, platforms.data(), 0);
  if (err != CL_SUCCESS) {
    warn("OpenCLinit", "Could not get OpenCL platforms: " + toString(err) );
    return false;
  }

  for (int plidx=0; plidx < platforms.size() ; plidx++ ) {

    cl_uint nof_devices = 0;

    err = clGetDeviceIDs( platforms[plidx], CL_DEVICE_TYPE_GPU, 0, 0, &nof_devices);
    if (err != CL_SUCCESS) {
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

          cl_device_id dev = platform_devices[devidx];
          bool errHappened=false;

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

          if ( ! errHappened &&
               devIsAvailable &&
               devCompilerIsAvailable &&
               ( devExecCapabilities & CL_EXEC_KERNEL ) )
            devices.push_back(dev);

        }

      }

    }

  }


  if (devices.empty()) {

    warn("OpenCLinit", "No OpenCL devices found.");
    return false;

  } else { // more than one device found

    int idx=-1;
    cl_ulong devmem, devmaxmem=0;
    for (int devidx=0; devidx < devices.size(); devidx++) {
      err = clGetDeviceInfo(devices[devidx], CL_DEVICE_GLOBAL_MEM_SIZE,
                            sizeof(cl_ulong),  &devmem, 0);
      if (err == CL_SUCCESS  &&  devmem > devmaxmem) {
        devmaxmem = devmem;
        idx=devidx;
      }
    }

    if (idx >= 0)
      _CL_device = devices[idx];

    /* TODO:
     * complete this part to read device name from the config
     * file instead of choosing the device with maximum memory
     * above.
     */

  }

  cl_platform_id platform;
  err = clGetDeviceInfo(_CL_device, CL_DEVICE_PLATFORM, sizeof(cl_platform_id),  &platform, 0);
  if (err != CL_SUCCESS) {
    warn("OpenCLinit", "Could not get OpenCL device info \"CL_DEVICE_PLATFORM\": "
         + toString(err) );
    return false;
  }

  _CL_context = clCreateContext(0, 1, &_CL_device, 0, 0, &err);
  if (err != CL_SUCCESS) {
    warn("OpenCLinit", "Could not create OpenCL context: " + toString(err) );
    return false;
  }


  _CL_queue = clCreateCommandQueueWithProperties(_CL_context, _CL_device, NULL, &err);
  //_CL_queue = clCreateCommandQueue(_CL_context, _CL_device, 0, &err);
  if (err != CL_SUCCESS) {
    warn("OpenCLinit", "Could not create OpenCL queue: " + toString(err) );
    return false;
  }

  clInited = true;
  return clInited;

}



cl_program initProgram(const string & src, const string & modname) {

  if ( ! clIsInited() )
    return 0;

  cl_int err = CL_SUCCESS;
  const size_t length = src.size();
  const char * csrc = src.data();

  cl_program program = clCreateProgramWithSource(CL_context(), 1, &csrc, &length, &err);
  if (err != CL_SUCCESS) {
    warn(modname, "Could not load OpenCL program: " + toString(err) );
    return 0;
  }

  err = clBuildProgram( program, 0, 0, "", 0, 0);
  if (err != CL_SUCCESS) {

    warn(modname, (string) "Could not build OpenCL program: " + toString(err) +
    ". More detailsd below:" );

    cl_build_status stat;
    err = clGetProgramBuildInfo(program, CL_device(), CL_PROGRAM_BUILD_STATUS,
                                sizeof(cl_build_status), &stat, 0);
    if (err != CL_SUCCESS)
      warn(modname, "Could not get OpenCL program build status: " + toString(err) );
    else
      warn(modname, "   Build status: " + toString(stat));

    size_t len=0;
    err=clGetProgramBuildInfo(program, CL_device(), CL_PROGRAM_BUILD_OPTIONS,
                              0, 0, &len);
    char * buildOptions = (char*) calloc(len, sizeof(char));
    if (buildOptions)
      err=clGetProgramBuildInfo(program, CL_device(), CL_PROGRAM_BUILD_OPTIONS,
                                len, buildOptions, 0);
    if (err != CL_SUCCESS)
      warn(modname, "Could not get OpenCL program build options: " + toString(err) );
    else
      warn(modname, "   Build options: \"" + string(buildOptions, len) + "\"");
    if (buildOptions)
      free(buildOptions);

    err = clGetProgramBuildInfo(program, CL_device(), CL_PROGRAM_BUILD_LOG,
                                0, 0, &len);
    char * buildLog = (char*) calloc(len, sizeof(char));
    if (buildLog)
      err = clGetProgramBuildInfo(program, CL_device(), CL_PROGRAM_BUILD_LOG,
                                  len, buildLog, 0);
    if (err != CL_SUCCESS)
      warn(modname, "Could not get OpenCL program build log: " + toString(err) );
    else
      warn(modname, "   Build log:\n" + string(buildLog, len));
    if (buildLog)
      free(buildLog);

    warn(modname, "\n");

    return 0;

  }

  return program;

}


cl_program & initProgram(const string & src, cl_program & program, const string & modname) {
  static pthread_mutex_t protectProgramCompilation = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_lock(&protectProgramCompilation);
  if (!program)
    program = initProgram(src, modname);
  pthread_mutex_unlock(&protectProgramCompilation);
  return program;
}




CLkernel & CLkernel::operator()(cl_program program, const std::string & name) {
  if ( ! program || name.empty() ) {
    if (kern)
      clReleaseKernel(kern) ;
    kern = 0;
    return *this;
  }
  cl_int clerr;
  kern = clCreateKernel ( program, name.c_str(), &clerr);
  if (clerr != CL_SUCCESS) {
    kern = 0;
    throw_error("CLkernel",
                "Could not create OpenCL kernel \"" + name + "\": " + toString(clerr));
  }
  return *this;
}

std::string CLkernel::name() const {
  if (!kern)
    return "";
  size_t len=0;
  cl_int clerr = clGetKernelInfo ( kern, CL_KERNEL_FUNCTION_NAME, 0, 0, &len);
  if ( clerr != CL_SUCCESS )
    throw_error("kernelName", "Could not get OpenCL kernel name size: " + toString(clerr));
  char *kernel_function = (char *) calloc(len, sizeof(char));
  std::string _name;
  if (kernel_function) {
    clerr = clGetKernelInfo ( kern, CL_KERNEL_FUNCTION_NAME, len, kernel_function, 0);
    if (clerr == CL_SUCCESS)
      _name = std::string(kernel_function, len);
    free(kernel_function);
    if (clerr != CL_SUCCESS)
      throw_error("kernelName", "Could not get OpenCL kernel name: " + toString(clerr));
  }
  return _name;
}

cl_int CLkernel::exec(size_t size) const {
  if (!kern)
    return CL_SUCCESS;
  cl_int clerr = clEnqueueNDRangeKernel( CL_queue(), kern, 1, 0,  & size, 0, 0, 0, 0);
  if (clerr != CL_SUCCESS)
    throw_error("execKernel", "Failed to execute OpenCL kernel " + toString("%p", kern) + "\"" + name() + "\": " + toString(clerr));
  clerr = clFinish(CL_queue());
  if (clerr != CL_SUCCESS)
    throw_error("execKernel", "Failed to finish OpenCL kernel \"" + name() + "\": " + toString(clerr));
  return clerr;
}

cl_int CLkernel::exec(const Shape & sh) const {
  if (!kern)
    return CL_SUCCESS;
  size_t sizes[2] = {size_t(sh(1)), size_t(sh(0))};
  cl_int clerr = clEnqueueNDRangeKernel( CL_queue(), kern, 2, 0, sizes, 0, 0, 0, 0);
  if (clerr != CL_SUCCESS)
    throw_error("execKernel", "Failed to execute OpenCL kernel \"" + name() + "\": " + toString(clerr));
  clerr = clFinish(CL_queue());
  if (clerr != CL_SUCCESS)
    throw_error("execKernel", "Failed to finish OpenCL kernel \"" + name() + "\": " + toString(clerr));
  return clerr;
}

cl_int CLkernel::exec(const Shape3 & sh) const {
  if (!kern)
    return CL_SUCCESS;
  size_t sizes[3] = {size_t(sh(2)), size_t(sh(1)), size_t(sh(0))};
  cl_int clerr = clEnqueueNDRangeKernel( CL_queue(), kern, 3, 0, sizes, 0, 0, 0, 0);
  if (clerr != CL_SUCCESS)
    throw_error("execKernel", "Failed to execute OpenCL kernel \"" + name() + "\": " + toString(clerr));
  clerr = clFinish(CL_queue());
  if (clerr != CL_SUCCESS)
    throw_error("execKernel", "Failed to finish OpenCL kernel \"" + name() + "\": " + toString(clerr));
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


