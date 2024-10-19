cmake_minimum_required(VERSION 3.11)
project(mimetic VERSION 0.9.8)

set(PKGCONF_NAME ${CMAKE_PROJECT_NAME})
set(PKGCONF_VERSION ${CMAKE_PROJECT_VERSION})
include(GNUInstallDirs) #set CMAKE_INSTALL_LIBDIR, CMAKE_INSTALL_INCLUDEDIR

include(CheckIncludeFile)
include(CMakePackageConfigHelpers)




option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_STATIC_LIBS "Build static libraries" ON)


set(MIMETIC_SOURCES
    mimetic/body.cxx
    mimetic/contentdisposition.cxx
    mimetic/contenttransferencoding.cxx
    mimetic/fieldparam.cxx
    mimetic/message.cxx
    mimetic/mimeversion.cxx
    mimetic/utils.cxx
    mimetic/contentdescription.cxx
    mimetic/contentid.cxx
    mimetic/contenttype.cxx
    mimetic/header.cxx
    mimetic/mimeentity.cxx
    mimetic/strutils.cxx
    mimetic/version.cxx
    mimetic/rfc822/address.cxx
    mimetic/rfc822/addresslist.cxx
    mimetic/rfc822/datetime.cxx
    mimetic/rfc822/field.cxx
    mimetic/rfc822/fieldvalue.cxx
    mimetic/rfc822/group.cxx
    mimetic/rfc822/header.cxx
    mimetic/rfc822/mailbox.cxx
    mimetic/rfc822/mailboxlist.cxx
    mimetic/rfc822/message.cxx
    mimetic/rfc822/messageid.cxx
    mimetic/os/fileop.cxx
    mimetic/os/utils.cxx
    mimetic/os/file_iterator.cxx
    mimetic/codec/base64.cxx
    mimetic/codec/qp.cxx
    mimetic/os/stdfile.cxx
)



#feature detection
check_include_file("sys/mman.h" HAVE_MMAP)

if (HAVE_MMAP)
    list(APPEND MIMETIC_SOURCES mimetic/os/mmfile.cxx)
endif()

check_include_file("stdint.h" HAVE_STDINT_H)
check_include_file("inttypes.h" HAVE_INTTYPES_H)
check_include_file("unistd.h" HAVE_UNISTD_H)
check_include_file("sys/time.h" HAVE_SYS_TIME_H)
check_include_file("sys/stat.h" HAVE_SYS_STAT_H)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_config.h.in 
               ${CMAKE_CURRENT_SOURCE_DIR}/mimetic/config.h @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_config.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/mimetic/config_win32.h @ONLY)



# Define shared library
if(BUILD_SHARED_LIBS)
    add_library(mimetic SHARED ${MIMETIC_SOURCES})
    add_library(mimetic::mimetic ALIAS mimetic)
    target_compile_definitions(mimetic PUBLIC HAVE_MIMETIC_CONFIG)
    target_include_directories(mimetic
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>  # So mimetic can build and find <mimetic/header.h> files
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>                    # For installed use
    )
    install(TARGETS mimetic
            EXPORT mimeticTargets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    set_target_properties(mimetic PROPERTIES ENABLE_EXPORTS ON)
endif()

# Define static library
if(BUILD_STATIC_LIBS)
    add_library(mimetic_static STATIC ${MIMETIC_SOURCES})
    add_library(mimetic::mimetic_static ALIAS mimetic_static)
    target_compile_definitions(mimetic_static PUBLIC HAVE_MIMETIC_CONFIG)
    target_include_directories(mimetic_static
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>  # So mimetic can build and find <mimetic/header.h> files
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>                    # For installed use
    ) 
    install(TARGETS mimetic_static
            EXPORT mimeticTargets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )  
endif()




#install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/mimetic/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mimetic
        FILES_MATCHING PATTERN "*.h"
)     


install(EXPORT mimeticTargets
        FILE mimeticTargets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mimetic
        NAMESPACE mimetic::
)

# Generate the configuration file
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/mimeticConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/mimeticConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mimetic
)

# Create the configuration version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/mimeticConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Install the configuration file
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/mimeticConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/mimeticConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mimetic
)


# Configure the .pc file
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/mimetic.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/mimetic.pc"
    @ONLY
)

# Install the .pc file
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/mimetic.pc"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

# Create an uninstall target
add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

# Generate the uninstall script
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    @ONLY
)

