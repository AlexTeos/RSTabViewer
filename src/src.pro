QT += quick network

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Rijndael.cpp \
        arrangement.cpp \
        chord.cpp \
        common.cpp \
        main.cpp \
        psarc.cpp \
        sng.cpp

RESOURCES += ui/qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Rijndael.h \
    arrangement.h \
    chord.h \
    common.h \
    psarc.h \
    sng.h

