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

#ifndef PARRALELANIMATIONTEMPLATE_H
#define PARRALELANIMATIONTEMPLATE_H

#include <QParallelAnimationGroup>

namespace Draupnir::Ui::Animations
{

/*! @class ParralelAnimationsTemplate draupnir/include/ui_bricks/animations/ParralelAnimationsTemplate.h
 *  @ingroup UiBricks
 *  @brief Convenience template that composes multiple animation templates into a single `QParallelAnimationGroup`.
 *
 *  @tparam Animations  A variadic list of animation “template types”. Every type in @p Animations... is expected to provide
 *                      a static factory of the following form: `static QAbstractAnimation* create(Target* target);` The
 *                      returned pointer must refer to a heap-allocated object derived from `QAbstractAnimation`. Ownership
 *                      is typically transferred to the caller, which may further manage it via `QAbstractAnimation::DeleteWhenStopped`
 *                      or a `QWidget` parent.
 *
 *  @details This helper groups several animation templates into a single `QParallelAnimationGroup`, running them all at the
 *           same time on the same `target` object.
 *
 *           Example usage:
 *           @code
 *           using FadeAndShrink = ParralelAnimationTemplate<
 *               Draupnir::Ui::Animations::FadeOut,
 *               Draupnir::Ui::Animations::ShrinkToCenter
 *           >;
 *
 *           QAbstractAnimation* anim = FadeAndShrink::create(myWidget);
 *           anim->start(QAbstractAnimation::DeleteWhenStopped);
 *           @endcode
 * @todo Allow setting custom parameters of animations. */

template<class... Animations>
class ParralelAnimationTemplate
{
public:
    /*! @brief Creates a `QParallelAnimationGroup` that runs all `Animations...` in parallel for the given `target`.
     *  @tparam Target  Type of the animated object. It must be compatible with all `Animations::create<Target>(Target*)` calls,
     *                  i.e. each animation template must be able to construct a `QAbstractAnimation` for this target type.
     *  @param target   Pointer to the object that will be passed to each animation template’s `create()` method.
     *  @return A pointer to a newly allocated QParallelAnimationGroup that owns all child animations created by @p Animations.... */
    template<class Target>
    static QAbstractAnimation* create(Target* target) {
        QParallelAnimationGroup* result = new QParallelAnimationGroup{};
        (( result->addAnimation(Animations::template create<Target>(target)), ...));
        return result;
    }
};

}; // namespace Draupnir::Ui::Animations

#endif // PARRALELANIMATIONTEMPLATE_H
