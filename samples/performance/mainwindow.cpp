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

#include <QButtonGroup>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QTimer>
#include <QVBoxLayout>

#include <helpers.h>
#include <rectangle.h>

#include <QGeoView/QGVLayerGoogle.h>
#include <QGeoView/QGVWidgetCompass.h>

MainWindow::MainWindow()
{
    setWindowTitle("QGeoView Samples - performance profiles");

    mMap = new QGVMap(this);
    setCentralWidget(mMap);

    Helpers::setupCachedNetworkAccessManager(this);

    // Background layer
    mBackground = new QGVLayerGoogle();
    mMap->addItem(mBackground);

    // Widgets
    mMap->addWidget(new QGVWidgetCompass());

    // 10000 layer
    mMap->addItem(create10000Layer());

    // Options list
    centralWidget()->layout()->addWidget(createOptionsList());

    // Show whole world
    auto target = mMap->getProjection()->boundaryGeoRect();
    mMap->cameraTo(QGVCameraActions(mMap).scaleTo(target));

    // Enable debug
    QGV::setPrintDebug(true);
}

MainWindow::~MainWindow()
{
}

QGV::GeoRect MainWindow::target10000Area() const
{
    return mMap->getProjection()->boundaryGeoRect();
}

QGVLayer* MainWindow::create10000Layer() const
{
    /*
     * Layers will be owned by map.
     */
    auto target = target10000Area();
    auto layer = new QGVLayer();
    layer->setName("10000 elements");
    layer->setDescription("Demo for 10000 elements");

    /*
     * Items will be owned by layer.
     */
    const int size = 20000;
    for (int i = 0; i < 10000; i++) {
        auto item = new Rectangle(Helpers::randRect(mMap, target, size), Qt::red);
        layer->addItem(item);
    }

    return layer;
}

QGroupBox* MainWindow::createOptionsList()
{
    /*
     * Options in this list is only recommendations and hard-coded values can be mixed or adopted to your application.
     *
     * Background map has biggest impact to performance because it covers whole map area all the time.
     * QGVLayerTiles always calculates needed set of tiles for current camera state and parameters below will adjust
     * algorithm.
     *
     * TilesMarginWithZoomChange, TilesMarginNoZoomChange are defining margin around visible map area.
     * First right after zoom change and second after map move/rotate. Bigger margin gives better feedback for user
     * when map moved by mouse, but from other side it increases number of tiles loaded to map. Don't use values
     * below 1.
     *
     * AnimationUpdateDelayMs, CameraUpdatesDuringAnimation are defining layer behavior during animation (flyTo method)
     * First defines time interval between camera update processing (during animation only!) and second will disable
     * any processing during animation completely. This parameter will have large impact to performance in case
     * if application performs many "jumps" by flyTo method. When processing is disabled or interval is very large QGV
     * will show gray areas on missing tiles during animation.
     *
     * VisibleZoomLayersBelowCurrent, VisibleZoomLayersAboveCurrent are defining number of zoom levels which layer can
     * keep below and above current level. This is most impacting to performance parameters (especially
     * VisibleZoomLayersBelowCurrent). By default QGVLayerTiles cleanups tiles based on tiles coverage. For example when
     * tile below (bigger one) is fully covered by tiles above (smaller) then big one will be deleted. Or when bigger
     * tile of current zoom level is loaded all tiles above it will be deleted. This gives nice user feedback, but can
     * lead to high load on scene, especially when network had low latency and tiles from low levels are consistently
     * upscaled. VisibleZoomLayersBelowCurrent, VisibleZoomLayersAboveCurrent are limiting QGVLayerTiles to keep only
     * given number of zoom levels above or below current one. When is equals to 0 then only current level is allowed.
     */

    QGroupBox* groupBox = new QGroupBox(tr("Profiles"));
    groupBox->setLayout(new QVBoxLayout);

    {
        QPushButton* button = new QPushButton("Try it!");
        groupBox->layout()->addWidget(button);

        connect(button, &QPushButton::clicked, this, [this]() { flyToRandomArea(); });
    }

    QButtonGroup* group = new QButtonGroup(this);

    {
        QRadioButton* radioButton = new QRadioButton("Best look");
        group->addButton(radioButton);

        QLabel* description = new QLabel();
        description->setText("Set of parameters which gives best look for user but with price of performance.\nThis is "
                             "default values for any tile map.");

        QWidget* item = new QWidget();
        item->setLayout(new QVBoxLayout());
        item->layout()->addWidget(radioButton);
        item->layout()->addWidget(description);

        groupBox->layout()->addWidget(item);

        connect(radioButton, &QRadioButton::clicked, this, [this](const bool checked) {
            if (checked)
                setupProfileLook();
        });
    }

    {
        QRadioButton* radioButton = new QRadioButton("Balanced");
        group->addButton(radioButton);

        QLabel* description = new QLabel();
        description->setText("Set of parameters which gives optimal look for user with optimal performance. Can "
                             "produce 'gray' areas during animation and camera actions.");

        QWidget* item = new QWidget();
        item->setLayout(new QVBoxLayout());
        item->layout()->addWidget(radioButton);
        item->layout()->addWidget(description);

        groupBox->layout()->addWidget(item);

        connect(radioButton, &QRadioButton::clicked, this, [this](const bool checked) {
            if (checked)
                setupProfileBalance();
        });

        radioButton->click();
    }

    {
        QRadioButton* radioButton = new QRadioButton("Fast");
        group->addButton(radioButton);

        QLabel* description = new QLabel();
        description->setText("Set of parameters which gives best performance. This setup will produce 'gray' areas "
                             "every time during animation and camera actions.");

        QWidget* item = new QWidget();
        item->setLayout(new QVBoxLayout());
        item->layout()->addWidget(radioButton);
        item->layout()->addWidget(description);

        groupBox->layout()->addWidget(item);

        connect(radioButton, &QRadioButton::clicked, this, [this](const bool checked) {
            if (checked)
                setupProfileFast();
        });
    }

    return groupBox;
}

void MainWindow::flyToRandomArea()
{
    static int current = 0;
    static const std::vector<QGV::GeoRect> areas = {
        QGV::GeoRect(QGV::GeoPos(56.316425, 80.670445), QGV::GeoPos(53.280950, 86.641856)),
        QGV::GeoRect(QGV::GeoPos(52.131852, 4.989964), QGV::GeoPos(44.071465, 18.708665)),
        QGV::GeoRect(QGV::GeoPos(-17.631899, 20.654501), QGV::GeoPos(-29.494330, 35.357840)),
    };

    QTimer::singleShot(100, this, [&]() {
        mMap->flyTo(QGVCameraActions(mMap).scaleTo(areas[current % areas.size()]));
        current++;
    });
}

void MainWindow::setupProfileLook()
{
    mBackground->setTilesMarginWithZoomChange(1);
    mBackground->setTilesMarginNoZoomChange(3);
    mBackground->setAnimationUpdateDelayMs(200);
    mBackground->setVisibleZoomLayersBelowCurrent(10);
    mBackground->setVisibleZoomLayersAboveCurrent(10);
    mBackground->setCameraUpdatesDuringAnimation(true);
}

void MainWindow::setupProfileBalance()
{
    mBackground->setTilesMarginWithZoomChange(1);
    mBackground->setTilesMarginNoZoomChange(2);
    mBackground->setAnimationUpdateDelayMs(250);
    mBackground->setVisibleZoomLayersBelowCurrent(1);
    mBackground->setVisibleZoomLayersAboveCurrent(3);
    mBackground->setCameraUpdatesDuringAnimation(true);
}

void MainWindow::setupProfileFast()
{
    mBackground->setTilesMarginWithZoomChange(1);
    mBackground->setTilesMarginNoZoomChange(1);
    mBackground->setAnimationUpdateDelayMs(500);
    mBackground->setVisibleZoomLayersBelowCurrent(1);
    mBackground->setVisibleZoomLayersAboveCurrent(1);
    mBackground->setCameraUpdatesDuringAnimation(false);
}
