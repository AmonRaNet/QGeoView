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

#include "mytiles.h"
#include "mytile.h"

MyTiles::MyTiles(const QGV::GeoRect& activeRect, QColor color)
    : mActiveGeoRect(activeRect)
    , mColor(color)
{
    setZValue(-1);
}

void MyTiles::onProjection(QGVMap* geoMap)
{
    QGVLayerTiles::onProjection(geoMap);
    mActiveTileRect = QRect(QGV::GeoTilePos::geoToTilePos(minZoomlevel(), mActiveGeoRect.topLeft()).pos(),
                            QGV::GeoTilePos::geoToTilePos(minZoomlevel(), mActiveGeoRect.bottomRight()).pos());
}

int MyTiles::minZoomlevel() const
{
    return 9;
}

int MyTiles::maxZoomlevel() const
{
    return 14;
}

void MyTiles::request(const QGV::GeoTilePos& tilePos)
{
    auto tilePosParent = tilePos.parent(minZoomlevel()).pos();
    if (!mActiveTileRect.contains(tilePosParent)) {
        return;
    }
    QGVDrawItem* tile = new MyTile(tilePos, mColor);
    tile->setOpacity(0.5);
    tile->setSelectable(false);
    onTile(tilePos, tile);
}

void MyTiles::cancel(const QGV::GeoTilePos& /*tilePos*/)
{
    qt_noop();
}
