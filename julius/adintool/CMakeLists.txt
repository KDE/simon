project(juliusexe)

set (julius_SRCS ./adintool.c)

add_executable(adintool ${julius_SRCS})
target_link_libraries(adintool julius sent z portaudio)
