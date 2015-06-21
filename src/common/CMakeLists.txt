set(SRCS
log.cpp
x64_cpu_detect.cpp
emitters/x64_emitter.cpp
)

add_emotionless_library(common "${SRCS}" "")
