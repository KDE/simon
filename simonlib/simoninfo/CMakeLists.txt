set(simoninfo_LIB_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/simoninfo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/osd.cpp
)

set(simoninfo_LIB_HDRS
  simoninfo_export.h
  simoninfo.h
)

kde4_add_library(simoninfo SHARED ${simoninfo_LIB_SRCS})
target_link_libraries(simoninfo ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS})

set_target_properties(simoninfo
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install(FILES ${simoninfo_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simoninfo
  COMPONENT simoncore
)
 
install(TARGETS simoninfo DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore)

add_subdirectory(themes)