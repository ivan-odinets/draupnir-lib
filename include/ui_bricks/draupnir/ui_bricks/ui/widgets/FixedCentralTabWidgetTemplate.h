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

#ifndef FIXEDCENTRALTABWIDGETTEMPLATE_H
#define FIXEDCENTRALTABWIDGETTEMPLATE_H

#include "draupnir/ui_bricks/ui/widgets/FixedTabWidgetTemplate.h"

#include "draupnir/settings_registry/traits/settings/CentralWidgetIndexSetting.h"

namespace Draupnir::Ui
{

/*! @typedef FixedCentralTabWidgetTemplate draupnir/ui_bricks/ui/widgets/FixedCentralTabWidgetTemplate.h
 *  @ingroup UiBricks
 *  @brief Preconfigured FixedTabWidgetTemplate using ActiveWidgetIndexSetting.
 *  @tparam TabTraits List of tab trait types describing each tab.
 *
 *  @details This is a convenience alias for FixedTabWidgetTemplate where the stored tab index is bound to
 *           Draupnir::Settings::ActiveWidgetIndexSetting. Useful for central tab areas where tab state
 *           persistence is desired without redefining the setting every time. */

template<class... TabTraits>
using FixedCentralTabWidgetTemplate = FixedTabWidgetTemplate<
    Draupnir::Settings::CentralWidgetIndexSetting,
    TabTraits...
>;

};

#endif // FIXEDCENTRALTABWIDGETTEMPLATE_H
