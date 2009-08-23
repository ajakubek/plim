CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(plim)

# register path with additional cmake modules
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# build subdirectories
ADD_SUBDIRECTORY(libplimgui)
ADD_SUBDIRECTORY(src) 
