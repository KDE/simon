if (USE_PLASMA)
  set(PLAMSA_DEP_SRC plasmaboundvalue.cpp)
  set(PLAMSA_DEP_LIB ${KDE4_PLASMA_LIBS})
else()
  set(PLAMSA_DEP_SRC "")
  set(PLAMSA_DEP_LIB "")
endif()

set(simondialogengine_SRCS
  dialogcommand.cpp
  dialogstate.cpp
  dialogtext.cpp
  dialogtextparser.cpp
  dialogtemplateoptions.cpp
  boundvalue.cpp
  dialogboundvalues.cpp
  staticboundvalue.cpp
  argumentboundvalue.cpp
  scriptboundvalue.cpp
  avatar.cpp
  avatarmodel.cpp
  dialogmanager.cpp
  dialogview.cpp
  ${PLAMSA_DEP_SRC}
)

set(simondialogengine_LIB_HDRS
  dialogcommand.h
  dialogstate.h
  dialogtext.h
  dialogtextparser.h
  dialogvariablestore.h
  dialogtemplateoptions.h
  boundvalue.h
  dialogboundvalues.h
  staticboundvalue.h
  scriptboundvalue.h
  argumentboundvalue.h
  avatar.h
  avatarmodel.h
  dialogmanager.h
  simondialogengine_export.h
)

kde4_add_library(simondialogengine SHARED ${simondialogengine_SRCS})

set_target_properties(simondialogengine
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

target_link_libraries(simondialogengine
  ${QT_LIBRARIES} ${KDE4_KIO_LIBS} ${PLAMSA_DEP_LIB} simonscenarios
  ${QT_QTSCRIPT_LIBRARY} ${KDE4_KDEUI_LIBS} simonscenariobase simonactions)

install(TARGETS simondialogengine DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore )


install( FILES ${simondialogengine_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simondialogengine
  COMPONENT simondevel
)

add_subdirectory(graphicalview)
add_subdirectory(ttsview)
add_subdirectory(confui)

add_subdirectory(tests)
