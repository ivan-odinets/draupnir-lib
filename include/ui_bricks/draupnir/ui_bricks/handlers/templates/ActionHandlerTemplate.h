/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2025-2026 Ivan Odinets <i_odinets@protonmail.com>
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

#ifndef ACTIONHANDLERTEMPLATE_H
#define ACTIONHANDLERTEMPLATE_H

#include <QAction>

#include "draupnir/ui_bricks/concepts/ActionHandlerConcept.h"

namespace Draupnir::Handlers {

/*! @class ActionHandlerTemplate draupnir/ui_bricks/handlers/templates/ActionHandlerTemplate.h
 *  @ingroup UiBricks
 *  @brief CRTP base class for handling regular `QAction` objects.
 *  @tparam Implementation The derived class implementing either onTriggered() or onTriggered(bool)
 *
 *  @details Provides a standard connection from QAction::triggered() to Implementation::onTriggered(). Intended for simple
 *           actions without a checked/unchecked state. */

template<class Implementation>
class ActionHandlerTemplate
{
public:
    ActionHandlerTemplate() :
        p_action{nullptr}
    {
        static_assert(ActionHandlerConcept<Implementation>,
            "ActionHandler<Implementation>: Implementation must define exactly one of void onTriggered() or void onTriggered(bool).");
    };

    /*! @brief Connects QAction::triggered() to Implementation::onTriggered().
     *  @param action Pointer to a QAction (does not need to be checkable). */
    void connect(QAction* action) {

        Q_ASSERT(p_action == nullptr);
        Q_ASSERT(action);

        if constexpr (ActionHandler::HasRuntimeOnTriggered<Implementation>) {
            QObject::connect(action, &QAction::triggered, [this](){
                static_cast<Implementation*>(this)->onTriggered();
            });
        }

        if constexpr (ActionHandler::HasRuntimeOnTriggeredWithBool<Implementation>) {
            Q_ASSERT(action->isCheckable());
            p_action = action;

            QObject::connect(action, &QAction::triggered, [this](bool state){
                static_cast<Implementation*>(this)->onTriggered(state);
            });
        }
    }

    void setActionChecked(bool state) requires(ActionHandler::HasRuntimeOnTriggeredWithBool<Implementation>) {
        Q_ASSERT(p_action);
        p_action->setChecked(state);
    }

protected:
    QAction* action() { return p_action; }

private:
    QAction* p_action;
};

}; // namespace Draupnir::Handlers

#endif // ACTIONHANDLERTEMPLATE_H
