TEMPLATE = app
CONFIG += console c++11 stl rtti exceptions
CONFIG -= app_bundle
CONFIG -= qt

DEFINES +=JKLIB_TEST JKMATHPARSER_DEBUGFUNCTIONNAMES JKMATHPARSER_MATHPARSERTEST #JKMATHPARSER_BYTECODESTACK_ERRORCHECKING

TARGET = parser_test


SOURCES += main.cpp \ 
    ../jkmathparser.cpp \
    ../jkmathparserdefaultlib.cpp \
    ../jkmathparsertools.cpp \
    ../jkmathparserbase.cpp \
    ../jkmathparserstringtools.cpp \
    ../StatisticsTools/correlator_multitau.cc \
    ../StatisticsTools/simanneal.cpp \
    ../StatisticsTools/statistics_tools.cpp

HEADERS += \ 
    ../jkmathparser.h \
    ../jkmathparserbase.h \
    ../jkmathparserdefaultlib.h \
    ../jkmathparserlib_imexport.h \
    ../jkmathparsertools.h \
    ../extlibs/MersenneTwister.h \
    ../jkmathparserstringtools.h \
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

CONFIG += exceptions rtti stl

