QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    ../src/RS/3rdparty/Rijndael/Rijndael.cpp \
    ../src/RS/3rdparty/revorb/revorb.cpp \
    ../src/RS/arrangement.cpp \
    ../src/RS/chord.cpp \
    ../src/RS/common.cpp \
    ../src/RS/metadata.cpp \
    ../src/RS/psarc.cpp \
    ../src/RS/sng.cpp \
    main.cpp \
    tst_psarc.cpp

HEADERS += \
    ../src/RS/3rdparty/Rijndael/Rijndael.h \
    ../src/RS/3rdparty/revorb/revorb.h \
    ../src/RS/arrangement.h \
    ../src/RS/chord.h \
    ../src/RS/common.h \
    ../src/RS/metadata.h \
    ../src/RS/psarc.h \
    ../src/RS/sng.h \
    tst_psarc.h


# Vorbis
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/RS/3rdparty/revorb/vorbisQt/release/ -lvorbisQt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/RS/3rdparty/revorb/vorbisQt/debug/ -lvorbisQt

INCLUDEPATH += $$PWD/../src/RS/3rdparty/revorb/vorbisQt
DEPENDPATH += $$PWD/../src/RS/3rdparty/revorb/vorbisQt

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/RS/3rdparty/revorb/vorbisQt/release/libvorbisQt.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/RS/3rdparty/revorb/vorbisQt/debug/libvorbisQt.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/RS/3rdparty/revorb/vorbisQt/release/vorbisQt.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/RS/3rdparty/revorb/vorbisQt/debug/vorbisQt.lib


# OGG
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/RS/3rdparty/revorb/oggQt/release/ -loggQt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/RS/3rdparty/revorb/oggQt/debug/ -loggQt

INCLUDEPATH += $$PWD/../src/RS/3rdparty/revorb/oggQt
DEPENDPATH += $$PWD/../src/RS/3rdparty/revorb/oggQt

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/RS/3rdparty/revorb/oggQt/release/liboggQt.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/RS/3rdparty/revorb/oggQt/debug/liboggQt.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/RS/3rdparty/revorb/oggQt/release/oggQt.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/RS/3rdparty/revorb/oggQt/debug/oggQt.lib


# ww2ogg
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/RS/3rdparty/ww2oggQt/release/ -lww2oggQt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/RS/3rdparty/ww2oggQt/debug/ -lww2oggQt

INCLUDEPATH += $$PWD/../src/RS/3rdparty/ww2oggQt \
    ../src\RS\3rdparty\revorb\oggQt\ogg\include
DEPENDPATH += $$PWD/../src/RS/3rdparty/ww2oggQt

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/RS/3rdparty/ww2oggQt/release/libww2oggQt.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/RS/3rdparty/ww2oggQt/debug/libww2oggQt.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/RS/3rdparty/ww2oggQt/release/ww2oggQt.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/RS/3rdparty/ww2oggQt/debug/ww2oggQt.lib
