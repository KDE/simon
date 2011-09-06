project(adintool)

include_directories(../libjulius/include ../libsent/include)

set (adin_SRCS ./adintool.c)

add_executable(adintool ${adin_SRCS})
target_link_libraries(adintool julius sent)

install (TARGETS adintool DESTINATION bin COMPONENT julius)

