set(simontts_LIB_SRCS
  simontts.cpp
  recordingset.cpp
  recordingsetcollection.cpp
  simonttsprivate.cpp
  joviettsprovider.cpp
  recordedttsprovider.cpp
  webservicettsprovider.cpp
)

set(simontts_LIB_HDRS
  ${CMAKE_CURRENT_BINARY_DIR}/simontts_export.h
  simontts.h
)

kconfig_add_kcfg_files(simontts_LIB_SRCS config/ttsconfiguration.kcfgc)

qt5_add_dbus_adaptor(simontts_LIB_SRCS org.simon-listens.SimonTTS.xml simonttsprivate.h SimonTTSPrivate)

kde4_add_library(simontts SHARED ${simontts_LIB_SRCS})
target_link_libraries(simontts simonsound simonwav ${QT_LIBRARIES} KF5::KDELibs4Support KF5::KIOCore KF5::Archive)

generate_export_header(simontts)

set_target_properties(simontts
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install(FILES ${simontts_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/tts
  COMPONENT simoncoredevel
)
 
install(TARGETS simontts DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore)
install(FILES ttssets.xml DESTINATION ${DATA_INSTALL_DIR}/simon/ttsrec COMPONENT simoncore)

add_subdirectory(config)
