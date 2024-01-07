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

#include <QAction>
#include <QCheckBox>
#include <QClipboard>
#include <QGuiApplication>
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
    setWindowTitle("QGeoView Samples - mouse actions");

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

    // Custom layer
    mItemsLayer = new QGVLayer();
    mMap->addItem(mItemsLayer);

    // Add items to custom layer
    for (int i = 0; i < 10; i++) {
        const int size = 50;
        auto item = new Rectangle(Helpers::randRect(mMap, targetArea(), size), Qt::red);
        item->setFlags(QGV::ItemFlag::IgnoreScale | QGV::ItemFlag::IgnoreAzimuth | QGV::ItemFlag::Movable);
        item->setSelectable(true);
        mItemsLayer->addItem(item);
    }

    // Options list
    centralWidget()->layout()->addWidget(createOptionsList());

    // Extra components for test
    createContextMenu();
    createTrackingWidget();

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

void MainWindow::createContextMenu()
{
    static QVariant lastRect;

    auto actPosition = new QAction("Print position", this);
    mMap->addAction(actPosition);
    connect(actPosition, &QAction::triggered, actPosition, [this]() {
        auto cam = mMap->getCamera();
        auto rect = cam.getProjection()->projToGeo(cam.projRect());
        auto pos = cam.getProjection()->projToGeo(cam.projRect().center());
        lastRect = QVariant::fromValue(rect);
        qInfo() << "current geo-rect" << rect;
        qInfo() << "current geo-center" << pos;
    });

    auto actFlyToLast = new QAction("Fly to last printed area", this);
    mMap->addAction(actFlyToLast);
    connect(actFlyToLast, &QAction::triggered, actFlyToLast, [this]() {
        if (lastRect.isNull()) {
            return;
        }
        mMap->flyTo(QGVCameraActions(mMap).scaleTo(lastRect.value<QGV::GeoRect>()));
    });

    auto actSelectView = new QAction("Select all (view)", this);
    mMap->addAction(actSelectView);
    connect(actSelectView, &QAction::triggered, actSelectView, [this]() {
        mMap->unselectAll();
        auto selection = mMap->search(mMap->getCamera().projRect());
        for (auto item : selection) {
            item->select();
        }
    });

    auto actUnselect = new QAction("Unselect all", this);
    mMap->addAction(actUnselect);
    connect(actUnselect, &QAction::triggered, actUnselect, [this]() { mMap->unselectAll(); });

    auto actFront = new QAction("Bring to front (selected)", this);
    mMap->addAction(actFront);
    connect(actFront, &QAction::triggered, actFront, [this]() {
        for (QGVItem* item : mMap->getSelections()) {
            item->bringToFront();
        }
    });

    auto actBack = new QAction("Send to back (selected)", this);
    mMap->addAction(actBack);
    connect(actBack, &QAction::triggered, actBack, [this]() {
        for (QGVItem* item : mMap->getSelections()) {
            item->sendToBack();
        }
    });

    auto actOpacityPlus = new QAction("+25% opacity (selected)", this);
    mMap->addAction(actOpacityPlus);
    connect(actOpacityPlus, &QAction::triggered, actOpacityPlus, [this]() {
        for (QGVItem* item : mMap->getSelections()) {
            item->setOpacity(item->getOpacity() + 0.25);
        }
    });

    auto actOpacityMinus = new QAction("-25% opacity (selected)", this);
    mMap->addAction(actOpacityMinus);
    connect(actOpacityMinus, &QAction::triggered, actOpacityMinus, [this]() {
        for (QGVItem* item : mMap->getSelections()) {
            item->setOpacity(item->getOpacity() - 0.25);
        }
    });

    auto actImage = new QAction("Copy as image", this);
    mMap->addAction(actImage);
    connect(actImage, &QAction::triggered, actPosition, [this]() {
        QGuiApplication::clipboard()->setImage(mMap->grabMapView(true).toImage());
    });
}

void MainWindow::createTrackingWidget()
{
    // QGVWidgetText will be used to show current position.
    QGVWidgetText* text = new QGVWidgetText();
    text->setAnchor(QPoint(0, 0), { Qt::TopEdge });
    mMap->addWidget(text);
    connect(mMap, &QGVMap::mapMouseMove, text, [this, text](QPointF projPos) {
        // Current projection position can be converted to geo-coordinates and printed by corresponding functions.
        auto geoPos = mMap->getProjection()->projToGeo(projPos);
        text->setText(QString("<b>%1, %2</b>").arg(geoPos.latToString()).arg(geoPos.lonToString()));
    });
}

QGroupBox* MainWindow::createOptionsList()
{
    const QList<QPair<QString, std::function<void(bool)>>> actions = {
        { "Move position tracking", [this](bool enabled) { startTracking(enabled); } },
        { "Map move (LButton hold + move)", [this](bool enabled) { enableAction(enabled, QGV::MouseAction::Move); } },
        { "Zoom with mouse wheel", [this](bool enabled) { enableAction(enabled, QGV::MouseAction::ZoomWheel); } },
        { "Zoom with selection rect (RButton hold + move)",
          [this](bool enabled) { enableAction(enabled, QGV::MouseAction::ZoomRect); } },
        { "Item selection by mouse (single LButton or Shift/Ctrl + RButton hold + move)",
          [this](bool enabled) { enableAction(enabled, QGV::MouseAction::Selection); } },
        { "Item move by mouse (Alt + LButton hold)",
          [this](bool enabled) { enableAction(enabled, QGV::MouseAction::MoveObjects); } },
        { "Context menu", [this](bool enabled) { enableAction(enabled, QGV::MouseAction::ContextMenu); } },
        { "Show item tooltip (mouse hover)",
          [this](bool enabled) { enableAction(enabled, QGV::MouseAction::Tooltip); } },
    };

    QGroupBox* groupBox = new QGroupBox(tr("Mouse options"));
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

void MainWindow::startTracking(bool start)
{
    mMap->setMouseTracking(start);
}

void MainWindow::enableAction(bool enable, QGV::MouseAction action)
{
    mMap->setMouseAction(action, enable);
}
