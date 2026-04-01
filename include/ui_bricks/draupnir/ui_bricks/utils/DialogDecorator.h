/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2026 Ivan Odinets <i_odinets@protonmail.com>
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

/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2026 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef DIALOGDECORATOR_H
#define DIALOGDECORATOR_H

#include <QApplication>
#include <QDialog>
#include <QIcon>

namespace Draupnir::Ui
{

/*! @class DialogDecorator draupnir/ui_bricks/utils/DialogDecorator.h
 *  @ingroup UiBricks
 *  @brief Utility class for applying common compile-time dialog decorations.
 *  @details `DialogDecorator` provides a small set of reusable dialog decoration modes that can be combined as bit flags
 *           and applied at compile time via `decorateDialog<mode>(...)`.
 *
 *           Supported decoration modes (see enum @ref Draupnir::Ui::DialogDecorator::Mode):
 *           - @ref Draupnir::Ui::DialogDecorator::Mode::UseAppIcon - sets dialog window icon to `qApp->windowIcon()`;
 *           - @ref Draupnir::Ui::DialogDecorator::Mode::UseAppTitle - sets dialog window title to `qApp->applicationName()`;
 *           - @ref Draupnir::Ui::DialogDecorator::Mode::ResizeToTitle - enlarges the dialog width if needed so that the
 *             current title text is less likely to appear visually clipped.
 *
 * @note @ref Draupnir::Ui::DialogDecorator::Mode::ResizeToTitle uses the dialog widget font as an approximation for title
 *       bar text metrics. Native window title bars are drawn by the window manager, so this is only an estimate and not a
 *       guaranteed exact fit on all platforms. */

class DialogDecorator
{
public:
    /*! @enum Mode
     *  @brief Bit-flag decoration modes supported by DialogDecorator. */
    enum Mode {
        UseAppIcon  = 1u << 0,   /*!< Set dialog icon to `qApp->windowIcon()`. */
        UseAppTitle = 1u << 1,   /*!< Set dialog title to `qApp->applicationName()`. */
        ResizeToTitle = 1u << 2  /*!< Increase dialog width to better fit its title text. */
    };

    /*! @brief Applies selected compile-time decoration modes to the given dialog.
     *  @tparam mode Bitwise combination of @ref Mode values.
     *  @param dialog Pointer to the dialog to decorate.
     *  @details The selected decoration steps are resolved at compile time using `if constexpr`. Multiple modes can be
     *           combined, for example:
     *           @code
     *           DialogDecorator::decorateDialog<
     *               DialogDecorator::UseAppIcon | DialogDecorator::UseAppTitle
     *           >(dialog);
     *           @endcode */
    template<std::underlying_type_t<Mode> mode>
    static void decorateDialog(QDialog* dialog) {
        if constexpr (mode & UseAppIcon)
            dialog->setWindowIcon(qApp->windowIcon());
        if constexpr (mode & UseAppTitle)
            dialog->setWindowTitle(qApp->applicationName());

        if constexpr (mode & ResizeToTitle) {
            const QFontMetrics fm{dialog->font()};
            const int textWidth = fm.horizontalAdvance(dialog->windowTitle());

            const int extra = 120;

            dialog->adjustSize();
            dialog->resize(std::max(dialog->width(), textWidth + extra), dialog->height());
        }
    }
};

}; // namespace Draupnir::Ui

#endif // DIALOGDECORATOR_H
