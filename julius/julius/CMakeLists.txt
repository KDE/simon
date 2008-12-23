#project(julius)

include_directories(../libsent/include ../libjulius/include)
set (juliusexe_SRCS ./main.c
./output_stdout.c
./charconv.c
./module.c
./output_file.c
#./visual.c
./charconv_iconv.c
./recogloop.c
./charconv_libjcode.c
./output_module.c
./record.c)

add_executable(juliusexe ${juliusexe_SRCS})
target_link_libraries(juliusexe julius sent)
install (TARGETS juliusexe DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS} COMPONENT julius)
