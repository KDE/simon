set(simonmodeltesttestresulttest_SRCS
  testresulttest.cpp

  #deps
  ../testresult.cpp
  ../testresultinstance.cpp
  ../testresultleaf.cpp
)

add_executable(simonmodeltesttest-testresultNAME
add_test(simonmodeltesttest simonmodeltesttest)
ecm_mark_as_test(simonmodeltesttest)
  simonmodeltesttest-testresult
  ${simonmodeltesttestresulttest_SRCS}
)

target_link_libraries(simonmodeltesttest-testresult
  KF5::KDELibs4Support Qt5::Test ${QT_LIBRARIES}
  simonrecognitionresult
)
