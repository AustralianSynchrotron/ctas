#
# Written by Patrick Guio <patrick.guio@fys.uio.no>
#

ACLOCAL_AMFLAGS  = -I m4

SUBDIRS = blitz

EXTRA_DIST = LEGAL LICENSE README.binutils README-VS.NET.txt Blitz-VS.NET.zip \
	ChangeLog.1 

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = blitz.pc blitz-uninstalled.pc

DISTCLEANFILES = blitz.pc blitz.pc.in blitz-uninstalled.pc

clean-local:
	-rm -rf *.ii *.ti cxx_repository Template.dir ii_files ti_files

