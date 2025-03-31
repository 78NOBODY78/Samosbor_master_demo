cmake_minimum_required(VERSION 3.16)

project(SamosborGameDnD VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 🔹 Явно указываем путь к .ui файлам
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    gamemodewindow.cpp gamemodewindow.h gamemodewindow.ui
    playermodewindow.h playermodewindow.cpp playermodewindow.ui
    createcharacterwindow.h createcharacterwindow.cpp createcharacterwindow.ui
)

# 🔹 Убедимся, что путь к `SamosborGameDnD_autogen/include/` добавлен в инклуды
include_directories(${CMAKE_BINARY_DIR}/SamosborGameDnD_autogen/include)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SamosborGameDnD
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        character_traits.h character_traits.cpp
        character_classes.h character_classes.cpp
        character_skills.h character_skills.cpp
        selectcharacterwindow.h selectcharacterwindow.cpp
        selectcharacterwindow.ui
        mastercontrolwindow.h mastercontrolwindow.cpp
        mastercontrolwindow.ui
        samosborcontroller.h samosborcontroller.cpp
        terminalprinter.h terminalprinter.cpp
        eventgenerator.h eventgenerator.cpp


    )
else()
    if(ANDROID)
        add_library(SamosborGameDnD SHARED ${PROJECT_SOURCES})
    else()
        add_executable(SamosborGameDnD ${PROJECT_SOURCES})
    endif()
endif()

target_link_libraries(SamosborGameDnD PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core)

set_target_properties(SamosborGameDnD PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SamosborGameDnD
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SamosborGameDnD)
endif()
