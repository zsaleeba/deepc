TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

unix {
    INCLUDEPATH += /usr/include
}

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../cparser/release/ -lcparser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../cparser/debug/ -lcparser
else:unix: LIBS += -L$$OUT_PWD/../cparser/ -lcparser

INCLUDEPATH += $$PWD/../cparser
DEPENDPATH += $$PWD/../cparser

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../cparser/release/libcparser.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../cparser/debug/libcparser.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../cparser/release/cparser.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../cparser/debug/cparser.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../cparser/libcparser.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../persmem/release/ -lpersmem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../persmem/debug/ -lpersmem
else:unix: LIBS += -L$$OUT_PWD/../persmem/ -lpersmem

INCLUDEPATH += $$PWD/../persmem
DEPENDPATH += $$PWD/../persmem

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../persmem/release/libpersmem.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../persmem/debug/libpersmem.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../persmem/release/persmem.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../persmem/debug/persmem.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../persmem/libpersmem.a
