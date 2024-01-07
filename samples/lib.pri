PROJECT_SRC_ROOT = $$PWD/..
PROJECT_BUILD_ROOT = $$OUT_PWD/../..

INCLUDEPATH += \
    $$PROJECT_SRC_ROOT/lib/include/ \
    $$PROJECT_SRC_ROOT/lib/include/QGeoView/

CONFIG(release, debug|release): LIBS += -L$$PROJECT_BUILD_ROOT/lib/release
CONFIG(debug, debug|release): LIBS += -L$$PROJECT_BUILD_ROOT/lib/debug
LIBS += -L$$PROJECT_BUILD_ROOT/lib

LIBS += -lqgeoview
