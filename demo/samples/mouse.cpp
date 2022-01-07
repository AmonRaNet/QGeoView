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

#include "mouse.h"

MouseDemo::MouseDemo(QGVMap* geoMap, QObject* parent)
    : DemoItem(geoMap, SelectorDialog::Multi, parent)
{}

QString MouseDemo::label() const
{
    return "Mouse actions";
}

QString MouseDemo::comment() const
{
    return "QGV supports different mouse actions, like:<br>"
           "- mouse tracking<br>"
           "- zoom by mouse wheel<br>"
           "- zoom by rubber band(right button)<br>"
           "- item selection by left button or rubber band (right button + ctrl/shift)<br>"
           "- unselect all items by double<br>"
           "- map move by left button<br>"
           "- object move by left button + alt<br>"
           "- context menu by right button<br>"
           "- item tooltip<br>";
}

void MouseDemo::onInit()
{
    /*
     * QGVWidgetText will be used to show current position.
     * Widgets owned by map.
     */
    mText = new QGVWidgetText();
    mText->setAnchor(QPoint(0, 0), { Qt::TopEdge });
    geoMap()->addWidget(mText);
    connect(geoMap(), &QGVMap::mapMouseMove, this, &MouseDemo::onMouseMove);
    /*
     * Features selector
     */
    const QList<QPair<QString, FeatureFunc>> actions = {
        { "Move tracking", std::bind(&MouseDemo::startTracking, this, std::placeholders::_1) },
        { "Zoom(wheel)",
          std::bind(&MouseDemo::enableAction, this, QGV::MouseAction::ZoomWheel, std::placeholders::_1) },
        { "Zoom(rect)", std::bind(&MouseDemo::enableAction, this, QGV::MouseAction::ZoomRect, std::placeholders::_1) },
        { "Selection", std::bind(&MouseDemo::enableAction, this, QGV::MouseAction::Selection, std::placeholders::_1) },
        { "Move", std::bind(&MouseDemo::enableAction, this, QGV::MouseAction::Move, std::placeholders::_1) },
        { "MoveObjects",
          std::bind(&MouseDemo::enableAction, this, QGV::MouseAction::MoveObjects, std::placeholders::_1) },
        { "Context menu",
          std::bind(&MouseDemo::enableAction, this, QGV::MouseAction::ContextMenu, std::placeholders::_1) },
        { "Tooltip", std::bind(&MouseDemo::enableAction, this, QGV::MouseAction::Tooltip, std::placeholders::_1) },
    };
    for (auto pair : actions) {
        auto name = pair.first;
        auto func = pair.second;
        selector()->addItem(name, func);
    }
    selector()->selectAll();
}

void MouseDemo::onStart()
{
    selector()->show();
}

void MouseDemo::onEnd()
{
    selector()->hide();
}

void MouseDemo::startTracking(bool start)
{
    geoMap()->setMouseTracking(start);
}

void MouseDemo::enableAction(QGV::MouseAction action, bool enable)
{
    geoMap()->setMouseAction(action, enable);
}

void MouseDemo::onMouseMove(QPointF projPos)
{
    /*
     * Current projection position can be converted to geo-coordinates and
     * printed by corresponding functions
     */
    auto geoPos = geoMap()->getProjection()->projToGeo(projPos);
    mText->setText(QString("<b>%1, %2</b>").arg(geoPos.latToString()).arg(geoPos.lonToString()));
}
