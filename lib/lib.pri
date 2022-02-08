QT += gui widgets network

DEFINES += QGV_EXPORT

SOURCES += \
    $$PWD/src/QGVCamera.cpp \
    $$PWD/src/QGVDrawItem.cpp \
    $$PWD/src/QGVGlobal.cpp \
    $$PWD/src/QGVUtils.cpp \
    $$PWD/src/QGVImage.cpp \
    $$PWD/src/QGVItem.cpp \
    $$PWD/src/QGVLayer.cpp \
    $$PWD/src/QGVLayerBing.cpp \
    $$PWD/src/QGVLayerGoogle.cpp \
    $$PWD/src/QGVLayerOSM.cpp \
    $$PWD/src/QGVLayerTiles.cpp \
    $$PWD/src/QGVLayerTilesOnline.cpp \
    $$PWD/src/QGVMap.cpp \
    $$PWD/src/QGVMapQGItem.cpp \
    $$PWD/src/QGVMapQGView.cpp \
    $$PWD/src/QGVMapRubberBand.cpp \
    $$PWD/src/QGVProjection.cpp \
    $$PWD/src/QGVProjectionEPSG3857.cpp \
    $$PWD/src/QGVWidget.cpp \
    $$PWD/src/QGVWidgetCompass.cpp \
    $$PWD/src/QGVWidgetScale.cpp \
    $$PWD/src/QGVWidgetText.cpp \
    $$PWD/src/QGVWidgetZoom.cpp

HEADERS += \
    $$PWD/include/QGeoView/QGVCamera.h \
    $$PWD/include/QGeoView/QGVDrawItem.h \
    $$PWD/include/QGeoView/QGVGlobal.h \
    $$PWD/include/QGeoView/QGVUtils.h \
    $$PWD/include/QGeoView/QGVImage.h \
    $$PWD/include/QGeoView/QGVItem.h \
    $$PWD/include/QGeoView/QGVLayer.h \
    $$PWD/include/QGeoView/QGVLayerBing.h \
    $$PWD/include/QGeoView/QGVLayerGoogle.h \
    $$PWD/include/QGeoView/QGVLayerOSM.h \
    $$PWD/include/QGeoView/QGVLayerTiles.h \
    $$PWD/include/QGeoView/QGVLayerTilesOnline.h \
    $$PWD/include/QGeoView/QGVMap.h \
    $$PWD/include/QGeoView/QGVMapQGItem.h \
    $$PWD/include/QGeoView/QGVMapQGView.h \
    $$PWD/include/QGeoView/QGVMapRubberBand.h \
    $$PWD/include/QGeoView/QGVProjection.h \
    $$PWD/include/QGeoView/QGVProjectionEPSG3857.h \
    $$PWD/include/QGeoView/QGVWidget.h \
    $$PWD/include/QGeoView/QGVWidgetCompass.h \
    $$PWD/include/QGeoView/QGVWidgetScale.h \
    $$PWD/include/QGeoView/QGVWidgetText.h \
    $$PWD/include/QGeoView/QGVWidgetZoom.h

INCLUDEPATH += \
    $$PWD/include/ \
    $$PWD/include/QGeoView/
