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

#ifndef MAINWINDOWTEMPLATE_H
#define MAINWINDOWTEMPLATE_H

#include <QMainWindow>

#include <QCloseEvent>
#include <QEvent>

#include "draupnir/SettingsRegistry.h" // IWYU pragma: keep

#include "draupnir/ui_bricks/animations/AnimationBuilder.h"

#include "draupnir/ui_bricks/traits/features/AnimatedMinimizableToTray.h" // IWYU pragma: keep
#include "draupnir/ui_bricks/traits/features/MinimizableOnClose.h"
#include "draupnir/ui_bricks/traits/features/MinimizableToTray.h"
#include "draupnir/ui_bricks/traits/features/RememberWindowSize.h"
#include "draupnir/ui_bricks/traits/features/TrayIconSupported.h"         // IWYU pragma: keep

#include "draupnir/utils/type_presense.h"
#include "draupnir/utils/type_extractors.h"
#include "draupnir/utils/filter_if.h"

namespace Draupnir::Ui
{

/*! @class MainWindowTemplate draupnir/ui_bricks/ui/windows/MainWindowTemplate.h
 *  @ingroup UiBricks
 *  @brief Generic `QMainWindow`-based template that composes behavior from a set of feature types.
 *  @tparam Features    Variadic list of "feature" types that participate in this main window’s behavior. Each feature may:
 *                      - contribute settings via a nested `SettingsBundle` alias;
 *                      - optionally expose `static constexpr bool hasState_v` to indicate that it owns runtime state, which
 *                        will be stored inside the window.
 *
 *  @details This template wires together a number of reusable "feature" classes to form a concrete main window implementation.
 *           Features can, for example:
 *           - remember and restore window size;
 *           - allow minimizing the window instead of closing;
 *           - support minimizing to tray (with or without animation);
 *           - hold state related to the system tray icon.
 *
 *           The class itself is responsible for:
 *           - aggregating settings into a single @ref MainWindowTemplate::SettingsBundle;
 *           - handling of Qt events (e.g. `QCloseEvent`, `QEvent`) according to included features and current settings;
 *           - providing helper meta-functions to query feature presence and access their state, which can be used by subclassing
 *             @ref MainWindowTemplate.
 *
 * @todo Allow creating and setting central widget and menu bar via features.
 * @todo Use static_assert to validate that dependant and mutually-explusive features are used properly.
 * @todo Allow custom animations for other events, like window closure, window appearing, etc.
 * @todo Refractor showing / hiding of the window in a better manner. */

template<class... Features>
class MainWindowTemplate : public QMainWindow
{
private:
    /*! @brief This SFINAE detector is used to detect if feature class has state. By now state is detected by the presense of
     *         `static constexpr bool hasState_v` and if this field is set to `true`. */
    template<class, class = void>
    struct has_state : std::false_type {};

    template<class Feature>
    struct has_state<Feature, std::void_t<decltype(Feature::hasState_v)>> :
        std::bool_constant<Feature::hasState_v> {};

    /*! @brief Tuple that stores instances of all stateful features. Only those `Features...` for which @ref has_state evaluates
     *         to `true` are included into this tuple. Stateless features are not instantiated and are used purely as types. */
    using FeaturesStateTuple = typename draupnir::utils::filter_if<has_state,Features...>::template to_container<std::tuple>;

protected:
    /*! @brief Checks at compile time whether a specific feature type is present.
     *  @tparam Feature Concrete feature type to check for, e.g. @ref Draupnir::Ui::MainWindow::RememberWindowSize.
     *  @return `true` if `Feature` is present in `Features..., `false` otherwise. */
    template<class Feature>
    static constexpr bool featurePresent() { return draupnir::utils::is_one_of_v<Feature,Features...>; }

    /*! @brief Checks at compile time whether a feature instantiation of a given template is present.
     *  @tparam FeatureTemplate   Feature template to check for, e.g. @ref Draupnir::Ui::MainWindow::AnimatedMinimizableToTray.
     *  @return `true` if there exists at least one instantiation of `FeatureTemplate` within `Features...`, `false` otherwise. */
    template<template<class...> class FeatureTemplate>
    static constexpr bool featurePresent() { return draupnir::utils::is_template_instantiation_present_v<FeatureTemplate,Features...>; }

    /*! @brief Resolves a feature template to the concrete instantiation used in `Features...`
     *  @tparam FeatureTemplate   Feature template to resolve, e.g. @ref Draupnir::Ui::MainWindow::AnimatedMinimizableToTray.
     *  @details This alias resolves to the concrete type from `Features...` which is an instantiation of `FeatureTemplate`. If
     *           there is more than one instantiation, the behavior is defined by @ref draupnir::utils::get_template_instantiation_t
     *           (the first matching type). */
    template<template<class...> class FeatureTemplate>
    using getFeatureTemplateInstantiation = draupnir::utils::get_template_instantiation_t<FeatureTemplate,Features...>;

    /*! @brief Accessor for the runtime state of a specific feature type.
     *  @tparam Feature Concrete feature type whose state should be retrieved.
     *  @return Reference to the feature instance stored inside @ref FeaturesStateTuple.
     * @note `Feature` must be present in `Features...` and must be marked as stateful (i.e. `Feature::hasState_v == true`). */
    template<class Feature>
    inline auto& getFeatureState() {
        static_assert(draupnir::utils::is_one_of_v<Feature, Features...>,
            "Specified Feature is not available within this MainWindowTemplate instantiation.");
        static_assert(draupnir::utils::is_type_in_tuple_v<Feature, FeaturesStateTuple>,
            "The feature specified does not have state.");
        return std::get<Feature>(m_featuresStateTuple);
    }

    /*! @brief Accessor for the runtime state of a feature, addressed by feature template. This overload first resolves the concrete
     *         feature type via @ref getFeatureTemplateInstantiation and then forwards to the non-template overload of @ref getFeatureState.
     *  @tparam FeatureTemplate   Feature template whose concrete instantiation’s state should be accessed. */
    template<template<class...> class FeatureTemplate>
    inline auto& getFeatureState() {
        static_assert(draupnir::utils::is_template_instantiation_present_v<FeatureTemplate, Features...>,
            "Specified FeatureTemplate is not available within this MainWindowTemplate instantiation.");
        static_assert(draupnir::utils::is_template_instantiation_present_in_tuple_v<FeatureTemplate, FeaturesStateTuple>,
            "The feature template specified does not have state.");
        return getFeatureState<
            getFeatureTemplateInstantiation<FeatureTemplate>
        >();
    }

public:
    /*! @brief Aggregated settings bundle type, composed from all features.
     *  @details Each feature may contribute one or more setting traits to the final `SettingsBundle`. This bundle is then used to load
     *           and store window-related configuration. */
    using SettingsBundle = typename Draupnir::Settings::SettingsTraitsConcatenator<
        Features...
    >::toSettingsBundle;

    /*! @brief Constructs a main window with the given `parent`. */
    explicit MainWindowTemplate(QWidget* parent = nullptr) :
        QMainWindow{parent}
    {}

    /*! @brief Destructor that optionally persists window size. If the `MainWindow::RememberWindowSize` feature is present, the current
     *         window size is stored into the @ref SettingsBundle using `Draupnir::Settings::MainWindow::WindowSizeSetting`. */
    ~MainWindowTemplate() override {
        // If we have MainWindow::RememberWindowSize feature - do what should be done.
        if constexpr (featurePresent<MainWindow::RememberWindowSize>()) {
            m_settings.template set<Draupnir::Settings::MainWindow::WindowSizeSetting>(size());
        }
    };

    /*! @brief Loads window-related settings from a given @p SettingsSource.
     *  @tparam SettingsSource    Type that can populate @ref SettingsBundle of this MainWindowTemplate.
     *  @tparam hasSettings       Internal compile-time flag that disables this overload entirely when @ref SettingsBundle is empty.
     *  @param source             Pointer to a settings source instance. Must not be `nullptr`.
     *  @details After loading the settings bundle, if @ref Draupnir::Ui::MainWindow::RememberWindowSize is present, the stored window
     *           size is applied via `QMainWindow::resize()`. */
    template<class SettingsSource, bool hasSettings = !SettingsBundle::isEmpty()>
    std::enable_if_t<hasSettings,void> loadSettings(SettingsSource* source) {
        static_assert(SettingsBundle::template canBeFullyPopulatedFrom<SettingsSource>(),
                "SettingsSource specified can not populate the SettingsBundle within this MainWindowTemplate.");
        Q_ASSERT_X(!m_settings.isLoaded(), "MainWindowTemplate::loadSettings",
                   "This method should be called only once.");
        Q_ASSERT_X(source, "MainWindowTemplate::loadSettings",
                   "SettingsSource* pointer specified is nullptr.");

        m_settings = source->template getSettingsBundle<SettingsBundle>();

        // If we have MainWindow::RememberWindowSize feature - do what should be done.
        if constexpr (featurePresent<MainWindow::RememberWindowSize>()) {
            const QSize windowSize = m_settings.template get<Draupnir::Settings::MainWindow::WindowSizeSetting>();
            resize(windowSize);
        }
    }

    /*! @brief Enables or disables the "minimize on close" behavior, when @ref Draupnir::Ui::MainWindow::MinimizableOnClose feature
     *         is present within `Features...` pack.
     *  @tparam minimizableOnClose Internally deduced flag that is `true` if @ref Draupnir::Ui::MainWindow::MinimizableOnClose feature
     *          is present within `Features...` pack.
     *  @param state If `true`, the window will be minimized instead of actually closing when the user attempts to close it.
     * @note This method is only available when the corresponding feature is present. */
    template<bool minimizableOnClose = featurePresent<MainWindow::MinimizableOnClose>()>
    std::enable_if_t<minimizableOnClose,void> setMinimizeOnClose(bool state) {
        m_settings.template set<Draupnir::Settings::MainWindow::MinimizeOnCloseSetting>(state);
    }

    /*! @brief Returns current "minimize on close" flag when @ref Draupnir::Ui::MainWindow::MinimizableOnClose feature  is present
     *         within `Features...` pack.
     *  @tparam minimizableOnClose Internally deduced flag that is `true` if @ref Draupnir::Ui::MainWindow::MinimizableOnClose feature
     *          is present within `Features...` pack
     *  @return `true` if "minimize on close" is enabled in settings, `false` otherwise.
     * @note This method is only available when the corresponding feature is present. */
    template<bool minimizableOnClose = featurePresent<MainWindow::MinimizableOnClose>()>
    std::enable_if_t<minimizableOnClose,bool> minimizeOnClose() {
        return m_settings.template get<Draupnir::Settings::MainWindow::MinimizeOnCloseSetting>();
    }

    /*! @brief Enables or disables the "minimize to tray" behavior, when either @ref Draupnir::Ui::MainWindow::MinimizableToTray
     *         or @ref Draupnir::Ui::MainWindow::AnimatedMinimizableToTray feature is present within `Features...` pack.
     *  @tparam minimizableToTray Internally deduced flag that is `true` if either @ref Draupnir::Ui::MainWindow::MinimizableToTray
     *         or @ref Draupnir::Ui::MainWindow::AnimatedMinimizableToTray feature is present within `Features...` pack.
     *  @param state If `true`, the window will be minimized instead of actually closing when the user attempts to close it.
     * @note This method is only available when the corresponding feature is present. */
    template<bool minimizableToTray = featurePresent<MainWindow::MinimizableToTray>() || featurePresent<MainWindow::AnimatedMinimizableToTray>()>
    std::enable_if_t<minimizableToTray,void> setMinimizeToTray(bool state) {
        m_settings.template set<Draupnir::Settings::MainWindow::MinimizeToTraySetting>(state);
    }

    /*! @brief Returns current "minimize to tray" flag when either @ref Draupnir::Ui::MainWindow::MinimizableToTray or @ref
     *         Draupnir::Ui::MainWindow::AnimatedMinimizableToTray feature is present within `Features...` pack
     *  @tparam minimizableOnClose Internally deduced flag that is `true` if either @ref Draupnir::Ui::MainWindow::MinimizableToTray
     *         or @ref Draupnir::Ui::MainWindow::AnimatedMinimizableToTray feature is present within `Features...` pack
     *  @return `true` if "minimize to tray" is enabled in settings, `false` otherwise.
     * @note This method is only available when the corresponding feature is present. */
    template<bool minimizableToTray = featurePresent<MainWindow::MinimizableToTray>() || featurePresent<MainWindow::AnimatedMinimizableToTray>()>
    std::enable_if_t<minimizableToTray,bool> minimizeToTray() {
        return m_settings.template get<Draupnir::Settings::MainWindow::MinimizeToTraySetting>();
    }

    /*! @brief Registers a system tray within this main window.
     *  @tparam TrayIcon     Type of the tray icon, typically `QSystemTrayIcon` or class inherited from it.
     *  @tparam supportsTray Internally deduced flag that is `true` if a @ref Draupnir::Ui::MainWindow::TrayIconSupported feature
     *                       instantiation is present.
     *  @param icon Pointer to the tray icon instance. Lifetime is managed by the caller.
     *  @details The icon pointer is stored in the state of @ref Draupnir::Ui::MainWindow::TrayIconSupported feature. A connection
     *           is established to route tray icon activation events to @ref MainWindowTemplate::_onTrayIconActivated(). */
    template<class TrayIcon, bool supportsTray = featurePresent<MainWindow::TrayIconSupported>()>
    std::enable_if_t<supportsTray,void> setTrayIcon(TrayIcon* icon) {
        getFeatureState<MainWindow::TrayIconSupported>().icon = icon;

        connect(icon, &QSystemTrayIcon::activated, [this](QSystemTrayIcon::ActivationReason activationReason){
            _onTrayIconActivated(activationReason);
        });
    }

protected:
    /*! @brief Handles the window close event according to features specified within `Features...` pack and (if applicable) runtime
     *         configuration.
     *  @param QCloseEvent event to be handled.
     *  @details Behavior is the following:
     *           - If @ref Draupnir::Ui::MainWindow::MinimizableOnClose is present within `Features...` pack and correspnding setting,
     *             is enabled - the window is minimized instead of being closed and the `QCloseEvent` is ignored.
     *           - If @ref Draupnir::Ui::MainWindow::MainWindow::TrayIconSupported is present - when the window is actually closing,
     *             the icon is hidden to ensure the application terminates cleanly. `QCloseEvent` is passed to base implementation
     *             `QMainWindow::closeEvent()`;
     *           - Otherwise base implementation `QMainWindow::closeEvent()`is invoked. */
    void closeEvent(QCloseEvent* event) override {
        // If we have MainWindow::MinimizableOnClose feature - do what should be done.
        if constexpr (featurePresent<MainWindow::MinimizableOnClose>()) {
            if (m_settings.template get<Draupnir::Settings::MainWindow::MinimizeOnCloseSetting>()) {
                QMainWindow::setWindowState(Qt::WindowMinimized);
                event->ignore();
                return;
            }
        }

#ifndef QT_NO_SYSTEMTRAYICON
        // This part of code is required only if we have TrayIconSupported within the feature list
        if constexpr (featurePresent<MainWindow::TrayIconSupported>()) {
            // If the tray icon is still visible, after main window closed - application is still running
            // Hide the tray - kill the app
            if (getFeatureState<MainWindow::TrayIconSupported>().icon != nullptr)
                getFeatureState<MainWindow::TrayIconSupported>().icon->hide();
        }
#endif //QT_NO_SYSTEMTRAYICON

        QMainWindow::closeEvent(event);
    }

    /*! @brief Handles generic change events, including window state changes.
     *  @details When either @ref Draupnir::Ui::MainWindow::MinimizableToTray or @ref Draupnir::Ui::MainWindow::AnimatedMinimizableToTray
     *           feature is present, this method intercepts `QEvent::WindowStateChange` and, if "minimize to tray" is enabled in settings
     *           and the window was minimized, performs the following:
     *           - clears the `Qt::WindowMinimized` flag on the window;
     *           - either hides the window immediately (for @ref Draupnir::Ui::MinimizableToTray), or runs an animation specified within
     *             @ref Draupnir::Ui::MainWindow::AnimatedMinimizableToTray and only than hides the window just before or at animation
     *             start;
     *           - ignores the original event.
     *           In all other cases, the base class implementation is called. */
    void changeEvent(QEvent* event) override {
#ifndef QT_NO_SYSTEMTRAYICON
        if constexpr (featurePresent<MainWindow::MinimizableToTray>() || featurePresent<MainWindow::AnimatedMinimizableToTray>()) {
            if (event->type() == QEvent::WindowStateChange) {
                if ( isMinimized() && m_settings.template get<Draupnir::Settings::MainWindow::MinimizeToTraySetting>() ) {
                    setWindowState(windowState() & ~Qt::WindowMinimized);

                    if constexpr (featurePresent<MainWindow::MinimizableToTray>()) {
                        hide();
                    }

                    if constexpr (featurePresent<MainWindow::AnimatedMinimizableToTray>()) {
                        using Animation = typename draupnir::utils::get_template_instantiation_t<
                            MainWindow::AnimatedMinimizableToTray,Features...>::Animation;
                        AnimationBuilder::begin()
                            .createAnimation<Animation,true>(this)
                            .onStarted([this](){ hide(); })
                            .start();
                    }

                    event->ignore();
                    return;
                }
            }
        }
#endif //QT_NO_SYSTEMTRAYICON

        QMainWindow::changeEvent(event);
    }

private:
    SettingsBundle m_settings;
    FeaturesStateTuple m_featuresStateTuple;

#ifndef QT_NO_SYSTEMTRAYICON
    /*! @brief Handles activation of the system tray icon when the corresponding feature is present.
     *  @tparam hasTrayIcon  Compile-time flag that enables this method only when a @ref Draupnir::Ui::MainWindow::TrayIconSupported
     *                       feature instantiation is present.
     *  @param reason Activation reason as provided by `QSystemTrayIcon`.
     *  @details For click-based activations, this toggles the main window visibility using @ref _toggleWindowVisibility(). */
    template<bool hasTrayIcon = featurePresent<MainWindow::TrayIconSupported>()>
    std::enable_if_t<hasTrayIcon,void> _onTrayIconActivated(QSystemTrayIcon::ActivationReason reason) {
        switch (reason) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
        case QSystemTrayIcon::MiddleClick:
            _toggleWindowVisibility();
            break;
        default:
            break;
        }
    }

    /*! @brief Toggles the main window visibility in a tray-friendly manner.
     *  @details Behavior:
     *           - If the window is minimized, it is restored and activated.
     *           - If it is visible but not active, it is activated.
     *           - If it is hidden, it is shown in normal state.
     *           - If it is visible and active, it is hidden.
     *           This helper is typically called from the tray icon activation handler. */
    template<bool hasTrayIcon = featurePresent<MainWindow::TrayIconSupported>()>
    std::enable_if_t<hasTrayIcon,void> _toggleWindowVisibility() {
        //If we are minimized - restore and activate window
        if (isMinimized()) {
            showNormal();
            activateWindow();
            return;
        }

        //If we are visible, but not active - activate
        if (isVisible() && !isActiveWindow()) {
            activateWindow();
            return;
        }

        //If we are hidden - show back
        if (isHidden()) {
            showNormal();
            return;
        }

        //If we are visible and active - hide window or minimize
        if (isActiveWindow() && isVisible()) {
            hide();
            return;
        }
    }
#endif // QT_NO_SYSTEMTRAYICON

};

};

#endif // MAINWINDOWTEMPLATE_H
