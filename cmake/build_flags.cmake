
# target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef                    \
#                                                                  -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations               \
#                                                                  -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain          \
#                                                                  -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion                   \
#                                                                  -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2                  \
#                                                                  -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers                 \
#                                                                  -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo           \
#                                                                  -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits      \
#                                                                  -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE                                 \
#                                                                  -lasan -fsanitize=address,leak>")
# target_link_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:-lasan -fsanitize=address,leak>")


set(DEBUG_BUILD     -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef 
                    -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations 
                    -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain 
                    -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion 
                    -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 
                    -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers 
                    -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo 
                    -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits 
                    -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE 
                    -lasan -fsanitize=address,leak
                    )   
set(DEBUG_LINK -lasan -fsanitize=address,leak)

target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_BUILD}>")
target_link_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_LINK}>")