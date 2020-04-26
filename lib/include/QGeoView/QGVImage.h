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

#include "QGVDrawItem.h"
#include <QNetworkReply>

class QGV_LIB_DECL QGVImage : public QGVDrawItem
{
    Q_OBJECT

public:
    QGVImage();

    void setGeometry(const QGV::GeoRect& geoRect);
    void setGeometry(const QGV::GeoPos& geoPos, const QSize& imageSize = QSize(), const QPoint& imageAnchor = QPoint());

    QImage getImage() const;
    bool isImage() const;

    void load(const QString& url);
    void loadImage(const QByteArray& rawData);
    void loadImage(const QImage& image);

protected:
    void onProjection(QGVMap* geoMap) override;
    QPainterPath projShape() const override;
    QPointF projAnchor() const override;
    void projPaint(QPainter* painter) override;

private:
    void onReplyFinished();
    void calculateGeometry();

private:
    enum class GeometryType
    {
        None,
        ByRect,
        ByPos,
    } mGeometryType;
    QGV::GeoRect mGeoRect;
    QGV::GeoPos mGeoPos;
    QSize mImageSize;
    QPoint mImageAnchor;
    QRectF mProjRect;
    QPointF mProjAnchor;
    QString mUrl;
    QImage mImage;
    QScopedPointer<QNetworkReply> mReply;
};
