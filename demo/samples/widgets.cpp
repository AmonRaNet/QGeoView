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

#include "widgets.h"

#include <QGeoView/QGVGlobal.h>
#include <QGeoView/QGVWidgetCompass.h>
#include <QGeoView/QGVWidgetScale.h>
#include <QGeoView/QGVWidgetZoom.h>

WidgetsDemo::WidgetsDemo(QGVMap* geoMap, QObject* parent)
    : DemoItem(geoMap, SelectorDialog::Multi, parent)
{}

QString WidgetsDemo::label() const
{
    return "Widgets";
}

QString WidgetsDemo::comment() const
{
    return "QGV supports standard map widgets. This includes:<br>"
           "- Compass<br>"
           "- Zoom buttons<br>"
           "- Scale metrics<br>";
}

void WidgetsDemo::onInit()
{
    /*
     * List of available widgets.
     */
    QList<QPair<QString, QGVWidget*>> widgets = {
        { "Compass", new QGVWidgetCompass() },
        { "ZoomButtons", new QGVWidgetZoom() },
        { "ScaleHorizontal", new QGVWidgetScale(Qt::Horizontal) },
        { "ScaleVertical", new QGVWidgetScale(Qt::Vertical) },
    };
    /*
     * Widgets will be owned by map.
     */
    for (auto pair : widgets) {
        auto name = pair.first;
        auto widget = pair.second;
        geoMap()->addWidget(widget);
        selector()->addItem(name, std::bind(&WidgetsDemo::setSelected, this, widget, std::placeholders::_1));
    }
    selector()->selectAll();
}

void WidgetsDemo::onStart()
{
    selector()->show();
}

void WidgetsDemo::onEnd()
{
    selector()->hide();
}

void WidgetsDemo::setSelected(QGVWidget* widget, bool selected)
{
    if (widget == nullptr) {
        return;
    }
    widget->setVisible(selected);
}
