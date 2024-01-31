set(SRCS
        opencl.cpp
)

add_library(opencl ${SRCS})
target_include_directories(opencl PRIVATE ${CMAKE_BINARY_DIR})