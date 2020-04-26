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

#include "selectordialog.h"

SelectorDialog::SelectorDialog(Type type, QObject* parent)
    : QObject(parent)
{
    mSelector.reset(new QListWidget());
    mType = type;
    connect(mSelector.data(), &QListWidget::itemChanged, this, &SelectorDialog::onItemChanged);
}

int SelectorDialog::addItem(const QString& name, FeatureFunc func)
{
    QListWidgetItem* item = new QListWidgetItem(mSelector.data());
    item->setText(name);
    item->setToolTip(name);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Unchecked);
    item->setData(Qt::UserRole, QVariant::fromValue(func));
    return mSelector->row(item);
}

QString SelectorDialog::name(int idx) const
{
    return mSelector->item(idx)->text();
}

void SelectorDialog::select(int idx)
{
    QListWidgetItem* target = mSelector->item(idx);
    if (mType == Single) {
        for (int i = 0; i < mSelector->count(); i++) {
            auto item = mSelector->item(i);
            if (i != idx) {
                item->setCheckState(Qt::Unchecked);
            }
        }
    }
    target->setCheckState(Qt::Checked);
}

void SelectorDialog::selectAll()
{
    for (int i = 0; i < mSelector->count(); i++) {
        select(i);
    }
}

void SelectorDialog::unselect(int idx)
{
    QListWidgetItem* target = mSelector->item(idx);
    target->setCheckState(Qt::Unchecked);
}

void SelectorDialog::unselectAll()
{
    for (int i = 0; i < mSelector->count(); i++) {
        unselect(i);
    }
}

void SelectorDialog::show()
{
    mSelector->show();
}

void SelectorDialog::hide()
{
    mSelector->hide();
}

void SelectorDialog::onItemChanged(QListWidgetItem* item)
{
    int idx = mSelector->row(item);
    if (item->checkState() == Qt::Checked) {
        select(idx);
    }
    auto func = item->data(Qt::UserRole).value<FeatureFunc>();
    if (func) {
        func(item->checkState() == Qt::Checked);
    }
}
