#project(mkfa)
FIND_PACKAGE(FLEX)
FIND_PACKAGE(BISON)

add_definitions(-lang-c-c++-comments -std=c99 )

BISON_TARGET(Gram gram.y ${CMAKE_CURRENT_SOURCE_DIR}/gram.tab.c)
FLEX_TARGET(Lex gram.l  ${CMAKE_CURRENT_SOURCE_DIR}/gram.lex.c)

#ADD_FLEX_BISON_DEPENDENCY(Lex Gram)

set (mkfa_SRCS
${CMAKE_CURRENT_SOURCE_DIR}/dfa.c
${CMAKE_CURRENT_SOURCE_DIR}/main.c
${CMAKE_CURRENT_SOURCE_DIR}/nfa.c
${CMAKE_CURRENT_SOURCE_DIR}/triplet.c
${CMAKE_CURRENT_SOURCE_DIR}/voca.c)

add_executable(mkfa ${mkfa_SRCS} ${BISON_Gram_OUTPUTS})
target_link_libraries(mkfa ${FLEX_LIBRARIES})
