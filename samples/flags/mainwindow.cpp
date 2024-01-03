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
    setWindowTitle("QGeoView Samples - flags");

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
    for (int i = 0; i < 10; i++) {
        const int size = 50;
        auto item = new Rectangle(Helpers::randRect(mMap, targetArea(), size), Qt::red);
        mItemsLayer->addItem(item);
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
    const QList<QPair<QString, std::function<void(bool)>>> actions = {
        { "Item will ignore map zoom/scale", [this](bool enabled) { applyFlag(enabled, QGV::ItemFlag::IgnoreScale); } },
        { "Item will ignore map rotation/azimuth",
          [this](bool enabled) { applyFlag(enabled, QGV::ItemFlag::IgnoreAzimuth); } },
        { "Item is highlight-able (mouse hover)",
          [this](bool enabled) { applyFlag(enabled, QGV::ItemFlag::Highlightable); } },
        { "Item is highlight-able with custom indication",
          [this](bool enabled) { applyFlag(enabled, QGV::ItemFlag::HighlightCustom); } },
        { "Item is selectable (single LButton or Shift/Ctrl + RButton hold + move)",
          [this](bool enabled) { applySelectable(enabled); } },
        { "Item is selectable with custom indication",
          [this](bool enabled) { applyFlag(enabled, QGV::ItemFlag::SelectCustom); } },
        { "Item is movable (Alt + LButton hold + move)",
          [this](bool enabled) { applyFlag(enabled, QGV::ItemFlag::Movable); } },
        { "Item is click-able by mouse (single LButton, double LButton)",
          [this](bool enabled) { applyFlag(enabled, QGV::ItemFlag::Clickable); } },
        { "Enable custom transform", [this](bool enabled) { applyFlag(enabled, QGV::ItemFlag::Transformed); } },
    };

    QGroupBox* groupBox = new QGroupBox(tr("Item flags and options"));
    groupBox->setLayout(new QVBoxLayout);

    for (auto action : actions) {
        auto name = action.first;
        auto actionFunc = action.second;

        QCheckBox* checkButton = new QCheckBox(name);
        connect(checkButton, &QCheckBox::toggled, this, actionFunc);
        groupBox->layout()->addWidget(checkButton);

        checkButton->setChecked(true);
    }

    return groupBox;
}

void MainWindow::applyFlag(bool enabled, QGV::ItemFlag value)
{
    for (int i = 0; i < mItemsLayer->countItems(); i++) {
        QGVDrawItem* item = dynamic_cast<QGVDrawItem*>(mItemsLayer->getItem(i));
        item->setFlag(value, enabled);
    }
}

void MainWindow::applySelectable(bool enabled)
{
    for (int i = 0; i < mItemsLayer->countItems(); i++) {
        QGVDrawItem* item = dynamic_cast<QGVDrawItem*>(mItemsLayer->getItem(i));
        item->setSelectable(enabled);
    }
}
