TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++17

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libdeepcc/release/ -llibdeepcc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libdeepcc/debug/ -llibdeepcc
else:unix: LIBS += -L$$OUT_PWD/../libdeepcc/ -llibdeepcc

INCLUDEPATH += $$PWD/../libdeepcc
DEPENDPATH += $$PWD/../libdeepcc

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libdeepcc/release/liblibdeepcc.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libdeepcc/debug/liblibdeepcc.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libdeepcc/release/libdeepcc.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libdeepcc/debug/libdeepcc.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../libdeepcc/liblibdeepcc.a

unix|win32: LIBS += -llmdb -lgtest -lpthread
