liblouisincludedir = $(includedir)/liblouis

liblouisinclude_HEADERS = \
	liblouis.h \
	louis.h

lib_LTLIBRARIES = liblouis.la

AM_CPPFLAGS = -DTABLESDIR=\""$(datadir)"/liblouis/tables\"

AM_ETAGSFLAGS = --langmap=c:+.ci

liblouis_la_LDFLAGS =	\
	-version-info $(LIBLOUIS_CURRENT):$(LIBLOUIS_REVISION):$(LIBLOUIS_AGE) -no-undefined

liblouis_la_SOURCES = \
	lou_backTranslateString.c \
	compileTranslationTable.c \
	louis.h	\
	liblouis.h \
	lou_translateString.c \
	transcommon.ci \
	wrappers.c
