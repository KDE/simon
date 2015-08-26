set(simonactions_LIB_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/commandmodel.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/commandtablemodel.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/commandsettingsinternal.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/listconfiguration.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/actionmanager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/greedyreceiver.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/listcommand.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/commandlistwidget.cpp
)

set(simonactions_LIB_HDRS
  ${CMAKE_CURRENT_SOURCE_DIR}/commandmodel.h
  ${CMAKE_CURRENT_SOURCE_DIR}/actionmanager.h
  ${CMAKE_CURRENT_SOURCE_DIR}/greedyreceiver.h
  ${CMAKE_CURRENT_SOURCE_DIR}/commandsettingsinternal.h
  ${CMAKE_CURRENT_SOURCE_DIR}/listconfiguration.h
  ${CMAKE_CURRENT_SOURCE_DIR}/listcommand.h
  ${CMAKE_CURRENT_SOURCE_DIR}/commandlistwidget.h
)

ki18n_wrap_ui(simonactions_LIB_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/commandsettingsdlg.ui
  ${CMAKE_CURRENT_SOURCE_DIR}/listconfigurationdlg.ui
)

qt5_add_dbus_adaptor(simonactions_LIB_SRCS org.simon-listens.ActionManager.xml actionmanager.h ActionManager)

kde4_add_library(simonactions SHARED ${simonactions_LIB_SRCS})

target_link_libraries(simonactions ${QT_LIBRARIES} Qt5::Xml
  KF5::IconThemes KF5::KIOCore simonscenarios simonrecognitionresult
  simonscenariobase
)

set_target_properties(simonactions PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR})

install(FILES ${simonactions_LIB_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonactions COMPONENT simondevel)
install(TARGETS simonactions DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon)

############

set(kcm_simonactions_LIB_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/commandsettings.cpp
)

kde4_add_plugin(simonactionskcm ${kcm_simonactions_LIB_SRCS})
target_link_libraries(simonactionskcm  simonactions)

install(TARGETS simonactionskcm DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simon)
install(FILES simonactionsconfig.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simon)
