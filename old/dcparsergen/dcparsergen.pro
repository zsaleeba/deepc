#-------------------------------------------------
#
# Project created by QtCreator 2016-07-03T16:03:57
#
#-------------------------------------------------

TARGET = parsergen
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/include

SOURCES += grammar.c  lexergen.c  main.c  parsergen.c \
    util.c

HEADERS += grammar.h  lexergen.h  parsergen.h \
    util.h

DISTFILES += \
    c_lexical.pgen \
    c_syntax.pgen

