

# set the include path found by configure
INCLUDES = $(MAGICK_INCFLAGS) $(all_includes) -I$(top_srcdir)/src

METASOURCES = AUTO

noinst_HEADERS = common.h kernel.h poptmx.h abs.h dei.h edei.h experiment.h experiment.win32.cpp flatfield.h ipc.h

pkglib_LTLIBRARIES = libpoptmx.la libcommon.la libkernel.la libdei.la libedei.la libexperiment.la libipc.la

libpoptmx_la_SOURCES = poptmx.cpp
libpoptmx_la_LIBADD = -lstdc++

libcommon_la_SOURCES = common.cpp
libcommon_la_LIBADD = \
	$(MAGICK_LDFLAGS)

libkernel_la_SOURCES = kernel.cpp
libkernel_la_LIBADD = $(top_builddir)/src/common/libcommon.la

libipc_la_SOURCES = lipc.cpp
libipc_la_LIBADD = $(top_builddir)/src/common/libcommon.la

libdei_la_SOURCES = ldei.cpp
libdei_la_LIBADD  = $(top_builddir)/src/common/libcommon.la

libedei_la_SOURCES = ledei.cpp
libedei_la_LIBADD  = $(top_builddir)/src/common/libcommon.la

libexperiment_la_SOURCES = experiment.cpp
libexperiment_la_LIBADD  = \
	$(top_builddir)/src/common/libcommon.la
