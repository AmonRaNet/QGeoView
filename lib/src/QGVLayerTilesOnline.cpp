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

#include "QGVLayerTilesOnline.h"
#include "QGVImage.h"

QGVLayerTilesOnline::~QGVLayerTilesOnline()
{
    qDeleteAll(mRequest);
}

void QGVLayerTilesOnline::onProjection(QGVMap* geoMap)
{
    Q_ASSERT(QGV::getNetworkManager());
    QGVLayerTiles::onProjection(geoMap);
    connect(QGV::getNetworkManager(), &QNetworkAccessManager::finished, this, &QGVLayerTilesOnline::onReplyFinished);
}

void QGVLayerTilesOnline::onClean()
{
    Q_ASSERT(QGV::getNetworkManager());
    disconnect(QGV::getNetworkManager(), 0, this, 0);
}

void QGVLayerTilesOnline::request(const QGV::GeoTilePos& tilePos)
{
    const QUrl url(tilePosToUrl(tilePos));
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent",
                         "Mozilla/5.0 (Windows; U; MSIE "
                         "6.0; Windows NT 5.1; SV1; .NET "
                         "CLR 2.0.50727)");
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    QNetworkReply* reply = QGV::getNetworkManager()->get(request);
    reply->setProperty("TILE_OWNER", QVariant::fromValue(this));
    reply->setProperty("TILE_REQUEST", true);
    reply->setProperty("TILE_POS", QVariant::fromValue(tilePos));
    mRequest[tilePos] = reply;
    qgvDebug() << "request" << url;
}

void QGVLayerTilesOnline::cancel(const QGV::GeoTilePos& tilePos)
{
    removeReply(tilePos);
}

void QGVLayerTilesOnline::onReplyFinished(QNetworkReply* reply)
{
    const auto tileRequest = reply->property("TILE_REQUEST").toBool();
    if (!tileRequest) {
        return;
    }
    const auto tileOwner = reply->property("TILE_OWNER").value<QGVLayerTilesOnline*>();
    if (tileOwner != this) {
        return;
    }
    const auto tilePos = reply->property("TILE_POS").value<QGV::GeoTilePos>();

    if (reply->error() != QNetworkReply::NoError) {
        if (reply->error() != QNetworkReply::OperationCanceledError) {
            qgvCritical() << "ERROR" << reply->errorString();
        }
        removeReply(tilePos);
        return;
    }
    const auto rawImage = reply->readAll();
    auto tile = new QGVImage();
    tile->setGeometry(tilePos.toGeoRect());
    tile->loadImage(rawImage);
    tile->setProperty("drawDebug",
                      QString("%1\ntile(%2,%3,%4)")
                              .arg(reply->url().toString())
                              .arg(tilePos.zoom())
                              .arg(tilePos.pos().x())
                              .arg(tilePos.pos().y()));
    removeReply(tilePos);
    onTile(tilePos, tile);
}

void QGVLayerTilesOnline::removeReply(const QGV::GeoTilePos& tilePos)
{
    QNetworkReply* reply = mRequest.value(tilePos, nullptr);
    if (reply == nullptr) {
        return;
    }
    mRequest.remove(tilePos);
    reply->abort();
    reply->close();
    reply->deleteLater();
}
