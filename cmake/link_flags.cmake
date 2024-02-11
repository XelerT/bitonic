
set(LINK -lOpenCL -lboost_program_options)

target_link_options(${PROJECT_NAME} PUBLIC "${LINK}")
