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

#ifndef SHRINKTOCENTER_H
#define SHRINKTOCENTER_H

#include <QDebug>
#include <QMainWindow>
#include <QPropertyAnimation>

#include <cmath>

#include <QDebug>

namespace Draupnir::Ui::Animations
{

/*! @class ShrinkToCenter draupnir/ui_bricks/animations/ShrinkToCenter.h
 *  @ingroup UiBricks
 *  @brief Geometry-based animation template that shrinks a target towards its center.
 *  @details This animation template creates a `QPropertyAnimation` on the target’s "`geometry`" property. Over the course of
 *           the animation the target:
 *           - keeps its center point fixed;
 *           - shrinks from its current size down to `minimumSize()`.
 *
 *           The typical use case is a “shrink into nothing” or “collapse into the center” effect, often combined with an
 *           overlay (e.g. via @ref Draupnir::Ui::AnimationOverlayWidget) or other animations in a `QParallelAnimationGroup`
 *           (e.g. @ref Draupnir::Ui::Animations::FadeOut).
 *
 *           The `Target` type is expected to:
 *           - be a QWidget-like object exposing: `geometry()` and `minimumSize()`
 *           - support a "`geometry`" Qt property compatible with `QPropertyAnimation`.
 *
 *           Example usage:
 *           @code
 *           AnimationBuilder::begin()
 *               .createAnimation<Draupnir::Ui::Animations::ShrinkToCenter>(mainWindow)
 *               .onFinished([mainWindow]() { mainWindow->close(); })
 *               .start();
 *           @endcode */

class ShrinkToCenter
{
public:
    /*! @brief Factory method that constructs a shrink-to-center animation.
     *  @tparam Target  Widget-like type that provides `geometry()`, `minimumSize()` and a Qt "`geometry`" property that can
     *                  be animated.
     *  @param target   Pointer to the object whose geometry will be animated.
     *  @return A heap-allocated `QPropertyAnimation` configured to animate target’s geometry from its current rectangle down
     *          to a smaller rectangle centered in the same position, with size equal to `target->minimumSize()`.
     *
     * @note The caller is responsible for managing the returned animation’s lifetime, typically via `QAbstractAnimation::DeleteWhenStopped`
     *       or by assigning a suitable QObject parent. */
    template<class Target>
    static QAbstractAnimation* create(Target* target) {
        const QRect startGeom{target->geometry()};
        const QSize endSize{target->minimumSize()};

        // Keep the center fixed while shrinking to minimumSize():
        const int topLeftX = startGeom.left() +
            ((std::abs(startGeom.width()  - endSize.width()))  / 2);
        const int topLeftY = startGeom.top()  +
            ((std::abs(startGeom.height() - endSize.height())) / 2);

        const QRect targetGeom{QPoint{topLeftX, topLeftY}, endSize};

        auto* result = new QPropertyAnimation{target, "geometry"};
        result->setStartValue(startGeom);
        result->setEndValue(targetGeom);
        return result;
    }
};

}; // namespace Draupnir::Ui::Animations

#endif // SHRINKTOCENTER_H
