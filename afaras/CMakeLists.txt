project(afaras)


include (KDE4Defaults)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_subdirectory( po )
#add_subdirectory( doc )
add_subdirectory( src )
add_subdirectory( icons )
