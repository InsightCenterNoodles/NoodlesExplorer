
if(NOT "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-subbuild/noodles-populate-prefix/src/noodles-populate-stamp/noodles-populate-gitinfo.txt" IS_NEWER_THAN "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-subbuild/noodles-populate-prefix/src/noodles-populate-stamp/noodles-populate-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: '/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-subbuild/noodles-populate-prefix/src/noodles-populate-stamp/noodles-populate-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"  clone --no-checkout "https://github.com/InsightCenterNoodles/NoodlesPlusPlus.git" "noodles-src"
    WORKING_DIRECTORY "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/InsightCenterNoodles/NoodlesPlusPlus.git'")
endif()

execute_process(
  COMMAND "/usr/bin/git"  checkout 5c19fd7179855675b814da36fa5f160cf760600e --
  WORKING_DIRECTORY "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: '5c19fd7179855675b814da36fa5f160cf760600e'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git"  submodule update --recursive --init 
    WORKING_DIRECTORY "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-src"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-subbuild/noodles-populate-prefix/src/noodles-populate-stamp/noodles-populate-gitinfo.txt"
    "/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-subbuild/noodles-populate-prefix/src/noodles-populate-stamp/noodles-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/Users/lordzippy/Development/Noodles/NoodlesExplorer/build-src-Desktop_Qt_5_15_2_clang_64bit-Debug/_deps/noodles-subbuild/noodles-populate-prefix/src/noodles-populate-stamp/noodles-populate-gitclone-lastrun.txt'")
endif()

