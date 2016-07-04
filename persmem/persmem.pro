#-------------------------------------------------
#
# Project created by QtCreator 2016-07-03T16:03:57
#
#-------------------------------------------------

QT       -= core gui

TARGET = persmem
TEMPLATE = lib
CONFIG += staticlib

SOURCES += persmem.c \
    memsize.c \
    pool.c

HEADERS += persmem.h \
    persmem_internal.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
