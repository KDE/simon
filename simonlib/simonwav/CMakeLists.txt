set(simonwav_LIB_SRCS wav.cpp)
set(simonwav_LIB_HDRS wav.h)

kde4_add_library(simonwav SHARED ${simonwav_LIB_SRCS})
target_link_libraries(simonwav Qt5::Core KF5::I18n)

set_target_properties(simonwav PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR})

install(FILES ${simonwav_LIB_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonwav COMPONENT simondevel)
install(TARGETS simonwav DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore)
