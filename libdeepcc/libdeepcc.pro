#-------------------------------------------------
#
# Project created by QtCreator 2018-05-01T07:59:30
#
#-------------------------------------------------

QT       -= core gui

TARGET = libdeepcc
TEMPLATE = lib
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS += -std=c++17

flatc.name = flatc ${QMAKE_FILE_IN}
flatc.input = FLATC_SOURCES
flatc.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}_generated.h
flatc.commands = flatc --cpp --binary ${QMAKE_FILE_IN}
flatc.CONFIG += target_predeps no_link
silent:flatc.commands = @echo flatc ${QMAKE_FILE_IN} && $$flatc.commands
QMAKE_EXTRA_COMPILERS += flatc

INCLUDEPATH += $$OUT_PWD

SOURCES += \
    clexer.cpp \
    codegen.cpp \
    compileargs.cpp \
    compiler.cpp \
    cparser.cpp \
    fail.cpp \
    parsetree.cpp \
    preprocessor.cpp \
    programdb.cpp \
    sourcefile.cpp \
    storable.cpp \
    token.cpp

HEADERS += \
    clexer.h \
    codegen.h \
    compileargs.h \
    compiler.h \
    cparser.h \
    deeptypes.h \
    fail.h \
    parsetree.h \
    preprocessor.h \
    programdb.h \
    sourcefile.h \
    sourcepos.h \
    storable.h \
    token.h

FLATC_SOURCES += \
    storedobject.fbs

unix {
    target.path = /usr/lib
    INSTALLS += target
}
