bin_PROGRAMS = plim
plim_SOURCES = cPlim.cpp cPlimReactor.cpp cPlimRoom.cpp cPlimUser.cpp plim.cpp

# set the include path found by configure
INCLUDES = -I$(top_srcdir)/. $(all_includes)

# the library search path.
plim_LDFLAGS = $(all_libraries) 
plim_LDADD = $(top_builddir)/libplimgui/libplimgui.a -lsigc-2.0
AM_CFLAGS = -I../
AM_CXXFLAGS = -I../ `pkg-config --cflags --libs sigc++-2.0`
METASOURCES = AUTO
_SOURCES = cPlim.h
_SOURCES = cPlim.h cPlimReactor.h
_SOURCES = cPlim.h cPlimReactor.h cPlimUser.h
_SOURCES = cPlim.h cPlimReactor.h cPlimRoom.h cPlimUser.h
