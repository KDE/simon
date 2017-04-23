set(afaras_SRCS
  main.cpp
  afarasview.cpp
)

ki18n_wrap_ui( afaras_SRCS
  main.ui
)

file(GLOB ICONS_SRCS "../icons/hi*-app-afaras.png")
kde4_add_app_icon(afaras_SRCS ICONS ${ICONS_SRCS})
add_executable(afaras ${afaras_SRCS})

target_link_libraries(afaras ${QT_LIBRARIES} KF5::KCMUtils KF5::KIOCore simonsound)

install(TARGETS afaras DESTINATION ${BIN_INSTALL_DIR} COMPONENT afaras)

install(FILES org.kde.afaras.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} COMPONENT afaras)
install(FILES afarasui.rc DESTINATION ${DATA_INSTALL_DIR} COMPONENT afaras)
