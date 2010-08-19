project(sscd)

set (QT_USE_QTSVG false)
set (QT_USE_QTGUI false)
set (QT_USE_QTNETWORK true)
set (QT_USE_QTSQL true)

include(${QT_USE_FILE})

include_directories( ${QT_INCLUDES} ../simonlib )

add_subdirectory( src )
add_subdirectory( icons )
add_subdirectory( doc )

if(WIN32)
install( FILES sscd.conf mysql_create_script.sql DESTINATION ${BIN_INSTALL_DIR} COMPONENT sscd )
else(WIN32)
install( FILES sscd.conf mysql_create_script.sql DESTINATION share/sscd COMPONENT sscd )
endif(WIN32)
