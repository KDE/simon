#project(julius)

#add_definitions(-lang-c-c++-comments -std=c99)
include_directories(../libsent/include ../libjulius/include)
set (juliusexe_SRCS ./main.c
  ./output_stdout.c
  ./charconv.c
  ./module.c
  ./output_file.c
  #./visual.c
  ./recogloop.c
  ./output_module.c
  ./record.c)


if(WIN32)
set (juliusexe_NonPortable_SRCS ./charconv_win32.c)
ELSE(WIN32)
set (juliusexe_NonPortable_SRCS ./charconv_iconv.c)
ENDIF(WIN32)


add_executable(juliusexe ${juliusexe_SRCS} ${juliusexe_NonPortable_SRCS})
target_link_libraries(juliusexe julius sent)
install (TARGETS juliusexe DESTINATION bin COMPONENT julius)

