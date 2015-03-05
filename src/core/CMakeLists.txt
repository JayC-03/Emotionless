set(SRCS
main.cpp
memmap.cpp
ee/ee.cpp
ee/interpreter/interpreter.cpp
ee/interpreter/interpreter_branch.cpp
ee/interpreter/interpreter_cop0.cpp
ee/interpreter/interpreter_integer.cpp
ee/interpreter/interpreter_loadstore.cpp
ee/interpreter/interpreter_tables.cpp
hw/dmac.cpp
hle/elf.cpp
)

set(LIBS
common
)

add_emotionless_library(core "${SRCS}" "${LIBS}")