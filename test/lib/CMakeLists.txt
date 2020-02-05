file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_library(test_lib SHARED ${SOURCES})
