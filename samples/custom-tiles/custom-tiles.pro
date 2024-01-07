TARGET = qgeoview-samples-custom-tiles
TEMPLATE = app
CONFIG-= console

QT += gui widgets network

include(../lib.pri)
include(../shared.pri)

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mytile.cpp \
    mytiles.cpp

HEADERS += \
    mainwindow.h \
    mytile.h \
    mytiles.h
