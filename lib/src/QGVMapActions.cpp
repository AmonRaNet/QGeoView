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

#include "QGVMapActions.h"
#include "QGVDrawItem.h"
#include "QGVMap.h"
#include "QGVMapQGView.h"
#include "QGVMapRubberBand.h"

QGVMapActions::QGVMapActions(QGVMap* geoMap)
    : mGeoMap{ geoMap }
{
    mState = QGV::MapActionsState::Idle;

    mSelectionRect.reset(new QGVMapRubberBand(mGeoMap));
    mSelectionRect->setMinSelection(QSize(5, 5));
}

void QGVMapActions::startMovingMap(const QPoint& mousePos)
{
    qgvInfo() << "QGVMapActions startMovingMap " << mousePos;

    changeState(QGV::MapActionsState::MovingMap);
    mMoveProjAnchor = mapToScene(mousePos);
}

void QGVMapActions::startMovingObject(const QPoint& mousePos, QGVDrawItem* object)
{
    if (!object->isFlag(QGV::ItemFlag::Movable)) {
        changeState(QGV::MapActionsState::Idle);
        return;
    }

    qgvInfo() << "QGVMapActions startMovingObject " << mousePos << object->projDebug();

    const QPointF projMouse = mapToScene(mousePos);
    mMovingObject = object;
    mMovingObject->projOnObjectStartMove(projMouse);
    changeState(QGV::MapActionsState::MovingObjects);
}

void QGVMapActions::startSelectionRect(const QPoint& mousePos, const QGV::MapSelectionMode mode)
{
    qgvInfo() << "QGVMapActions startSelectionRect " << static_cast<int>(mode) << mousePos;

    changeState(QGV::MapActionsState::SelectionRect);
    mSelectionMode = mode;
    mSelectionRect->setStartPoint(mousePos);
    mSelectionRect->showRect();
}

void QGVMapActions::move(const QPoint& mousePos)
{
    qgvInfo() << "QGVMapActions move for " << static_cast<int>(mState) << mousePos;

    if (mState == QGV::MapActionsState::MovingMap) {
        moveMap(mousePos);
    } else if (mState == QGV::MapActionsState::MovingObjects) {
        moveObject(mousePos);
    } else if (mState == QGV::MapActionsState::SelectionRect) {
        moveRect(mousePos);
    }
}

void QGVMapActions::stop(const QPoint& mousePos)
{
    qgvInfo() << "QGVMapActions stop for " << static_cast<int>(mState);

    if (mState == QGV::MapActionsState::MovingObjects) {
        stopMoveObject(mousePos);
    } else if (mState == QGV::MapActionsState::SelectionRect) {
        stopRect();
    }
    changeState(QGV::MapActionsState::Idle);
}

void QGVMapActions::changeState(QGV::MapActionsState state)
{
    if (mState == state) {
        return;
    }

    qgvInfo() << "QGVMapActions change state from " << static_cast<int>(mState) << "to" << static_cast<int>(state);

    mState = state;

    if (mState == QGV::MapActionsState::Idle) {
        mMoveProjAnchor = {};
        mMovingObject = {};
        mSelectionMode = QGV::MapSelectionMode::Zoom;
        mSelectionRect->hideRect();
    }
}

QPointF QGVMapActions::mapToScene(const QPoint& mousePos) const
{
    const auto viewPoint = mGeoMap->geoView()->mapFromParent(mousePos);
    return mGeoMap->geoView()->mapToScene(viewPoint);
}

QPolygonF QGVMapActions::mapToScene(const QRect& mouseRect) const
{
    const auto viewRect = QRect(mGeoMap->geoView()->mapFromParent(mouseRect.topLeft()),
                                mGeoMap->geoView()->mapFromParent(mouseRect.bottomRight()));
    return mGeoMap->geoView()->mapToScene(viewRect);
}

void QGVMapActions::moveMap(const QPoint& mousePos)
{
    const QPointF projCenter = mGeoMap->geoView()->viewProjRect().center();
    const QPointF projMouse = mapToScene(mousePos);
    const double xDelta = (mMoveProjAnchor.x() - projMouse.x());
    const double yDelta = (mMoveProjAnchor.y() - projMouse.y());

    mGeoMap->geoView()->cameraTo(QGVCameraActions(mGeoMap).moveTo(projCenter + QPointF(xDelta, yDelta)), false);
}

void QGVMapActions::moveObject(const QPoint& mousePos)
{
    Q_ASSERT(mMovingObject);
    const QPointF projMouse = mapToScene(mousePos);
    mMovingObject->projOnObjectMovePos(projMouse);
}

void QGVMapActions::moveRect(const QPoint& mousePos)
{
    mSelectionRect->setEndPoint(mousePos);
}

void QGVMapActions::stopMoveObject(const QPoint& mousePos)
{
    Q_ASSERT(mMovingObject);
    const QPointF projPos = mapToScene(mousePos);
    mMovingObject->projOnObjectStopMove(projPos);
}

void QGVMapActions::stopRect()
{
    if (!mSelectionRect->isSelection()) {
        return;
    }

    const QRect rect = mSelectionRect->getRect();

    if (mSelectionMode == QGV::MapSelectionMode::Zoom) {
        applyZoomRect(rect);
    } else if (mSelectionMode == QGV::MapSelectionMode::SelectionAddition) {
        applySelectionRect(rect, false);
    } else if (mSelectionMode == QGV::MapSelectionMode::SelectionReplace) {
        applySelectionRect(rect, true);
    }
}

void QGVMapActions::applyZoomRect(QRect rect)
{
    const QRectF newProjRect = mapToScene(rect).boundingRect();
    const QGVCameraState oldState = mGeoMap->geoView()->getCamera();
    const QRectF oldProjRect = oldState.projRect();
    const double scaleFactor =
            qMin(qAbs(oldProjRect.width() / newProjRect.width()), qAbs(oldProjRect.height() / newProjRect.height()));
    auto fly =
            new QGVCameraSimpleAnimation(QGVCameraActions(mGeoMap).scaleBy(scaleFactor).moveTo(newProjRect.center()));
    fly->setDuration(1500);
    fly->start(QAbstractAnimation::DeleteWhenStopped);
}

void QGVMapActions::applySelectionRect(QRect rect, bool replaceSelection)
{
    if (replaceSelection) {
        mGeoMap->unselectAll();
    }

    const QPolygonF projSelPolygon = QPolygonF() << mapToScene(rect.topLeft()) << mapToScene(rect.topRight())
                                                 << mapToScene(rect.bottomRight()) << mapToScene(rect.bottomLeft())
                                                 << mapToScene(rect.topLeft());

    auto selList = mGeoMap->search(projSelPolygon, Qt::ContainsItemShape);
    for (auto* geoObject : selList) {
        geoObject->setSelected(true);
    }
}
