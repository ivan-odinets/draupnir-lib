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

#include "draupnir/logging/core/AbstractMessageViewIconProvider.h"

#include <QApplication>
#include <QStyle>

namespace Draupnir::Logging
{

AbstractMessageViewIconProvider::AbstractMessageViewIconProvider()
{}

const QIcon& AbstractMessageViewIconProvider::getIcon(const MessageType messageType) const
{
    switch (messageType.level()) {
    case MessageLevel::Debug:     return _defaultDebugIcon();
    case MessageLevel::Info:      return _defaultInfoIcon();
    case MessageLevel::Warning:   return _defaultWarningIcon();
    case MessageLevel::Error:     return _defaultErrorIcon();
    }

    return _noIcon();
};

const QIcon& AbstractMessageViewIconProvider::_noIcon()
{
    static QIcon empty;
    return empty;
}

const QIcon& AbstractMessageViewIconProvider::_defaultDebugIcon() {
    return _noIcon();
}

const QIcon& AbstractMessageViewIconProvider::_defaultInfoIcon() {
    static QIcon icon{qApp->style()->standardIcon(QStyle::SP_MessageBoxInformation)};
    return icon;
}

const QIcon& AbstractMessageViewIconProvider::_defaultWarningIcon()
{
    static QIcon icon{qApp->style()->standardIcon(QStyle::SP_MessageBoxWarning)};
    return icon;
}

const QIcon& AbstractMessageViewIconProvider::_defaultErrorIcon()
{
    static QIcon icon{qApp->style()->standardIcon(QStyle::SP_MessageBoxCritical)};
    return icon;
}

}; // namespace Draupnir::Logging

