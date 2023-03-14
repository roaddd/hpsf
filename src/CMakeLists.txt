include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS *.cpp)

add_library(hpsf OBJECT ${sources})