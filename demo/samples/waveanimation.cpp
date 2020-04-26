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

#include "waveanimation.h"

WaveAnimation::WaveAnimation(QGVLayer* layer, double waveWidth, int msecs)
    : mLayer(layer)
    , mWaveWidth(waveWidth)
    , mDuration(msecs)
{
    prepareWaves();
}

int WaveAnimation::duration() const
{
    return mDuration;
}

void WaveAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    if (newState == QAbstractAnimation::Running && oldState == QAbstractAnimation::Stopped) {
        mCurrentWave = 0;
    }
    if (newState == QAbstractAnimation::Stopped && oldState != QAbstractAnimation::Stopped) {
        showWave(mCurrentWave, false);
    }
}

void WaveAnimation::updateCurrentTime(int currentTime)
{
    double progress = static_cast<double>(currentTime) / duration();
    if (direction() == Direction::Backward) {
        progress = 1.0 - progress;
    }
    const int newWave = static_cast<int>(progress * mWaves.length());
    if (newWave == mCurrentWave) {
        return;
    }
    showWave(mCurrentWave, false);
    mCurrentWave = newWave;
    showWave(mCurrentWave, true);
}

void WaveAnimation::prepareWaves()
{
    QMap<int, QList<QGVDrawItem*>> waves;
    for (int index = 0; index < mLayer->countItems(); index++) {
        auto* drawItem = dynamic_cast<QGVDrawItem*>(mLayer->getItem(index));
        if (drawItem == nullptr) {
            continue;
        }
        const double x = drawItem->projShape().boundingRect().center().x();
        const int wave = static_cast<int>(x / mWaveWidth);
        waves[wave].append(drawItem);
    }
    mWaves = waves.values();
    mCurrentWave = 0;
}

void WaveAnimation::showWave(int num, bool show)
{
    if (num >= mWaves.size()) {
        return;
    }
    for (QGVDrawItem* drawItem : mWaves[num]) {
        drawItem->setFlag(QGV::ItemFlag::Highlighted, show);
    }
}
