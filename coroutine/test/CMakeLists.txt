# for each "test/x.cpp", generate target "x"
file(GLOB_RECURSE all_tests *.cpp)

include_directories(${PROJECT_SOURCE_DIR}/coroutine/include)

foreach(v ${all_tests})
    string(REGEX MATCH "test/.*" relative_path ${v})
    # message(${relative_path})
    string(REGEX REPLACE "test/" "" target_name ${relative_path})
    # message(${target_name})
    string(REGEX REPLACE ".cpp" "" target_name ${target_name})
    # message(${target_name})
    add_executable(${target_name} ${v} ../src/co/io_context.cpp)
endforeach()

# add_executable(io_context_test
# io_context_test.cpp
# ../src/co/io_context.cpp
# )