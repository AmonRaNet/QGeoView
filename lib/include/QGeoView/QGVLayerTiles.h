/***************************************************************************
 * QGeoView is a Qt / C ++ widget for visualizing geographic data.
 * Copyright (C) 2018-2022 Andrey Yaroshenko.
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

#include "QGVLayer.h"

#include <QElapsedTimer>

class QGV_LIB_DECL QGVLayerTiles : public QGVLayer
{
    Q_OBJECT

public:
    QGVLayerTiles();

protected:
    void onProjection(QGVMap* geoMap) override;
    void onCamera(const QGVCameraState& oldState, const QGVCameraState& newState) override;
    void onUpdate() override;
    void onClean() override;
    void onTile(const QGV::GeoTilePos& tilePos, QGVDrawItem* tileObj);

    virtual int minZoomlevel() const = 0;
    virtual int maxZoomlevel() const = 0;
    virtual int scaleToZoom(double scale) const;
    virtual void request(const QGV::GeoTilePos& tilePos) = 0;
    virtual void cancel(const QGV::GeoTilePos& tilePos) = 0;

private:
    void processCamera();
    void removeAllAbove(const QGV::GeoTilePos& tilePos);
    void removeWhenCovered(const QGV::GeoTilePos& tilePos);
    void addTile(const QGV::GeoTilePos& tilePos, QGVDrawItem* tileObj);
    void removeTile(const QGV::GeoTilePos& tilePos);
    bool isTileExists(const QGV::GeoTilePos& tilePos) const;
    bool isTileFinished(const QGV::GeoTilePos& tilePos) const;
    QList<QGV::GeoTilePos> existingTiles(int zoom) const;

private:
    int mCurZoom;
    QRect mCurRect;
    QMap<int, QMap<QGV::GeoTilePos, QGVDrawItem*>> mIndex;
    QElapsedTimer mLastAnimation;
};
