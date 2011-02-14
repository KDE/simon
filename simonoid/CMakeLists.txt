# Project Needs a name ofcourse
project(plasma-simonoid2)
 
# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )
 
# We add our source code here
set(simonoid2_SRCS simonoid2.cpp)
 
# Now make sure all files get to the right place
kde4_add_plugin(plasma_applet_simonoid2 ${simonoid2_SRCS})
target_link_libraries(plasma_applet_simonoid2 
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS})
 
install(TARGETS plasma_applet_simonoid2
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-simonoid2.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
