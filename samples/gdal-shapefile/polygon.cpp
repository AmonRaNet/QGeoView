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

#include "polygon.h"

#include <QPainter>

Polygon::Polygon(const PointList& geoPoints, QColor stroke, QColor fill)
    : mGeoPoints(geoPoints)
    , mColorStroke(stroke)
    , mColorFill(fill)
{
}

void Polygon::setPoints(const PointList& geoPoints)
{
    mGeoPoints = geoPoints;

    // Geo coordinates need to be converted manually again to projection
    onProjection(getMap());

    // Now we can inform QGV about changes for this
    resetBoundary();
    refresh();
}

PointList Polygon::getPoints() const
{
    return mGeoPoints;
}

void Polygon::onProjection(QGVMap* geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    mProjPoints.clear();
    for (const QGV::GeoPos& pos : mGeoPoints)
        mProjPoints << geoMap->getProjection()->geoToProj(pos);
}

QPainterPath Polygon::projShape() const
{
    QPainterPath path;
    path.addPolygon(mProjPoints);
    return path;
}

void Polygon::projPaint(QPainter* painter)
{
    QPen pen = QPen(QBrush(mColorStroke), 1);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setBrush(QBrush(mColorFill));
    painter->drawPolygon(mProjPoints);
}

QPointF Polygon::projAnchor() const
{
    return mProjPoints.boundingRect().center();
}

QTransform Polygon::projTransform() const
{
    // This method is optional (needed flag is QGV::ItemFlag::Transformed).
    // Custom transformation for item.
    // In this case we rotate item by 45 degree.

    return QGV::createTransfromAzimuth(projAnchor(), 45);
}

QString Polygon::projTooltip(const QPointF& projPos) const
{
    // This method is optional (when empty return then no tooltip).
    // Text for mouse tool tip.

    auto geo = getMap()->getProjection()->projToGeo(projPos);

    return "Polygon with color " + mColorFill.name() + "\nPosition " + geo.latToString() + " " + geo.lonToString();
}

void Polygon::projOnMouseClick(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Clickable).
    // Custom reaction to item single mouse click.
    // To avoid collision with item selection this code applies only if item selection disabled.
    // In this case we change opacity for item.

    if (!isSelectable()) {
        if (getOpacity() <= 0.5)
            setOpacity(1.0);
        else
            setOpacity(0.5);

        qInfo() << "single click" << projPos;
    } else {
        setOpacity(1.0);
    }
}

void Polygon::projOnMouseDoubleClick(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Clickable).
    // Custom reaction to item double mouse click.
    // In this case we change color for item.

    const QList<QColor> colors = { Qt::red, Qt::blue, Qt::green, Qt::gray, Qt::cyan, Qt::magenta, Qt::yellow };

    const auto iter =
            std::find_if(colors.begin(), colors.end(), [this](const QColor& color) { return color == mColorFill; });
    mColorFill = colors[(iter - colors.begin() + 1) % colors.size()];
    repaint();

    setOpacity(1.0);

    qInfo() << "double click" << projPos;
}

void Polygon::projOnObjectStartMove(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Movable).
    // Custom reaction to item move start.
    // In this case we only log message.

    qInfo() << "object move started at" << projPos;
}

void Polygon::projOnObjectMovePos(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Movable).
    // Custom reaction to mouse pos change when item move is started.
    // In this case actually changing location of object.

    PointList newPoints;
    for (const QPointF& pt : mProjPoints)
        newPoints << getMap()->getProjection()->projToGeo(pt + projPos);

    setPoints(newPoints);

    qInfo() << "object moved" << mProjPoints;
}

void Polygon::projOnObjectStopMove(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Movable).
    // Custom reaction to item move finished.
    // In this case we only log message.

    qInfo() << "object move stopped" << projPos;
}
