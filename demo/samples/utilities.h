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

#include "demoitem.h"

#include <QGeoView/QGVLayer.h>
#include <QPixmap>

class UtilitiesDemo : public DemoItem
{
    Q_OBJECT

public:
    explicit UtilitiesDemo(QGVMap* geoMap, QObject* parent = 0);

    QString label() const override;
    QString comment() const override;

private:
    void onInit() override;
    void onStart() override;
    void onEnd() override;

    QGV::GeoRect targetQGVArea() const;
    QPixmap createQGVImage();
    QGVLayer* createQGVLayer();
    QAbstractAnimation* createQGVAnimation();
    QGV::GeoRect target10000Area() const;
    QGVLayer* create10000Layer();

    void showQGV(bool selected);
    void animationQGV(bool selected);
    void show10000(bool selected);
    void drawDebug(bool selected);
    void printDebug(bool selected);

private:
    QGVLayer* mQGV;
    QGVLayer* m10000;
    QScopedPointer<QAbstractAnimation> mQGVAnimation;
};
