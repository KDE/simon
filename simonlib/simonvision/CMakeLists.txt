include(SimonDirs)

include(KDE4Defaults)
#include(KDE4Macros)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${OpenCV_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/../ ${OpenCV_INCLUDE_DIRS})
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
FIND_PACKAGE( OpenCV REQUIRED )
ADD_DEFINITIONS(-fexceptions)
link_directories(${OpenCV_LIB_DIR})
set (QT_USE_QTMAIN true)
set (simonvision_LIB_SRCS
	webcamdispatcher.cpp
	imageanalyzer.cpp
	faceanalyzer.cpp
#	cvhelper.cpp
)

set( simonvision_LIB_HDRS
    simonvision_export.h
    webcamdispatcher.h
    imageanalyzer.h
    faceanalyzer.h
#    cvhelper.h
)

set(SIMONCV_PACKAGE "OpenCV" CACHE STRING OpenCV)
mark_as_advanced("${SIMONCV_PACKAGE}")

kde4_add_library(simonvision  SHARED ${simonvision_LIB_SRCS} )

target_link_libraries(simonvision ${KDE4_KPARTS_LIBS} ${KDE4_KDECORE_LIBS}  ${OpenCV_LIBS} ${QT_LIBRARIES})


set_target_properties( simonvision
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simonvision_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonvision
  COMPONENT simoncoredevel
)
 
install( TARGETS simonvision DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore )
install( FILES haarcascade_frontalface_default.xml DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncore )





