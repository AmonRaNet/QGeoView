TARGET = qgeoview-demo
TEMPLATE = app
CONFIG-= console

include(../lib/lib.pri)

SOURCES += \
    demoitem.cpp \
    main.cpp \
    mainwindow.cpp \
    selectordialog.cpp \
    samples/background.cpp \
    samples/customtiles.cpp \
    samples/ellipse.cpp \
    samples/flags.cpp \
    samples/items.cpp \
    samples/mouse.cpp \
    samples/mytiles.cpp \
    samples/mytile.cpp \
    samples/placemark.cpp \
    samples/rectangle.cpp \
    samples/utilities.cpp \
    samples/waveanimation.cpp \
    samples/widgets.cpp

HEADERS += \
    demoitem.h \
    mainwindow.h \
    selectordialog.h \
    samples/background.h \
    samples/customtiles.h \
    samples/ellipse.h \
    samples/flags.h \
    samples/items.h \
    samples/mouse.h \
    samples/mytiles.h \
    samples/mytile.h \
    samples/placemark.h \
    samples/rectangle.h \
    samples/utilities.h \
    samples/waveanimation.h \
    samples/widgets.h

FORMS   += \
    mainwindow.ui
