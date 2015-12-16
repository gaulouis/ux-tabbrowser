## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(UX_TABBROWSER_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = ux_tabbrowser

ux_tabbrowser_SOURCES = \
	main.c


ux_tabbrowser_LDFLAGS =

ux_tabbrowser_LDADD = $(UX_TABBROWSER_LIBS)


if PLATFORM_WIN32
ux_tabbrowser_LDFLAGS += -Wl,--export-all-symbols
else
ux_tabbrowser_LDFLAGS += -Wl,--export-dynamic
endif

if NATIVE_WIN32
ux_tabbrowser_LDFLAGS += -mwindows
endif


