/***************************************************************************
 * QGeoView is a Qt / C ++ widget for visualizing geographic data.
 * Copyright (C) 2018-2024 Andrey Yaroshenko.
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

#include "placemark.h"

#include <QNetworkReply>

Placemark::Placemark(const QGV::GeoPos& geoPos)
{
    setFlag(QGV::ItemFlag::Highlightable);
    setFlag(QGV::ItemFlag::Transformed);
    setGeometry(geoPos, QSize(32, 32));

    load(QUrl{ "http://maps.google.com/mapfiles/kml/paddle/blu-circle.png" });
}

void Placemark::load(const QUrl& url)
{
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent",
                         "Mozilla/5.0 (Windows; U; MSIE "
                         "6.0; Windows NT 5.1; SV1; .NET "
                         "CLR 2.0.50727)");
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);

    QNetworkReply* reply = QGV::getNetworkManager()->get(request);
    connect(reply, &QNetworkReply::finished, reply, [reply, this]() {
        if (reply->error() != QNetworkReply::NoError) {
            qgvCritical() << "ERROR" << reply->errorString();
            reply->deleteLater();
            return;
        }
        loadImage(reply->readAll());
        reply->deleteLater();
    });

    qgvDebug() << "request" << url;
}

QTransform Placemark::projTransform() const
{
    return isFlag(QGV::ItemFlag::Highlighted) ? QGV::createTransfromScale(projAnchor(), 1.2) : QTransform();
}

void Placemark::projOnFlags()
{
    setOpacity(isFlag(QGV::ItemFlag::Highlighted) ? 0.3 : 1.0);
}
