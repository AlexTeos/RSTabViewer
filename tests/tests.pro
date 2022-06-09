QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    ../src/RS/Rijndael.cpp \
    ../src/RS/arrangement.cpp \
    ../src/RS/chord.cpp \
    ../src/RS/common.cpp \
    ../src/RS/psarc.cpp \
    ../src/RS/sng.cpp \
    main.cpp \
    tst_psarc.cpp

HEADERS += \
    ../src/RS/chord.h \
    ../src/RS/psarc.h \
    ../src/RS/sng.h \
    tst_psarc.h
