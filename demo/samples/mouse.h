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

#include "demoitem.h"

#include <QGeoView/QGVWidgetText.h>

class MouseDemo : public DemoItem
{
    Q_OBJECT

public:
    explicit MouseDemo(QGVMap* geoMap, QObject* parent = 0);

    QString label() const override;
    QString comment() const override;

private:
    void onInit() override;
    void onStart() override;
    void onEnd() override;
    void startTracking(bool start);
    void enableAction(QGV::MouseAction action, bool enable);
    void onMouseMove(QPointF projPos);

private:
    QGVWidgetText* mText;
};
