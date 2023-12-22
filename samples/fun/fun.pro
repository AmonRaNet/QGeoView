TARGET = qgeoview-samples-fun
TEMPLATE = app
CONFIG-= console

include(../../lib/lib.pri)
include(../shared/shared.pri)

LIBS += -L$$OUT_PWD/../../lib -lqgeoview

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    waveanimation.cpp \
    placemark.cpp

HEADERS += \
    mainwindow.h \
    waveanimation.h \
    placemark.h
