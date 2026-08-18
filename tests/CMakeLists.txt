find_package(GTest CONFIG REQUIRED)

add_executable(line_tests
    src/framebuffer_smoke_tests.cpp
    src/line_contract_tests.cpp
    src/gradient_line_tests.cpp
)

target_link_libraries(line_tests PRIVATE common_drawing GTest::gtest_main)
target_include_directories(line_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(line_tests PRIVATE cxx_std_20)

include(GoogleTest)
gtest_discover_tests(line_tests)
