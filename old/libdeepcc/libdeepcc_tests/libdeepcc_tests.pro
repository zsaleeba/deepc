TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += ..

unix {
    INCLUDEPATH += /usr/include /usr/local/include
}

SOURCES += main.cpp \
    cbtree_test.cpp

unix|win32: LIBS += -lgtest

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../release/ -llibdeepcc
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../debug/ -llibdeepcc
#else:unix: LIBS += -L$$OUT_PWD/../ -llibdeepcc

#INCLUDEPATH += $$PWD/../
#DEPENDPATH += $$PWD/../

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../release/liblibdeepcc.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../debug/liblibdeepcc.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../release/libdeepcc.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../debug/libdeepcc.lib
#else:unix: PRE_TARGETDEPS += $$OUT_PWD/../liblibdeepcc.a

unix|win32: LIBS += -lpthread
