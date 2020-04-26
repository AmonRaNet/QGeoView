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

#include "customtiles.h"
#include "mytiles.h"

CustomTiles::CustomTiles(QGVMap* geoMap, QObject* parent)
    : DemoItem(geoMap, SelectorDialog::Multi, parent)
{}

QString CustomTiles::label() const
{
    return "Custom tiles";
}

QString CustomTiles::comment() const
{
    return "Here is a simple example of custom tile layer.<br>"
           "Typical usage is:<br>"
           "- the custom background maps<br>"
           "- the georeferenced maps (for example heat maps)<br>";
}

void CustomTiles::onInit()
{
    /*
     * List of demo tile layers.
     */
    const QList<QPair<QString, QGVLayer*>> layers = {
        { "My tiles(blue)", new MyTiles(targetAreaOut1(), Qt::blue) },
        { "My tiles(red)", new MyTiles(targetAreaOut2(), Qt::red) },
    };
    /*
     * Layers will be owned by map.
     */
    for (auto pair : layers) {
        auto name = pair.first;
        auto layer = pair.second;
        layer->hide();
        geoMap()->addItem(layer);
        selector()->addItem(name, std::bind(&CustomTiles::setSelected, this, layer, std::placeholders::_1));
    }
    selector()->select(0);
}

void CustomTiles::onStart()
{
    selector()->show();
    geoMap()->flyTo(QGVCameraActions(geoMap()).scaleTo(targetAreaIn()));
}

void CustomTiles::onEnd()
{
    selector()->hide();
}

QGV::GeoRect CustomTiles::targetAreaIn() const
{
    return QGV::GeoRect(QGV::GeoPos(48.236117, 11.499786), QGV::GeoPos(48.061851, 11.637178));
}

QGV::GeoRect CustomTiles::targetAreaOut1() const
{
    return QGV::GeoRect(QGV::GeoPos(48.406227, 9.731185), QGV::GeoPos(47.829682, 11.25));
}

QGV::GeoRect CustomTiles::targetAreaOut2() const
{
    return QGV::GeoRect(QGV::GeoPos(48.381619, 11.610039), QGV::GeoPos(47.804796, 13.449587));
}

void CustomTiles::setSelected(QGVLayer* layer, bool selected)
{
    layer->setVisible(selected);
}
