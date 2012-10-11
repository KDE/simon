set(simonmodeltesttestresulttest_SRCS
  testresulttest.cpp

  #deps
  ../testresult.cpp
  ../testresultinstance.cpp
  ../testresultleaf.cpp
)

kde4_add_unit_test(simonmodeltesttest-testresult TESTNAME
  simonmodeltesttest-testresult
  ${simonmodeltesttestresulttest_SRCS}
)

target_link_libraries(simonmodeltesttest-testresult
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} 
  simonrecognitionresult
)