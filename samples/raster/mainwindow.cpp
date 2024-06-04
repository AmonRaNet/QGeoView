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
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

#include <helpers.h>
#include <rectangle.h>

#include <QGeoView/QGVLayerOSM.h>
#include <QGeoView/QGVWidgetCompass.h>
#include <QGeoView/QGVWidgetScale.h>
#include <QGeoView/QGVWidgetZoom.h>
#include <QGeoView/Raster/QGVIcon.h>
#include <QGeoView/Raster/QGVImage.h>

MainWindow::MainWindow()
{
    setWindowTitle("QGeoView Samples - raster");

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

    // Raster layer
    mLayer = new QGVLayer();
    mMap->addItem(mLayer);

    // Options list
    QWidget* widget = new QWidget();
    widget->setLayout(new QHBoxLayout());
    widget->layout()->addWidget(createOptionsList());
    widget->layout()->addWidget(createItemsList());
    centralWidget()->layout()->addWidget(widget);

    // Show target area
    QTimer::singleShot(100, this, [this]() { mMap->cameraTo(QGVCameraActions(mMap).scaleTo(targetArea())); });

    // Pre-load image sources
    preloadImages();
}

MainWindow::~MainWindow()
{
}

QGV::GeoRect MainWindow::targetArea() const
{
    return QGV::GeoRect(QGV::GeoPos(50, 14), QGV::GeoPos(52, 15));
}

QGroupBox* MainWindow::createOptionsList()
{
    QGroupBox* groupBox = new QGroupBox(tr("Raster"));
    groupBox->setLayout(new QVBoxLayout);

    {
        QPushButton* button = new QPushButton("Add image");
        groupBox->layout()->addWidget(button);

        connect(button, &QPushButton::clicked, this, &MainWindow::addImage);
    }

    {
        QPushButton* button = new QPushButton("Add icon");
        groupBox->layout()->addWidget(button);

        connect(button, &QPushButton::clicked, this, &MainWindow::addIcon);
    }

    {
        QPushButton* button = new QPushButton("Remove last");
        groupBox->layout()->addWidget(button);

        connect(button, &QPushButton::clicked, this, &MainWindow::removeLast);
    }

    return groupBox;
}

QGroupBox* MainWindow::createItemsList()
{
    QGroupBox* groupBox = new QGroupBox(tr("List of items"));
    groupBox->setLayout(new QVBoxLayout);

    mList = new QListWidget();
    groupBox->layout()->addWidget(mList);
    updateListOfItems();

    return groupBox;
}

void MainWindow::preloadImages()
{
    loadImage(mImage,
              QUrl{ "https://upload.wikimedia.org/wikipedia/commons/thumb/7/79/Face-smile.svg/"
                    "1024px-Face-smile.svg.png" });
    loadImage(mIcon, QUrl{ "http://maps.google.com/mapfiles/kml/paddle/red-circle.png" });
}

void MainWindow::loadImage(QImage& dest, QUrl url)
{
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent",
                         "Mozilla/5.0 (Windows; U; MSIE "
                         "6.0; Windows NT 5.1; SV1; .NET "
                         "CLR 2.0.50727)");
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);

    QNetworkReply* reply = QGV::getNetworkManager()->get(request);
    connect(reply, &QNetworkReply::finished, reply, [reply, &dest]() {
        if (reply->error() != QNetworkReply::NoError) {
            qgvCritical() << "ERROR" << reply->errorString();
            reply->deleteLater();
            return;
        }
        dest.loadFromData(reply->readAll());
        reply->deleteLater();
    });

    qgvDebug() << "request" << url;
}

void MainWindow::addImage()
{
    const QGV::GeoRect itemTargetArea = mMap->getProjection()->projToGeo(mMap->getCamera().projRect());

    auto* item = new QGVImage();
    item->setGeometry(Helpers::randRect(mMap, itemTargetArea));
    item->loadImage(mImage);

    mLayer->addItem(item);

    updateListOfItems();
}

void MainWindow::addIcon()
{
    const QGV::GeoRect itemTargetArea = mMap->getProjection()->projToGeo(mMap->getCamera().projRect());

    auto* item = new QGVIcon();
    item->setGeometry(Helpers::randPos(itemTargetArea));
    item->loadImage(mIcon);

    mLayer->addItem(item);

    updateListOfItems();
}

void MainWindow::removeLast()
{
    if (mLayer->countItems() == 0) {
        return;
    }

    mLayer->removeItem(mLayer->getItem(mLayer->countItems() - 1));

    updateListOfItems();
}

void MainWindow::updateListOfItems()
{
    mList->clear();

    for (int i = 0; i < mLayer->countItems(); i++) {
        QGVItem* item = mLayer->getItem(i);
        mList->addItem(item->metaObject()->className());
    }
}
