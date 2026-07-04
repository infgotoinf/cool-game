cmake_minimum_required(VERSION 3.24...3.30)
project(game)

include(FetchContent)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# Dependencies
set(RAYLIB_VERSION 6.0)

set(PLATFORM Web)

FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS
)

FetchContent_MakeAvailable(raylib)


# Our Project
add_executable(${PROJECT_NAME} WIN32
    src/main/main.cpp
)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/main/gamejam_template.cpp
        src/main/python_vc_cpp.cpp
        src/main/game_3d_example.cpp
        src/main/paralax_bg_example.cpp
        src/main/music_example.cpp

    PUBLIC
        FILE_SET headers
        TYPE HEADERS
        BASE_DIRS
            vendored/raygui/src
            src/main/include
        FILES
            vendored/raygui/src/raygui.h

            src/main/include/window_codes.hpp
            src/main/include/gamejam_template.hpp
            src/main/include/python_vc_cpp.hpp
            src/main/include/game_3d_example.hpp
            src/main/include/paralax_bg_example.hpp
            src/main/include/music_example.hpp
)

add_subdirectory(src)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>)

if ("${PLATFORM}" STREQUAL "Web")
    add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/../resources
    )
    #DEPENDS ${PROJECT_NAME}
else()
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    )
    #DEPENDS ${PROJECT_NAME}
endif()

#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)

# Web Configurations
if ("${PLATFORM}" STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    target_link_options(${PROJECT_NAME} PUBLIC -sUSE_GLFW=3 PUBLIC --preload-file resources)
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
