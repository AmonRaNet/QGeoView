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

#include "items.h"
#include "ellipse.h"
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <random>
#endif

ItemsDemo::ItemsDemo(QGVMap* geoMap, QObject* parent)
    : DemoItem(geoMap, SelectorDialog::Multi, parent)
{}

QString ItemsDemo::label() const
{
    return "Layers and items";
}

QString ItemsDemo::comment() const
{
    return "QGV supports the tree-like storage for geo-map items. All QGV items should be derived "
           "from QGVItem, where each QGVItem can belong to a map or any other QGVItem.<br>"
           "QGVItem itself is not rendered on QGVMap and is only used to organize items. "
           "For example, QGVLayer is not directly represented on map, but instead is used as a container "
           "of other items. All visual items are derived from QGVDrawItem.<br>"
           "Items always get parental properties  (like visibility, opacity, etc).<br>"
           "QGVMap::rootItem() is a map root item, which owns all other items.";
}

void ItemsDemo::onInit()
{
    /*
     * List of demo color layers.
     */
    const QList<QPair<QString, QColor>> colors = {
        { "Blue Layer", Qt::blue },
        { "Red Layer", Qt::red },
        { "Green Layer", Qt::green },
    };
    /*
     * Layers will be owned by map.
     */
    for (auto pair : colors) {
        auto name = pair.first;
        auto color = pair.second;
        auto layer = new QGVLayer();
        layer->setName(name);
        layer->setDescription("Demo for " + name);
        geoMap()->addItem(layer);
        selector()->addItem(name, std::bind(&ItemsDemo::setSelected, this, layer, std::placeholders::_1));
        /*
         * Items will be owned by layer.
         */
        for (int i = 0; i < 10; i++) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            auto radius = 100 + std::rand() % 1000;
#else
            auto radius = 100 + qrand() % 1000;
#endif
            layer->addItem(new Ellipse(randRect(targetArea(), QSizeF(radius, radius)), color));
        }
    }
    selector()->selectAll();
}

void ItemsDemo::onStart()
{
    selector()->show();
    geoMap()->flyTo(QGVCameraActions(geoMap()).scaleTo(targetArea()));
}

void ItemsDemo::onEnd()
{
    selector()->hide();
}

QGV::GeoRect ItemsDemo::targetArea() const
{
    return QGV::GeoRect(QGV::GeoPos(46.419424, 28.888922), QGV::GeoPos(46.282726, 29.119152));
    ;
}

void ItemsDemo::setSelected(QGVLayer* layer, bool selected)
{
    if (layer == nullptr) {
        return;
    }
    layer->setVisible(selected);
}
