TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /home/zik/build/deepc/deepcserv

SOURCES += main.cpp \
    lspserver.cpp

HEADERS += \
    lspserver.h

DISTFILES += \
    lsp_spec.json
