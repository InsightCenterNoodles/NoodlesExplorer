# Install script for directory: /Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/flatbuffers-build/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-build/src/cmake_install.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xnoodles_Developmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/noodles-0.1" TYPE DIRECTORY FILES "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-build/PackageProjectInclude/")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xnoodles_Runtimex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/noodles-0.1" TYPE SHARED_LIBRARY FILES "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-build/libnoodles.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/noodles-0.1/libnoodles.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/noodles-0.1/libnoodles.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Users/lordzippy/Qt/5.15.2/clang_64/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/noodles-0.1/libnoodles.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/noodles-0.1/libnoodles.dylib")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xnoodles_Developmentx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xnoodles_Developmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/noodles-0.1/noodlesTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/noodles-0.1/noodlesTargets.cmake"
         "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-build/CMakeFiles/Export/lib/cmake/noodles-0.1/noodlesTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/noodles-0.1/noodlesTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/noodles-0.1/noodlesTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/noodles-0.1" TYPE FILE FILES "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-build/CMakeFiles/Export/lib/cmake/noodles-0.1/noodlesTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/noodles-0.1" TYPE FILE FILES "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-build/CMakeFiles/Export/lib/cmake/noodles-0.1/noodlesTargets-debug.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xnoodles_Developmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/noodles-0.1" TYPE FILE FILES
    "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-build/noodlesConfigVersion.cmake"
    "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-build/noodlesConfig.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xnoodles_Developmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/noodles-0.1" TYPE DIRECTORY FILES "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-src/include/")
endif()

