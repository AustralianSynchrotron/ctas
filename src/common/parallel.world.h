#ifndef PARALLEL_WORLD_H
#define PARALLEL_WORLD_H

#ifndef _H_CTAS_H_
#pragma message "File" __FILE__ "is not supposed to be included implicitly."
#pragma message "Expect compilation failure."
#include "common.h"
#endif

/// \brief Number of threads for the process.
///
/// @param _threads Requested number of threads (0 for auto).
///
/// @return Number of threads for the architecture where the process is running
/// if automatic number of threads was requested and just _threads if set in stone.
///
long
nof_threads(long _threads=0);






/// \brief Base class used for multithreading.
///
/// This virtual class helps to ease the implementation of multithread parallel computation.
/// The subclasses must implement only three entities:
/// 1. Data members - to store and make accessible all the data needed for threads to work.
/// 2. Constructor which initializes the data and, if needed, deconstructor to clean up.
/// 3. Pure virtual method bool inThread(long int).
///    This method is called from threads with the index to work on as the input.
///    It must return false if there left nothing to do in the queue and true otherwise.
///
/// Once it is done the execute() method can be used on the subclass to run the tasks.
/// Two static execute(...) methods are here to implement even easier computation for the cases
/// where no data are required and/or the index is not used for the inThread implementation:
/// just provide the input _thread_routine function.
class InThread {

private:

  pthread_mutex_t proglock;
  std::unordered_map<int, pthread_mutex_t*> locks; // to be used by the sub-classes users via lock/unlock methods

  virtual bool inThread(long int) = 0;

  inline static bool
  inThread(void * args, long int idx) {
    if (!args)
      throw_error("InThread", "Wrongly used class. Report to developers.");
    return ((InThread*)args)->inThread(idx);
  }

public:

  ProgressBar bar;

  InThread(bool verbose=false, const std::string procName = std::string(), int steps=0)
    : proglock(PTHREAD_MUTEX_INITIALIZER)
    , bar(verbose, procName, steps)
  { if (steps) bar.start(); }

  ~InThread() {
    for (auto mylock : locks) {
      pthread_mutex_destroy(mylock.second);
      delete mylock.second;
    }
    pthread_mutex_destroy(&proglock);
  }

  void execute(int nThreads=0);
  static void execute( bool (*_thread_routine) (long int), int nThreads=0 );
  static void execute( bool (*_thread_routine) (), int nThreads=0 );

  void lock(int idx=0);
  void unlock(int idx=0);

};




#ifdef OPENCL_FOUND

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.h>

cl_device_id CL_device();
cl_context CL_context();
cl_command_queue CL_queue();

class CLmem {
    cl_mem clR;
    cl_mem & cl;
public:
    inline explicit CLmem(cl_mem _cl=0) : clR(_cl) , cl(clR) {}
    inline CLmem(const CLmem & other) : clR(0) , cl(other.cl) {}
    inline ~CLmem() { free(); }
    inline cl_mem & operator()() const { return cl; }
    inline CLmem & operator()(cl_mem _cl) { free(); clR=_cl; cl=clR;  return *this; }
    inline CLmem & operator()(const CLmem & other) { free(); cl=other.cl; return *this; }
    inline operator bool() const { return cl; }
    inline void free() { if (clR) clReleaseMemObject(clR); clR=0; }
};

class CLkernel {
  cl_kernel kern;
public:
  inline CLkernel(cl_program program=0, const std::string & _name = std::string())
    : kern(0) { this->operator()(program, _name); }
  inline ~CLkernel() { if (kern) clReleaseKernel(kern) ; }
  CLkernel & operator()(cl_program program=0, const std::string & name = std::string());
  inline operator bool() const { return kern; }
  std::string name() const;
  cl_int exec(size_t size=1) const;
  cl_int exec(const Shape & sh) const;
  cl_int exec(const Shape3 & sh) const ;
  template <class T> cl_int setArg (cl_uint arg_idx, const T & val) const {
    cl_int clerr = clSetKernelArg (kern, arg_idx, sizeof(T), &val);
    if (clerr != CL_SUCCESS)
      throw_error("setArg", "Could not set argument " + toString(arg_idx) +
                           " for OpenCL kernel \"" + name() + "\": " + toString(clerr));
    return clerr;
  }
};


// this version protects with pthread mutex lock
cl_program & initProgram(const std::string & src, cl_program & program, const std::string & modname);

cl_program initProgram(const std::string & src, const std::string & modname);


template <class T>
cl_mem var2cl(cl_mem_flags flag = CL_MEM_WRITE_ONLY) {
  cl_int clerr;
  cl_mem clStorage = clCreateBuffer ( CL_context(), flag, sizeof(T), 0, &clerr);
  if (clerr != CL_SUCCESS)
    throw_error("OpenCL", "Could not create OpenCL buffer: " + toString(clerr) );
  return clStorage;
}


template <class T>
T cl2var(const cl_mem & buff) {
  T var;
  cl_int clerr = clEnqueueReadBuffer(CL_queue(), buff, CL_TRUE, 0, sizeof(T), &var, 0, 0, 0 );
  if (clerr != CL_SUCCESS)
    throw_error("OpenCL", "Could not read OpenCL buffer: " + toString(clerr) );
  return var;
}


template <typename T>
cl_mem clAllocArray(size_t arrSize, cl_mem_flags flag=CL_MEM_READ_WRITE) {
  cl_int err;
  const size_t iStorageSize = sizeof(T) * arrSize ;
  cl_mem clStorage = clCreateBuffer ( CL_context(), flag, iStorageSize, 0, &err);
  if (err != CL_SUCCESS)
    throw_error("OpenCL", "Could not create OpenCL buffer: " + toString(err) );
  return clStorage;
}


template <typename T, int N>
cl_mem blitz2cl(const blitz::Array<T,N> & storage, cl_mem clStorage, cl_mem_flags flag=CL_MEM_READ_WRITE) {
  blitz::Array<T,N> _storage(safe(storage));
  cl_int err = clEnqueueWriteBuffer(  CL_queue(), clStorage, CL_TRUE, 0, sizeof(T) * _storage.size(),
                                      _storage.data(), 0, 0, 0);
  if (err != CL_SUCCESS) {
  //  prdn(_storage.size());
  //  std::cout << _storage.data() << "\n";
  //  prdn(clStorage);
    throw_error("OpenCL", "Could not write OpenCL buffer: " + toString(err) );
  }
  return clStorage;
}


template <typename T, int N>
cl_mem blitz2cl(const blitz::Array<T,N> & storage, cl_mem_flags flag=CL_MEM_READ_WRITE) {
  cl_mem clStorage = clAllocArray<T>(storage.size(), flag);
  return blitz2cl<T,N>(storage, clStorage, flag);
}


template <typename T, int N>
blitz::Array<T,N> & cl2blitz(cl_mem clbuffer, blitz::Array<T,N> & storage) {

  blitz::Array<T,N> _storage(safe(storage));
  cl_int err = clEnqueueReadBuffer(CL_queue(), clbuffer, CL_TRUE, 0,
                                   sizeof(T) * _storage.size(),
                                   _storage.data(), 0, 0, 0 );
  if (err != CL_SUCCESS)
    throw_error("OpenCL", "Could not read OpenCL buffer: " + toString(err) );
  if ( storage.data() != _storage.data() )
    storage = _storage;
  return storage;

}

template <typename T>
cl_int fillClArray(cl_mem clStorage, size_t size, T val) {
  cl_int clerr = clEnqueueFillBuffer(CL_queue(), clStorage, &val, sizeof(T), 0, sizeof(T) * size, 0,0,0);
  if (clerr != CL_SUCCESS)
    throw_error("Fill CL buffer", "Failed to fill the buffer: " + toString(clerr));
  return clerr;
}


/*
cl_int fillClImage(cl_mem image, float val);

cl_mem clAllocFImage(const Shape sh, cl_mem_flags flag=CL_MEM_READ_WRITE);

cl_mem blitz2clfi(const Map & storage, cl_mem clStorage, cl_mem_flags flag=CL_MEM_READ_WRITE);

cl_mem blitz2clfi(const Map & storage, cl_mem_flags flag=CL_MEM_READ_WRITE);

cl_mem clAllocFImage(const Shape3 sh, cl_mem_flags flag=CL_MEM_READ_WRITE);

cl_mem blitz2clfi(const Volume & storage, cl_mem clStorage, cl_mem_flags flag=CL_MEM_READ_WRITE);

cl_mem blitz2clfi(const Volume & storage, cl_mem_flags flag=CL_MEM_READ_WRITE);

void clfi2blitz(cl_mem clStorage, Map & storage);

void clfi2blitz(cl_mem clStorage, Volume & storage);
*/


#endif // OPENCL_FOUND


/// @}



#endif // PARALLEL_WORLD_H
