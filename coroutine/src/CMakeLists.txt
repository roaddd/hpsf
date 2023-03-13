include_directories(${PROJECT_SOURCE_DIR}/coroutine/include)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS *.cpp)

add_library(io_context OBJECT ${sources})
