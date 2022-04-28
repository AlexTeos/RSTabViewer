QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    ../src/Rijndael.cpp \
    ../src/arrangement.cpp \
    ../src/chord.cpp \
    ../src/common.cpp \
    ../src/psarc.cpp \
    ../src/sng.cpp \
    main.cpp \
    tst_psarc.cpp \
    tst_sng.cpp

HEADERS += \
    ../src/chord.h \
    ../src/psarc.h \
    ../src/sng.h \
    tst_psarc.h \
    tst_sng.h
