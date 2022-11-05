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

#include "QGVGlobal.h"
#include "QGVMapRubberBand.h"

class QGVMap;
class QGVDrawItem;

class QGV_LIB_DECL QGVMapActions : public QObject
{
    Q_OBJECT

public:
    QGVMapActions(QGVMap* geoMap);

    void startMovingMap(const QPoint& mousePos);
    void startMovingObject(const QPoint& mousePos, QGVDrawItem* object);
    void startSelectionRect(const QPoint& mousePos, const QGV::MapSelectionMode mode);

    void move(const QPoint& mousePos);

    void stop(const QPoint& mousePos);

private:
    void changeState(QGV::MapActionsState state);
    QPointF mapToScene(const QPoint& mousePos) const;
    QPolygonF mapToScene(const QRect& mouseRect) const;

    void moveMap(const QPoint& mousePos);
    void moveObject(const QPoint& mousePos);
    void moveRect(const QPoint& mousePos);

    void stopMoveObject(const QPoint& mousePos);
    void stopRect();
    void applyZoomRect(QRect areaRect);
    void applySelectionRect(QRect areaRect, bool replaceSelection);

private:
    QGVMap* mGeoMap;
    QGV::MapActionsState mState;
    QPointF mMoveProjAnchor;
    QGVDrawItem* mMovingObject;
    QGV::MapSelectionMode mSelectionMode;
    QScopedPointer<QGVMapRubberBand> mSelectionRect;
};
