#project(mkfa)
FIND_PACKAGE(FLEX)
FIND_PACKAGE(BISON)

#add_definitions(-lang-c-c++-comments -std=c99 )

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

FLEX_TARGET(Lex ${CMAKE_CURRENT_SOURCE_DIR}/gram.l
${CMAKE_CURRENT_BINARY_DIR}/gram.lex.c)
BISON_TARGET(Gram ${CMAKE_CURRENT_SOURCE_DIR}/gram.y
${CMAKE_CURRENT_BINARY_DIR}/gram.tab.c)

#ADD_FLEX_BISON_DEPENDENCY(Lex Gram)

set (mkfa_SRCS
  dfa.c
  main.c
  nfa.c
  triplet.c
  voca.c
  ${BISON_Gram_OUTPUTS}
)

add_executable(mkfa ${mkfa_SRCS})
target_link_libraries(mkfa ${FLEX_LIBRARIES})
install (TARGETS mkfa DESTINATION bin COMPONENT julius)

