# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.19.2)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(flatbuffers-populate NONE)

include(ExternalProject)
ExternalProject_Add(flatbuffers-populate
                     "UPDATE_DISCONNECTED" "False" "URL" "https://github.com/google/flatbuffers/archive/v1.12.0.zip"
                    SOURCE_DIR          "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/flatbuffers-src"
                    BINARY_DIR          "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/flatbuffers-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
)


