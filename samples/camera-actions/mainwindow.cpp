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
#include <QPushButton>
#include <QRandomGenerator>
#include <QTimer>
#include <QVBoxLayout>

#include <helpers.h>
#include <rectangle.h>

#include <QGeoView/QGVLayerOSM.h>
#include <QGeoView/QGVWidgetCompass.h>
#include <QGeoView/QGVWidgetScale.h>
#include <QGeoView/QGVWidgetText.h>
#include <QGeoView/QGVWidgetZoom.h>

MainWindow::MainWindow()
{
    setWindowTitle("QGeoView Samples - camera actions");

    setCentralWidget(new QWidget());
    centralWidget()->setLayout(new QVBoxLayout());

    Helpers::setupCachedNetworkAccessManager(this);

    mMap = new QGVMap(this);
    centralWidget()->layout()->addWidget(mMap);

    // Widgets
    mMap->addWidget(new QGVWidgetCompass());
    mMap->addWidget(new QGVWidgetZoom());

    // Background layer
    auto osmLayer = new QGVLayerOSM();
    mMap->addItem(osmLayer);

    // Custom layer
    mItemsLayer = new QGVLayer();
    mMap->addItem(mItemsLayer);

    // Add items to custom layer
    const QGV::GeoRect wholeWorld{ -70, -180, 70, 180 };
    for (int i = 0; i < 50; i++) {
        const int size = 20;
        auto item = new Rectangle(Helpers::randRect(mMap, wholeWorld, size), Qt::red);
        item->setFlags(QGV::ItemFlag::IgnoreScale | QGV::ItemFlag::IgnoreAzimuth);
        item->setSelectable(true);
        mItemsLayer->addItem(item);
    }

    // Options list
    centralWidget()->layout()->addWidget(createOptionsList());

    // Show target area
    QTimer::singleShot(100, this, [this]() {
        auto target = mMap->getProjection()->boundaryGeoRect();
        mMap->cameraTo(QGVCameraActions(mMap).scaleTo(target));
    });
}

MainWindow::~MainWindow()
{
}

QGroupBox* MainWindow::createOptionsList()
{
    const QList<QPair<QString, std::function<void()>>> actions = {
        { "Move camera to item position", [this]() { moveToItem(); } },
        { "Scale camera to show item area", [this]() { scaleToItem(); } },
        { "Zoom +2x", [this]() { zoomBy(2.0); } },
        { "Zoom -2x", [this]() { zoomBy(0.5); } },
        { "Rotate +90 degree", [this]() { rotateBy(90); } },
        { "Rotate -90 degree", [this]() { rotateBy(-90); } },
        { "Rotate to north", [this]() { rotateNorth(); } },
    };

    QGroupBox* groupBox = new QGroupBox(tr("Camera options"));
    groupBox->setLayout(new QVBoxLayout);

    QCheckBox* checkButton = new QCheckBox("Animate camera action");
    connect(checkButton, &QCheckBox::toggled, this, [this](bool checked) { setFlyMode(checked); });
    groupBox->layout()->addWidget(checkButton);
    checkButton->setChecked(true);

    for (auto action : actions) {
        auto name = action.first;
        auto actionFunc = action.second;

        QPushButton* button = new QPushButton(name);
        connect(button, &QPushButton::clicked, this, actionFunc);
        groupBox->layout()->addWidget(button);
    }

    return groupBox;
}

void MainWindow::moveToItem()
{
    auto item = getRandomItem();
    auto action = QGVCameraActions(mMap).moveTo(item->getRect().topLeft());

    if (!mFlyMode)
        mMap->cameraTo(action);
    else
        mMap->flyTo(action);
}

void MainWindow::scaleToItem()
{
    auto item = getRandomItem();
    auto geoRect = item->getRect();
    auto scaleRect =
            QGV::GeoRect{ geoRect.latTop() - 1, geoRect.lonLeft() - 1, geoRect.latTop() + 1, geoRect.lonLeft() + 1 };

    auto action = QGVCameraActions(mMap).scaleTo(scaleRect);

    if (!mFlyMode)
        mMap->cameraTo(action);
    else
        mMap->flyTo(action);
}

void MainWindow::zoomBy(double factor)
{
    auto action = QGVCameraActions(mMap).scaleBy(factor);
    if (!mFlyMode)
        mMap->cameraTo(action);
    else
        mMap->flyTo(action);
}

void MainWindow::rotateBy(double degree)
{
    auto action = QGVCameraActions(mMap).rotateBy(degree);

    if (!mFlyMode)
        mMap->cameraTo(action);
    else
        mMap->flyTo(action);
}

void MainWindow::rotateNorth()
{
    auto action = QGVCameraActions(mMap).rotateTo(0.0);

    if (!mFlyMode)
        mMap->cameraTo(action);
    else
        mMap->flyTo(action);
}

void MainWindow::setFlyMode(bool enabled)
{
    mFlyMode = enabled;
}

Rectangle* MainWindow::getRandomItem()
{
    quint32 index = QRandomGenerator::global()->bounded(0, mItemsLayer->countItems());
    return dynamic_cast<Rectangle*>(mItemsLayer->getItem(index));
}
