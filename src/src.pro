QT += quick network multimedia

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        RS/3rdparty/Rijndael/Rijndael.cpp \
        RS/arrangement.cpp \
        RS/chord.cpp \
        RS/common.cpp \
        RS/metadata.cpp \
        main.cpp \
        RS/psarc.cpp \
        RS/sng.cpp \
        musiclibrary.cpp \
        tablature.cpp

RESOURCES += ui/qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    RS/3rdparty/Rijndael/Rijndael.h \
    RS/arrangement.h \
    RS/chord.h \
    RS/common.h \
    RS/metadata.h \
    RS/psarc.h \
    RS/sng.h \
    musiclibrary.h \
    tablature.h

