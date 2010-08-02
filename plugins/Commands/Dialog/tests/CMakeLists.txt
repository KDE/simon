ENABLE_TESTING()
find_package(Qt4 COMPONENTS QtCore QtGui QtTest QtXml QtScript REQUIRED)
find_package(KDE4 REQUIRED)

include(KDE4Defaults)

set (simondialogcommandplugintest_SRCS
  boundvaluetest.cpp

  #deps
  ../boundvalue.cpp
  ../staticboundvalue.cpp
  ../scriptboundvalue.cpp
)

if (USE_PLASMA)
  set (simondialogcommandplugintest_ADITIONAL_SRCS
      ../plasmaboundvalue.cpp
      )
endif (USE_PLASMA)


kde4_add_unit_test(simondialogcommandplugintest-boundvalues TESTNAME
  boundValues
  ${simondialogcommandplugintest_SRCS}
  ${simondialogcommandplugintest_ADITIONAL_SRCS}
)

if (USE_PLASMA)
  set (simondialogcommandplugintest_ADITIONAL_LIBS
      ${KDE4_PLASMA_LIBS}
      )
endif (USE_PLASMA)

target_link_libraries( simondialogcommandplugintest-boundvalues
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY}
  ${QT_LIBRARIES} ${simondialogcommandplugintest_ADITIONAL_LIBS}
)
