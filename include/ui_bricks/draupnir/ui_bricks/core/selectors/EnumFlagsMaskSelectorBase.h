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

#ifndef ENUMFLAGSMASKSELECTORBASE_H
#define ENUMFLAGSMASKSELECTORBASE_H

#include "draupnir/ui_bricks/core/selectors/FlagsMaskSelectorBase.h"

namespace Draupnir::Ui
{

/*! @class EnumFlagsMaskSelectorBase
 *  @brief This is a class.
 * @todo Documentation: Write documentation for this class. */

template<class Implementation, class UiElement, class _EnumFlags>
class EnumFlagsMaskSelectorBase : public FlagsMaskSelectorBase<Implementation, UiElement, _EnumFlags>
{
    using _Base = FlagsMaskSelectorBase<Implementation, UiElement, _EnumFlags>;

public:
    using EnumFlags = _EnumFlags;

    /*! @brief Constructs the selector and creates all generated UI elements.
     *  @param initialState Initial flags mask to be displayed by the selector. */
    EnumFlagsMaskSelectorBase(EnumFlags initialState = 0) :
        _Base{initialState}
    {};
};

}; // namespace Draupnir::UiBricks

#endif // ENUMFLAGSMASKSELECTORBASE_H
