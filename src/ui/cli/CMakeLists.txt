set(SRCS
main.cpp
)

set(LIBS
core
)

add_executable(pcps2-nogui ${SRCS})
target_link_libraries(pcps2-nogui ${LIBS})