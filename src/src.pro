QT += quick network multimedia

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        RS/3rdparty/Rijndael/Rijndael.cpp \
        RS/3rdparty/revorb/revorb.cpp \
        RS/3rdparty/ww2oggQt/ww2ogg.cpp \
        RS/arrangement.cpp \
        RS/chord.cpp \
        RS/common.cpp \
        RS/metadata.cpp \
        main.cpp \
        RS/psarc.cpp \
        RS/sng.cpp \
        musiclibrary.cpp \
        tablature.cpp

RESOURCES += ui/qml.qrc \
    ui/resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    RS/3rdparty/Rijndael/Rijndael.h \
    RS/3rdparty/revorb/revorb.h \
    # TODO: fix it
    RS/3rdparty/ww2oggQt/ww2ogg.h \
    RS/arrangement.h \
    RS/chord.h \
    RS/common.h \
    RS/metadata.h \
    RS/psarc.h \
    RS/sng.h \
    musiclibrary.h \
    tablature.h


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

