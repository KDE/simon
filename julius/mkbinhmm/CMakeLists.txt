#project(mkbinhmm)

#add_definitions(-lang-c-c++-comments -std=c99)

include_directories(../libsent/include ../libjulius/include)
set (mkbinhmm_SRCS 
  ./mkbinhmm.c)

set (mkbinhmmlist_SRCS 
  ./mkbinhmmlist.c)


add_executable(mkbinhmmexe ${mkbinhmm_SRCS})
target_link_libraries(mkbinhmmexe sent)

add_executable(mkbinhmmlistexe ${mkbinhmmlist_SRCS})
target_link_libraries(mkbinhmmlistexe sent)

install (TARGETS mkbinhmmexe mkbinhmmlistexe DESTINATION bin COMPONENT julius)

