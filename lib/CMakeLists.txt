set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)

# Set the QT version
find_package(Qt6 COMPONENTS Core QUIET)
if (NOT Qt6_FOUND)
    set(QT_VERSION 5 CACHE STRING "Qt version for QGeoView")
else()
    set(QT_VERSION 6 CACHE STRING "Qt version for QGeoView")
endif()

find_package(Qt${QT_VERSION} REQUIRED COMPONENTS
     Core
     Gui
     Widgets
     Network
)

add_library(qgeoview SHARED
    include/QGeoView/QGVGlobal.h
    include/QGeoView/QGVUtils.h
    include/QGeoView/QGVProjection.h
    include/QGeoView/QGVProjectionEPSG3857.h
    include/QGeoView/QGVCamera.h
    include/QGeoView/QGVMap.h
    include/QGeoView/QGVMapQGItem.h
    include/QGeoView/QGVMapQGView.h
    include/QGeoView/QGVMapRubberBand.h
    include/QGeoView/QGVItem.h
    include/QGeoView/QGVDrawItem.h
    include/QGeoView/QGVLayer.h
    include/QGeoView/QGVImage.h
    include/QGeoView/QGVLayerTiles.h
    include/QGeoView/QGVLayerTilesOnline.h
    include/QGeoView/QGVLayerGoogle.h
    include/QGeoView/QGVLayerBing.h
    include/QGeoView/QGVLayerOSM.h
    include/QGeoView/QGVWidget.h
    include/QGeoView/QGVWidgetCompass.h
    include/QGeoView/QGVWidgetScale.h
    include/QGeoView/QGVWidgetZoom.h
    include/QGeoView/QGVWidgetText.h
    src/QGVUtils.cpp
    src/QGVGlobal.cpp
    src/QGVProjection.cpp
    src/QGVProjectionEPSG3857.cpp
    src/QGVCamera.cpp
    src/QGVMap.cpp
    src/QGVMapQGItem.cpp
    src/QGVMapQGView.cpp
    src/QGVMapRubberBand.cpp
    src/QGVItem.cpp
    src/QGVDrawItem.cpp
    src/QGVLayer.cpp
    src/QGVImage.cpp
    src/QGVLayerTiles.cpp
    src/QGVLayerTilesOnline.cpp
    src/QGVLayerGoogle.cpp
    src/QGVLayerBing.cpp
    src/QGVLayerOSM.cpp
    src/QGVWidget.cpp
    src/QGVWidgetCompass.cpp
    src/QGVWidgetScale.cpp
    src/QGVWidgetZoom.cpp
    src/QGVWidgetText.cpp
)

target_include_directories(qgeoview
    PUBLIC
        include
    PRIVATE
        include/QGeoView
)

target_compile_definitions(qgeoview
    PRIVATE
        QGV_EXPORT
)


target_link_libraries(qgeoview
    PRIVATE
        Qt${QT_VERSION}::Core
        Qt${QT_VERSION}::Gui
        Qt${QT_VERSION}::Widgets
        Qt${QT_VERSION}::Network
)

add_library(QGeoView ALIAS qgeoview)

install(TARGETS qgeoview LIBRARY
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h*"
)
