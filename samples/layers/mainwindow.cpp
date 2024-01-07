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
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

#include <helpers.h>
#include <rectangle.h>

#include <QGeoView/QGVLayer.h>
#include <QGeoView/QGVLayerOSM.h>
#include <QGeoView/QGVWidgetCompass.h>
#include <QGeoView/QGVWidgetScale.h>
#include <QGeoView/QGVWidgetZoom.h>

MainWindow::MainWindow()
{
    setWindowTitle("QGeoView Samples - layers");

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

    // Options list
    QWidget* widget = new QWidget();
    widget->setLayout(new QHBoxLayout());
    widget->layout()->addWidget(createOptionsList());
    widget->layout()->addWidget(createLayersList());
    centralWidget()->layout()->addWidget(widget);

    // Show target area
    QTimer::singleShot(100, this, [this]() {
        mMap->cameraTo(QGVCameraActions(mMap).scaleTo(targetArea()));

        addLayer();
        addLayer();
        addLayer();
    });
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
    QGroupBox* groupBox = new QGroupBox(tr("Control"));
    groupBox->setLayout(new QVBoxLayout);

    {
        QPushButton* addButton = new QPushButton("Add layer");
        QPushButton* removeButton = new QPushButton("Remove last layer");
        QWidget* widget = new QWidget();
        widget->setLayout(new QHBoxLayout());
        widget->layout()->addWidget(addButton);
        widget->layout()->addWidget(removeButton);

        groupBox->layout()->addWidget(widget);

        connect(addButton, &QPushButton::clicked, this, &MainWindow::addLayer);
        connect(removeButton, &QPushButton::clicked, this, &MainWindow::removeLayer);
    }

    return groupBox;
}

QGroupBox* MainWindow::createLayersList()
{
    QGroupBox* groupBox = new QGroupBox(tr("List of layers"));
    groupBox->setLayout(new QVBoxLayout);

    mList = new QListWidget();
    groupBox->layout()->addWidget(mList);
    updateListOfLayers();

    return groupBox;
}

void MainWindow::addLayer()
{
    const QGV::GeoRect layerTargetArea = mMap->getProjection()->projToGeo(mMap->getCamera().projRect());
    const Qt::GlobalColor layerColor = static_cast<Qt::GlobalColor>(Qt::red + Helpers::randomInt(0, 10));

    auto* itemsLayer = new QGVLayer();
    itemsLayer->setName("Layer with color " + QVariant::fromValue(layerColor).toString());

    // Add items to custom layer
    for (int i = 0; i < 10; i++) {
        const int size = 50;

        auto item = new Rectangle(Helpers::randRect(mMap, layerTargetArea, size), layerColor);
        item->setFlag(QGV::ItemFlag::IgnoreAzimuth);
        item->setFlag(QGV::ItemFlag::IgnoreScale);

        itemsLayer->addItem(item);
    }

    mMap->addItem(itemsLayer);

    updateListOfLayers();
}

void MainWindow::removeLayer()
{
    if (mMap->countItems() > 1) {
        mMap->removeItem(mMap->getItem(mMap->countItems() - 1));
    }

    updateListOfLayers();
}

void MainWindow::updateListOfLayers()
{
    mList->clear();

    for (int i = 0; i < mMap->countItems(); i++) {
        QGVLayer* layer = dynamic_cast<QGVLayer*>(mMap->getItem(i));
        if (layer == nullptr)
            continue;

        mList->addItem(layer->getName());
    }
}
