CONFIG -= qt

TEMPLATE = lib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    vorbis/lib/analysis.c \
    vorbis/lib/bitrate.c \
    vorbis/lib/block.c \
    vorbis/lib/codebook.c \
    vorbis/lib/envelope.c \
    vorbis/lib/floor0.c \
    vorbis/lib/floor1.c \
    vorbis/lib/info.c \
    vorbis/lib/lookup.c \
    vorbis/lib/lpc.c \
    vorbis/lib/lsp.c \
    vorbis/lib/mapping0.c \
    vorbis/lib/mdct.c \
    vorbis/lib/psy.c \
    vorbis/lib/registry.c \
    vorbis/lib/res0.c \
    vorbis/lib/sharedbook.c \
    vorbis/lib/smallft.c \
    vorbis/lib/synthesis.c \
    vorbis/lib/vorbisenc.c \
    vorbis/lib/window.c


HEADERS += \
    vorbis/lib/backends.h \
    vorbis/lib/bitrate.h \
    vorbis/lib/codebook.h \
    vorbis/include/vorbis/codec.h \
    vorbis/lib/codec_internal.h \
    vorbis/lib/envelope.h \
    vorbis/lib/modes/floor_all.h \
    vorbis/lib/books/floor/floor_books.h \
    vorbis/lib/highlevel.h \
    vorbis/lib/lookup.h \
    vorbis/lib/lookup_data.h \
    vorbis/lib/lpc.h \
    vorbis/lib/lsp.h \
    vorbis/lib/masking.h \
    vorbis/lib/mdct.h \
    vorbis/lib/misc.h \
    vorbis/lib/os.h \
    vorbis/lib/psy.h \
    vorbis/lib/modes.psych_11.h \
    vorbis/lib/modes.psych_16.h \
    vorbis/lib/modes.psych_44.h \
    vorbis/lib/modes.psych_8.h \
    vorbis/lib/registry.h \
    vorbis/lib/books.coupled.res_books_stereo.h \
    vorbis/lib/books.uncoupled.res_books_uncoupled.h \
    vorbis/lib/modes.residue_16.h \
    vorbis/lib/modes.residue_44.h \
    vorbis/lib/modes.residue_44u.h \
    vorbis/lib/modes.residue_8.h \
    vorbis/lib/scales.h \
    vorbis/lib/modes/setup_11.h \
    vorbis/lib/modes/setup_16.h \
    vorbis/lib/modes/setup_22.h \
    vorbis/lib/modes/setup_32.h \
    vorbis/lib/modes/setup_44.h \
    vorbis/lib/modes/setup_44u.h \
    vorbis/lib/modes/setup_8.h \
    vorbis/lib/modes/setup_X.h \
    vorbis/lib/smallft.h \
    vorbis/include/vorbis/vorbisenc.h \
    vorbis/include/vorbis/vorbisfile.h \
    vorbis/lib/window.h \


INCLUDEPATH += \
    vorbis/include \
    vorbis/lib


# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

# OGG
unix:!macx: DEFINES += "__SYMBIAN32__" # crutch for android compability

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../oggQt/release/ -loggQt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../oggQt/debug/ -loggQt
else:unix:!macx: LIBS += -L$$OUT_PWD/../oggQt/ -loggQt_armeabi-v7a

INCLUDEPATH += $$PWD/../oggQt \
    $$PWD/../oggQt/ogg/include
DEPENDPATH += $$PWD/../oggQt
