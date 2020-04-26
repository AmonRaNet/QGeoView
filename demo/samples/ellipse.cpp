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

#include "ellipse.h"

#include <QBrush>
#include <QPainter>
#include <QPen>

Ellipse::Ellipse(const QGV::GeoRect& geoRect, QColor color)
    : mGeoRect(geoRect)
    , mColor(color)
{
    setSelectable(true);
}

void Ellipse::onProjection(QGVMap* geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    mProjRect = geoMap->getProjection()->geoToProj(mGeoRect);
}

QPainterPath Ellipse::projShape() const
{
    QPainterPath path;
    path.addEllipse(mProjRect);
    return path;
}

void Ellipse::projPaint(QPainter* painter)
{
    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(QBrush(mColor));
    painter->drawEllipse(mProjRect);
}

QString Ellipse::projTooltip(const QPointF& projPos) const
{
    auto geo = getMap()->getProjection()->projToGeo(projPos);
    return "Ellipse with color " + mColor.name() + "\nPosition " + geo.latToString() + " " + geo.lonToString();
}
