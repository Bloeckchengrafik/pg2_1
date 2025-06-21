cmake_minimum_required(VERSION 3.16)
project(pg2_1 VERSION 0.1 LANGUAGES CXX)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include(FetchContent)

FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets WebEngineWidgets)
qt_standard_project_setup()

FILE(GLOB_RECURSE sources src/*.cpp)
FILE(GLOB_RECURSE headers src/*.h)

qt_add_executable(
        pg2_1
        ${sources}
        ${headers}
        vendor/QDarkStyleSheet/qdarkstyle/dark/darkstyle.qrc
)
target_link_libraries(pg2_1 PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(pg2_1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::WebEngineWidgets)

qt_add_resources(pg2_1 "icons"
        PREFIX "/"
        FILES
            icons/airplane-in-flight.svg
            icons/bed.svg
            icons/car-profile.svg
            icons/train.svg
)

qt_add_resources(pg2_1 "resources"
        PREFIX "/"
        FILES
        iatacodes.json
)

# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.test)
endif()

set_target_properties(pg2_1 PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS pg2_1
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(pg2_1)
endif()
