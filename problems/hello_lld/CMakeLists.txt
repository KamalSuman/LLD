add_library(hello_lld src/greeter.cpp)
target_include_directories(hello_lld PUBLIC include)
target_link_libraries(hello_lld PUBLIC lld_project_options)

add_executable(hello_lld_app app/main.cpp)
target_link_libraries(hello_lld_app PRIVATE hello_lld)

if(BUILD_TESTING)
    add_executable(hello_lld_tests tests/greeter_test.cpp)
    target_link_libraries(hello_lld_tests PRIVATE hello_lld)
    add_test(NAME hello_lld_tests COMMAND hello_lld_tests)
endif()
