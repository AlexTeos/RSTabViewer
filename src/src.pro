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
        RS/chordnote.cpp \
        RS/common.cpp \
        RS/metadata.cpp \
        RS/psarcarchive.cpp \
        RS/soundbank.cpp \
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
    RS/benddata.h \
    RS/chord.h \
    RS/chordnote.h \
    RS/common.h \
    RS/metadata.h \
    RS/psarc.h \
    RS/psarcarchive.h \
    RS/sng.h \
    RS/soundbank.h \
    musiclibrary.h \
    tablature.h


# Vorbis
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/RS/3rdparty/revorb/vorbisQt/release/ -lvorbisQt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/RS/3rdparty/revorb/vorbisQt/debug/ -lvorbisQt
else:unix:!macx: LIBS += -L$$OUT_PWD/RS/3rdparty/revorb/vorbisQt/ -lvorbisQt_armeabi-v7a

INCLUDEPATH += $$PWD/../src/RS/3rdparty/revorb/vorbisQt
DEPENDPATH += $$PWD/../src/RS/3rdparty/revorb/vorbisQt

# OGG
unix:!macx: DEFINES += "__SYMBIAN32__" # crutch for android compability

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/RS/3rdparty/revorb/oggQt/release/ -loggQt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/RS/3rdparty/revorb/oggQt/debug/ -loggQt
else:unix:!macx: LIBS += -L$$OUT_PWD/RS/3rdparty/revorb/oggQt/ -loggQt_armeabi-v7a

INCLUDEPATH += $$PWD/../src/RS/3rdparty/revorb/oggQt
DEPENDPATH += $$PWD/../src/RS/3rdparty/revorb/oggQt

# ww2ogg
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/RS/3rdparty/ww2oggQt/release/ -lww2oggQt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/RS/3rdparty/ww2oggQt/debug/ -lww2oggQt
else:unix:!macx: LIBS += -L$$OUT_PWD/RS/3rdparty/ww2oggQt/ -lww2oggQt_armeabi-v7a

INCLUDEPATH += $$PWD/../src/RS/3rdparty/ww2oggQt \
    ../src\RS\3rdparty\revorb\oggQt\ogg\include
DEPENDPATH += $$PWD/../src/RS/3rdparty/ww2oggQt

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
