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

#include <QTimer>

#include <QGeoView/QGVLayerOSM.h>
#include <helpers.h>

MainWindow::MainWindow()
{
    setWindowTitle("QGeoView Samples - basic");

    mMap = new QGVMap(this);
    setCentralWidget(mMap);

    Helpers::setupCachedNetworkAccessManager(this);

    // Background layer
    auto osmLayer = new QGVLayerOSM();
    mMap->addItem(osmLayer);

    // Show whole world
    QTimer::singleShot(100, this, [this]() {
        auto target = mMap->getProjection()->boundaryGeoRect();
        mMap->cameraTo(QGVCameraActions(mMap).scaleTo(target));
    });
}

MainWindow::~MainWindow()
{
}
