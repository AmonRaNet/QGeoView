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

#include "QGVImage.h"
#include "QGVMap.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPainter>

QGVImage::QGVImage()
    : mGeometryType(GeometryType::ByRect)
{}

void QGVImage::setGeometry(const QGV::GeoRect& geoRect)
{
    mGeometryType = GeometryType::ByRect;
    mGeoRect = geoRect;
    calculateGeometry();
}

void QGVImage::setGeometry(const QGV::GeoPos& geoPos, const QSize& imageSize, const QPoint& imageAnchor)
{
    mGeometryType = GeometryType::ByPos;
    mGeoPos = geoPos;
    mImageSize = imageSize;
    mImageAnchor = imageAnchor;
    calculateGeometry();
}

QImage QGVImage::getImage() const
{
    return mImage;
}

bool QGVImage::isImage() const
{
    return !mImage.isNull();
}

void QGVImage::load(const QString& url)
{
    Q_ASSERT(QGV::getNetworkManager());
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent",
                         "Mozilla/5.0 (Windows; U; MSIE "
                         "6.0; Windows NT 5.1; SV1; .NET "
                         "CLR 2.0.50727)");
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    mReply.reset(QGV::getNetworkManager()->get(request));
    connect(mReply.data(), &QNetworkReply::finished, this, &QGVImage::onReplyFinished);
    qgvDebug() << "request" << url;
}

void QGVImage::loadImage(const QByteArray& rawData)
{
    QImage image;
    image.loadFromData(rawData);
    loadImage(image);
}

void QGVImage::loadImage(const QImage& image)
{
    mImage = image;
}

void QGVImage::onProjection(QGVMap* geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    calculateGeometry();
}

QPainterPath QGVImage::projShape() const
{
    QPainterPath path;
    path.addRect(mProjRect);
    return path;
}

QPointF QGVImage::projAnchor() const
{
    if (mProjAnchor.isNull()) {
        return QGVDrawItem::projAnchor();
    }
    return mProjAnchor;
}

void QGVImage::projPaint(QPainter* painter)
{
    if (mImage.isNull() || mProjRect.isEmpty()) {
        return;
    }
    QRectF paintRect = mProjRect;
    if (!isFlag(QGV::ItemFlag::IgnoreScale)) {
        const double pixelFactor = 1.0 / getMap()->getCamera().scale();
        paintRect.setSize(paintRect.size() + QSizeF(pixelFactor, pixelFactor));
    }
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawImage(paintRect, getImage());
}

void QGVImage::onReplyFinished()
{
    if (mReply.isNull()) {
        return;
    }
    if (mReply->error() != QNetworkReply::NoError) {
        qgvCritical() << "ERROR" << mReply->errorString();
        mReply.reset(nullptr);
        return;
    }
    loadImage(mReply->readAll());
    mReply.reset(nullptr);
}

void QGVImage::calculateGeometry()
{
    mProjRect = {};
    mProjAnchor = {};
    if (mGeometryType == GeometryType::None || getMap() == nullptr) {
        return;
    }
    if (mGeometryType == GeometryType::ByRect) {
        mProjRect = getMap()->getProjection()->geoToProj(mGeoRect);
        resetBoundary();
        refresh();
    } else if (mGeometryType == GeometryType::ByPos) {
        if (mImageSize.isEmpty() && mImage.isNull()) {
            return;
        }
        const QPointF anchor =
                (mImageAnchor.isNull()) ? QPointF(mImageSize.width() / 2, mImageSize.height() / 2) : mImageAnchor;
        const QPointF basePos = getMap()->getProjection()->geoToProj(mGeoPos);
        mProjRect = QRectF(basePos - anchor, mImageSize);
        mProjAnchor = basePos;
        resetBoundary();
        refresh();
    }
}
