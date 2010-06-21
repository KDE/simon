#project(libsent)

find_package(ZLIB REQUIRED)

if(UNIX)
find_package(Pthreads REQUIRED)
endif(UNIX)

include(SimonDirs)

add_definitions(-DHAVE_ZLIB)
#add_definitions(-DHAVE_ZLIB -lang-c-c++-comments -std=c99)

if(UNIX)
add_definitions(-DHAVE_SLEEP -Dlinux)
include_directories(${PTHREADS_INCLUDE_DIR})
endif(UNIX)
if(WIN32)
add_definitions(-D__WIN32 -D__NO_ISOCEXT)
endif(WIN32)

if (PORTAUDIO_FOUND)
include_directories(${PORTAUDIO_INCLUDE_DIRS})
endif (PORTAUDIO_FOUND)

include_directories(include ${ZLIB_INCLUDE_DIR})
set (libsent_SRCS
./src/voca/voca_load_wordlist.c
./src/voca/init_voca.c
./src/voca/voca_lookup.c
./src/voca/voca_malloc.c
./src/voca/voca_load_htkdict.c
./src/voca/voca_util.c
./src/ngram/ngram_malloc.c
./src/ngram/ngram_lookup.c
./src/ngram/init_ngram.c
./src/ngram/ngram_read_bin.c
./src/ngram/ngram_read_arpa.c
./src/ngram/ngram_write_bin.c
./src/ngram/ngram_compact_context.c
./src/ngram/ngram_access.c
./src/ngram/ngram_util.c
./src/anlz/rdparam.c
./src/anlz/param_malloc.c
./src/anlz/strip.c
./src/anlz/paramtypes.c
./src/anlz/paramselect.c
./src/anlz/strip_mfcc.c
./src/anlz/wrsamp.c
./src/anlz/wrwav.c
./src/dfa/rddfa.c
./src/dfa/init_dfa.c
./src/dfa/mkterminfo.c
./src/dfa/mkcpair.c
./src/dfa/cpair.c
./src/dfa/dfa_malloc.c
./src/dfa/dfa_lookup.c
./src/dfa/dfa_util.c
./src/wav2mfcc/ss.c
./src/wav2mfcc/para.c
./src/wav2mfcc/wav2mfcc-pipe.c
./src/wav2mfcc/mfcc-core.c
./src/wav2mfcc/wav2mfcc-buffer.c
./src/hmminfo/rdhmmdef_options.c
./src/hmminfo/rdhmmlist.c
./src/hmminfo/put_htkdata_info.c
./src/hmminfo/rdhmmdef_data.c
./src/hmminfo/rdhmmdef_mpdf.c
./src/hmminfo/cdhmm.c
./src/hmminfo/rdhmmdef_var.c
./src/hmminfo/write_binhmm.c
./src/hmminfo/chkhmmlist.c
./src/hmminfo/cdset.c
./src/hmminfo/rdhmmdef_streamweight.c
./src/hmminfo/read_binhmm.c
./src/hmminfo/rdhmmdef_state.c
./src/hmminfo/hmm_lookup.c
./src/hmminfo/rdhmmdef.c
./src/hmminfo/rdhmmdef_tiedmix.c
./src/hmminfo/rdhmmdef_trans.c
./src/hmminfo/write_binhmmlist.c
./src/hmminfo/rdhmmdef_dens.c
./src/hmminfo/init_phmm.c
./src/hmminfo/rdhmmdef_regtree.c
./src/hmminfo/check_hmmtype.c
./src/hmminfo/check_hmm_restriction.c
./src/hmminfo/read_binhmmlist.c
./src/hmminfo/guess_cdHMM.c
./src/net/server-client.c
./src/net/rdwt.c
./src/adin/adin_file.c
#./src/adin/adin_mic_linux_alsa.c
#./src/adin/adin_mic_linux.c
#./src/adin/pa/pablio.c
#./src/adin/pa/pa_convert.c
#./src/adin/pa/pa_lib.c
#./src/adin/pa/pa_unix.c
#./src/adin/pa/pa_unix_oss.c
#./src/adin/pa/pa_trace.c
#./src/adin/pa/ringbuffer.c

./src/adin/ds48to16.c
./src/adin/zc-e.c
./src/adin/adin_tcpip.c
./src/adin/zmean.c
#./src/adin/adin_sndfile.c
./src/phmm/addlog.c
./src/phmm/gms_gprune.c
./src/phmm/outprob.c
./src/phmm/gprune_beam.c
./src/phmm/outprob_init.c
./src/phmm/gprune_safe.c
./src/phmm/mkwhmm.c
./src/phmm/gprune_common.c
./src/phmm/vsegment.c
./src/phmm/gms.c
./src/phmm/calc_tied_mix.c
./src/phmm/calc_mix.c
./src/phmm/gprune_heu.c
./src/phmm/gprune_none.c
./src/util/mymalloc.c
./src/util/gzfile.c
./src/util/qsort.c
./src/util/mybmalloc.c
./src/util/mystrtok.c
./src/util/strcasecmp.c
./src/util/aptree.c
./src/util/ptree.c
./src/util/jlog.c
./src/util/confout.c
./src/util/readfile.c
./src/util/endian.c
)

set (libsent_SRCS_PA
./src/adin/adin_portaudio.c
)

FILE(GLOB libsent_LIB_HDRS ./include/sent/*.h) 

if (PORTAUDIO_FOUND)
add_library(sent SHARED ${libsent_SRCS} ${libsent_SRCS_PA})
else (PORTAUDIO_FOUND)
add_library(sent SHARED ${libsent_SRCS})
endif (PORTAUDIO_FOUND)

if(UNIX)
if (PORTAUDIO_FOUND)
target_link_libraries(sent ${PORTAUDIO_LIBRARY} ${ZLIB_LIBRARY}
	${PTHREADS_LIBRARY} m)
else (PORTAUDIO_FOUND)
target_link_libraries(sent ${ZLIB_LIBRARY} ${PTHREADS_LIBRARY} m)
endif (PORTAUDIO_FOUND)
endif(UNIX)

if(WIN32)
if (PORTAUDIO_FOUND)
target_link_libraries(sent ${ZLIB_LIBRARY} ${PORTAUDIO_LIBRARY}
	wsock32 winmm ws2_32)
else (PORTAUDIO_FOUND)
target_link_libraries(sent ${ZLIB_LIBRARY} wsock32 winmm ws2_32)
endif (PORTAUDIO_FOUND)
endif(WIN32)

set_target_properties( sent
  PROPERTIES VERSION 4 SOVERSION 1
)

install( FILES ${libsent_LIB_HDRS}
  DESTINATION include/sent
  COMPONENT juliusdevel
)
 
install( TARGETS sent DESTINATION  ${SIMON_LIB_INSTALL_DIR} COMPONENT julius	)
