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
    setWindowTitle("QGeoView Samples - layers");

    setCentralWidget(new QWidget());
    centralWidget()->setLayout(new QVBoxLayout());

    QGV::setNetworkManager(new QNetworkAccessManager());

    mMap = new QGVMap(this);
    centralWidget()->layout()->addWidget(mMap);

    // Widgets
    mMap->addWidget(new QGVWidgetCompass());
    mMap->addWidget(new QGVWidgetZoom());

    // Background layer
    auto osmLayer = new QGVLayerOSM();
    mMap->addItem(osmLayer);

    // Add layers
    for (int l = 0; l < 3; l++) {
        auto* itemsLayer = new QGVLayer();
        mMap->addItem(itemsLayer);

        // Add items to custom layer
        for (int i = 0; i < 10; i++) {
            const int size = 50;

            auto item = new Rectangle(Helpers::randRect(mMap, targetArea(), size),
                                      static_cast<Qt::GlobalColor>(Qt::red + l));
            item->setFlag(QGV::ItemFlag::IgnoreAzimuth);
            item->setFlag(QGV::ItemFlag::IgnoreScale);

            itemsLayer->addItem(item);
        }
    }

    // Options list
    centralWidget()->layout()->addWidget(createLayersList());

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

QGroupBox* MainWindow::createLayersList()
{
    const QList<QPair<QString, QGVItem*>> layers = {
        { "Layer 1", mMap->getItem(1) },
        { "Layer 2", mMap->getItem(2) },
        { "Layer 3", mMap->getItem(3) },
    };

    QGroupBox* groupBox = new QGroupBox(tr("Layers"));
    groupBox->setLayout(new QVBoxLayout);

    for (auto pair : layers) {
        auto name = pair.first;
        auto layer = pair.second;

        QCheckBox* checkButton = new QCheckBox(name);
        groupBox->layout()->addWidget(checkButton);

        connect(checkButton, &QCheckBox::toggled, this, [layer, layers](const bool checked) {
            layer->setVisible(checked);
        });

        checkButton->setChecked(true);
    }

    return groupBox;
}
