# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.19.2)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(noodles_lib-populate NONE)

include(ExternalProject)
ExternalProject_Add(noodles_lib-populate
                     "UPDATE_DISCONNECTED" "False" "GIT_REPOSITORY" "https://github.com/InsightCenterNoodles/NoodlesPlusPlus.git" "GIT_TAG" "87c584524fd641c80622e58905ecb5c46ae7f520"
                    SOURCE_DIR          "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles_lib-src"
                    BINARY_DIR          "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles_lib-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
)


