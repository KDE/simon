set(afaras_SRCS
  main.cpp
  afarasview.cpp
)

kde4_add_ui_files( afaras_SRCS
  main.ui
)

kde4_add_app_icon(afaras_SRCS "../icons/hi*-app-afaras.png")
kde4_add_executable(afaras ${afaras_SRCS})

target_link_libraries(afaras ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KUTILS_LIBS}
  ${KDE4_KIO_LIBS} simonsound)

install(TARGETS afaras DESTINATION ${BIN_INSTALL_DIR} COMPONENT afaras)

install(FILES afaras.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} COMPONENT afaras)
install(FILES afarasui.rc DESTINATION ${DATA_INSTALL_DIR} COMPONENT afaras)