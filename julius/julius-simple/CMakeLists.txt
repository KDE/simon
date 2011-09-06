#project(julius)

#add_definitions(-lang-c-c++-comments -std=c99)
include_directories(../libsent/include ../libjulius/include)
set (juliussimple_SRCS ./julius-simple.c)

add_executable(juliussimple ${juliussimple_SRCS})
target_link_libraries(juliussimple julius sent)
install (TARGETS juliussimple DESTINATION bin COMPONENT julius)

