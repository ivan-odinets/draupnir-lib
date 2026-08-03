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

#include "draupnir/messages/core/MessageViewItem.h"

#include "draupnir/messages/core/MessageTypeIconProvider.h"

namespace Draupnir::Messages
{

void MessageViewItem::registerIconProvider(MessageTypeIconProvider* provider)
{
    registerIconProvider(std::unique_ptr<MessageTypeIconProvider>(provider));
}

void MessageViewItem::registerIconProvider(std::unique_ptr<MessageTypeIconProvider> provider)
{
    Q_ASSERT(provider);
    Q_ASSERT(p_iconProvider == nullptr);
    p_iconProvider = std::move(provider);
}

std::unique_ptr<MessageTypeIconProvider> MessageViewItem::p_iconProvider = nullptr;

MessageViewItem::MessageViewItem(MessagePtr message) :
    p_message{message}
{}

QString MessageViewItem::getViewString(const MessageViewItemFields& fields) const
{
    static constexpr MessageViewItemFields notIconFields{MessageViewItemField::Brief | MessageViewItemField::What | MessageViewItemField::DateTime};
    // This method shoud return QString, which is independent of icon display status.
    // To get rid of anything but not Brief, What, DateTime - we use this.
    if ((fields & notIconFields) != m_cachedFields) {
        m_cachedFields = (fields & notIconFields);
        m_cachedView.clear();
        if (fields & MessageViewItemField::Brief)
            m_cachedView += p_message->brief();
        if (fields & MessageViewItemField::What)
            m_cachedView += (m_cachedView.isEmpty() ? "" : "\n") + p_message->what();
        if (fields & MessageViewItemField::DateTime)
            m_cachedView += (m_cachedView.isEmpty() ? "" : "\n") + p_message->dateTime().toString();
    }
    return m_cachedView;
}

const QIcon& MessageViewItem::icon() const
{
    Q_ASSERT(p_iconProvider);
    return iconFor(type());
}

const QIcon& MessageViewItem::iconFor(MessageType type)
{
    Q_ASSERT(p_iconProvider);
    return p_iconProvider->getIcon(type);
}

} // namespace Draupnir::Messages
