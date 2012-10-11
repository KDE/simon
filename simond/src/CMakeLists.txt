add_subdirectory( config )

set(simond_SRCS
  main.cpp
  simondcontrol.cpp
  clientsocket.cpp
  synchronisationmanager.cpp
  recognitioncontrolfactory.cpp
  recognitioncontrol.cpp
  juliuscontrol.cpp
)

if(${BackendType} STREQUAL both)
  set(simond_SRCS ${simond_SRCS} sphinxcontrol.cpp)
endif()

kde4_add_app_icon(simond_SRCS "../icons/hi*-app-simond.png")
kde4_add_executable(simond ${simond_SRCS})

target_link_libraries(simond ${KDE4_KDECORE_LIBS} ${QT_QTNETWORK_LIBRARY} simonrecognizer
  ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY} simonscenarios simonmodelcompilation simonddatabaseaccess
  simonrecognitionresult simonwav simoncontextadapter simonutils simonrecognizer)

install(TARGETS simond DESTINATION ${BIN_INSTALL_DIR} COMPONENT simond )
install( FILES simond.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} COMPONENT simond )
