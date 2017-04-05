#-------------------------------------------------
#
# Project created by QtCreator 2016-07-03T16:03:57
#
#-------------------------------------------------

QT       -= core gui

TARGET = libdeepcc
TEMPLATE = lib
CONFIG += staticlib
QMAKE_CXXFLAGS += -std=c++11

unix {
    INCLUDEPATH += /usr/include
}


SOURCES += sourcepos.cpp \
    sourcebag.cpp \
    sourcespan.cpp \
    sourcefile.cpp

HEADERS += sourcepos.h \
    chain.h \
    sourcebag.h \
    sourcefile.h \
    cbtree.h \
    sourcespan.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
