####
# Set minimum version of CMake.
cmake_minimum_required(VERSION 3.13)

set(LIBRARY_NAME "libkami")

project(${LIBRARY_NAME} LANGUAGES CXX)

project(${LIBRARY_NAME}
        VERSION ${VERSION_STRING}
        LANGUAGES CXX)

file(GLOB LIBRARY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")

create_library(
        NAME ${LIBRARY_NAME}
        NAMESPACE ${LIBRARY_NAME}
        SOURCES ${LIBRARY_SOURCES}
        PUBLIC_INCLUDE_PATHS "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>" "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated_headers>"
        PRIVATE_LINKED_TARGETS ${COVERAGE_TARGET}
        PUBLIC_LINKED_TARGETS fmt
        EXPORT_FILE_PATH "${CMAKE_CURRENT_BINARY_DIR}/generated_headers/kami/KAMI_EXPORT.h"
)

configure_file(
        "${CMAKE_SOURCE_DIR}/include/kami/config.h.in"
        "${CMAKE_CURRENT_BINARY_DIR}/generated_headers/kami/config.h"
)

set_target_properties(
        ${LIBRARY_NAME}
        PROPERTIES VERSION ${VERSION_STRING}
        SOVERSION ${VERSION_MAJOR}
        OUTPUT_NAME kami
)

# Introduce variables:
# * CMAKE_INSTALL_LIBDIR
# * CMAKE_INSTALL_BINDIR
# * CMAKE_INSTALL_INCLUDEDIR
include(GNUInstallDirs)

# Headers:
#   * include/foo/bar/bar.h -> <prefix>/include/NAMESPACE/LIBRARY_NAME/*.h
#   * include/foo/bar/bar.h -> <prefix>/include/foo/bar/bar.h
install(
        DIRECTORY "${CMAKE_SOURCE_DIR}/include/kami"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        FILES_MATCHING PATTERN "*"
)

# Export headers:
#   The export header will be stored in:
#            <prefix>/include/${NAMESPACE}/LIBRARY_NAME/LIBRARY_NAME_export.h
install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/generated_headers/kami/KAMI_EXPORT.h"
        "${CMAKE_CURRENT_BINARY_DIR}/generated_headers/kami/config.h"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/kami"
)

add_library(kami::libkami ALIAS libkami)