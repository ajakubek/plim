# detect curses
FIND_PACKAGE(Curses REQUIRED)
INCLUDE_DIRECTORIES(${CURSES_INCLUDE_DIR})

# detect sigc++2
FIND_PACKAGE(LibSigCXX2 REQUIRED)
INCLUDE_DIRECTORIES(${LIBSIGCXX2_INCLUDE_DIRS})

# build GUI plugin
ADD_LIBRARY(plimgui SHARED
			application.cpp
			box.cpp
			input.cpp
			keyboard.cpp
			statusbar.cpp
			strings.cpp
			textviewer.cpp
			treenodes.cpp
			base.cpp
			descriptor.cpp
			keybindings.cpp
			pallete.cpp
			stdafx.cpp
			textline.cpp
			treenode.cpp
			windows.cpp)

# link with curses and libsigc++2
TARGET_LINK_LIBRARIES(plimgui
			${CURSES_LIBRARIES}
			${LIBSIGCXX2_LIBRARIES})

# install GUI plugin
INSTALL(TARGETS plimgui
		LIBRARY DESTINATION lib)
