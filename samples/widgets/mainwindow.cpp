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

#include <QGeoView/QGVLayerOSM.h>
#include <QGeoView/QGVWidgetCompass.h>
#include <QGeoView/QGVWidgetScale.h>
#include <QGeoView/QGVWidgetZoom.h>
#include <helpers.h>

MainWindow::MainWindow()
{
    setWindowTitle("QGeoView Samples - widgets");

    setCentralWidget(new QWidget());
    centralWidget()->setLayout(new QVBoxLayout());

    Helpers::setupCachedNetworkAccessManager(this);

    mMap = new QGVMap(this);
    centralWidget()->layout()->addWidget(mMap);

    // Background layer
    auto osmLayer = new QGVLayerOSM();
    mMap->addItem(osmLayer);

    // Options list
    centralWidget()->layout()->addWidget(createOptionsList());

    // Show whole world
    QTimer::singleShot(100, this, [this]() {
        auto target = targetArea();
        mMap->cameraTo(QGVCameraActions(mMap).scaleTo(target));
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
    QList<QPair<QString, QGVWidget*>> widgets = {
        { "Compass", new QGVWidgetCompass() },
        { "ZoomButtons", new QGVWidgetZoom() },
        { "ScaleHorizontal", new QGVWidgetScale(Qt::Horizontal) },
        { "ScaleVertical", new QGVWidgetScale(Qt::Vertical) },
    };

    QGroupBox* groupBox = new QGroupBox(tr("Map widgets"));
    groupBox->setLayout(new QVBoxLayout);

    for (auto pair : widgets) {
        auto name = pair.first;
        auto widget = pair.second;

        mMap->addWidget(widget);

        QCheckBox* checkButton = new QCheckBox(name);
        checkButton->setChecked(true);

        connect(checkButton, &QCheckBox::clicked, this, [widget](const bool checked) { widget->setVisible(checked); });

        groupBox->layout()->addWidget(checkButton);
    }

    return groupBox;
}
