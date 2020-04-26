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

#include <QListWidget>
#include <QObject>
#include <functional>

using FeatureFunc = std::function<void(bool)>;
Q_DECLARE_METATYPE(FeatureFunc);

class SelectorDialog : public QObject
{
    Q_OBJECT

public:
    enum Type
    {
        Single,
        Multi
    };
    explicit SelectorDialog(Type type, QObject* parent = 0);

    int addItem(const QString& name, FeatureFunc func);
    QString name(int idx) const;

    void select(int idx);
    void selectAll();
    void unselect(int idx);
    void unselectAll();

    void show();
    void hide();

private Q_SLOTS:
    void onItemChanged(QListWidgetItem* item);

private:
    QScopedPointer<QListWidget> mSelector;
    Type mType;
};
