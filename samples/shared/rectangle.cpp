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

#include "rectangle.h"

#include <QBrush>
#include <QPainter>
#include <QPen>

Rectangle::Rectangle(const QGV::GeoRect& geoRect, QColor color)
    : mGeoRect(geoRect)
    , mColor(color)
{
}

void Rectangle::setRect(const QGV::GeoRect& geoRect)
{
    mGeoRect = geoRect;

    // Geo coordinates need to be converted manually again to projection
    onProjection(getMap());

    // Now we can inform QGV about changes for this
    resetBoundary();
    refresh();
}

QGV::GeoRect Rectangle::getRect() const
{
    return mGeoRect;
}

void Rectangle::onProjection(QGVMap* geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    mProjRect = geoMap->getProjection()->geoToProj(mGeoRect);
}

QPainterPath Rectangle::projShape() const
{
    QPainterPath path;
    path.addRect(mProjRect);
    return path;
}

void Rectangle::projPaint(QPainter* painter)
{
    QPen pen = QPen(QBrush(Qt::black), 1);

    // Custom item highlight indicator
    if (isFlag(QGV::ItemFlag::Highlighted) && isFlag(QGV::ItemFlag::HighlightCustom)) {
        // We will use pen with bigger width
        pen = QPen(QBrush(Qt::black), 5);
    }

    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setBrush(QBrush(mColor));
    painter->drawRect(mProjRect);

    // Custom item select indicator
    if (isSelected() && isFlag(QGV::ItemFlag::SelectCustom)) {
        // We will draw additional rect around our item
        painter->drawLine(mProjRect.topLeft(), mProjRect.bottomRight());
        painter->drawLine(mProjRect.topRight(), mProjRect.bottomLeft());
    }
}

QPointF Rectangle::projAnchor() const
{
    // This method is optional (needed flag is QGV::ItemFlag::Transformed).
    // In this case we will use center of item as base

    return mProjRect.center();
}

QTransform Rectangle::projTransform() const
{
    // This method is optional (needed flag is QGV::ItemFlag::Transformed).
    // Custom transformation for item.
    // In this case we rotate item by 45 degree.

    return QGV::createTransfromAzimuth(projAnchor(), 45);
}

QString Rectangle::projTooltip(const QPointF& projPos) const
{
    // This method is optional (when empty return then no tooltip).
    // Text for mouse tool tip.

    auto geo = getMap()->getProjection()->projToGeo(projPos);

    return "Rectangle with color " + mColor.name() + "\nPosition " + geo.latToString() + " " + geo.lonToString();
}

void Rectangle::projOnMouseClick(const QPointF& projPos)
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

void Rectangle::projOnMouseDoubleClick(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Clickable).
    // Custom reaction to item double mouse click.
    // In this case we change color for item.

    const QList<QColor> colors = { Qt::red, Qt::blue, Qt::green, Qt::gray, Qt::cyan, Qt::magenta, Qt::yellow };

    const auto iter =
            std::find_if(colors.begin(), colors.end(), [this](const QColor& color) { return color == mColor; });
    mColor = colors[(iter - colors.begin() + 1) % colors.size()];
    repaint();

    setOpacity(1.0);

    qInfo() << "double click" << projPos;
}

void Rectangle::projOnObjectStartMove(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Movable).
    // Custom reaction to item move start.
    // In this case we only log message.

    qInfo() << "object move started at" << projPos;
}

void Rectangle::projOnObjectMovePos(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Movable).
    // Custom reaction to mouse pos change when item move is started.
    // In this case actually changing location of object.

    auto newRect = mProjRect;
    newRect.moveCenter(projPos);

    setRect(getMap()->getProjection()->projToGeo(newRect));

    qInfo() << "object moved" << mGeoRect;
}

void Rectangle::projOnObjectStopMove(const QPointF& projPos)
{
    // This method is optional (needed flag is QGV::ItemFlag::Movable).
    // Custom reaction to item move finished.
    // In this case we only log message.

    qInfo() << "object move stopped" << projPos;
}
