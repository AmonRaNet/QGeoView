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

#include "demoitem.h"
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <random>
#endif

DemoItem::DemoItem(QGVMap* geoMap, SelectorDialog::Type type, QObject* parent)
    : QObject(parent)
    , mGeoMap(geoMap)
{
    mSelectorDialog.reset(new SelectorDialog(type));
}

QGVMap* DemoItem::geoMap() const
{
    return mGeoMap;
}

SelectorDialog* DemoItem::selector() const
{
    return mSelectorDialog.data();
}

void DemoItem::init()
{
    onInit();
    Q_EMIT ready(this);
}
void DemoItem::start()
{
    onStart();
    Q_EMIT started(this);
}
void DemoItem::end()
{
    onEnd();
    Q_EMIT ended(this);
}

QGV::GeoPos DemoItem::randPos(const QGV::GeoRect& targetArea)
{
    const double latRange = targetArea.latTop() - targetArea.latBottom();
    const double lonRange = targetArea.lonRigth() - targetArea.lonLeft();
    static const int range = 1000;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return { targetArea.latBottom() + latRange * (std::rand() % range) / range,
             targetArea.lonLeft() + lonRange * (std::rand() % range) / range };
#else
    return { targetArea.latBottom() + latRange * (qrand() % range) / range,
             targetArea.lonLeft() + lonRange * (qrand() % range) / range };
#endif
}

QGV::GeoRect DemoItem::randRect(const QGV::GeoRect& targetArea, const QSizeF& size)
{
    const auto baseGeo = randPos(targetArea);
    const auto base = geoMap()->getProjection()->geoToProj(baseGeo);
    return geoMap()->getProjection()->projToGeo({ base, base + QPointF(size.width(), size.height()) });
}

QGV::GeoRect DemoItem::randRect(const QGV::GeoRect& targetArea, int baseSize)
{
    const auto size = randSize(baseSize);
    return randRect(targetArea, size);
}

QSizeF DemoItem::randSize(int baseSize)
{
    const int range = -baseSize / 2;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return QSize(baseSize + (std::rand() % range), baseSize + (std::rand() % range));
#else
    return QSize(baseSize + (qrand() % range), baseSize + (qrand() % range));
#endif
}
