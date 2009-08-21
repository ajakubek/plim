bin_PROGRAMS = plim
plim_SOURCES = plim.cpp

# set the include path found by configure
INCLUDES = -I$(top_srcdir)/. $(all_includes)

# the library search path.
plim_LDFLAGS = $(all_libraries) 
plim_LDADD = $(top_builddir)/libplimgui/libplimgui.a
AM_CFLAGS = -I../
AM_CXXFLAGS = -I../
METASOURCES = AUTO
