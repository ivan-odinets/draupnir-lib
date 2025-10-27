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

#ifndef CHECKABLEACTIONHANDLER_H
#define CHECKABLEACTIONHANDLER_H

#include <QAction>

namespace Draupnir::Handlers {

/*! @class CheckableActionHandler draupnir/handlers/templates/CheckableActionHandler.h
 *  @ingroup HandlerTemplates
 *  @brief CRTP base class for handling QAction objects that are checkable (emit triggered(bool)).
 *  @tparam Implementation The derived class implementing method onTriggered(bool).
 *
 *  @details Provides a standard connection to QAction::triggered(bool) and delegates the event handling to provided
 *           Implementation class (method Implementation::onTriggered(bool)).
 *
 *           To be used via CRTP:
 *           @code
 *           class MyCheckable : public CheckableActionHandler<MyCheckable>
 *           {
 *           public:
 *               void onTriggered(bool checked) {
 *                   // your logic...
 *               }
 *           };
 *           @endcode
 *
 * @note The class stores a raw pointer to QAction; make sure the action outlives the handler.
 * @note The slot is connected without a QObject context; if needed, add a context in the derived
 *       class or disconnect before destroying the QAction / handler. */

template<class Implementation>
class CheckableActionHandler
{
public:
    /*! @brief Constructor. Initializes internal p_action pointer with nullptr. */
    CheckableActionHandler() :
        p_action{nullptr}
    {}

    /*! @brief Connects the QAction::triggered(bool) signal to Implementation::onTriggered(bool).
     *  @param action Pointer to a QAction. Must be checkable. */
    void connect(QAction* action) {
        p_action = action;

        QObject::connect(action, &QAction::triggered, [this](bool state){
            static_cast<Implementation*>(this)->onTriggered(state);
        });
    }

protected:
    /*! @brief Access the connected QAction.
     *  @return Stored pointer to the QAction (may be nullptr before connectAction()). */
    QAction* action() { return p_action; }

private:
    QAction* p_action;
};

};

#endif // CHECKABLEACTIONHANDLER_H
