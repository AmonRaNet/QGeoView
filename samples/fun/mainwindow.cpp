/***************************************************************************
 * QGeoView is a Qt / C ++ widget for visualizing geographic data.
 * Copyright (C) 2018-2024 Andrey Yaroshenko.
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
#include <QPainter>
#include <QTimer>
#include <QVBoxLayout>

#include <functional>

#include <helpers.h>
#include <placemark.h>
#include <waveanimation.h>

#include <QGeoView/QGVLayerOSM.h>
#include <QGeoView/QGVWidgetCompass.h>
#include <QGeoView/QGVWidgetScale.h>
#include <QGeoView/QGVWidgetZoom.h>

MainWindow::MainWindow()
{
    setWindowTitle("QGeoView Samples - fun");

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

    // QGV layer
    mQGVLayer = createQGVLayer();
    mMap->addItem(mQGVLayer);
    mQGVAnimation = createQGVAnimation();

    // Options list
    centralWidget()->layout()->addWidget(createOptionsList());

    // Show whole world
    auto target = mMap->getProjection()->boundaryGeoRect();
    mMap->cameraTo(QGVCameraActions(mMap).scaleTo(target));

    // Show target area
    QTimer::singleShot(100, this, [this]() { mMap->flyTo(QGVCameraActions(mMap).scaleTo(targetQGVArea())); });
}

MainWindow::~MainWindow()
{
}

QGV::GeoRect MainWindow::targetQGVArea() const
{
    return QGV::GeoRect(QGV::GeoPos(55.096754, 82.682533), QGV::GeoPos(54.862608, 83.138179));
}

QPixmap MainWindow::createQGVImage() const
{
    const auto target = QSize(150, 150);
    const auto path = QGV::createTextPath(QRect(QPoint(0, 0), target), "QGeoView", QFont(), 1);
    QImage image(target, QImage::Format_ARGB32_Premultiplied);
    image.fill(qRgba(0, 0, 0, 0));
    QPixmap pixmap = QPixmap::fromImage(image, Qt::NoFormatConversion);
    QPainter painter(&pixmap);
    QPen pen = QPen(Qt::black);
    pen.setWidth(1);
    pen.setCosmetic(true);
    painter.setPen(pen);
    QBrush brush = QBrush(Qt::black);
    painter.setBrush(brush);
    painter.drawPath(path);
    return pixmap;
}

QGVLayer* MainWindow::createQGVLayer() const
{
    const auto target = targetQGVArea();
    const auto sizeLon = target.lonLeft() - target.lonRigth();
    const auto sizeLat = target.latTop() - target.latBottom();

    /*
     * Layers owned by map.
     */
    auto layer = new QGVLayer();
    layer->setName("QGV");
    layer->setDescription("Demo for QGV");

    /*
     * Items will be owned by layer.
     */
    QImage image = createQGVImage().toImage();
    for (int x = 0; x < image.width(); x++) {
        for (int y = 0; y < image.height(); y++) {
            if (image.pixelColor(x, y) != Qt::black)
                continue;
            double xScale = (double)x / image.width();
            double yScale = (double)y / image.height();
            QGV::GeoPos geoPos = QGV::GeoPos(target.latTop() - sizeLat * yScale, target.lonLeft() - sizeLon * xScale);
            auto image = new Placemark(geoPos);
            layer->addItem(image);
        }
    }

    qInfo() << layer->countItems() << "items in QGV demo layer";
    return layer;
}

QAbstractAnimation* MainWindow::createQGVAnimation() const
{
    const double w = mMap->getProjection()->geoToProj(targetQGVArea()).width();
    auto animation = new WaveAnimation(mQGVLayer, w / 20, 3000);
    animation->setLoopCount(-1);
    return animation;
}

QGroupBox* MainWindow::createOptionsList()
{
    QGroupBox* groupBox = new QGroupBox(tr("Options"));
    groupBox->setLayout(new QVBoxLayout);

    {
        QCheckBox* checkButton = new QCheckBox("QGV layer");
        groupBox->layout()->addWidget(checkButton);

        connect(checkButton, &QCheckBox::toggled, this, [this](const bool checked) { mQGVLayer->setVisible(checked); });

        checkButton->setChecked(true);
    }

    {
        QCheckBox* checkButton = new QCheckBox("Animate");
        groupBox->layout()->addWidget(checkButton);

        connect(checkButton, &QCheckBox::toggled, this, [this](const bool checked) {
            if (checked) {
                mQGVAnimation->start();
            } else {
                mQGVAnimation->stop();
            }
        });

        checkButton->setChecked(true);
    }

    return groupBox;
}
