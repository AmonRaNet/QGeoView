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

#include "rectangle.h"

#include <QBrush>
#include <QPainter>
#include <QPen>

Rectangle::Rectangle(const QGV::GeoRect& geoRect, QGV::ItemFlags flags, QColor color)
    : mGeoRect(geoRect)
    , mColor(color)
{
    setFlags(flags);
    setSelectable(true);
}

void Rectangle::onProjection(QGVMap* geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    mProjRect = geoMap->getProjection()->geoToProj(mGeoRect);
}

void Rectangle::onUpdate()
{
    QGVDrawItem::onUpdate();
}

QPainterPath Rectangle::projShape() const
{
    QPainterPath path;
    path.addRect(mProjRect);
    return path;
}

void Rectangle::projPaint(QPainter* painter)
{
    QPen pen;
    if (isFlag(QGV::ItemFlag::Highlighted) && isFlag(QGV::ItemFlag::HighlightCustom)) {
        pen = QPen(QBrush(Qt::black), 5);
    } else {
        pen = QPen(QBrush(Qt::black), 1);
    }
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setBrush(QBrush(mColor));
    painter->drawRect(mProjRect);
    if (isSelected() && isFlag(QGV::ItemFlag::SelectCustom)) {
        painter->drawLine(mProjRect.topLeft(), mProjRect.bottomRight());
        painter->drawLine(mProjRect.topRight(), mProjRect.bottomLeft());
    }
}

QPointF Rectangle::projAnchor() const
{
    return mProjRect.center();
}

QTransform Rectangle::projTransform() const
{
    return QGV::createTransfromAzimuth(projAnchor(), 45);
}

QString Rectangle::projTooltip(const QPointF& projPos) const
{
    auto geo = getMap()->getProjection()->projToGeo(projPos);
    return "Rectangle with color " + mColor.name() + "\nPosition " + geo.latToString() + " " + geo.lonToString();
}

void Rectangle::projOnMouseClick(const QPointF& projPos)
{
    setOpacity(qMax(0.2, getOpacity() - 0.2));
    qInfo() << "single click" << projPos;
}

void Rectangle::projOnMouseDoubleClick(const QPointF& projPos)
{
    setOpacity(1.0);
    qInfo() << "double click" << projPos;
}

void Rectangle::projOnObjectStartMove(const QPointF& projPos)
{
    qInfo() << "object move started at" << projPos;
}

void Rectangle::projOnObjectMovePos(const QPointF& projPos)
{
    mProjRect.moveCenter(projPos);
    mGeoRect = getMap()->getProjection()->projToGeo(mProjRect);
    refresh();
    qInfo() << "object moved" << mGeoRect;
}

void Rectangle::projOnObjectStopMove(const QPointF& projPos)
{
    qInfo() << "object move stopped" << projPos;
}
