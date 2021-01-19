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

#include "QGVLayerTilesOffline.h"
#include "QGVImage.h"
#include <QFile>

QGvTilesLoader::QGvTilesLoader(QObject* parent)
    : QThread(parent)
{
    setObjectName("TMSLoader");
    moveToThread(this);
    start();
}

void QGvTilesLoader::request(QString url, QGV::GeoTilePos tilePos)
{
    if (QFile::exists(url)) {
        auto image = QImagePtr::create(url);
        Q_EMIT replyFinished(image, tilePos);
    }
}

void QGvTilesLoader::run()
{
    exec();
}

QGVLayerTilesOffline::QGVLayerTilesOffline(QString url)
    : mUrl(url)
{
    setName("OfflineTMSMap");
    setDescription("TMS Map");
    qRegisterMetaType<QImagePtr>("QImagePtr");
    qRegisterMetaType<QGV::GeoTilePos>("QGV::GeoTilePos");

    mQGvTilesLoader = new QGvTilesLoader();
    connect(mQGvTilesLoader,
            &QGvTilesLoader::replyFinished,
            this,
            &QGVLayerTilesOffline::replyFinished,
            Qt::QueuedConnection);
    connect(this, &QGVLayerTilesOffline::requestCome, mQGvTilesLoader, &QGvTilesLoader::request, Qt::QueuedConnection);
    mQGvTilesLoader->start();
}

QGVLayerTilesOffline::~QGVLayerTilesOffline()
{
    disconnect(mQGvTilesLoader, &QGvTilesLoader::replyFinished, this, &QGVLayerTilesOffline::replyFinished);

    mQGvTilesLoader->quit();
    if (!mQGvTilesLoader->wait()) {
        mQGvTilesLoader->terminate();
    }
}

QString QGVLayerTilesOffline::tilePosToUrl(const QGV::GeoTilePos& tilePos) const
{
    QString url = mUrl;

    url.replace("${z}", QString::number(tilePos.zoom()));
    url.replace("${x}", QString::number(tilePos.pos().x()));
    url.replace("${y}", QString::number(tilePos.pos().y()));

    return url;
}

void QGVLayerTilesOffline::onProjection(QGVMap* geoMap)
{
    QGVLayerTiles::onProjection(geoMap);
}

void QGVLayerTilesOffline::onClean()
{}

void QGVLayerTilesOffline::request(const QGV::GeoTilePos& tilePos)
{
    const QString url(tilePosToUrl(tilePos));

    Q_EMIT requestCome(url, tilePos);
}

void QGVLayerTilesOffline::cancel(const QGV::GeoTilePos& tilePos)
{}

int QGVLayerTilesOffline::minZoomlevel() const
{
    return 0;
}

int QGVLayerTilesOffline::maxZoomlevel() const
{
    return 20;
}

void QGVLayerTilesOffline::replyFinished(QImagePtr image, QGV::GeoTilePos tilePos)
{
    if (image) {
        const QString url(tilePosToUrl(tilePos));
        auto tile = new QGVImage();
        tile->setGeometry(tilePos.toGeoRect());
        tile->loadImage(*image);
        tile->setProperty("drawDebug",
                          QString("%1\ntile(%2,%3,%4)")
                                  .arg(url)
                                  .arg(tilePos.zoom())
                                  .arg(tilePos.pos().x())
                                  .arg(tilePos.pos().y()));
        onTile(tilePos, tile);
        qgvDebug() << "request" << url;
    }
}
