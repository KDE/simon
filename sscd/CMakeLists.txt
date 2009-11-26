project(sscd)

set (QT_USE_QTSVG false)
set (QT_USE_QTGUI false)
set (QT_USE_QTNETWORK true)
set (QT_USE_QTSQL true)

include(${QT_USE_FILE})

include_directories( ${QT_INCLUDES} ../simonlib )

add_subdirectory( po )
add_subdirectory( config )
add_subdirectory( src )
add_subdirectory( icons )
