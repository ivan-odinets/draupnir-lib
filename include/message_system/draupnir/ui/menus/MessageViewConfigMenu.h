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

#ifndef MESSAGEVIEWCONFIGMENU_H
#define MESSAGEVIEWCONFIGMENU_H

#include <QMenu>
#include <QObject>

#include "draupnir/core/MessageType.h"

namespace Draupnir::Messages
{

/*! @class MessageViewConfigMenu draupnir/ui/menus/MessageViewConfigMenu.h
 *  @brief An QMenu-based menu capable of displaying/editing settings of the MessageListView widget.
 *  @details As many other things within this MessageSystem - this menu consists of two parts: a MessageViewConfigMenu - interface
 *           to the Qt-world and MessageViewConfigMenuTemplate - class implementing template part of this interface. */

class MessageViewConfigMenu : public QMenu
{
    Q_OBJECT
public:
    /*! @brief Default constructor. */
    explicit MessageViewConfigMenu(QWidget* parent = nullptr);

    /*! @brief Trivial destructor. */
    ~MessageViewConfigMenu() override = default;

    /*! @brief Virtual method implemented in MessageViewConfigMenuTemplate which marks if specified MessageType is selected.
     * @note No signals should be emitted. */
    virtual void displayTypeSelected(MessageType type, bool isSelected) = 0;

    /*! @brief Virtual method implemented in MessageViewConfigMenuTemplate which displays state of message filtering.
     * @note No signals should be emitted. */
    virtual void displayFilterConfig(MessageType config) = 0;

signals:
    /*! @brief This signal is emitted when user checks/unchecks QAction representing certain MessageType. */
    void messageTypeVisibilityChanged(MessageType messageType, bool isVisible);

protected:
    /*! @brief Handles dynamic retranslation when the application language changes.
     *  @details Updates all visible labels and translated strings in the UI.
     *  @param event The event being handled. */
    void changeEvent(QEvent* event) final;

    /*! @brief This method is used to add QAction's representing individual message types. To be used by the MessageViewConfigTemplate. */
    void addMessageTypeAction(QAction* action);

    /*! @brief This methid is implemented within MessageViewConfigTemplate and retranslates all displayed QAction's. It is called
     *         from the MessageHandler::_retranslateUi method. */
    virtual void retranslateTypesActions() = 0;

private:
    void _retranslateUi();
    QAction* w_messageTypesSection;
};

}; // namespace Draupnir::Messages

#include "draupnir/containers/fixed_map.h" // IWYU pragma: keep

/*! @class MessageViewConfigMenuTemplate draupnir/ui/MessageViewConfigMenu.h
 *  @brief This template class implements message-type-list dependant functionality of the MessageViewConfigMenu. */

namespace Draupnir::Messages {

template<class... MessageTraits>
class MessageViewConfigMenuTemplate final : public MessageViewConfigMenu
{
public:
    /*! @brief Default constructor. By calling private method MessageViewConfigMenuTemplate::_initActions goes through list
     *         of provided MessageTypes and for each of them adds corresponding entry to the MessageViewConfigMenu. */
    explicit MessageViewConfigMenuTemplate(QWidget* parent = nullptr) :
        MessageViewConfigMenu{parent}
    {
        _initActions<MessageTraits...>();
    }

    /*! @brief Trivial destructor. */
    ~MessageViewConfigMenuTemplate() final = default;

    /*! @brief Implementation of MessageViewConfigMenu::displayTypeSelected method. Marks action associated with provided
     *         MessageType as selected / deselected.
     * @note No signals are emitted. */
    void displayTypeSelected(MessageType type, bool isSelected) final {
        m_actionMap[type]->setChecked(isSelected);
    }

    /*! @brief Template version of MessageViewConfigMenuTemplate::displayTypeSelected method.
     * @todo This method can be optimized. */
    template<class MessageTrait>
    void displayTypeSelected(bool isSelected) {
        displayTypeSelected(MessageTrait::type,isSelected);
    }

    /*! @brief Implementation of MessageViewConfigMenu::displayFilterConfig method. According to the provided filter configuration
     *         marks related QAction entries as selected / deselected.
     * @note No signals should are emitted. */
    void displayFilterConfig(MessageType config) final {
        m_actionMap.for_each_pair([config](auto& pair){
            pair.second->setChecked(pair.first & config);
        });
    };

    /*! @brief Implementation of MessageViewConfigMenu::retranslateTypesActions method. Retranslates QAction's for registered message
     *         types. */
    void retranslateTypesActions() {
        _retranslateTypesActionsImpl<MessageTraits...>();
    }

private:
    static constexpr MessageType supportedMessageIds[] = { MessageTraits::type... };

    fixed_map<
        supportedMessageIds,
        QAction*
    > m_actionMap;

    template<class First, class... Rest>
    inline void _initActions() {
        const MessageType type = First::type;
        QAction* act = new QAction{First::displayName()};
        act->setCheckable(true);
        connect(act,&QAction::triggered,[this,type](bool state){
            emit messageTypeVisibilityChanged(type,state);
        });
        addMessageTypeAction(act);
        m_actionMap[type] = act;

        if constexpr (sizeof...(Rest) > 0)
            _initActions<Rest...>();
    }

    template<class First, class... Rest>
    inline void _retranslateTypesActionsImpl() {
        m_actionMap[First::type]->setText(
            First::displayName()
        );

        if constexpr (sizeof...(Rest) > 0)
            _retranslateTypesActionsImpl<Rest...>();
    }
};

}; // namespace Draupnir::Messages

#endif // MESSAGEVIEWCONFIGMENU_H
