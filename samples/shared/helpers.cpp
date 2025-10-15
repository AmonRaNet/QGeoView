/***************************************************************************
 * QGeoView is a Qt / C ++ widget for visualizing geographic data.
 * Copyright (C) 2018-2025 Andrey Yaroshenko.
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

#include "helpers.h"

#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QRandomGenerator>

int Helpers::randomInt(int lowest, int highest)
{
    return static_cast<int>(QRandomGenerator::global()->bounded(lowest, highest));
}

QGV::GeoRect Helpers::randRect(QGVMap* geoMap, const QGV::GeoRect& targetArea, const QSizeF& size)
{
    const auto baseGeo = randPos(targetArea);
    const auto base = geoMap->getProjection()->geoToProj(baseGeo);
    return geoMap->getProjection()->projToGeo({ base, base + QPointF(size.width(), size.height()) });
}

QGV::GeoRect Helpers::randRect(QGVMap* geoMap, const QGV::GeoRect& targetArea, int baseSize)
{
    const auto size = randSize(baseSize);
    return randRect(geoMap, targetArea, size);
}

QGV::GeoRect Helpers::randRect(QGVMap* /*geoMap*/, const QGV::GeoRect& targetArea)
{
    const auto baseGeo1 = randPos(targetArea);
    const auto baseGeo2 = randPos(targetArea);
    return QGV::GeoRect{ baseGeo1.latitude(), baseGeo1.longitude(), baseGeo2.latitude(), baseGeo2.longitude() };
}

QGV::GeoPos Helpers::randPos(const QGV::GeoRect& targetArea)
{
    const double latRange = targetArea.latTop() - targetArea.latBottom();
    const double lonRange = targetArea.lonRight() - targetArea.lonLeft();
    static const int range = 1000;

    return { targetArea.latBottom() + latRange * (randomInt(0, range)) / range,
             targetArea.lonLeft() + lonRange * (randomInt(0, range)) / range };
}

QSizeF Helpers::randSize(int baseSize)
{
    const int range = baseSize / 2;

    return QSize(baseSize + (randomInt(0, range)), baseSize - (randomInt(0, range)));
}

void Helpers::setupCachedNetworkAccessManager(QObject* parent)
{
    QDir("cacheDir").removeRecursively();
    auto cache = new QNetworkDiskCache(parent);
    cache->setCacheDirectory("cacheDir");
    auto manager = new QNetworkAccessManager(parent);
    manager->setCache(cache);
    QGV::setNetworkManager(manager);
}
