TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++17

flatc.name = flatc ${QMAKE_FILE_IN}
flatc.input = FLATC_SOURCES
flatc.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}_generated.h
flatc.commands = flatc --cpp --binary ${QMAKE_FILE_IN}
flatc.CONFIG += target_predeps no_link
silent:flatc.commands = @echo flatc ${QMAKE_FILE_IN} && $$flatc.commands
QMAKE_EXTRA_COMPILERS += flatc

INCLUDEPATH += $$OUT_PWD

SOURCES += main.cpp \
    token.cpp \
    parsetree.cpp \
    codegen.cpp \
    compileargs.cpp \
    compiler.cpp \
    programdb.cpp \
    preprocessor.cpp \
    fail.cpp \
    sourcefile.cpp \
    cparser.cpp \
    clexer.cpp

HEADERS += \
    token.h \
    parsetree.h \
    codegen.h \
    compileargs.h \
    compiler.h \
    programdb.h \
    preprocessor.h \
    fail.h \
    sourcepos.h \
    sourcefile.h \
    cparser.h \
    clexer.h \
    storable.h \
    deeptypes.h

FLATC_SOURCES += \
    sourcefile.fbs

unix|win32: LIBS += -llmdb
