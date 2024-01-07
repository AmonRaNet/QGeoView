PROJECT_SRC_ROOT = $$PWD/..
PROJECT_BUILD_ROOT = $$OUT_PWD/..

INCLUDEPATH += \
    $$PROJECT_SRC_ROOT/samples/shared

CONFIG(release, debug|release): LIBS += -L$$PROJECT_BUILD_ROOT/shared/release
CONFIG(debug, debug|release): LIBS += -L$$PROJECT_BUILD_ROOT/shared/debug
LIBS += -L$$PROJECT_BUILD_ROOT/shared

LIBS += -lqgeoview-samples-shared
