# detect curses
FIND_PACKAGE(Curses REQUIRED)
INCLUDE_DIRECTORIES(${CURSES_INCLUDE_DIR})

# detect sigc++2
FIND_PACKAGE(LibSigCXX2 REQUIRED)
INCLUDE_DIRECTORIES(${LIBSIGCXX2_INCLUDE_DIRS})

# build GUI plugin
ADD_LIBRARY(plimgui SHARED
			application.cpp
			base.cpp
			box.cpp
			cursesstring.cpp
			descriptor.cpp
			hashnodes.cpp
			input.cpp
			keybindings.cpp
			keyboard.cpp
			lexer.cpp
			pallete.cpp
			roster.cpp
			rosteritems.cpp
			statusbar.cpp
			stdafx.cpp
			strings.cpp
			textline.cpp
			textviewer.cpp
			treenode.cpp
			treenodes.cpp
			stringlist.cpp
			windows.cpp)

# link with curses and libsigc++2
TARGET_LINK_LIBRARIES(plimgui
			${CURSES_LIBRARIES}
			${LIBSIGCXX2_LIBRARIES})

if (WIN32)
INSTALL(TARGETS plimgui
		DESTINATION bin)
else()
# install GUI plugin
INSTALL(TARGETS plimgui
		LIBRARY DESTINATION lib)
endif ()