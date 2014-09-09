set(simonsound_LIB_SRCS
  recwidget.cpp
  volumewidget.cpp
  devicevolumewidget.cpp
  postprocessing.cpp

  soundserver.cpp
  soundclient.cpp
  soundinputclient.cpp
  soundoutputclient.cpp
  wavrecorderclient.cpp
  nullrecorderclient.cpp
  wavplayerclient.cpp
  wavplayersubclient.cpp
  wavfilewidget.cpp
  soundprocessor.h
  simonsoundinput.cpp
  simonsoundoutput.cpp
  loudnessmetersoundprocessor.cpp
  vadsoundprocessor.cpp

  trainsamplevolumepage.cpp

  devicesettings.cpp
  singledevicesettings.cpp

  soundbuffer.cpp
  soundinputbuffer.cpp
  soundoutputbuffer.cpp
  soundbackend.cpp

  qsemaphore2.cpp
)

IF(WIN32)
  set(simonsound_LIB_SRCS ${simonsound_LIB_SRCS} directsound/directsoundbackend.cpp)
ENDIF()
IF(UNIX AND NOT WIN32 AND NOT APPLE)
  set(simonsound_LIB_SRCS ${simonsound_LIB_SRCS} alsa/alsabackend.cpp)
ENDIF()
IF(APPLE)
  set(CMAKE_SHARED_LINKER_FLAGS "-framework Foundation -framework CoreAudio -framework AudioToolbox")
  set(simonsound_LIB_SRCS ${simonsound_LIB_SRCS} coreaudio/coreaudiobackend.mm)
ENDIF()

if(LIBSAMPLERATE_FOUND)
  set(simonsound_LIB_SRCS ${simonsound_LIB_SRCS} resamplesoundprocessor.cpp)
endif(LIBSAMPLERATE_FOUND)

set(simonsound_LIB_HDRS
  recwidget.h
  devicesettings.h
  volumewidget.h
  devicevolumewidget.h
  soundclient.h
  postprocessing.h
  soundserver.h
  soundinputclient.h
  simonsound.h
  simonsoundinput.h
  simonsamples.h
  simonsoundoutput.h
  soundoutputclient.h
  wavplayerclient.h
  wavfilewidget.h
  soundbackendclient.h

  simonsound_export.h

  soundprocessor.h
  loudnessmetersoundprocessor.h
  vadsoundprocessor.h
  
  trainsamplevolumepage.h
)

kde4_add_kcfg_files(simonsound_LIB_SRCS soundconfig.kcfgc)

kde4_add_ui_files(simonsound_LIB_SRCS
  prompt.ui
  vad.ui
  postprocessing.ui
  recwidget.ui
  volumewidget.ui
  devicevolumewidget.ui
  wavfilewidget.ui
  trainsamplevolumepage.ui
  trainingsettings.ui
)

kde4_add_ui_files(simonsound_LIB_SRCS deviceconfiguration.ui singledeviceconfiguration.ui)
kde4_add_library(simonsound SHARED ${simonsound_LIB_SRCS})

if(LIBSAMPLERATE_FOUND)
  target_link_libraries(simonsound ${QT_LIBRARIES} ${plattformLibraries}
    simonlogging simonuicomponents simoncontextcoreui simoncontextdetection
    simonwav ${QT_QTXML_LIBRARY}
    ${LIBSAMPLERATE_LIBRARY}
    ${KDE4_KDEUI_LIBS})
else()
  target_link_libraries(simonsound ${QT_LIBRARIES} ${plattformLibraries}
    simonlogging simonuicomponents simoncontextcoreui simoncontextdetection
    simonwav ${QT_QTXML_LIBRARY}
    ${KDE4_KDEUI_LIBS})
endif()

set_target_properties(simonsound PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR})

install(FILES ${simonsound_LIB_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonsound COMPONENT simondevel)
install(TARGETS simonsound DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore)

############

set(kcm_simonsound_LIB_SRCS
  soundsettings.cpp
  samplegroupcontextsettings.cpp
  samplegroupitemdelegate.cpp
)

kde4_add_ui_files(kcm_simonsound_LIB_SRCS samplegroupcontextsettings.ui)
kde4_add_kcfg_files(kcm_simonsound_LIB_SRCS soundconfig.kcfgc)

kde4_add_plugin(simonsoundkcm ${kcm_simonsound_LIB_SRCS})
target_link_libraries(simonsoundkcm ${KDE4_KDEUI_LIBS} 
  simonsound simonuicomponents simoncontextcoreui simoncontextdetection
  simonscenarios ${QT_QTXML_LIBRARY}
)

install(TARGETS simonsoundkcm DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncore)
install(FILES simonsoundconfig.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncore)

add_subdirectory(test)
