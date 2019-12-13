file(GLOB SOURCES *.cpp)

find_package(GTest REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(factory_test ${SOURCES})
target_link_libraries(factory_test GTest::GTest GTest::Main)

add_test(tests factory_test)
