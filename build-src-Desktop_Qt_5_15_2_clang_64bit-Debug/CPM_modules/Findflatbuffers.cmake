include(/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/cmake/CPM_0.32.1.cmake)
CPMAddPackage(NAME;flatbuffers;URL;https://github.com/google/flatbuffers/archive/v1.12.0.zip;VERSION;1.12.0;OPTIONS;FLATBUFFERS_CODE_COVERAGE OFF;FLATBUFFERS_BUILD_TESTS OFF;FLATBUFFERS_BUILD_FLATLIB OFF;FLATBUFFERS_BUILD_FLATC OFF;FLATBUFFERS_BUILD_FLATHASH OFF;FLATBUFFERS_LIBCXX_WITH_CLANG ON)
set(flatbuffers_FOUND TRUE)