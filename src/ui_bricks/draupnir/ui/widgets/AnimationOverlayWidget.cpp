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

#include "draupnir/ui_bricks/ui/widgets/AnimationOverlayWidget.h"

namespace Draupnir::Ui
{

AnimationOverlayWidget::AnimationOverlayWidget(const QPixmap& pixmap, QWidget* parent) :
    QWidget{parent},
    m_originalPixmap{pixmap}
{
    resize(m_originalPixmap.size());
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAutoFillBackground(false);
}

void AnimationOverlayWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::changeEvent(event);

    m_currentPixmap = m_originalPixmap.scaled(event->size());
}

void AnimationOverlayWidget::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);
    p.setOpacity(m_opacity);

    p.translate(m_offset);
    p.scale(m_scale, m_scale);

    p.drawPixmap(0, 0, m_currentPixmap);
    QWidget::paintEvent(event);
}


}; // namespace Draupnir::Ui
