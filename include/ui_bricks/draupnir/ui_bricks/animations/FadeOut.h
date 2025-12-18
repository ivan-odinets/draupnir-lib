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

#ifndef FADEOUT_H
#define FADEOUT_H

#include <QPropertyAnimation>

namespace Draupnir::Ui::Animations
{

/*! @class FadeOut draupnir/ui_bricks/animations/FadeOut.h
 *  @ingroup UiBricks
 *  @brief Simple fade-out animation.
 *
 *  @details This animation template creates a @c QPropertyAnimation that linearly fades the target’s `opacity` property to
 *           `0.0` over a certain period of time.
 *
 *           The `Target` type is expected to expose a Qt property named "`opacity`" that is compatible with `QPropertyAnimation`,
 *           for example:
 *           - a custom widget with `Q_PROPERTY(double opacity READ opacity WRITE setOpacity)`;
 *           - an overlay widget such as @ref Draupnir::Ui::AnimationOverlayWidget;
 *           - or any QObject-derived type with a matching property.
 *
 *           The typical usage is in combination with @ref Draupnir::Ui::AnimationBuilder and an overlay-based animation flow;
 *           for example:
 *
 *           @code
 *           AnimationBuilder::begin()
 *               .createAnimation<Draupnir::Ui::Animations::FadeOut>(mainWindow)
 *               .onFinished([mainWindow]() { mainWindow->close(); })
 *               .start();
 *           @endcode */

class FadeOut
{
public:
    /*! @brief Factory method that constructs a fade-out animation for the @p target.
     *  @tparam Target  Type of the animated object. Must provide an "`opacity`" Qt property that is readable and writable,
     *          and a compatible `windowOpacity()` getter if the initial value is to be taken from the current opacity.
     *  @param target   Pointer to the object whose opacity will be animated.
     *
     *  @return A heap-allocated `QPropertyAnimation` configured to animate target's `opacity` from its current value down
     *          to `0.0`.
     *
     * @note The caller is responsible for managing the animation’s lifetime, typically via `QAbstractAnimation::DeleteWhenStopped`
     *       or by assigning a suitable `QObject` parent. */
    template<class Target>
    static QAbstractAnimation* create(Target* target) {
        const int duration = 300;
        QPropertyAnimation* result = new QPropertyAnimation{target, "opacity" };
        result->setDuration(duration);
        result->setStartValue(target->windowOpacity());
        result->setEndValue(0.0);
        return result;
    }
};

}; // namespace Draupnir::Ui::Animations

#endif // FADEOUT_H
