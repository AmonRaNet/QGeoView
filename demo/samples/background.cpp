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

#include "background.h"

#include <QGeoView/QGVGlobal.h>
#include <QGeoView/QGVLayerBing.h>
#include <QGeoView/QGVLayerGoogle.h>
#include <QGeoView/QGVLayerOSM.h>

BackgroundDemo::BackgroundDemo(QGVMap* geoMap, QObject* parent)
    : DemoItem(geoMap, SelectorDialog::Single, parent)
{}

QString BackgroundDemo::label() const
{
    return "Background maps";
}

QString BackgroundDemo::comment() const
{
    return "QGV supports multiple tile map types. This includes:<br>"
           "- OpenStreetMaps<br>"
           "- Google Maps<br>"
           "- Bing Maps<br>"
           "- Custom maps(OSM-like, for e.g MapServer)";
}

void BackgroundDemo::onInit()
{
    /*
     * Footer will be used to show fixed text about selected background layer.
     * Widget owned by map.
     */
    mFooter = new QGVWidgetText();
    geoMap()->addWidget(mFooter);
    /*
     * List of available tile maps.
     */
    const QString customURI = "http://c.tile.stamen.com/watercolor/${z}/${x}/${y}.jpg";
    const QList<QPair<QString, QGVLayer*>> layers = {
        { "OSM", new QGVLayerOSM() },
        { "GOOGLE_SATELLITE", new QGVLayerGoogle(QGV::TilesType::Satellite) },
        { "GOOGLE_HYBRID", new QGVLayerGoogle(QGV::TilesType::Hybrid) },
        { "GOOGLE_SCHEMA", new QGVLayerGoogle(QGV::TilesType::Schema) },
        { "BING_SATELLITE", new QGVLayerBing(QGV::TilesType::Satellite) },
        { "BING_HYBRID", new QGVLayerBing(QGV::TilesType::Hybrid) },
        { "BING_SCHEMA", new QGVLayerBing(QGV::TilesType::Schema) },
        { "CUSTOM_OSM", new QGVLayerOSM(customURI) },
    };
    /*
     * Layers will be owned by map.
     */
    for (auto pair : layers) {
        auto name = pair.first;
        auto layer = pair.second;
        layer->hide();
        geoMap()->addItem(layer);
        selector()->addItem(name, std::bind(&BackgroundDemo::setSelected, this, layer, std::placeholders::_1));
    }
    selector()->select(0);
}

void BackgroundDemo::onStart()
{
    selector()->show();
}

void BackgroundDemo::onEnd()
{
    selector()->hide();
}

void BackgroundDemo::setSelected(QGVLayer* layer, bool selected)
{
    if (layer == nullptr) {
        return;
    }
    layer->setVisible(selected);
    if (selected) {
        mFooter->setText(layer->getName() + ", " + layer->getDescription());
    } else {
        mFooter->setText("");
    }
}
