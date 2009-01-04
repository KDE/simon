#project(dfa_minimize)
include_directories(../../libsent/include ../../libjulius/include)
#add_definitions(-lang-c-c++-comments -std=c99 )
set (dfa_minimize_SRCS ./dfa_minimize.c)

add_executable(dfa_minimize ${dfa_minimize_SRCS})
target_link_libraries(dfa_minimize sent)
install (TARGETS dfa_minimize DESTINATION bin COMPONENT julius)
