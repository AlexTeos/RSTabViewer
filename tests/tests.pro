QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    ../src/RS/3rdparty/Rijndael/Rijndael.cpp \
    ../src/RS/3rdparty/revorb/revorb.cpp \
    ../src/RS/3rdparty/ww2oggQt/ww2ogg.cpp \
    ../src/RS/arrangement.cpp \
    ../src/RS/chord.cpp \
    ../src/RS/common.cpp \
    ../src/RS/metadata.cpp \
    ../src/RS/psarcarchive.cpp \
    ../src/RS/sng.cpp \
    ../src/RS/soundbank.cpp \
    main.cpp \
    tst_psarc.cpp

HEADERS += \
    ../src/RS/3rdparty/Rijndael/Rijndael.h \
    ../src/RS/3rdparty/revorb/revorb.h \
    ../src/RS/3rdparty/ww2oggQt/ww2ogg.h \
    ../src/RS/arrangement.h \
    ../src/RS/chord.h \
    ../src/RS/common.h \
    ../src/RS/metadata.h \
    ../src/RS/psarcarchive.h \
    ../src/RS/sng.h \
    ../src/RS/soundbank.h \
    tst_psarc.h

# Vorbis
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/RS/3rdparty/revorb/vorbisQt/release/ -lvorbisQt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/RS/3rdparty/revorb/vorbisQt/debug/ -lvorbisQt
else:unix:!macx: LIBS += -L$$OUT_PWD/../src/RS/3rdparty/revorb/vorbisQt/ -lvorbisQt_armeabi-v7a

INCLUDEPATH += $$PWD/../src/RS/3rdparty/revorb/vorbisQt
DEPENDPATH += $$PWD/../src/RS/3rdparty/revorb/vorbisQt

# OGG
unix:!macx: DEFINES += "__SYMBIAN32__" # crutch for android compability

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/RS/3rdparty/revorb/oggQt/release/ -loggQt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/RS/3rdparty/revorb/oggQt/debug/ -loggQt
else:unix:!macx: LIBS += -L$$OUT_PWD/../src/RS/3rdparty/revorb/oggQt/ -loggQt_armeabi-v7a

INCLUDEPATH += $$PWD/../src/RS/3rdparty/revorb/oggQt
DEPENDPATH += $$PWD/../src/RS/3rdparty/revorb/oggQt

# ww2ogg
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/RS/3rdparty/ww2oggQt/release/ -lww2oggQt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/RS/3rdparty/ww2oggQt/debug/ -lww2oggQt
else:unix:!macx: LIBS += -L$$OUT_PWD/../src/RS/3rdparty/ww2oggQt/ -lww2oggQt_armeabi-v7a

INCLUDEPATH += $$PWD/../src/RS/3rdparty/ww2oggQt \
    ../src\RS\3rdparty\revorb\oggQt\ogg\include
DEPENDPATH += $$PWD/../src/RS/3rdparty/ww2oggQt
