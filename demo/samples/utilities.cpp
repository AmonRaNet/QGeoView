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

#include "utilities.h"
#include "ellipse.h"
#include "placemark.h"
#include "waveanimation.h"

#include <QPainter>

UtilitiesDemo::UtilitiesDemo(QGVMap* geoMap, QObject* parent)
    : DemoItem(geoMap, SelectorDialog::Multi, parent)
{}

QString UtilitiesDemo::label() const
{
    return "Utilities";
}

QString UtilitiesDemo::comment() const
{
    return "A set of additional features and demos.";
}

void UtilitiesDemo::onInit()
{
    /*
     * Layers owned by map.
     */
    mQGV = createQGVLayer();
    m10000 = create10000Layer();
    mQGVAnimation.reset(createQGVAnimation());

    selector()->addItem("QGV", std::bind(&UtilitiesDemo::showQGV, this, std::placeholders::_1));
    selector()->addItem("QGV animation", std::bind(&UtilitiesDemo::animationQGV, this, std::placeholders::_1));
    selector()->addItem("10000 elements", std::bind(&UtilitiesDemo::show10000, this, std::placeholders::_1));
    selector()->addItem("Draw debug", std::bind(&UtilitiesDemo::drawDebug, this, std::placeholders::_1));
    selector()->addItem("Print debug(log)", std::bind(&UtilitiesDemo::printDebug, this, std::placeholders::_1));
}

void UtilitiesDemo::onStart()
{
    selector()->show();
}

void UtilitiesDemo::onEnd()
{
    selector()->hide();
}

QGV::GeoRect UtilitiesDemo::targetQGVArea() const
{
    return QGV::GeoRect(QGV::GeoPos(55.096754, 82.682533), QGV::GeoPos(54.862608, 83.138179));
}

QPixmap UtilitiesDemo::createQGVImage()
{
    const auto target = QSize(150, 150);
    const auto path = QGV::createTextPath(QRect(QPoint(0, 0), target), "QGeoView", QFont(), 1);
    QImage image(target, QImage::Format_ARGB32_Premultiplied);
    image.fill(qRgba(0, 0, 0, 0));
    QPixmap pixmap = QPixmap::fromImage(image, Qt::NoFormatConversion);
    QPainter painter(&pixmap);
    QPen pen = QPen(Qt::black);
    pen.setWidth(1);
    pen.setCosmetic(true);
    painter.setPen(pen);
    QBrush brush = QBrush(Qt::black);
    painter.setBrush(brush);
    painter.drawPath(path);
    return pixmap;
}

QGVLayer* UtilitiesDemo::createQGVLayer()
{
    const auto target = targetQGVArea();
    const auto sizeLon = target.lonLeft() - target.lonRigth();
    const auto sizeLat = target.latTop() - target.latBottom();
    /*
     * Layers owned by map.
     */
    auto layer = new QGVLayer();
    layer->setName("QGV");
    layer->setDescription("Demo for QGV");
    geoMap()->addItem(layer);
    /*
     * Items will be owned by layer.
     */
    QImage image = createQGVImage().toImage();
    for (int x = 0; x < image.width(); x++) {
        for (int y = 0; y < image.height(); y++) {
            if (image.pixelColor(x, y) != Qt::black)
                continue;
            double xScale = (double)x / image.width();
            double yScale = (double)y / image.height();
            QGV::GeoPos geoPos = QGV::GeoPos(target.latTop() - sizeLat * yScale, target.lonLeft() - sizeLon * xScale);
            auto image = new Placemark(geoPos);
            layer->addItem(image);
        }
    }

    qInfo() << layer->countItems() << "items in QGV demo layer";
    return layer;
}

QAbstractAnimation* UtilitiesDemo::createQGVAnimation()
{
    const double w = geoMap()->getProjection()->geoToProj(targetQGVArea()).width();
    auto animation = new WaveAnimation(mQGV, w / 20, 3000);
    animation->setLoopCount(-1);
    return animation;
}

QGV::GeoRect UtilitiesDemo::target10000Area() const
{
    return geoMap()->getProjection()->boundaryGeoRect();
}

QGVLayer* UtilitiesDemo::create10000Layer()
{
    /*
     * Layers will be owned by map.
     */
    auto target = target10000Area();
    auto layer = new QGVLayer();
    layer->setName("10000 elements");
    layer->setDescription("Demo for 10000 elements");
    geoMap()->addItem(layer);
    /*
     * Items will be owned by layer.
     */
    const int radius = 30000;
    for (int i = 0; i < 10000; i++) {
        auto ellipse = new Ellipse(randRect(target, QSizeF(radius, radius)), Qt::red);
        layer->addItem(ellipse);
    }
    return layer;
}

void UtilitiesDemo::showQGV(bool selected)
{
    mQGV->setVisible(selected);
    if (selected) {
        geoMap()->flyTo(QGVCameraActions(geoMap()).scaleTo(targetQGVArea()));
    }
}

void UtilitiesDemo::animationQGV(bool selected)
{
    if (selected) {
        mQGVAnimation->start();
    } else {
        mQGVAnimation->stop();
    }
}

void UtilitiesDemo::show10000(bool selected)
{
    m10000->setVisible(selected);
    if (selected) {
        geoMap()->flyTo(QGVCameraActions(geoMap()).scaleTo(target10000Area()));
    }
}

void UtilitiesDemo::drawDebug(bool selected)
{
    QGV::setDrawDebug(selected);
    geoMap()->refreshMap();
}

void UtilitiesDemo::printDebug(bool selected)
{
    QGV::setPrintDebug(selected);
}
