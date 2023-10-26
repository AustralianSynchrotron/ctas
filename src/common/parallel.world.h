#ifndef PARALLEL_WORLD_H
#define PARALLEL_WORLD_H

#include "common.world.h"
#include "matrix.world.h"

#include<functional>

/// \brief Number of threads for the process.
///
/// @param _threads Requested number of threads (0 for auto).
///
/// @return Number of threads for the architecture where the process is running
/// if automatic number of threads was requested and just _threads if set in stone.
///
long
nof_threads(long _threads=0);



class Relocker {
private:
  pthread_mutex_t * mutex = 0;
  pthread_cond_t * condition = 0;
  bool _state = false;
  bool & state;
public:
  Relocker();
  Relocker(const Relocker & other);
  ~Relocker();
  void lock();
  void unlock();
};




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

  static const std::string modname;
  std::deque<pthread_mutex_t> locks; // to be used by the sub-classes users via lock/unlock methods

  virtual bool beforeThread() {
    return true;
  }

  inline static bool
  beforeThread(void * args) {
    if (!args)
      throw_error(modname, "Incorrectly used class. Report to developers.");
    return ((InThread*)args)->beforeThread();
  }

  virtual bool inThread(long int) = 0;

  inline static bool
  inThread(void * args, long int idx) {
    if (!args)
      throw_error(modname, "Incorrectly used class. Report to developers.");
    return ((InThread*)args)->inThread(idx);
  }

public:

  ProgressBar bar;

  InThread(bool verbose=false, const std::string procName = std::string(), int steps=0)
    : bar(verbose, procName, steps)
  {
    if (steps)
      bar.start();
    needMutexes(1);
  }

  ~InThread() {
    for (auto mylock : locks)
      pthread_mutex_destroy(&mylock);
  }

  virtual void execute(int nThreads=0);
  static void execute( std::function<bool()> _thread_routine, int nThreads=0 );
  static void execute( std::function<bool(long int)> _thread_routine, int nThreads=0 );
  static void execute( int from, int to, std::function<void(long int)> _thread_routine, int nThreads=0 );
  static void execute( int to, std::function<void(long int)> _thread_routine, int nThreads = 0) {
    execute(0, to, _thread_routine, nThreads);
  }


  void needMutexes(uint nof_mut=1);
  void lock(int idx=0);
  void unlock(int idx=0);

};




#ifdef OPENCL_FOUND

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.h>

cl_device_id CL_device();
cl_context CL_context();
cl_command_queue CL_queue();
inline bool CL_isReady() {
  return CL_device() && CL_queue() && CL_context();
}


struct CLenv {
  cl_device_id dev;
  cl_context cont;
  cl_command_queue que;
};

extern std::vector<CLenv> clenvs;

CLenv & clenv(const cl_context cont);


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
    inline void write(void * data, ssize_t sz, cl_command_queue clque=CL_queue()) {
      cl_int err = clEnqueueWriteBuffer(clque, cl, CL_TRUE, 0, sz, data, 0, 0, 0);
      if (err != CL_SUCCESS)
        throw_error("CLmem", "Could not write OpenCL buffer: " + toString(err) );
    }
    inline void read(void * data, ssize_t sz, cl_command_queue clque=CL_queue()) {
      cl_int err = clEnqueueReadBuffer(clque, cl, CL_TRUE, 0, sz, data, 0, 0, 0 );
      if (err != CL_SUCCESS)
        throw_error("CLmem", "Could not read OpenCL buffer: " + toString(err) );
    }
};



class CLprogram {
    cl_program prog = 0;
public:
    inline CLprogram() {};
    inline explicit CLprogram(const std::string & source, cl_context context=CL_context()) {
      this->operator()(source, context);
    };
    inline ~CLprogram() {free();}
    CLprogram & operator()(const std::string & source, cl_context context=CL_context());
    inline const cl_program & operator()() const { return prog; }
    inline operator bool() const { return prog; }
    cl_context context() const;
    void free();
};


class CLkernel {

private:

  cl_kernel kern = 0;
  cl_int exec(size_t dims, size_t * sizes, cl_command_queue clque) const;
  bool fixedWGsize=true;
  static const std::string modname;

public:

  inline CLkernel() {}
  inline CLkernel(const CLprogram & program, const std::string & _name, bool _fixedWGsize=true){
    this->operator()(program, _name, _fixedWGsize);
  }
  inline ~CLkernel() { free(); }
  void free() {if (kern) clReleaseKernel(kern) ; kern=0;}
  CLkernel & operator()(const CLprogram & program, const std::string & name, bool _fixedWGsize=true);
  inline operator bool() const { return kern; }
  std::string name() const;
  cl_context context() const;
  cl_program program() const;
  cl_int exec(size_t size=1, cl_command_queue clque=CL_queue()) const { return exec(1, &size, clque);}

  template <int Dim> cl_int exec(const Shape<Dim> & sh, cl_command_queue clque=CL_queue()) const {
    size_t sizes[Dim];
    for (uint curD=0; curD<Dim; ++curD)
      sizes[curD] = size_t(sh(Dim-1-curD)) ;
    return exec(Dim, sizes, clque);
  }

  template <class T> cl_int setArg (cl_uint arg_idx, const T & val) const {
    cl_int clerr = clSetKernelArg (kern, arg_idx, sizeof(T), &val);
    if (clerr != CL_SUCCESS)
      throw_error(modname, "Could not set argument " + toString(arg_idx) +
                           " for OpenCL kernel \"" + name() + "\": " + toString(clerr));
    return clerr;
  }

};



template <typename T>
cl_mem clAllocArray(size_t arrSize, cl_mem_flags flag=CL_MEM_READ_WRITE, cl_context context=CL_context()) {
  cl_int err;
  const size_t iStorageSize = sizeof(T) * arrSize ;
  static pthread_mutex_t locker = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_lock(&locker);
  cl_mem clStorage = clCreateBuffer(context, flag, iStorageSize, 0, &err);
  pthread_mutex_unlock(&locker);
  if (err != CL_SUCCESS)
    throw_error("OpenCL", "Could not create OpenCL buffer: " + toString(err) );
  //T val;
  //err = clEnqueueFillBuffer(CL_queue(), clStorage, &val, sizeof(T), 0, iStorageSize, 0,0,0);
  //if (err != CL_SUCCESS)
  //  throw_error("OpenCL", "Could not allocate OpenCL buffer: " + toString(err) );
  return clStorage;
}

template <typename T>
cl_mem clAllocArray(size_t arrSize, cl_context context) {
  return clAllocArray<T>(arrSize, CL_MEM_READ_WRITE, context);
}


template <typename T, int N>
cl_mem _blitz2cl( const blitz::Array<T,N> & storage, cl_mem clStorage
                , cl_mem_flags flag=CL_MEM_READ_WRITE, cl_command_queue clque=CL_queue()) {
  blitz::Array<T,N> _storage(safe(storage));
  cl_int err = clEnqueueWriteBuffer( clque, clStorage, CL_TRUE, 0, sizeof(T) * _storage.size()
                                     , _storage.data(), 0, 0, 0);
  if (err != CL_SUCCESS)
    throw_error("OpenCL", "Could not write OpenCL buffer: " + toString(err) );
  return clStorage;
}

template <typename CT, typename T, int N>
cl_mem _blitz2cl( const blitz::Array<T,N> & storage, cl_mem clStorage
                , cl_mem_flags flag=CL_MEM_READ_WRITE, cl_command_queue clque=CL_queue()) {
  blitz::Array<CT,N> _storage(storage.shape());
  _storage = blitz::cast<CT>(storage);
  return _blitz2cl(_storage, clStorage, flag, clque);
}

template <typename CT, typename T, int N>
cl_mem blitz2cl( const blitz::Array<T,N> & storage, cl_mem clStorage
               , cl_mem_flags flag=CL_MEM_READ_WRITE, cl_command_queue clque=CL_queue()) {
  return std::is_same<CT, T>::value
      ? _blitz2cl<T,N>(storage, clStorage, CL_MEM_READ_WRITE, clque)
      : _blitz2cl<CT,T,N>(storage, clStorage, CL_MEM_READ_WRITE, clque);
}

template <typename CT, typename T, int N>
cl_mem blitz2cl( const blitz::Array<T,N> & storage, cl_mem clStorage, cl_command_queue clque) {
  return blitz2cl<CT,T,N>(storage, clStorage, CL_MEM_READ_WRITE, clque);
}

template <typename CT, typename T, int N>
cl_mem blitz2cl( const blitz::Array<T,N> & storage
               , cl_mem_flags flag=CL_MEM_READ_WRITE, cl_command_queue clque=CL_queue()) {
  cl_context context;
  cl_int err = clGetCommandQueueInfo(clque, CL_QUEUE_CONTEXT, sizeof(cl_context), &context, 0);
  if (err != CL_SUCCESS)
    throw_error("OpenCL", "Could not get context from queue: " + toString(err));
  cl_mem clStorage = clAllocArray<CT>(storage.size(), flag, context);
  return blitz2cl<CT,T,N>(storage, clStorage, flag, clque);
}

template <typename CT, typename T, int N>
cl_mem blitz2cl(const blitz::Array<T,N> & storage, cl_command_queue clque) {
  return blitz2cl<CT,T,N>(storage, CL_MEM_READ_WRITE, clque);
}


template <typename T, int N>
blitz::Array<T,N> & cl2blitz(cl_mem clbuffer, blitz::Array<T,N> & storage, cl_command_queue clque=CL_queue()) {
  blitz::Array<T,N> _storage(safe(storage));
  cl_int err = clEnqueueReadBuffer(clque, clbuffer, CL_TRUE, 0,
                                   sizeof(T) * _storage.size(),
                                   _storage.data(), 0, 0, 0 );
  if (err != CL_SUCCESS)
    throw_error("OpenCL", "Could not read OpenCL buffer: " + toString(err) );
  if ( storage.data() != _storage.data() )
    storage = _storage;
  return storage;
}


template <typename T>
cl_int fillClArray(cl_mem clStorage, size_t size, T val, cl_command_queue clque=CL_queue()) {
  cl_int clerr = clEnqueueFillBuffer(clque, clStorage, &val, sizeof(T), 0, sizeof(T) * size, 0,0,0);
  if (clerr != CL_SUCCESS)
    throw_error("Fill CL buffer", "Failed to fill the buffer: " + toString(clerr));
  return clerr;
}


//template <class T>
//cl_mem var2cl(cl_mem_flags flag = CL_MEM_WRITE_ONLY) {
//  cl_int clerr;
//  cl_mem clStorage = clCreateBuffer ( CL_context(), flag, sizeof(T), 0, &clerr);
//  if (clerr != CL_SUCCESS)
//    throw_error("OpenCL", "Could not create OpenCL buffer: " + toString(clerr) );
//  return clStorage;
//}
//
//
//template <class T>
//T cl2var(const cl_mem & buff) {
//  T var;
//  cl_int clerr = clEnqueueReadBuffer(CL_queue(), buff, CL_TRUE, 0, sizeof(T), &var, 0, 0, 0 );
//  if (clerr != CL_SUCCESS)
//    throw_error("OpenCL", "Could not read OpenCL buffer: " + toString(clerr) );
//  return var;
//}



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
