QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    ../src/Rijndael.cpp \
    ../src/psarc.cpp \
    main.cpp \
    tst_psarc.cpp

HEADERS += \
    ../src/Rijndael.h \
    ../src/psarc.h \
    tst_psarc.h
