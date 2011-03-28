#include <fcntl.h>
#include <io.h>         /* _mktemp */
#include <Windows.h>
#define _S_IREAD 256
#define _S_IWRITE 128

static inline int
mkstemp(char *tmpl) {
  int ret=-1;
  mktemp(tmpl);
  return open(tmpl,O_RDWR|O_BINARY|O_CREAT|O_EXCL|_O_SHORT_LIVED, _S_IREAD|_S_IWRITE);
}

/* getpagesize for windows */
static inline long
getpagesize (void) {
  static long g_pagesize = 0;
  if (! g_pagesize) {
	SYSTEM_INFO system_info;
	GetSystemInfo (&system_info);
	g_pagesize = system_info.dwPageSize;
  }
  return g_pagesize;
}

static inline long
getregionsize (void) {
  static long g_regionsize = 0;
  if (! g_regionsize) {
	SYSTEM_INFO system_info;
	GetSystemInfo (&system_info);
	g_regionsize = system_info.dwAllocationGranularity;
  }
  return g_regionsize;
}

#define PROT_WRITE     0
#define PROT_READ      0
#define MAP_SHARED     0
#define MMAP_FAILURE   0
#define MUNMAP_FAILURE 0

/* mmap for windows */
static void *
mmap (void *ptr, long size, long prot, long type, long handle, long arg) {
  static long g_pagesize;
  static long g_regionsize;
  /* First time initialization */
  if (! g_pagesize)
	g_pagesize = getpagesize ();
  if (! g_regionsize)
	g_regionsize = getregionsize ();
  /* Allocate this */
  ptr = VirtualAlloc (ptr, size,
					  MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN,
					  PAGE_READWRITE);
  if (! ptr) {
	ptr = MMAP_FAILURE;
	goto mmap_exit;
  }
mmap_exit:
  return ptr;
}

/* munmap for windows */
static long
munmap (void *ptr, long size) {
  static long g_pagesize;
  static long g_regionsize;
  int rc = MUNMAP_FAILURE;
  /* First time initialization */
  if (! g_pagesize)
	g_pagesize = getpagesize ();
  if (! g_regionsize)
	g_regionsize = getregionsize ();
  /* Free this */
  if (! VirtualFree (ptr, 0, MEM_RELEASE))
	goto munmap_exit;
  rc = 0;
munmap_exit:
  return rc;
}
