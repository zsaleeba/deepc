#-------------------------------------------------
#
# Project created by QtCreator 2016-07-03T16:03:57
#
#-------------------------------------------------

QT       -= core gui

TARGET = libdeepcc
TEMPLATE = lib
CONFIG += staticlib

SOURCES += dcdbfile.c

HEADERS += dcdbfile.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
