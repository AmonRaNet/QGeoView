# QGeoView(QGV)

QGeoView is a Qt / C ++ widget for visualizing geographic data, like tiled layers, raster or vector data. QGeoView has only Qt as dependency. This component uses QGraphicsView and focuses on usability and performance.
QGeoView public interface follows best practices from OpenLayers and Google Maps API and supports all basic map features, like:

 * EPSG3857 projection (others can be added)
 * Camera changes: scaling, rotation, moving
 * TMS (OSM, Google, Bing or custom)
 * Map control widgets

QGeoView uses a hierarchical object storage system, which can store  both visual objects (for example, raster or shapes) and non-visual objects (for example, a layer of items). Override in paint event for the visual object allow you to show on the map any geo-data from any source. However, QGeoView itself does not provide methods for reading/parsing of geo-data sources (files). The only exception are TMS servers and bitmap images, for all other sources you need to extend solution.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

What things you need:

 * C++11 compiler (GCC, Clang, MinGW,...)
 * Qt 5.6 or higher (core, gui, widgets, network)
 * qmake or cmake
 * doxygen (optional)
 * pre-commits (optional) https://pre-commit.com/

### Installing

If you use qmake

```
cd <build-dir>
qmake <source-dir>/QGeoView.pro
make
make install
```

If you use cmake

```
cd <build-dir>
cmake <source-dir>
cmake --build . --config Release --target install
```

If you use doxygen (documentation)

```
cd <source-dir>
doxygen
```

### Demo

You can see here small video: [QGeoView Demo](https://youtu.be/t0D21r_s-8E)
or use demo example to see all QGeoView features:

```
<build-dir>/qgeoview-demo
```

## Versioning

QGeoView uses [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/AmonRaNet/QGeoView/tags).

## Author

**Andrey Yaroshenko** ([AmonRaNet](https://github.com/AmonRaNet))

## License

This project is licensed under the [LGPL License v3](http://www.gnu.org/licenses/).

Please contact the author if you need development with closed source.
