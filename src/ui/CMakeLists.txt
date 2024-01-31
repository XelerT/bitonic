set(SRCS
        ui.cpp
)

add_library(ui ${SRCS})
target_include_directories(ui PRIVATE ${CMAKE_BINARY_DIR})