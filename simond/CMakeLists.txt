project(simond)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_subdirectory( db )
add_subdirectory( config )
add_subdirectory( doc )
add_subdirectory( src )
add_subdirectory( icons )
add_subdirectory(scripts)
