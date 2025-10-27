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

#ifndef MESSAGEVIEWCONFIGDIALOGTEMPLATE_H
#define MESSAGEVIEWCONFIGDIALOGTEMPLATE_H

#include <QCheckBox>
#include <QLabel>

#include "draupnir/containers/fixed_map.h"
#include "draupnir/ui/windows/MessageViewConfigDialog.h"

namespace Draupnir::Messages
{

/*! @class MessageViewConfigDialogTemplate ui/windows/messages/MessageViewConfigDialogTemplate.h
 *  @brief This is a class.
 *
 * @todo Write test for this class.
 * @todo Write a documentation for this class.
 * @todo Add support of selecting message parts to be displayed. */

template<class... MessageTypes>
class MessageViewConfigDialogTemplate final : public MessageViewConfigDialog
{
public:
    explicit MessageViewConfigDialogTemplate(QWidget* parent = nullptr) :
        MessageViewConfigDialog{parent}
    {
        _setupUi<MessageTypes...>();
    }

    void displayTypeSelected(MessageType type, bool isSelected) {
        m_widgetMap[type]->setChecked(isSelected);
    }

    void displayFilterConfig(MessageType config) {
        m_widgetMap.for_each_pair([config](auto& pair){
            pair.second->setChecked(pair.first & config);
        });
    }

private:

    static constexpr MessageType types[] = {MessageTypes::type...};

    draupnir::containers::fixed_map<
        types,
        QCheckBox*
    > m_widgetMap;

    template<class First,class... Other>
    void _setupUi() {
        constexpr auto type = First::type;
        QCheckBox* checkBox = new QCheckBox{First::displayName()};
        m_widgetMap[type] = checkBox;
        addTypeVisibilityCheckBox(checkBox);
        connect(checkBox, &QCheckBox::clicked,[this,type](bool state){
            this->messageTypeViewChanged(type,state);
        });

        if constexpr (sizeof...(Other) > 0)
            _setupUi<Other...>();
    };
};

}; // namespace Draupnir::Messages

#endif // MESSAGEVIEWCONFIGDIALOGTEMPLATE_H
