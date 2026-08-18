add_executable(Graphics
    src/main.cpp
    include/sfml_renderer.h
    src/sfml_renderer.cpp
)

find_package(SFML COMPONENTS System Window Graphics CONFIG REQUIRED)
target_link_libraries(Graphics PRIVATE
    common_drawing
    SFML::System
    SFML::Window
    SFML::Graphics
)

target_include_directories(Graphics PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(Graphics PRIVATE cxx_std_20)
