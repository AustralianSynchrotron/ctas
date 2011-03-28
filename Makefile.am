# not a GNU package. You can remove this line, if
# have all needed files, that a GNU package needs
ACLOCAL_AMFLAGS  = -I m4

AUTOMAKE_OPTIONS = foreign 1.4

msvs_dist = ctas.sln msvs/ReadMe.txt $(shell find msvs -iname "*.vcproj") msvs/CTAS/LIB msvs/CTAS/INC msvs/CTAS/ctas.bat

EXTRA_DIST = doc Doxyfile src/Mainpage.dox src/dir.dox $(msvs_dist)

dist-hook:
	rm -rf `find $(distdir)/msvs/CTAS/LIB -iname "ctas-*"`


SUBDIRS = src

winbin: all
	cd msvs ; \
	bin_dir="CTAS/BIN" ; \
	dll_dir="CTAS/DLL" ; \
	doc_dir="CTAS/DOC" ; \
        ctas_bat="CTAS/ctas.bat" ; \
	if [ ! -e $${bin_dir} -o ! -d $${bin_dir} ] ; then \
		echo "WARNING! The directory \"$${bin_dir}\" with" \
			" windows executables does not exists." ; \
	fi ; \
	if [ ! -e $${dll_dir} -o ! -d $${dll_dir} ] ; then \
		echo "WARNING! The directory \"$${dll_dir}\" with" \
			" windows dll's does not exists." ; \
	fi ; \
	for file in $$(find ../src/executables -maxdepth 1 -executable -type f) ; do \
		if [ ! -e "$${bin_dir}/$$(basename $${file}).exe" ] ; then \
			echo "WARNING! Unix tool \"$${file}\" does not have" \
				" corresponding windows executable." ; \
		fi ; \
	done; \
	ln -s ../../doc $${doc_dir} ; \
	zipname="../$(PACKAGE)-$(VERSION).winbin.zip" ; \
	rm -f $${zipname} ; \
	zip -r $${zipname} $${bin_dir} $${dll_dir} $${ctas_bat} $${doc_dir}; \
	rm $${doc_dir}; \
	cd ..

small-example: all
	zipname="$(PACKAGE)-$(VERSION).small-examples.zip" ; \
	rm -f $${zipname} ; \
	zip -r $${zipname} example/small-size

full-example: all
	zipname="$(PACKAGE)-$(VERSION).full-examples.zip" ; \
	rm -f $${zipname} ; \
	zip -9 -r $${zipname} example/full-size
