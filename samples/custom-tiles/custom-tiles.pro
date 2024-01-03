TARGET = qgeoview-samples-custom-tiles
TEMPLATE = app
CONFIG-= console

include(../../lib/lib.pri)
include(../shared/shared.pri)

LIBS += -L$$OUT_PWD/../../lib -lqgeoview
LIBS += -L$$OUT_PWD/../shared -lqgeoview-samples-shared

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mytile.cpp \
    mytiles.cpp

HEADERS += \
    mainwindow.h \
    mytile.h \
    mytiles.h
