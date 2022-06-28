TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    ww2ogg.cpp \
    ww2ogg/src/codebook.cpp \
    ww2ogg/src/crc.c \
    ww2ogg/src/wwriff.cpp

HEADERS += \
    ww2ogg.h \
    ww2ogg/src/Bit_stream.h \
    ww2ogg/src/codebook.h \
    ww2ogg/src/crc.h \
    ww2ogg/src/errors.h \
    ww2ogg/src/wwriff.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    codebooks.qrc
