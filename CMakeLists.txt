cmake_minimum_required(VERSION 3.14)

project(bitonic_sort)

# version
set(CMAKE_CXX_STANDARD 20)

message("CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}")
message("CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")

set(default_build_type "Debug")
set(GTESTS False CACHE BOOL "To build unitests")

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

# Required libs

find_package( Boost 1.40 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

# collect srcs 

add_subdirectory(src)
message(STATUS "Collected srcs: ${SRC_LIST}")

message("Building ${PROJECT_NAME} in ${CMAKE_BUILD_TYPE}.")
        
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# srcs

foreach(SRC ${SRC_LIST})
        target_include_directories(${SRC} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/${SRC})
        target_include_directories(${SRC} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
                
        install(TARGETS ${SRC} DESTINATION bin)
endforeach()
        
# create main.cpp

add_executable(${PROJECT_NAME} main.cpp)

#build flags
include(cmake/build_flags.cmake)
include(cmake/link_flags.cmake)

target_link_options(${PROJECT_NAME} PUBLIC "${LINK}")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
        
install(TARGETS ${PROJECT_NAME})


# tests

option(UNITESTS "Enable/disable tests" OFF)
if (UNITESTS)
        add_subdirectory(tests)
endif()
# 

install()
