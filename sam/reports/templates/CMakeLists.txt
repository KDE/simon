FILE(GLOB sam_report_templates *.tex)
install( FILES ${sam_report_templates} DESTINATION
  ${DATA_INSTALL_DIR}/sam/reports/templates COMPONENT sam)