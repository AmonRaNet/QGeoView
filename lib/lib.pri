QT += gui widgets network

DEFINES += QGV_EXPORT

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
    $$PWD/include/QGeoView/QGVLayerBDGEx.h \
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
