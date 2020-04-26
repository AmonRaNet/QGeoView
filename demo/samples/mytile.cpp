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

#include "mytile.h"

#include <QBrush>
#include <QPainter>
#include <QPen>

MyTile::MyTile(const QGV::GeoTilePos& tilePos, QColor color)
    : mTilePos(tilePos)
    , mColor(color)
{}

void MyTile::onProjection(QGVMap* geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    mProjRect = geoMap->getProjection()->geoToProj(mTilePos.toGeoRect());
}

QPainterPath MyTile::projShape() const
{
    QPainterPath path;
    path.addRect(mProjRect);
    return path;
}

void MyTile::projPaint(QPainter* painter)
{
    QPen pen = QPen(QBrush(Qt::black), 1);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setBrush(QBrush(mColor));
    painter->drawRect(mProjRect);
    drawText(painter);
}

void MyTile::drawText(QPainter* painter)
{
    QString text = QString("Custom tile\nZ:%1\nX:%2\nY:%3")
                           .arg(mTilePos.zoom())
                           .arg(mTilePos.pos().x())
                           .arg(mTilePos.pos().y());
    QPen pen = QPen(Qt::black);
    pen.setWidth(1);
    pen.setCosmetic(true);
    QBrush brush = QBrush(Qt::white);
    painter->setPen(pen);
    painter->setBrush(brush);
    auto path = QGV::createTextPath(mProjRect.toRect(), text, QFont(), pen.width());
    path = QGV::createTransfromScale(mProjRect.center(), 0.75).map(path);
    painter->drawPath(path);
}
