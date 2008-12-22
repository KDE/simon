#project(dfa_minimize)

set (dfaminimize_SRCS ./dfa_minimize.c)

add_executable(dfaminimize ${dfaminimize_SRCS})
target_link_libraries(dfaminimize)
install (TARGETS dfaminimize DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})
