cmake_minimum_required(VERSION 3.20)

project(Paint++)

set(CMAKE_CXX_STANDARD 20)

add_executable(app
    src/main.cpp
    src/paint_core.cpp
)

set_target_properties(app PROPERTIES OUTPUT_NAME "paint++") # ++ in name

target_include_directories(app PRIVATE
    src
    C:/sfml/include
)

target_compile_definitions(app PRIVATE SFML_STATIC)

target_link_directories(app PRIVATE
    C:/sfml/lib
)

target_link_libraries(app PRIVATE
    sfml-graphics-s
    sfml-window-s
    sfml-system-s

    harfbuzz
    freetype

    flac
    vorbisenc
    vorbisfile
    vorbis
    ogg

    opengl32
    winmm
    gdi32
    shell32
    user32
    bcrypt
    shlwapi
    comdlg32

    -static-libgcc
    -static-libstdc++
)

# font
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/font.ttf"
    $<TARGET_FILE_DIR:app>
)