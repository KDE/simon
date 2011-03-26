find_package(Qt4 COMPONENTS QtCore QtDbus QtGui)
find_package(KDE4)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
########### next target ###############

include_directories(${QT_INCLUDES} ${KDE4_INCLUDES}
	${QT_QTDBUS_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/libskype
	${CMAKE_CURRENT_SOURCE_DIR}/libskype/skypedbus

	${CMAKE_CURRENT_BUILD_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(-DSKYPE_DEBUG_GLOBAL=14311)

########### next target ###############

set(
	skypedbus_SRCS
	libskype/skypedbus/skypeconnection.cpp
)

QT4_ADD_DBUS_ADAPTOR(
	skypedbus_SRCS
	libskype/skypedbus/com.Skype.API.Client.xml
	libskype/skypedbus/skypeconnection.h
	SkypeConnection
	skypeadaptor
	SkypeAdaptor
)

set(
	libskype_SRCS

	libskype/skype.cpp
	libskype/skypewindow.cpp
)

set(
	simon_skype_SRCS
  main.cpp
  simonskype.cpp
	${skypedbus_SRCS}
	${libskype_SRCS}
)

kde4_add_executable(simonskype ${simon_skype_SRCS})

target_link_libraries(
	simonskype
	${KDE4_KDEUI_LIBS}
	${X11_X11_LIB}
)

########### install files ###############

install( TARGETS simonskype DESTINATION ${BIN_INSTALL_DIR} )
