set(simonprocessinfogatherertest_SRCS
  processinfogatherertest.cpp
)

kde4_add_unit_test(simonprocessinfogatherertest-processinfogatherer TESTNAME
  simonprocessinfogatherertest-processinfogatherer
  ${simonprocessinfogatherertest_SRCS}
)

target_link_libraries(simonprocessinfogatherertest-processinfogatherer
  ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} 
  simoncontextdetection
)

kde4_add_executable(dummyapplication dummyapplication.cpp)
target_link_libraries(dummyapplication ${QT_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})