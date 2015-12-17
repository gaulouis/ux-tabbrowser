UX_LIBS=$(top_builddir)/lib/libux.la
UX_INCLUDES=-I$(top_srcdir)/lib

lib_LTLIBRARIES=libgladeux.la
libgladeux_la_SOURCES=glade-ux.c
libgladeux_la_CFLAGS = $(GLADE_GUI_CFLAGS) $(UX_INCLUDES)
libgladeux_la_LIBADD = $(GLADE_GUI_LIBS) $(UX_LIBS)
