project (juliusd)
find_package(KDE4 REQUIRED)

include(FindKDE4)
include(KDE4Defaults)
include(FindQt4)

set (QT_USE_QTMAIN true)
set (QT_USE_QTNETWORK true)
set (QT_USE_QTSQL true)
set (QT_USE_QTSVG true)

include(${QT_USE_FILE})

include_directories( ${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} )
#add_definitions( ${KDE4_DEFINITIONS} )

add_subdirectory( src )
#include (src/src.cmake)

