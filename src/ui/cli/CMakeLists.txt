set(SRCS
main.cpp
)

set(LIBS
core
)

add_executable(emotionless-nogui ${SRCS})
target_link_libraries(emotionless-nogui ${LIBS})