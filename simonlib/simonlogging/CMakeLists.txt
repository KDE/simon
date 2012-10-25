set(simonlogging_LIB_SRCS logger.cpp)

set(simonlogging_LIB_HDRS
  simonlogging_export.h
  logger.h
)

kde4_add_library(simonlogging SHARED ${simonlogging_LIB_SRCS})
target_link_libraries(simonlogging ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS})

set_target_properties(simonlogging PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR})

install(FILES ${simonlogging_LIB_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/simon/logging COMPONENT simoncoredevel)
 
install(TARGETS simonlogging DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore)
