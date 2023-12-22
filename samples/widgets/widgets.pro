TARGET = qgeoview-samples-widgets
TEMPLATE = app
CONFIG-= console

include(../../lib/lib.pri)

LIBS += -L$$OUT_PWD/../../lib -lqgeoview

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h
