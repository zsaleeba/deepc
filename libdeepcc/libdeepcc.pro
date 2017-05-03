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
    INCLUDEPATH += /usr/include /usr/local/include /usr/include/x86_64-linux-gnu
}


SOURCES += sourcepos.cpp \
    sourcespan.cpp \
    sourcefile.cpp \
    allsources.cpp \
    programdb.cpp \
    sourcemonitor.cpp \
    ctree.cpp \
    sourcediff.cpp \
    diff.cpp

HEADERS += sourcepos.h \
    chain.h \
    sourcefile.h \
    cbtree.h \
    sourcespan.h \
    allsources.h \
    programdb.h \
    sourcefile_generated.h \
    sourcemonitor.h \
    clexer.h \
    ctree.h \
    sourcediff.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    programdb.fbs
