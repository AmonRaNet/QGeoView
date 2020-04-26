/***************************************************************************
 * QGeoView is a Qt / C ++ widget for visualizing geographic data.
 * Copyright (C) 2018-2020 Andrey Yaroshenko.
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
#include "samples/background.h"
#include "samples/customtiles.h"
#include "samples/flags.h"
#include "samples/items.h"
#include "samples/mouse.h"
#include "samples/utilities.h"
#include "samples/widgets.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QClipboard>
#include <QDir>
#include <QGeoView/QGVDrawItem.h>

MainWindow::MainWindow()
    : ui(new Ui::MainWindow)
    , mCurrentItem(nullptr)
{
    ui->setupUi(this);
    connect(ui->demoList, &QListWidget::doubleClicked, this, &MainWindow::onItemDoubleClick);

    auto actPosition = new QAction("Print position", this);
    ui->geoMap->addAction(actPosition);
    connect(actPosition, &QAction::triggered, actPosition, [this]() {
        auto cam = ui->geoMap->getCamera();
        auto rect = cam.getProjection()->projToGeo(cam.projRect());
        auto pos = cam.getProjection()->projToGeo(cam.projRect().center());
        qInfo() << "current geo-rect" << rect;
        qInfo() << "current geo-center" << pos;
    });

    auto actSelectView = new QAction("Select all (view)", this);
    ui->geoMap->addAction(actSelectView);
    connect(actSelectView, &QAction::triggered, actSelectView, [this]() {
        ui->geoMap->unselectAll();
        auto selection = ui->geoMap->search(ui->geoMap->getCamera().projRect());
        for (auto item : selection) {
            item->select();
        }
    });

    auto actUnselect = new QAction("Unselect all", this);
    ui->geoMap->addAction(actUnselect);
    connect(actUnselect, &QAction::triggered, actUnselect, [this]() { ui->geoMap->unselectAll(); });

    auto actFront = new QAction("Bring to front (selected)", this);
    ui->geoMap->addAction(actFront);
    connect(actFront, &QAction::triggered, actFront, [this]() {
        for (QGVItem* item : ui->geoMap->getSelections()) {
            item->bringToFront();
        }
    });

    auto actBack = new QAction("Send to back (selected)", this);
    ui->geoMap->addAction(actBack);
    connect(actBack, &QAction::triggered, actBack, [this]() {
        for (QGVItem* item : ui->geoMap->getSelections()) {
            item->sendToBack();
        }
    });

    auto actOpacityPlus = new QAction("+25% opacity (selected)", this);
    ui->geoMap->addAction(actOpacityPlus);
    connect(actOpacityPlus, &QAction::triggered, actOpacityPlus, [this]() {
        for (QGVItem* item : ui->geoMap->getSelections()) {
            item->setOpacity(item->getOpacity() + 0.25);
        }
    });

    auto actOpacityMinus = new QAction("-25% opacity (selected)", this);
    ui->geoMap->addAction(actOpacityMinus);
    connect(actOpacityMinus, &QAction::triggered, actOpacityMinus, [this]() {
        for (QGVItem* item : ui->geoMap->getSelections()) {
            item->setOpacity(item->getOpacity() - 0.25);
        }
    });

    auto actImage = new QAction("Copy as image", this);
    ui->geoMap->addAction(actImage);
    connect(actImage, &QAction::triggered, actPosition, [this]() {
        QGuiApplication::clipboard()->setImage(ui->geoMap->grabMapView(true).toImage());
    });

    QMetaObject::invokeMethod(this, "init", Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    /*
     * All "online" items required instance of QNetworkAccessManager.
     * Also it is recommended to use QNetworkCache for this manager to reduce
     * network load and speed-up download.
     */
    QDir("cacheDir").removeRecursively();
    mCache = new QNetworkDiskCache(this);
    mCache->setCacheDirectory("cacheDir");
    mManager = new QNetworkAccessManager(this);
    mManager->setCache(mCache);
    QGV::setNetworkManager(mManager);

    mDemo = {
        new WidgetsDemo(ui->geoMap, this),   new BackgroundDemo(ui->geoMap, this), new MouseDemo(ui->geoMap, this),
        new ItemsDemo(ui->geoMap, this),     new FlagsDemo(ui->geoMap, this),      new CustomTiles(ui->geoMap, this),
        new UtilitiesDemo(ui->geoMap, this),
    };
    for (DemoItem* item : mDemo) {
        ui->demoList->addItem(item->label());
        auto lstItem = ui->demoList->item(ui->demoList->count() - 1);
        lstItem->setData(Qt::UserRole, QVariant::fromValue<DemoItem*>(item));
        connect(item, &DemoItem::started, this, &MainWindow::onItemStarted);
        connect(item, &DemoItem::ended, this, &MainWindow::onItemEnded);
        item->init();
    }
    ui->demoList->setCurrentRow(0);

    /*
     * By default geomap started with zoom 1, which is usually a to high
     * resolution for view.
     * With this code we change camera to show "worldwide" area.
     */
    auto target = ui->geoMap->getProjection()->boundaryGeoRect();
    ui->geoMap->cameraTo(QGVCameraActions(ui->geoMap).scaleTo(target));
}

void MainWindow::stopCurrent()
{
    if (mCurrentItem == nullptr) {
        return;
    }
    ui->demoComment->setText("");
    auto old = mCurrentItem;
    mCurrentItem = nullptr;
    old->end();
}

void MainWindow::onItemDoubleClick()
{
    stopCurrent();
    mCurrentItem = ui->demoList->currentItem()->data(Qt::UserRole).value<DemoItem*>();
    mCurrentItem->start();
}

void MainWindow::onItemStarted(DemoItem* item)
{
    ui->demoComment->setText(item->comment());
}

void MainWindow::onItemEnded(DemoItem* /*item*/)
{
    stopCurrent();
}

void MainWindow::hideEvent(QHideEvent* /*event*/)
{
    QApplication::quit();
}
