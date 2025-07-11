cmake_minimum_required(VERSION 3.16)

project(album VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(album
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        icons.qrc
        wizard.h wizard.cpp wizard.ui
        prosetpage.h prosetpage.cpp prosetpage.ui
        confirmpage.h confirmpage.cpp confirmpage.ui
        protreedialog.h protreedialog.cpp protreedialog.ui
        protreewidget.h protreewidget.cpp
        protreeitem.h protreeitem.cpp
        consts.h
        protreethread.h protreethread.cpp
        removeprodialog.h removeprodialog.cpp removeprodialog.ui
        picshowdialog.h picshowdialog.cpp picshowdialog.ui
        imagebutton.h imagebutton.cpp
        slideshowdialog.h slideshowdialog.cpp slideshowdialog.ui
        toggleimagebutton.h toggleimagebutton.cpp
        animationwidget.h animationwidget.cpp
        previewlistwidget.h previewlistwidget.cpp
        previewlistitem.h previewlistitem.cpp
        mymediaplaylist.h mymediaplaylist.cpp
        style.qrc

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET album APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(album SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(album
            ${PROJECT_SOURCES}
        )
    endif()
endif()

if(WIN32)
    target_sources(album PRIVATE app_icon.rc)
endif()

target_link_libraries(album PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
)

target_include_directories(album PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.album)
endif()
set_target_properties(album PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS album
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(album)
endif()
