/***************************************************************************
 * QGeoView is a Qt / C ++ widget for visualizing geographic data.
 * Copyright (C) 2018-2025 Andrey Yaroshenko.
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

#include <QGeoView/QGVLayerOSM.h>
#include <QTimer>

#include <helpers.h>
#include <mytiles.h>

MainWindow::MainWindow()
{
    setWindowTitle("QGeoView Samples - custom tiles");

    mMap = new QGVMap(this);
    setCentralWidget(mMap);

    Helpers::setupCachedNetworkAccessManager(this);

    // Background layer
    auto osmLayer = new QGVLayerOSM();
    mMap->addItem(osmLayer);

    // Custom tile layer
    auto myTiles = new MyTiles(targetArea(), Qt::blue);
    mMap->addItem(myTiles);

    // Show whole world
    auto target = mMap->getProjection()->boundaryGeoRect();
    mMap->cameraTo(QGVCameraActions(mMap).scaleTo(target));

    // Show target area
    QTimer::singleShot(100, this, [this]() {
        auto target = targetArea();
        mMap->flyTo(QGVCameraActions(mMap).scaleTo(target));
    });
}

MainWindow::~MainWindow()
{
}

QGV::GeoRect MainWindow::targetArea() const
{
    return QGV::GeoRect(QGV::GeoPos(48.406227, 9.731185), QGV::GeoPos(47.829682, 11.25));
}
