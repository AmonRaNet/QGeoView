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

#include <QWidget>

#include "QGVCamera.h"
#include "QGVGlobal.h"
#include "QGVProjection.h"

class QGVItem;
class QGVDrawItem;
class QGVWidget;
class QGVMapQGScene;
class QGVMapQGView;

class QGV_LIB_DECL QGVMap : public QWidget
{
    Q_OBJECT

public:
    explicit QGVMap(QWidget* parent = 0);
    ~QGVMap();

    const QGVCameraState getCamera() const;
    void cameraTo(const QGVCameraActions& actions, bool animation = false);
    void flyTo(const QGVCameraActions& actions);

    void setProjection(QGV::Projection id);
    void setProjection(QGVProjection* projection);
    QGVProjection* getProjection() const;

    void setMouseActions(QGV::MouseActions actions);
    void setMouseAction(QGV::MouseAction action, bool enabled = true);
    QGV::MouseActions getMouseActions() const;
    bool isMouseAction(QGV::MouseAction action) const;

    QGVItem* rootItem() const;
    QGVMapQGView* geoView() const;

    void addItem(QGVItem* item);
    void removeItem(QGVItem* item);
    void deleteItems();
    int countItems() const;
    QGVItem* getItem(int index) const;

    void addWidget(QGVWidget* widget);
    void removeWidget(QGVWidget* widget);
    void deleteWidgets();
    int countWidgets() const;
    QGVWidget* getWigdet(int index) const;

    void select(QGVItem* item);
    void unselect(QGVItem* item);
    void unselectAll();
    QSet<QGVItem*> getSelections() const;

    QList<QGVDrawItem*> search(const QPointF& projPos, Qt::ItemSelectionMode mode = Qt::ContainsItemShape) const;
    QList<QGVDrawItem*> search(const QRectF& projRect, Qt::ItemSelectionMode mode = Qt::ContainsItemShape) const;
    QPixmap grabMapView(bool includeWidgets = true) const;

    QPointF mapToProj(QPoint pos);
    QPoint mapFromProj(QPointF projPos);

    void refreshMap();
    void refreshProjection();
    void anchoreWidgets();

    virtual void onMapState(QGV::MapState state);
    virtual void onMapCamera(const QGVCameraState& oldState, const QGVCameraState& newState);

protected:
    void mouseMoveEvent(QMouseEvent* event) override;

Q_SIGNALS:
    void projectionChanged();
    void scaleChanged();
    void azimuthChanged();
    void areaChanged();
    void itemsChanged(QGVItem* parent);
    void stateChanged(QGV::MapState state);
    void itemClicked(QGVItem* item, QPointF projPos);
    void itemDoubleClicked(QGVItem* item, QPointF projPos);
    void mapMouseMove(QPointF projPos);

private:
    QScopedPointer<QGVProjection> mProjection;
    QScopedPointer<QGVMapQGView> mQGView;
    QScopedPointer<QGVItem> mRootItem;
    QList<QGVWidget*> mWidgets;
    QSet<QGVItem*> mSelections;
};
