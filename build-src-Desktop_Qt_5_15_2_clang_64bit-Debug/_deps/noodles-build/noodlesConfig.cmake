include(CMakeFindDependencyMacro)

string(REGEX MATCHALL "[^;]+" SEPARATE_DEPENDENCIES "glm 0.9.9.8")

foreach(dependency ${SEPARATE_DEPENDENCIES})
  string(REPLACE " " ";" args "${dependency}")
  find_dependency(${args})
endforeach()

include("${CMAKE_CURRENT_LIST_DIR}/noodlesTargets.cmake")
