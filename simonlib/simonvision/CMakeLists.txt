add_subdirectory(config)

set (simonvision_LIB_SRCS
        webcamdispatcher.cpp
        imageanalyzer.cpp
        faceanalyzer.cpp
        lipanalyzer.cpp
        simoncv.cpp
)
set( simonvision_LIB_HDRS
    simonvision_export.h
    webcamdispatcher.h
    imageanalyzer.h
    faceanalyzer.h
    lipanalyzer.h
    simoncv.h
)
kde4_add_kcfg_files(simonvision_LIB_SRCS config/webcamconfiguration.kcfgc)

kde4_add_library(simonvision SHARED ${simonvision_LIB_SRCS} )

target_link_libraries(simonvision ${KDE4_KPARTS_LIBS} ${KDE4_KDECORE_LIBS} ${OpenCV_LIBS} ${QT_LIBRARIES})

set_target_properties( simonvision
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simonvision_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonvision
  COMPONENT simoncoredevel
)

install( TARGETS simonvision DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore )
install( FILES haarcascade_frontalface_default.xml haarcascade_mcs_mouth.xml
               DESTINATION ${DATA_INSTALL_DIR} COMPONENT simoncore )
