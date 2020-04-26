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

#pragma once

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>

#include <QGeoView/QGVWidgetCompass.h>
#include <QGeoView/QGVWidgetScale.h>
#include <QGeoView/QGVWidgetZoom.h>

#include "demoitem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private Q_SLOTS:
    void init();

private:
    void stopCurrent();
    void onItemDoubleClick();
    void onItemStarted(DemoItem* item);
    void onItemEnded(DemoItem* item);
    void hideEvent(QHideEvent* event);

private:
    Ui::MainWindow* ui;
    QNetworkAccessManager* mManager;
    QNetworkDiskCache* mCache;
    DemoItem* mCurrentItem;
    QList<DemoItem*> mDemo;
};
