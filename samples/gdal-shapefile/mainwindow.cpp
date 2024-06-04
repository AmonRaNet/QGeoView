/***************************************************************************
 * QGeoView is a Qt / C ++ widget for visualizing geographic data.
 * Copyright (C) 2018-2023 Andrey Yaroshenko.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, see https://www.gnu.org/licenses.
 ****************************************************************************/

#include "mainwindow.h"

#include <QTimer>
#include <QApplication>
#include <QDir>

#include <QGeoView/QGVLayerOSM.h>
#include <helpers.h>
#include "polygon.h"

#include "cpl_conv.h"
#include "ogrsf_frmts.h"

QList<QGV::GeoPos> convert(OGRPolygon* poPolygon)
{
    OGRPoint ptTemp;
    QList<QGV::GeoPos> result;
    OGRLinearRing* poExteriorRing = poPolygon->getExteriorRing();
    int NumberOfExteriorRingVertices = poExteriorRing->getNumPoints();
    for (int k = 0; k < NumberOfExteriorRingVertices; k++) {
        poExteriorRing->getPoint(k, &ptTemp);
        result << QGV::GeoPos(ptTemp.getY(), ptTemp.getX());
    }
    return result;
}

MainWindow::MainWindow()
{
    setWindowTitle("QGeoView Samples - Shapefile");

    mMap = new QGVMap(this);
    setCentralWidget(mMap);

    Helpers::setupCachedNetworkAccessManager(this);

    // Background layer
    auto osmLayer = new QGVLayerOSM();
    mMap->addItem(osmLayer);

    // Please adapt the following two points to your system:
    //  1. GDAL data path
    QDir gdalData(QApplication::applicationDirPath());
    gdalData.cd("gdal");
    //  2. PROJ data path
    QDir projData(gdalData);

    // GDAL configuration (https://gdal.org/user/configoptions.html)
    std::string GDAL_DATA = QDir::toNativeSeparators(gdalData.absolutePath()).toStdString();
    CPLSetConfigOption("GDAL_DATA", GDAL_DATA.c_str());
    std::string PROJ_DATA = QDir::toNativeSeparators(projData.absolutePath()).toStdString();
    CPLSetConfigOption("PROJ_DATA", PROJ_DATA.c_str());

    GDALAllRegister(); // Load GDAL drivers
    GDALDataset *poDS = static_cast<GDALDataset*>(GDALOpenEx("countries.shp", GDAL_OF_VECTOR, NULL, NULL, NULL)); // Open vector file
    if (poDS == NULL) {
        printf("Open failed.\n");
        exit(1);
    }

    for (int iLayer = 0; iLayer < poDS->GetLayerCount(); iLayer++)
    {
        OGRLayer* poLayer = poDS->GetLayer(iLayer);
        OGRFeatureDefn* poFDefn = poLayer->GetLayerDefn();
        poLayer->ResetReading();
        OGRFeature* poFeature;
        while ((poFeature = poLayer->GetNextFeature()) != NULL) {
            OGRGeometry* poGeometry = poFeature->GetGeometryRef();
            auto type = wkbFlatten(poGeometry->getGeometryType());
            if (poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbPolygon) {
                OGRPolygon* poPolygon = (OGRPolygon*)poGeometry;
                if (poPolygon->IsValid()) {
                    QList<QGV::GeoPos> points = convert(poPolygon);
                    if (points.count() > 2)
                        mMap->addItem(new Polygon(points, Qt::red, Qt::blue));
                }
            } else if (poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbMultiPolygon) {
                OGRMultiPolygon* poMultiPolygon = (OGRMultiPolygon*)poGeometry;
                for (int i = 0; i < poMultiPolygon->getNumGeometries(); i++) {
                    OGRPolygon* poPolygon = (OGRPolygon*)poMultiPolygon->getGeometryRef(i);
                    if (poPolygon->IsValid()) {
                        QList<QGV::GeoPos> points = convert(poPolygon);
                        if (points.count() > 2)
                            mMap->addItem(new Polygon(points, Qt::red, Qt::blue));
                    }
                }
            } else {
                printf("no polygon geometry\n");
            }
            OGRFeature::DestroyFeature(poFeature);
        }
    }
    GDALClose(poDS);

    // Show whole world
    QTimer::singleShot(100, this, [this]() {
        auto target = mMap->getProjection()->boundaryGeoRect();
        mMap->cameraTo(QGVCameraActions(mMap).scaleTo(target));
    });
}

MainWindow::~MainWindow()
{
}
