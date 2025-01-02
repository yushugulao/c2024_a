cmake_minimum_required(VERSION 3.16)

project(gobangForAssignment VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Gui Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui Multimedia)
find_package(Qt6 REQUIRED COMPONENTS Core Concurrent)


set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.ui
    mainwindow.h
    boardwidget.h
    boardwidget.cpp
    aiworker.h
    aiworker.cpp
    resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)


qt_add_executable(gobangForAssignment
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(gobangForAssignment SHARED
            ${PROJECT_SOURCES}
        )
        set_property(TARGET gobangForAssignment APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
                     ${CMAKE_CURRENT_SOURCE_DIR}/android)
    else()
        add_executable(gobangForAssignment
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_sources(gobangForAssignment PRIVATE resources.qrc)

target_link_libraries(gobangForAssignment PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::Concurrent
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(gobangForAssignment)
endif()

set_target_properties(gobangForAssignment PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS gobangForAssignment
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
