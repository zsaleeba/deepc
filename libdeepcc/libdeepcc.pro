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
    sourcespan.cpp \
    sourcefile.cpp \
    allsources.cpp \
    programdb.cpp \
    programdb.pb.cpp

HEADERS += sourcepos.h \
    chain.h \
    sourcefile.h \
    cbtree.h \
    sourcespan.h \
    allsources.h \
    programdb.h \
    lmdb++.h \
    programdb.pb.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
