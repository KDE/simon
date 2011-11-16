#project(julius)

#add_definitions(-lang-c-c++-comments -std=c99)
if(WIN32)
    find_package(KDEWin REQUIRED)
    include_directories(${KDEWIN_INCLUDES})
    if(MSVC)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    endif(MSVC)
endif(WIN32)

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
if(WIN32)
    target_link_libraries(juliusexe julius sent ${KDEWIN_LIBRARIES})
else(WIN32)
    target_link_libraries(juliusexe julius sent)
endif(WIN32)
install (TARGETS juliusexe DESTINATION bin COMPONENT julius)

