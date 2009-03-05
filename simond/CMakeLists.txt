project(simond)


include(FindKDE4)
include(KDE4Defaults)
include(FindQt4)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

set (QT_USE_QTMAIN true)
set (QT_USE_QTNETWORK true)
set (QT_USE_QTSQL true)
set (QT_USE_QTSVG true)

include(${QT_USE_FILE})

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../simonlib )

add_subdirectory( po )
add_subdirectory( config )
add_subdirectory( doc )
add_subdirectory( src )
add_subdirectory( icons )
add_subdirectory(scripts)

install( FILES default.jconf simond.db DESTINATION ${DATA_INSTALL_DIR}/simond COMPONENT simond )
