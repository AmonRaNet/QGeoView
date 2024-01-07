TARGET = qgeoview-samples-shared
TEMPLATE = lib
CONFIG += staticlib

QT += gui widgets network

include(../lib.pri)

SOURCES += \
    $$PWD/helpers.cpp \
    $$PWD/placemarkcircle.cpp \
    $$PWD/rectangle.cpp

HEADERS += \
    $$PWD/helpers.h \
    $$PWD/placemarkcircle.h \
    $$PWD/rectangle.h
