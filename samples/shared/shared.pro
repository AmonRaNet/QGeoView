TARGET = qgeoview-samples-shared
TEMPLATE = lib

QT += gui widgets network

include(../../lib/lib.pri)

LIBS += -L$$OUT_PWD/../../lib -lqgeoview

SOURCES += \
    $$PWD/helpers.cpp \
    $$PWD/placemarkcircle.cpp \
    $$PWD/rectangle.cpp

HEADERS += \
    $$PWD/helpers.h \
    $$PWD/placemarkcircle.h \
    $$PWD/rectangle.h

INCLUDEPATH += \
    $$PWD/
