#-------------------------------------------------
#
# Project created by QtCreator 2016-07-03T16:03:57
#
#-------------------------------------------------

QT       -= core gui

TARGET = cparser
TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH += /home/zik/build/deepc/cparser

SOURCES += ctree.cpp \
    cparserlistener.cpp

HEADERS += ctree.h \
    cparserlistener.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
