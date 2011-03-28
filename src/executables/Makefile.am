

# set the include path found by configure
INCLUDES = $(MAGICK_INCFLAGS) $(all_includes) -I$(top_srcdir)/src

METASOURCES = AUTO

pkglibexecdir = @PKGLIBEXECDIR@

pkglibexec_PROGRAMS = ct sino sino-abs sino-dei sino-edei sino-ipc ff rc2fd dei edei ipc ts ct-abs ct-dei ct-edei ct-ipc f2i ct-line

noinst_PROGRAMS = simulate-ipc


ct_SOURCES = ct.cpp
ct_LDADD = \
	$(top_builddir)/src/common/libpoptmx.la \
	$(top_builddir)/src/common/libkernel.la \
	$(top_builddir)/src/common/libcommon.la

ct_line_SOURCES = ct-line.cpp
ct_line_LDADD = \
	$(top_builddir)/src/common/libpoptmx.la \
	$(top_builddir)/src/common/libkernel.la \
	$(top_builddir)/src/common/libcommon.la



sino_SOURCES = sino.cpp
sino_LDADD = \
	$(top_builddir)/src/common/libpoptmx.la \
	$(top_builddir)/src/common/libcommon.la


sino_abs_SOURCES = sino-abs.cpp
sino_abs_LDADD = \
	$(top_builddir)/src/common/libpoptmx.la \
	$(top_builddir)/src/common/libexperiment.la \
	$(top_builddir)/src/common/libcommon.la


sino_dei_SOURCES = sino-dei.cpp
sino_dei_LDADD = \
	$(top_builddir)/src/common/libpoptmx.la \
	$(top_builddir)/src/common/libexperiment.la \
	$(top_builddir)/src/common/libdei.la \
	$(top_builddir)/src/common/libcommon.la

sino_edei_SOURCES = sino-edei.cpp
sino_edei_LDADD = \
	$(top_builddir)/src/common/libpoptmx.la \
	$(top_builddir)/src/common/libexperiment.la \
	$(top_builddir)/src/common/libedei.la \
	$(top_builddir)/src/common/libcommon.la

sino_ipc_SOURCES = sino-ipc.cpp
sino_ipc_LDADD = \
	$(top_builddir)/src/common/libpoptmx.la \
	$(top_builddir)/src/common/libexperiment.la \
	$(top_builddir)/src/common/libipc.la \
	$(top_builddir)/src/common/libcommon.la	

ff_SOURCES = ff.cpp
ff_LDADD = \
	$(top_builddir)/src/common/libpoptmx.la \
	$(top_builddir)/src/common/libcommon.la


rc2fd_SOURCES = rc2fd.cpp
rc2fd_LDADD = \
	$(top_builddir)/src/common/libpoptmx.la \
	$(top_builddir)/src/common/libedei.la \
	$(top_builddir)/src/common/libcommon.la

ipc_SOURCES = ipc.cpp
ipc_LDADD = \
	$(top_builddir)/src/common/libpoptmx.la \
	$(top_builddir)/src/common/libipc.la \
	$(top_builddir)/src/common/libcommon.la

dei_SOURCES = dei.cpp
dei_LDADD = \
	$(top_builddir)/src/common/libpoptmx.la \
	$(top_builddir)/src/common/libdei.la \
	$(top_builddir)/src/common/libcommon.la

edei_SOURCES = edei.cpp
edei_LDADD = \
	$(top_builddir)/src/common/libpoptmx.la \
	$(top_builddir)/src/common/libedei.la \
	$(top_builddir)/src/common/libcommon.la


ts_SOURCES = ts.cpp
ts_LDADD = \
	$(top_builddir)/src/common/libpoptmx.la \
	$(top_builddir)/src/common/libcommon.la \
	$(top_builddir)/src/common/libkernel.la


ct_abs_SOURCES = ct-abs.cpp
ct_abs_LDADD = \
	$(top_builddir)/src/common/libpoptmx.la \
	$(top_builddir)/src/common/libexperiment.la \
	$(top_builddir)/src/common/libcommon.la \
	$(top_builddir)/src/common/libkernel.la


ct_dei_SOURCES = ct-dei.cpp
ct_dei_LDADD = \
	$(top_builddir)/src/common/libpoptmx.la \
	$(top_builddir)/src/common/libdei.la \
	$(top_builddir)/src/common/libexperiment.la \
	$(top_builddir)/src/common/libcommon.la \
	$(top_builddir)/src/common/libkernel.la

ct_edei_SOURCES = ct-edei.cpp
ct_edei_LDADD = \
	$(top_builddir)/src/common/libpoptmx.la \
	$(top_builddir)/src/common/libedei.la \
	$(top_builddir)/src/common/libexperiment.la \
	$(top_builddir)/src/common/libcommon.la \
	$(top_builddir)/src/common/libkernel.la

ct_ipc_SOURCES = ct-ipc.cpp
ct_ipc_LDADD = \
	$(top_builddir)/src/common/libpoptmx.la \
	$(top_builddir)/src/common/libipc.la \
	$(top_builddir)/src/common/libexperiment.la \
	$(top_builddir)/src/common/libcommon.la \
	$(top_builddir)/src/common/libkernel.la



f2i_SOURCES = f2i.cpp
f2i_LDADD = \
	$(top_builddir)/src/common/libpoptmx.la \
	$(top_builddir)/src/common/libcommon.la

simulate_ipc_SOURCES = simulate-ipc.cpp
simulate_ipc_LDADD = \
	$(top_builddir)/src/common/libpoptmx.la \
	$(top_builddir)/src/common/libcommon.la \
	$(top_builddir)/src/common/libipc.la



# manual page and html help generation
%.man: $(pkglibexec_PROGRAMS)
	str="$*" ; \
	fstr="$@" ; \
	manprefix="$(PACKAGE)-" ; \
	manpostfix=$${fstr#$${str}}; \
	execfile=$${str#$${manprefix}} ; \
	execpostfix="-very-long-key-phrase-to-output-man-pages" ; \
	manfile="$${manprefix}$${execfile}$${manpostfix}" ; \
	eval ./"$${execfile}" "$${execpostfix}" > "$${manfile}" ; \
	rm -f "$${lexecfile}" ; \
	htmlfile="../../doc/site/userdoc/$${manprefix}$${execfile}.1.html" ; \
	man2html -r $${manfile} | sed -e "s:../man1/::g" > "$${htmlfile}"


dist_man1_MANS = ctas-ff.man  ctas-ct.man  ctas-ct-abs.man  ctas-ct-dei.man ctas-ct-edei.man ctas-ct-ipc.man ctas-dei.man ctas-edei.man ctas-ipc.man ctas-f2i.man  ctas-rc2fd.man  ctas-sino.man  ctas-sino-abs.man  ctas-sino-dei.man ctas-sino-edei.man ctas-sino-ipc.man ctas-ts.man ctas-ct-line.man

clean-local:
	-rm $(dist_man1_MANS)
