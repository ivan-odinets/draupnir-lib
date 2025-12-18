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

#ifndef ANIMATIONBUILDER_H
#define ANIMATIONBUILDER_H

#include <QAbstractAnimation>
#include <QApplication>
#include <QScreen>
#include <QWindow>

#include "draupnir/ui_bricks/ui/widgets/AnimationOverlayWidget.h"

namespace Draupnir::Ui
{

/*! @class AnimationBuilder draupnir/ui_bricks/animations/AnimationBuilder.h
 *  @ingroup UiBricks
 *  @brief Fluent helper to construct and run Qt animations on a `QWidget`, optionally via an overlay widget.
 *
 *  @details This class encapsulates a common pattern when working with Qt animations:
 *           - create a concrete animation via an `AnimationTemplate` type;
 *           - optionally build an @ref Draupnir::Ui::AnimationOverlayWidget based on a screenshot of a source widget;
 *           - wire up completion callbacks;
 *           - start the animation with `QAbstractAnimation::DeleteWhenStopped` policy.
 *
 *           The builder is intentionally minimal and focused on wiring and lifetime: it does not know anything about the
 *           specific animation details, which are delegated to the `AnimationTemplate`.
 *
 * @note The builder does not own the source `QWidget` passed to Draupnir::Ui::AnimationBuilder::createAnimation(). It
 *       does, however, control the lifetime of the `QAbstractAnimation` instance when Draupnir::Ui::AnimationBuilder::start()
 *       is used.
 *
 * @todo Allow users to configure times of animation both within code and from the UI of final application. */

class AnimationBuilder
{
public:
    /*! @brief Default destructor. */
    ~AnimationBuilder() = default;

    /*! @brief Creates a fresh builder instance with no animation attached yet.
     *  @return A new @ref Draupnir::Ui::AnimationBuilder instance. */
    static AnimationBuilder begin() {
        return AnimationBuilder{};
    }

    /*! @brief Creates an animation for the given widget.
     *  @tparam AnimationTemplate A type that provides a static factory: `static QAbstractAnimation* create(QWidget* target)`.
     *                            The returned pointer must refer to a heap-allocated object derived from @c QAbstractAnimation.
     *  @tparam useOverlay        If `true` (default), the animation will run on a dedicated @ref Draupnir::Ui::AnimationOverlayWidget
     *                            constructed from a screenshot of `animatable`. The overlay widget adopts the geometry of `animatable`
     *                            and is shown when the animation enters the `QAbstractAnimation::Running` state. The overlay is
     *                            destroyed via `deleteLater()` once the animation finishes.
     *
     *                            If `false`, the `AnimationTemplate` is applied directly to `animatable` and no overlay is created.
     *  @param animatable    Source `QWidget` to animate or to base the overlay on.
     *  @return Reference to this Draupnir::Ui::AnimationBuilder instance.
     * @todo Modify this method to allow animating backwards. For this we need to store AnimationOverlayWidget after animation finishes. */
    template<class AnimationTemplate, bool useOverlay = true>
    AnimationBuilder& createAnimation(QWidget* animatable) {
        if constexpr (useOverlay) {
            const QRect frameGeometry = animatable->frameGeometry();
            QScreen* screen = animatable->windowHandle()
                ? animatable->windowHandle()->screen()
                : QApplication::primaryScreen();
            const QPixmap fullShot = screen->grabWindow(0,frameGeometry.x(),frameGeometry.y(),frameGeometry.width(),frameGeometry.height());

            AnimationOverlayWidget* overlay = new AnimationOverlayWidget{fullShot};
            overlay->setGeometry(frameGeometry);

            p_animation = AnimationTemplate::create(overlay);

            QObject::connect(p_animation, &QAbstractAnimation::finished, overlay, &QWidget::deleteLater);
            QObject::connect(p_animation,  &QAbstractAnimation::stateChanged, [overlay](QAbstractAnimation::State newState, QAbstractAnimation::State){
                if (newState == QAbstractAnimation::Running)
                    overlay->show();
            });
        } else {
            p_animation = AnimationTemplate::create(animatable);
        }

        return *this;
    }

    /*! @brief Registers a callback to be invoked when the animation starts.
     *  @tparam F  Callable type compatible with `void()`.
     *  @param callback Callback to be invoked after the animation emits `QAbstractAnimation::stateChanged` signal with `newState
     *                  == QAbstractAnimation::Running`.
     *  @return Reference to this @ref Draupnir::Ui::AnimationBuilder instance. */
    template<class F>
    AnimationBuilder& onStarted(F&& callback) {
        Q_ASSERT_X(p_animation,"AnimationBuilder::onStarted",
                   "Animation must have been created before using AnimationBuilder::createAnimation method.");
        QObject::connect(p_animation, &QAbstractAnimation::stateChanged, [callback](QAbstractAnimation::State newState, QAbstractAnimation::State){
            if (newState == QAbstractAnimation::Running)
                callback();
        });
        return *this;
    }

    /*! @brief Registers a callback to be invoked when the animation finishes.
     *  @tparam F  Callable type compatible with `void()`.
     *  @param callback Callback to be invoked after the animation emits `QAbstractAnimation::finished()`.
     *  @return Reference to this @ref Draupnir::Ui::AnimationBuilder instance. */
    template<class F>
    AnimationBuilder& onFinished(F&& callback) {
        Q_ASSERT_X(p_animation,"AnimationBuilder::onFinished",
                   "Animation must have been created before using AnimationBuilder::createAnimation method.");
        QObject::connect(p_animation, &QAbstractAnimation::finished, callback);
        return *this;
    }

    /*! @brief Starts the configured animation. The animation is started with `QAbstractAnimation::DeleteWhenStopped`, so Qt will
     *         destroy the animation object automatically after it has finished and stopped. */
    void start() {
        p_animation->start(QAbstractAnimation::DeleteWhenStopped);
    }

    /*! @brief Returns the raw pointer to the underlying animation.
     *  @return Pointer to the `QAbstractAnimation` instance created in AnimationBuilder::createAnimation(), or `nullptr` if no
     *          animation has been created yet.
     * @note Ownership of the animation is managed by Qt once AnimationBuilder::start() is called. Do not delete this pointer
     *       manually unless you are not using the AnimationBuilder::start method. */
    QAbstractAnimation* getAnimation() { return p_animation; }

private:
    /*! @brief Private default constructor; use AnimationBuilder::begin() instead. */
    AnimationBuilder() :
        p_animation{nullptr}
    {}

    QAbstractAnimation* p_animation;
};

}; // namespace Draupnir::Ui::Animations

#endif // ANIMATIONBUILDER_H
