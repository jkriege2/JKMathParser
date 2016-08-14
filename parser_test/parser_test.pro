TEMPLATE = app
CONFIG += console c++11 stl rtti exceptions
CONFIG -= app_bundle
CONFIG -= qt

DEFINES +=JKLIB_TEST JKMATHPARSER_DEBUGFUNCTIONNAMES JKMATHPARSER_MATHPARSERTEST #JKMATHPARSER_BYTECODESTACK_ERRORCHECKING

TARGET = parser_test


SOURCES += main.cpp \ 
    ../jkmathparser.cpp \
    ../jkmpdefaultlib.cpp \
    ../jkmptools.cpp \
    ../jkmpbase.cpp \
    ../jkmpstringtools.cpp \
    ../jkmpmathtools.cpp \
    ../jkmpfilenametools.cpp \
    ../StatisticsTools/correlator_multitau.cc \
    ../StatisticsTools/simanneal.cpp \
    ../StatisticsTools/statistics_tools.cpp

HEADERS += \ 
    ../jkmathparser.h \
    ../jkmpbase.h \
    ../jkmpdefaultlib.h \
    ../jkmplib_imexport.h \
    ../jkmptools.h \
    ../jkmpstringtools.h \
    ../jkmpmathtools.h \
    ../jkmpfilenametools.h \
    ../extlibs/MersenneTwister.h \
    ../StatisticsTools/correlator.h \
    ../StatisticsTools/correlator_block.h \
    ../StatisticsTools/correlator_lag.h \
    ../StatisticsTools/correlator_multitau.h \
    ../StatisticsTools/lib_imexport.h \
    ../StatisticsTools/multitau-correlator.h \
    ../StatisticsTools/multitau-msd.h \
    ../StatisticsTools/shiftregister.h \
    ../StatisticsTools/simanneal.h \
    ../StatisticsTools/statistics_tools.h

