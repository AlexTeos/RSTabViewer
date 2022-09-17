CONFIG -= qt

TEMPLATE = lib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

unix:!macx: DEFINES += "__SYMBIAN32__" # crutch for android compability

SOURCES += \
    ogg/src/bitwise.c \
    ogg/src/framing.c

HEADERS += \
    ogg/ogg.h \
    ogg/os_types.h \
    ogg/src/crctable.h

INCLUDEPATH += \
    ogg/include

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
