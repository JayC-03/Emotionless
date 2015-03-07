set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SRCS
main.cpp
debugger/debugger.cpp
debugger/register.cpp
)

set(HEADERS
main.h
debugger/debugger.h
debugger/register.h
)

set(UIS
main.ui
debugger/debugger.ui
debugger/register.ui
)

set(LIBS
common
core
)

add_definitions(-fpermissive)

qt5_wrap_ui(UI_HDRS ${UIS})

add_executable(emotionless-qt ${SRCS} ${HEADERS} ${UI_HDRS})
target_link_libraries(emotionless-qt ${LIBS} ${QT_LIBS} pthread)
