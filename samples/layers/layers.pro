TARGET = qgeoview-samples-layers
TEMPLATE = app
CONFIG-= console

include(../../lib/lib.pri)
include(../shared/shared.pri)

LIBS += -L$$OUT_PWD/../../lib -lqgeoview

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h
