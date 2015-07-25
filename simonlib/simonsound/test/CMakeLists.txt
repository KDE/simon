set(simonvadtest_SRCS
  vadtest.cpp
)

kde4_add_unit_test(simonvadtest-vad TESTNAME
  simonvadtest-vad
  ${simonvadtest_SRCS}
)

target_link_libraries(simonvadtest-vad
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} 
  simonsound
)
