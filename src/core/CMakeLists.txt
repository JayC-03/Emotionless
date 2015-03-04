set(SRCS
main.cpp
memmap.cpp
ee/ee.cpp
ee/interpreter/interpreter.cpp
ee/interpreter/interpreter_tables.cpp
ee/interpreter/interpreter_integer.cpp
)

set(LIBS
common
)

add_pcps2_library(core "${SRCS}" "${LIBS}")