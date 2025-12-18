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

#ifndef ANIMATIONOVERLAYWIDGET_H
#define ANIMATIONOVERLAYWIDGET_H

#include <QWidget>

#include <QDebug>
#include <QPainter>
#include <QResizeEvent>

namespace Draupnir::Ui
{

/*! @class AnimationOverlayWidget draupnir/ui_bricks/ui/widgets/AnimationOverlayWidget.h
 *  @ingroup UiBricks
 *  @brief Lightweight top-level widget used as an animated overlay over another widget.
 *
 *  @details AnimationOverlayWidget is a frameless, always-on-top widget that renders a static pixmap snapshot (typically grabbed
 *           from another widget or a portion of the screen) and exposes a few animatable properties:
 *           - @ref opacity  — controls the alpha of the rendered pixmap;
 *           - @ref scale    — uniform scale factor applied around the widget origin;
 *           - @ref offset   — translation applied before drawing the pixmap.
 *
 *           These properties are declared as Qt `Q_PROPERTY` values, so they can be animated directly via `QPropertyAnimation`
 *           or grouped into more complex animation sets. */

class AnimationOverlayWidget : public QWidget
{
    Q_OBJECT

    /*! @property AnimationOverlayWidget::opacity
     *  @brief Opacity factor applied to the pixmap during painting. Valid range is typically `[0.0; 1.0]`, where `0.0` means fully
     *         transparent and `1.0` means fully opaque. The property is intended to be animated via `QPropertyAnimation`. */
    Q_PROPERTY(double opacity READ opacity WRITE setOpacity)

    /*! @property AnimationOverlayWidget::scale
     *  @brief Uniform scale factor applied to the pixmap. Values greater than `1.0` will enlarge the pixmap, values in `(0.0; 1.0)`
     *         will shrink it. The scaling is performed around the widget origin, after applying @ref offset. */
    Q_PROPERTY(double scale   READ scale   WRITE setScale)

    /*! @property AnimationOverlayWidget::offset
     *  @brief Translation applied before drawing the pixmap. This offset allows simple "slide" or "shift" animations to be
     *         implemented by animating a `QPointF` value. */
    Q_PROPERTY(QPointF offset READ offset WRITE setOffset)

public:
    /*! @brief Constructs an overlay widget displaying the given pixmap.
     *  @param pixmap   Source pixmap to be rendered by this overlay. It is stored internally as @ref m_originalPixmap and used as
     *                  the base for scaled versions.
     *  @param parent   Optional parent widget. In typical "top-level overlay" scenarios  this is left as `nullptr` and geometry
     *                  is set explicitly.
     *  @details The widget is configured as: frameless, always-on-top, with a translucent background, with disabled automatic
     *           background filling. */
    explicit AnimationOverlayWidget(const QPixmap& pixmap, QWidget* parent = nullptr);

    /*! @brief Default destructor. */
    ~AnimationOverlayWidget() = default;

    /*! @copydoc AnimationOverlayWidget::opacity */
    double opacity() const { return m_opacity; }

    /*! @copydoc AnimationOverlayWidget::opacity */
    void setOpacity(double o) { m_opacity = o; update(); }

    /*! @copydoc AnimationOverlayWidget::scale */
    double scale() const { return m_scale; }

    /*! @copydoc AnimationOverlayWidget::scale */
    void setScale(double s) { m_scale = s; update(); }

    /*! @copydoc AnimationOverlayWidget::offset */
    QPointF offset() const { return m_offset; }

    /*! @copydoc AnimationOverlayWidget::offset */
    void setOffset(const QPointF& p) { m_offset = p; update(); }

protected:
    /*! @brief Handles widget resize events. The original pixmap is rescaled to fit the new widget size and stored in @ref
     *         m_currentPixmap. This ensures that subsequent painting operations always operate on a pixmap tha matches the
     *         current widget geometry.
     *  @param event    Pointer to the resize event. */
    void resizeEvent(QResizeEvent *event) override;

    /*! @brief Paints the overlay pixmap using the current transform properties. The painting sequence is: create a
     *         `QPainter`, enable smooth pixmap transforms, apply opacity, translation and scale, draw the current pixmap
     *         starting from `(0, 0)`.
     *  @param event Pointer to the paint event. */
    void paintEvent(QPaintEvent* event) override;

private:
    const QPixmap m_originalPixmap;
    QPixmap m_currentPixmap;
    qreal   m_opacity {1.0};
    qreal   m_scale   {1.0};
    QPointF m_offset  {0, 0};
};

}; // namespace Draupnir::Ui

#endif // ANIMATIONOVERLAYWIDGET_H
