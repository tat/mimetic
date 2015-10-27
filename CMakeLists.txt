cmake_minimum_required (VERSION 2.8)
project (mimetic)

# extract current version from configure.in
FILE(STRINGS configure.in aminit REGEX "AM_INIT_AUTOMAKE")
STRING(REGEX REPLACE "AM_INIT_AUTOMAKE\\([ ]*mimetic[ ]*,(.*)\\)" "\\1" VERSION "${aminit}")
add_definitions(-DVERSION="${VERSION}")

if(WIN32)
    set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
    foreach(CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /FD /EHsc /W3 /TP /wd4267 /wd4244")
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} /FD /EHsc /W3 /TP /wd4267 /wd4244")
    add_definitions(-DWIN32 -D_LIB -D_MBCS)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")
    set(CMAKE_C_FLAGS "$CMAKE_C_FLAGS -fPIC")
    add_definitions(-DHAVE_STDINT_H -DHAVE_INTTYPES_H -DHAVE_UNISTD_H -DHAVE_SYS_TYPES_H -DHAVE_SYS_STAT_H)
endif()

if (IOS)
    if (NOT CMAKE_GENERATOR STREQUAL "Xcode")
        message(FATAL_ERROR "only xcode generator is supported for iOS")
    endif ()

    set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "7.0")
    if (IOS STREQUAL "sim")
        set(CMAKE_OSX_SYSROOT iphonesimulator)
    else ()
        set(CMAKE_OSX_SYSROOT iphoneos)
    endif ()
    set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD_INCLUDING_64_BIT)")
    set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR})
    SET(CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos;-iphonesimulator" )
    add_definitions(-DIOS)
endif (IOS)

add_subdirectory(mimetic)
if(NOT IOS)
    add_subdirectory(examples)
endif()

