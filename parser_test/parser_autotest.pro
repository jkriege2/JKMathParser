TEMPLATE = app
CONFIG += console c++11 stl rtti exceptions
CONFIG -= app_bundle
CONFIG -= qt

DEFINES +=JKLIB_TEST JKMATHPARSER_DEBUGFUNCTIONNAMES JKMATHPARSER_MATHPARSERTEST #JKMATHPARSER_BYTECODESTACK_ERRORCHECKING

TARGET = parser_test


SOURCES += parser_autotest.cpp

HEADERS +=

include(jkmp.inc)

DISTFILES += \
    jkmp.inc

