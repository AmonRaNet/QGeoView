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

#include <QGeoView/QGVMap.h>
#include <QObject>

#include "selectordialog.h"

class DemoItem : public QObject
{
    Q_OBJECT
public:
    explicit DemoItem(QGVMap* geoMap, SelectorDialog::Type type, QObject* parent = 0);
    virtual ~DemoItem() = default;

    QGVMap* geoMap() const;
    SelectorDialog* selector() const;

    virtual QString label() const = 0;
    virtual QString comment() const = 0;
    void init();
    void start();
    void end();

    QGV::GeoPos randPos(const QGV::GeoRect& targetArea);
    QGV::GeoRect randRect(const QGV::GeoRect& targetArea, const QSizeF& size);
    QGV::GeoRect randRect(const QGV::GeoRect& targetArea, int baseSize);
    QSizeF randSize(int baseSize);

protected:
    virtual void onInit() = 0;
    virtual void onStart() = 0;
    virtual void onEnd() = 0;

Q_SIGNALS:
    void ready(DemoItem* item);
    void started(DemoItem* item);
    void ended(DemoItem* item);

private:
    QGVMap* mGeoMap;
    QScopedPointer<SelectorDialog> mSelectorDialog;
};
