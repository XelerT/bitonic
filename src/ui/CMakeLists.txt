set(SRCS
        ui.cpp
        cmd_parser.cpp
)

add_library(ui ${SRCS})
target_include_directories(ui PRIVATE ${CMAKE_BINARY_DIR})