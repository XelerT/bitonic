set(SRCS
        bitonic.cpp
)

add_library(bitonic ${SRCS})
target_include_directories(bitonic PRIVATE ${CMAKE_BINARY_DIR})