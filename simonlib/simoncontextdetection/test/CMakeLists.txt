set(simonprocessinfogatherertest_SRCS
  processinfogatherertest.cpp
)

add_test(simonprocessinfogatherertest-processinfogatherer NAME
  simonprocessinfogatherertest-processinfogatherer
  ${simonprocessinfogatherertest_SRCS}
)

target_link_libraries(simonprocessinfogatherertest-processinfogatherer
  KF5::KDELibs4Support  Qt5::Test ${QT_LIBRARIES}
  simoncontextdetection
)

add_executable(dummyapplication dummyapplication.cpp)
target_link_libraries(dummyapplication ${QT_LIBRARIES} Qt5::Core Qt5::Gui)