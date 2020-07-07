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

#include "QGVLayerTiles.h"
#include <QThread>
#include <QFile>

class QGvTilesLoader: public QThread
{
  Q_OBJECT

public:
  QGvTilesLoader(QObject *parent = 0):
    QThread(parent)
  {
    setObjectName("TMSLoader");
    moveToThread(this);
    start();
  }

public slots:
  void  request(QString url, QGV::GeoTilePos tilePos)
  {
    if (QFile::exists(url))
    {
      const auto  image = new QImage(url);
      emit        replyFinished(image, tilePos);
    }
  }

signals:
  void  replyFinished(QImage *image, QGV::GeoTilePos tilePos);

  // QThread interface

protected:
  void  run()
  {
    exec();
  }
};

class QGV_LIB_DECL  QGVLayerTilesOffline: public QGVLayerTiles
{
  Q_OBJECT

public:
  QGVLayerTilesOffline(QString url);

  ~QGVLayerTilesOffline();

protected:
  virtual QString  tilePosToUrl(const QGV::GeoTilePos &tilePos) const;

  // QGVLayerTiles interface

protected:
  int   minZoomlevel() const;

  int   maxZoomlevel() const;

private slots:
  void  replyFinished(QImage *image, QGV::GeoTilePos tilePos);

private:
  void  onProjection(QGVMap *geoMap) override;

  void  onClean() override;

  void  request(const QGV::GeoTilePos &tilePos) override;

  void  cancel(const QGV::GeoTilePos &tilePos) override;

private:
  QGvTilesLoader                         *mQGvTilesLoader;
  QMap<QGV::GeoTilePos, QNetworkReply *>  mRequest;
  QString                                 mUrl;
};
