cmake_minimum_required(VERSION 3.16)

project(NoodlesExplorer VERSION 0.2 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# CMake Modules ================================================================

include(cmake/CPM.cmake)

CPMAddPackage(
  NAME GroupSourcesByFolder.cmake
  GITHUB_REPOSITORY TheLartians/GroupSourcesByFolder.cmake
  VERSION 1.0
)

CPMAddPackage(
    NAME glm
    URL  https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.7z
    VERSION 0.9.9.8
)

CPMAddPackage(
    NAME noodles
    GITHUB_REPOSITORY InsightCenterNoodles/NoodlesPlusPlus
    GIT_TAG 7964d970aa2d5f4136c7ecaf2d7112791288b651
)

find_package(Qt6 COMPONENTS Gui Widgets WebSockets Core 3DCore 3DExtras 3DRender Quick Charts)

# Options ======================================================================

set(SANITIZER "none" CACHE STRING "Build with a given sanitizer")
set_property(CACHE SANITIZER PROPERTY STRINGS none address)

SET(sanitizer_compile_flag "")

if (${SANITIZER} STREQUAL "address")
    SET(sanitizer_compile_flag "-fsanitize=address")
endif()

add_executable(NoodlesExplorer "")
target_compile_features(NoodlesExplorer PUBLIC cxx_std_20)
target_compile_options(NoodlesExplorer PUBLIC ${sanitizer_compile_flag})
target_link_options(NoodlesExplorer PUBLIC ${sanitizer_compile_flag})

target_link_libraries(NoodlesExplorer PRIVATE glm)

target_link_libraries(NoodlesExplorer PRIVATE noodles)

target_link_libraries(NoodlesExplorer PUBLIC
    Qt::Gui Qt::Widgets Qt::Core
    Qt::3DCore Qt::3DExtras Qt::3DRender
    Qt::Quick Qt::Charts
)

target_sources(NoodlesExplorer
PRIVATE
    argumenttablemodel.cpp
    argumenttablemodel.h
    main.cpp
    structtablemodel.cpp
    structtablemodel.h
    componentlistmodel.cpp
    componentlistmodel.h
    qml.qrc
    state.cpp
    state.h
    delegates/delegates.h
    delegates/delegates.cpp
    instance_material/qinstancedmetalroughmaterial.cpp
    instance_material/qinstancedmetalroughmaterial.h
    attachedmethodlistmodel.cpp
    attachedmethodlistmodel.h
    tabledata.cpp
    tabledata.h
    tableviewer.cpp
    tableviewer.h
    tableviewer.ui
    chartviewer.cpp
    chartviewer.h
    chartviewer.ui
    variant_tools.h
    adddatadialog.cpp
    adddatadialog.h
    adddatadialog.ui
    delegates/exmethod.cpp
    delegates/exmethod.h
    delegates/exsignal.cpp
    delegates/exsignal.h
    delegates/exbuffer.cpp
    delegates/exbuffer.h
    delegates/extexture.cpp
    delegates/extexture.h
    delegates/exmaterial.cpp
    delegates/exmaterial.h
    delegates/exmesh.cpp
    delegates/exmesh.h
    delegates/exobject.cpp
    delegates/exobject.h
    delegates/extable.cpp
    delegates/extable.h
    delegates/exdoc.cpp
    delegates/exdoc.h
    delegates/exlight.cpp
    delegates/exlight.h
    newselectiondialog.cpp
    newselectiondialog.h
    newselectiondialog.ui
    chartplotview.h
    chartplotview.cpp
)
