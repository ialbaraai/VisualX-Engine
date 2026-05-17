cmake_minimum_required(VERSION 3.20)
project(VisualXEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)
set(LIBS_DIR ${PROJECT_ROOT}/libs)

set(IMGUI_DIR ${LIBS_DIR}/ImGui)

set(IMGUI_SRC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

file(GLOB LUA_SOURCES ${LIBS_DIR}/lua/*.c)
list(FILTER LUA_SOURCES EXCLUDE REGEX ".*/(lua|luac)\\.c$")

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)

file(GLOB_RECURSE APP_SRC CONFIGURE_DEPENDS ${PROJECT_ROOT}/engine/src/application/*.cpp)
file(GLOB_RECURSE SYS_SRC CONFIGURE_DEPENDS ${PROJECT_ROOT}/engine/src/system/*.cpp)
file(GLOB_RECURSE UTIL_SRC CONFIGURE_DEPENDS ${PROJECT_ROOT}/engine/src/utilities/*.cpp)

add_executable(VisualXEngine
    main.cpp
    ${APP_SRC}
    ${SYS_SRC}
    ${UTIL_SRC}
    ${IMGUI_SRC}
    ${LUA_SOURCES}
)

target_include_directories(VisualXEngine PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${PROJECT_ROOT}/assets
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${LIBS_DIR}/lua
    ${LIBS_DIR}
)

target_link_libraries(VisualXEngine PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
)

add_custom_command(TARGET VisualXEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_ROOT}/assets
    $<TARGET_FILE_DIR:VisualXEngine>/assets
)
add_custom_command(
    TARGET VisualXEngine
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_ROOT}/games.txt
            $<TARGET_FILE_DIR:VisualXEngine>/games.txt
)   