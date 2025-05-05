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

#include <QCheckBox>
#include <QTimer>
#include <QVBoxLayout>

#include <helpers.h>
#include <rectangle.h>

#include <QGeoView/QGVLayerOSM.h>
#include <QGeoView/QGVWidgetCompass.h>
#include <QGeoView/QGVWidgetScale.h>
#include <QGeoView/QGVWidgetZoom.h>

MainWindow::MainWindow()
{
    setWindowTitle("QGeoView Samples - debug output");

    setCentralWidget(new QWidget());
    centralWidget()->setLayout(new QVBoxLayout());

    Helpers::setupCachedNetworkAccessManager(this);

    mMap = new QGVMap(this);
    centralWidget()->layout()->addWidget(mMap);

    // Widgets
    mMap->addWidget(new QGVWidgetCompass());
    mMap->addWidget(new QGVWidgetZoom());
    mMap->addWidget(new QGVWidgetScale());

    // Background layer
    auto osmLayer = new QGVLayerOSM();
    mMap->addItem(osmLayer);

    // Add layers
    for (int l = 0; l < 3; l++) {
        auto* itemsLayer = new QGVLayer();
        mMap->addItem(itemsLayer);

        // Add items to custom layer
        for (int i = 0; i < 10; i++) {
            const int size = 1000;

            auto item = new Rectangle(Helpers::randRect(mMap, targetArea(), size),
                                      static_cast<Qt::GlobalColor>(Qt::red + l));

            item->setProperty("drawDebug", QString("My index is %1").arg(i));

            itemsLayer->addItem(item);
        }
    }

    // Options list
    centralWidget()->layout()->addWidget(createOptionsList());

    // Show target area
    QTimer::singleShot(100, this, [this]() { mMap->cameraTo(QGVCameraActions(mMap).scaleTo(targetArea())); });
}

MainWindow::~MainWindow()
{
}

QGV::GeoRect MainWindow::targetArea() const
{
    return QGV::GeoRect(QGV::GeoPos(51.848624, 14.7), QGV::GeoPos(51.743758, 14.9));
}

QGroupBox* MainWindow::createOptionsList()
{
    QGroupBox* groupBox = new QGroupBox(tr("Options"));
    groupBox->setLayout(new QVBoxLayout);

    {
        QCheckBox* checkButton = new QCheckBox("Draw debug info on map");
        groupBox->layout()->addWidget(checkButton);

        connect(checkButton, &QCheckBox::toggled, this, [this](const bool checked) {
            QGV::setDrawDebug(checked);
            mMap->refreshMap();
        });

        checkButton->setChecked(true);
    }

    {
        QCheckBox* checkButton = new QCheckBox("Print debug to QDebug");
        groupBox->layout()->addWidget(checkButton);

        connect(checkButton, &QCheckBox::toggled, this, [this](const bool checked) { QGV::setPrintDebug(checked); });

        checkButton->setChecked(true);
    }

    return groupBox;
}
