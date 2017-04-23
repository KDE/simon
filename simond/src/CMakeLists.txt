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

file(GLOB ICONS_SRCS "../icons/hi*-app-simond.png")
kde4_add_app_icon(simond_SRCS ICONS ${ICONS_SRCS})
add_executable(simond ${simond_SRCS})

target_link_libraries(simond  Qt5::Network simonrecognizer
  Qt5::Core Qt5::Xml KF5::ConfigCore simonscenarios simonmodelcompilation simonddatabaseaccess
  simonrecognitionresult simonwav simoncontextadapter simonutils simonrecognizer)

install(TARGETS simond DESTINATION ${BIN_INSTALL_DIR} COMPONENT simond )
install( FILES org.kde.simond.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} COMPONENT simond )
