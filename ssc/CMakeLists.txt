project(ssc)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_subdirectory( doc )
add_subdirectory( po )
add_subdirectory( config )
#add_subdirectory( sscdaccess )
add_subdirectory( src )
add_subdirectory( themes )
add_subdirectory( icons )
add_subdirectory( defaults )
