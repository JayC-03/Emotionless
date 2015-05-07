set(SRCS
main.cpp
memmap.cpp
ee/ee.cpp
ee/interpreter/interpreter.cpp
ee/interpreter/interpreter_branch.cpp
ee/interpreter/interpreter_cop0.cpp
ee/interpreter/interpreter_cop1.cpp
ee/interpreter/interpreter_integer.cpp
ee/interpreter/interpreter_loadstore.cpp
ee/interpreter/interpreter_mmi.cpp
ee/interpreter/interpreter_tables.cpp
ee/jit/jit-x64/jit_x64.cpp
iop/iop.cpp
iop/interpreter/interpreter.cpp
iop/interpreter/interpreter_branch.cpp
iop/interpreter/interpreter_cop0.cpp
iop/interpreter/interpreter_integer.cpp
iop/interpreter/interpreter_loadstore.cpp
iop/interpreter/interpreter_tables.cpp
hw/dmac.cpp
hw/gif.cpp
hw/gs/gs.cpp
hle/elf.cpp
hle/bios_hle.cpp
)

set(LIBS
common
asmjit.a
)

add_emotionless_library(core "${SRCS}" "${LIBS}")
