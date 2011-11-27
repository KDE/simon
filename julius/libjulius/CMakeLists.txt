#project(libjulius)
include(SimonDirs)

include_directories(../libjulius/include ../libsent/include)
#add_definitions(-lang-c-c++-comments -std=c99 )

set (libjulius_SRCS ./src/m_usage.c 
  ./src/realtime-1stpass.c 
  ./src/plugin.c 
  ./src/pass1.c 
  ./src/beam.c 
  ./src/recogmain.c 
  ./src/instance.c 
  ./src/dfa_decode.c 
  ./src/word_align.c 
  ./src/search_bestfirst_v1.c 
  ./src/search_bestfirst_v2.c 
  ./src/graphout.c 
  ./src/gmm.c 
  ./src/version.c 
  ./src/hmm_check.c 
  ./src/m_jconf.c 
  ./src/m_adin.c 
  ./src/jfunc.c 
  ./src/ngram_decode.c 
  ./src/multi-gram.c 
  ./src/m_fusion.c 
  ./src/outprob_style.c 
  ./src/factoring_sub.c 
  ./src/useropt.c 
  ./src/wchmm.c 
  ./src/callback.c 
  ./src/gramlist.c 
  ./src/wchmm_check.c 
  ./src/wav2mfcc.c 
  ./src/default.c 
  ./src/m_info.c 
  ./src/m_options.c 
  ./src/m_chkparam.c 
  ./src/backtrellis.c 
  ./src/spsegment.c 
  ./src/adin-cut.c 
  ./src/search_bestfirst_main.c 
  ./src/confnet.c)

add_library(julius SHARED ${libjulius_SRCS})

if(WIN32)
  target_link_libraries(julius sent)
ENDIF(WIN32)
if(UNIX)
  target_link_libraries(julius sent dl)
ENDIF(UNIX)

FILE(GLOB libjulius_LIB_HDRS ./include/julius/*.h) 

set_target_properties( julius PROPERTIES
 VERSION "4.2"
 SOVERSION 4
)

install( FILES ${libjulius_LIB_HDRS}
  DESTINATION include/julius
  COMPONENT juliusdevel
)

install( TARGETS julius DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT julius)

