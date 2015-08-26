set(simonvadtest_SRCS
  vadtest.cpp
)

add_executable(simonvadtest-vadNAME
add_test(simonvadtest simonvadtest)
ecm_mark_as_test(simonvadtest)
  simonvadtest-vad
  ${simonvadtest_SRCS}
)

target_link_libraries(simonvadtest-vad
  KF5::KDELibs4Support Qt5::Test ${QT_LIBRARIES}
  simonsound
)
