TARGET = qgeoview-samples-custom-tiles
TEMPLATE = app
CONFIG-= console

include(../../lib/lib.pri)

LIBS += -L$$OUT_PWD/../../lib -lqgeoview

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mytile.cpp \
    mytiles.cpp

HEADERS += \
    mainwindow.h \
    mytile.h \
    mytiles.h
