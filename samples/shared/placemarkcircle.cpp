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

#include "placemarkcircle.h"

#include <QBrush>
#include <QPainter>
#include <QPen>

PlacemarkCircle::PlacemarkCircle(const QGV::GeoPos& geoPos, int radius, const QColor& color)
    : mGeoCenter(geoPos)
    , mColor(color)
    , mRadius(radius)
{
    // We want to use radius "as pixels", therefore we ignore scale
    setFlag(QGV::ItemFlag::IgnoreScale);
}

void PlacemarkCircle::setRadius(int radius)
{
    mRadius = radius;
    resetBoundary();
    refresh();
}

void PlacemarkCircle::setCenter(const QGV::GeoPos& geoPos)
{
    mGeoCenter = geoPos;

    // Geo coordinates need to be converted manually again to projection
    onProjection(getMap());

    // Now we can inform QGV about changes for this
    resetBoundary();
    refresh();
}

QGV::GeoPos PlacemarkCircle::getCenter() const
{
    return mGeoCenter;
}

void PlacemarkCircle::onProjection(QGVMap* geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    mProjCenter = geoMap->getProjection()->geoToProj(mGeoCenter);
}

QPainterPath PlacemarkCircle::projShape() const
{
    QPainterPath path;
    path.addEllipse(mProjCenter.x(), mProjCenter.y(), mRadius, mRadius);
    return path;
}

void PlacemarkCircle::projPaint(QPainter* painter)
{
    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(QBrush(mColor));
    painter->drawEllipse(mProjCenter.x(), mProjCenter.y(), mRadius, mRadius);
}
