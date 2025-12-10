/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025 Ivan Odinets <i_odinets@protonmail.com>
 *
 * This file is part of draupnir-lib
 *
 * draupnir-lib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * draupnir-lib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with draupnir-lib; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "draupnir/loptr/ui/widgets/RotateScreenOverlay.h"

#include <QPainter>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QMouseEvent>

namespace Draupnir::Loptr
{

RotateScreenOverlay::RotateScreenOverlay(const QPixmap& pix, QWidget* parent) :
    QWidget{parent},
    m_pixmap(pix),
    m_angle(0.0)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground, false);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);

    setCursor(Qt::BlankCursor);
    resize(m_pixmap.size());
}

void RotateScreenOverlay::setAngle(double a) {
    if (qFuzzyCompare(m_angle, a))
        return;

    m_angle = a;
    update();
}

void RotateScreenOverlay::startAnimation(int durationMs)
{
    auto* anim = new QPropertyAnimation(this, "angle", this);
    anim->setStartValue(0.0);
    anim->setEndValue(360.0);
    anim->setDuration(durationMs);
    anim->setEasingCurve(QEasingCurve::InOutQuad);

    connect(anim, &QPropertyAnimation::finished, this, [this] {
        close();
    });

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void RotateScreenOverlay::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);

    QPointF center(width() / 2.0, height() / 2.0);
    p.translate(center);
    p.rotate(m_angle);
    p.translate(-m_pixmap.width() / 2.0, -m_pixmap.height() / 2.0);

    p.drawPixmap(0, 0, m_pixmap);
}

}; // namespace Draupnir::Loptr
