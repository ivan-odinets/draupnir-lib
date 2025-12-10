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

#ifndef ROTATESCREENOVERLAY_H
#define ROTATESCREENOVERLAY_H

#include <QWidget>

namespace Draupnir::Loptr
{

/*! @class RotateScreenOverlay draupnir/loptr/ui/widgets/RotateScreenOverlay.h
 *  @ingroup Loptr
 *  @brief This is a helper class to do a barrel roll. Fullscreen widget that displays a screenshot and animates its rotation.
 *
 *  @details This helper widget renders a provided pixmap (typically a screenshot) fullscreen and exposes an animatable @ref
 *           angle property that can be driven by a @c QPropertyAnimation. The usage pattern is:
 *           - grab a screenshot from @c QScreen;
 *           - construct @ref RotateScreenOverlay with that pixmap;
 *           - show the overlay (e.g. @c showFullScreen());
 *           - call @ref startAnimation() to rotate the image by 360 degrees and close the overlay when done.
 *
 *           A mouse press on the overlay also closes it immediately.
 *
 * @note This class is part of Loptr module, which is mostly for fun. However feel free to use code from this module in
 *       reasonable places. */

class RotateScreenOverlay final  : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double angle READ angle WRITE setAngle)

public:
    /*! @brief Constructs the overlay with a given pixmap.
     *  @param pix      Pixmap to display and rotate (for example, a screenshot of the screen).
     *  @param parent   Optional parent widget.
     *  @details The widget is created frameless and is typically shown fullscreen on top of other windows. The pixmap is rendered
     *           centered and rotated around the widget's center according to the @ref angle property. */
    explicit RotateScreenOverlay(const QPixmap& pix, QWidget* parent = nullptr);

    /*! @brief Returns the current rotation angle in degrees. */
    double angle() const { return m_angle; }

    /*! @brief Sets the current rotation angle in degrees.
     *  @param a New angle value in degrees.
     *  @details If the new angle differs from the current one, the widget is repainted. This method is typically invoked indirectly
     *           by a @c QPropertyAnimation that targets the @ref angle property. */
    void setAngle(double a);

    /*! @brief Starts a one-shot rotation animation.
     *  @param durationMs    Duration of the animation in milliseconds. Defaults to 1000 ms.
     *  @details This convenience method creates and starts a @c QPropertyAnimation on the @ref angle property, interpolating it from
     *           @c 0.0 to @c 360.0 over the specified duration. Once the animation finishes, the widget closes itself, effectively
     *           restoring the application to its previous visual state. */
    void startAnimation(int durationMs = 1000);

protected:
    /*! @brief Renders the rotated pixmap.
     *  @details The pixmap is drawn centered in the widget. The painter's transform is adjusted so that the rotation is performed
     *           around the widget's center, then the pixmap is rendered with the current @ref angle applied. */
    void paintEvent(QPaintEvent*) final;

    /*! @brief Closes the overlay on mouse press.
     *  @details Any mouse press inside the overlay immediately closes the widget, cancelling any ongoing visual effect and returning
     *           control to the underlying application windows. */
    void mousePressEvent(QMouseEvent*) final { close(); }

private:
    QPixmap m_pixmap;
    double  m_angle;
};

}; // namespace Draupnir::Loptr

#endif // ROTATESCREENOVERLAY_H
