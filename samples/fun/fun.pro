TARGET = qgeoview-samples-fun
TEMPLATE = app
CONFIG-= console

QT += gui widgets network

include(../lib.pri)
include(../shared.pri)

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    waveanimation.cpp \
    placemark.cpp

HEADERS += \
    mainwindow.h \
    waveanimation.h \
    placemark.h
