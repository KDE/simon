set(simonscenariobase_LIB_SRCS
  versionnumber.cpp
  scenarioobject.cpp
)

set(simonscenariobase_LIB_HDRS
  simonscenariobase_export.h
  versionnumber.h
  scenarioobject.h
)

kde4_add_library(simonscenariobase SHARED ${simonscenariobase_LIB_SRCS})
target_link_libraries(simonscenariobase ${QT_LIBRARIES} Qt5::Xml KF5::KDELibs4Support)

set_target_properties(simonscenariobase
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install(FILES ${simonscenariobase_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonscenariobase
  COMPONENT simoncoredevel
)
 
install(TARGETS simonscenariobase DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore)
