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

#include "flags.h"
#include "rectangle.h"

#include <QGeoView/QGVLayer.h>

FlagsDemo::FlagsDemo(QGVMap* geoMap, QObject* parent)
    : DemoItem(geoMap, SelectorDialog::Multi, parent)
{}

QString FlagsDemo::label() const
{
    return "Item flags";
}

QString FlagsDemo::comment() const
{
    return "QGVDrawItem supports several flags or they combinations. Like:<br>"
           "- ignore the map azimuth<br>"
           "- ignore the map scale<br>"
           "- item highlight<br>"
           "- item move<br>"
           "- mouse tracking<br>"
           "- custom transformation, highlight and selection<br><br>"
           "The behavior of the flag should be visible by appropriate action (for example: selection, zooming or "
           "mouse move).<br>"
           "Use the context menu to see other properties, such as the z-level, opacity.";
}

void FlagsDemo::onInit()
{
    const auto target = targetArea();
    const int size = 1000;
    /*
     * List of demo custom items.
     */
    const QList<QPair<QString, QGVDrawItem*>> items = {
        { "Ignore azimuth(blue)", new Rectangle(randRect(target, size), QGV::ItemFlag::IgnoreAzimuth, Qt::blue) },
        { "Ignore scale(red)", new Rectangle(randRect(target, 40), QGV::ItemFlag::IgnoreScale, Qt::red) },
        { "Highlightable(green)", new Rectangle(randRect(target, size), QGV::ItemFlag::Highlightable, Qt::green) },
        { "Highlightable-custom(gray)",
          new Rectangle(
                  randRect(target, size), QGV::ItemFlag::Highlightable | QGV::ItemFlag::HighlightCustom, Qt::gray) },
        { "Custom selectable(cyan)", new Rectangle(randRect(target, size), QGV::ItemFlag::SelectCustom, Qt::cyan) },
        { "Clickable by mouse(magenta)", new Rectangle(randRect(target, size), QGV::ItemFlag::Clickable, Qt::magenta) },
        { "Custom transform(yellow)", new Rectangle(randRect(target, size), QGV::ItemFlag::Transformed, Qt::yellow) },
        { "Movable(darkCyan)",
          new Rectangle(randRect(target, size), QGV::ItemFlag::Movable | QGV::ItemFlag::Transformed, Qt::darkCyan) },
    };
    /*
     * Layers will be owned by map.
     */
    auto layer = new QGVLayer();
    layer->setName("Demo for flags");
    layer->setDescription("Demo for flags");
    /*
     * Items will be owned by layer.
     */
    for (auto pair : items) {
        auto name = pair.first;
        auto item = pair.second;
        item->setProperty("drawDebug", name);
        if (item->isFlag(QGV::ItemFlag::Clickable)) {
            item->setSelectable(false);
        }
        layer->addItem(item);
        selector()->addItem(name, std::bind(&FlagsDemo::setSelected, this, item, std::placeholders::_1));
    }
    geoMap()->addItem(layer);
    selector()->selectAll();
}

void FlagsDemo::onStart()
{
    selector()->show();
    geoMap()->flyTo(QGVCameraActions(geoMap()).scaleTo(targetArea()));
}

void FlagsDemo::onEnd()
{
    selector()->hide();
}

QGV::GeoRect FlagsDemo::targetArea() const
{
    return QGV::GeoRect(QGV::GeoPos(51.848624, 14.325923), QGV::GeoPos(51.743758, 14.453527));
}

void FlagsDemo::setSelected(QGVItem* item, bool selected)
{
    if (item == nullptr) {
        return;
    }
    item->setVisible(selected);
}
