set( simonxml_LIB_SRCS xmldomreader.cpp xmlsaxreader.cpp xmlreader.cpp)
set( simonxml_LIB_HDRS ${CMAKE_CURRENT_BINARY_DIR}/simonxml_export.h xmlreader.h xmldomreader.h xmlsaxreader.h)

kde4_add_library(simonxml SHARED ${simonxml_LIB_SRCS})
target_link_libraries(simonxml Qt5::Xml KF5::Archive KF5::I18n)

generate_export_header(simonxml)

set_target_properties(simonxml
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install(FILES ${simonxml_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonxml
  COMPONENT simondevel
)

install(TARGETS simonxml DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore)
