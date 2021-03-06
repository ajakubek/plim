# this will be removed once we implement the plugin system
INCLUDE_DIRECTORIES(..)

# ditto, curses should be included only by the GUI plugin
FIND_PACKAGE(Curses REQUIRED)

FIND_PACKAGE(LibSigCXX2 REQUIRED)
INCLUDE_DIRECTORIES(${LIBSIGCXX2_INCLUDE_DIRS})

# update rpath so that we always search for plugins in
# "${CMAKE_INSTALL_PREFIX}/lib"
SET(CMAKE_SKIP_BUILD_RPATH FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# build main executable
ADD_EXECUTABLE(plim
			   cPlim.cpp
			   cPlimConfig.cpp
			   cReactor.cpp
			   cReactorPlugin.cpp
			   cReactorRoom.cpp
			   cReactorUser.cpp
			   cReactorSession.cpp
			   plim.cpp
			   plugins/irc/cIrc.cpp
			   plugins/irc/cIrcProto.cpp
			   plugins/irc/cIrcRoom.cpp
			   plugins/irc/cIrcSession.cpp
			   plugins/irc/cIrcUser.cpp)

# this should be removed (GUI should be loaded dynamically)
LINK_DIRECTORIES(libplimgui)

TARGET_LINK_LIBRARIES(plim
	plimgui						# remove this dependency later
	${LIBSIGCXX2_LIBRARIES})

# install main plim executable
INSTALL(TARGETS plim
	RUNTIME DESTINATION bin)
