set(UNITESTS "unitests")

message("Building unitests.")

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(UNITESTS_LIST
        unitests
)

foreach(TEST ${UNITESTS_LIST})
        add_subdirectory(${TEST})
endforeach()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(${UNITESTS} ${SRCS})

set_target_properties(${UNITESTS} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${UNITESTS} ${SRC_LIST})

target_link_libraries(${UNITESTS} gtest gtest_main gmock)
