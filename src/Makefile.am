

# set the include path found by configure
INCLUDES = $(MAGICK_INCFLAGS) $(all_includes)

SUBDIRS =  blitz-long common executables
METASOURCES = AUTO

man_MANS = ctas.1
EXTRA_DIST = $(man_MANS)

bin_SCRIPTS = ctas

all:
	htmlfile="../doc/site/userdoc/ctas.1.html" ; \
	man2html -r $(man_MANS) | sed -e "s:../man1/::g" > $${htmlfile}