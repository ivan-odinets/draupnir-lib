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

#ifndef MESSAGEVIEWCONFIGDIALOG_H
#define MESSAGEVIEWCONFIGDIALOG_H

#include <QDialog>

#include <QDebug>

class QCheckBox;
class QDialogButtonBox;
class QFormLayout;
class QLabel;
class QVBoxLayout;

#include "MessageType.h"

/*! @class MessageViewConfigDialog draupnir-lib/src/messages/MessageViewConfigDialog.h
 *  @brief An dialog capable of displaying/editing settings of the MessageListView widget. */

class MessageViewConfigDialog : public QDialog
{
    Q_OBJECT
public:
    /*! @brief Constructor. Initializes internal UI elements. */
    explicit MessageViewConfigDialog(QWidget* parent = nullptr);

    /*! @brief Destructor. Trivial. */
    ~MessageViewConfigDialog() override = default;

    /*! @brief Virtual method implemented in MessageViewConfigDialogTemplate which marks if specified MessageType is selected.
     * @note No signals should be emitted. */
    virtual void displayTypeSelected(MessageType type, bool isSelected) = 0;

    /*! @brief Virtual method implemented in MessageViewConfigDialogTemplate which displays state of message filtering.
     * @note No signals should be emitted. */
    virtual void displayFilterConfig(MessageType config) = 0;

signals:
    /*! @brief This signal is emitted when user checks/unchecks QAction representing certain MessageType. */
    void messageTypeViewChanged(MessageType type, bool state);

protected:
    void addTypeVisibilityCheckBox(QCheckBox* checkBox);

private:
    QVBoxLayout* p_typeSelectorLayout;
    QDialogButtonBox* p_buttons;
};

#include <QCheckBox>
#include <QLabel>

#include "../../../include/containers/fixed_map.h"

template<class... MessageTypes>
class MessageViewConfigDialogTemplate : public MessageViewConfigDialog
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

    fixed_map<
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

#endif // MESSAGEVIEWCONFIGDIALOG_H
