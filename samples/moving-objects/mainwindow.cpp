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

#include <QRandomGenerator>
#include <QTimer>

#include <helpers.h>
#include <placemarkcircle.h>

#include <QGeoView/QGVLayerOSM.h>
#include <QGeoView/QGVMapQGView.h>

MainWindow::MainWindow()
{
    setWindowTitle("QGeoView Samples - moving objects");

    mMap = new QGVMap(this);
    setCentralWidget(mMap);

    // fix leaving residuals when dragging the map rapidly
    mMap->geoView()->setViewportUpdateMode(QGraphicsView::ViewportUpdateMode::FullViewportUpdate);
    mMap->geoView()->setCacheMode(QGraphicsView::CacheModeFlag::CacheBackground);

    Helpers::setupCachedNetworkAccessManager(this);

    // Background layer
    auto osmLayer = new QGVLayerOSM();
    mMap->addItem(osmLayer);

    // Custom layer
    auto customLayer = new QGVLayer();
    mMap->addItem(customLayer);

    // Add moving item in custom layer
    auto item = new PlacemarkCircle(QGV::GeoPos(10, 20), 30, Qt::red);
    customLayer->addItem(item);

    // Moving timer
    mTimer = new QTimer();
    mTimer->setInterval(100);
    mTimer->setSingleShot(false);
    connect(mTimer, &QTimer::timeout, this, [this, item]() { moveObject(item); });
    mTimer->start();

    // Show whole world
    QTimer::singleShot(100, this, [this]() {
        auto target = mMap->getProjection()->boundaryGeoRect();
        mMap->cameraTo(QGVCameraActions(mMap).scaleTo(target));
    });
}

MainWindow::~MainWindow()
{
}

void MainWindow::moveObject(PlacemarkCircle* item)
{
    static double deltaLat = 0;
    static double deltaLon = 0;

    const QGV::GeoPos curPos = item->getCenter();

    if (curPos.latitude() > 50) {
        deltaLat = -1;
        deltaLon = 0;
    }
    if (curPos.latitude() < -50) {
        deltaLat = 1;
        deltaLon = 0;
    }
    if (curPos.longitude() > 50) {
        deltaLat = 0;
        deltaLon = -1;
    }
    if (curPos.longitude() < -50) {
        deltaLat = 0;
        deltaLon = +1;
    }
    if (deltaLat == 0) {
        deltaLat = QRandomGenerator::global()->generate() % 2 == 0 ? 2 : -2;
    }
    if (deltaLon == 0) {
        deltaLon = QRandomGenerator::global()->generate() % 2 == 0 ? 2 : -2;
    }

    const QGV::GeoPos newPos = QGV::GeoPos(curPos.latitude() + deltaLat, curPos.longitude() + deltaLon);
    item->setCenter(newPos);
}
