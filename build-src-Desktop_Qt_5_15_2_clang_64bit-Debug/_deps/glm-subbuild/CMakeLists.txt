# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.19.2)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(glm-populate NONE)

include(ExternalProject)
ExternalProject_Add(glm-populate
                     "UPDATE_DISCONNECTED" "False" "URL" "https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.7z"
                    SOURCE_DIR          "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/glm-src"
                    BINARY_DIR          "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/glm-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
)


