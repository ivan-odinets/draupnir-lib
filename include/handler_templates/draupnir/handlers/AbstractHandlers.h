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

#ifndef ABSTRACTHANDLERS_H
#define ABSTRACTHANDLERS_H

#include <QAction>
#include <QDebug>

namespace Draupnir::Handlers {

/*! @class CheckableActionHandler draupnir/handlers/AbstractHandlers.h
 *  @brief CRTP base class for handling QAction objects that are checkable (emit triggered(bool)).
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
 *  @tparam Implementation The derived class implementing method onTriggered(bool).
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

/*! @class ActionHandler draupnir/handlers/AbstractHandlers.h
 *  @brief CRTP base class for handling regular - QAction objects (without a - bool parameter in the signal).
 *  @tparam Implementation The derived class implementing - onTriggered().
 *  @tparam MenuEntry The menu entry trait (kept for type association; not used internally).
 *
 *  @details Provides a standard connection from QAction::triggered() to Implementation::onTriggered(). Intended for simple
 *           actions without a checked/unchecked state.
 *
 *           @code
 *           class MyAction : public ActionHandler<MyAction, FileOpenEntry>
 *           {
 *               public:
 *                   void onTriggered() {
 *                       // your logic...
 *                   }
 *           };
 *           @endcode */

template<class Implementation, class MenuEntry>
class ActionHandler
{
public:
    /*! @brief Connects QAction::triggered() to Implementation::onTriggered().
     *  @param action Pointer to a QAction (does not need to be checkable). */
    void connect(QAction* action) {
        QObject::connect(action, &QAction::triggered, [this](){
            static_cast<Implementation*>(this)->onTriggered();
        });
    }
};

/*! @class CustomMenuHandler draupnir/handlers/AbstractHandlers.h
 *  @brief CRTP base class for handling “custom” menu entries (MenuEntry::Type is not QAction but a custom widget/menu).
 *  @tparam Implementation The derived class implementing \c connect(T*).
 *  @tparam MenuEntry The menu entry trait with \c using Type = ... (actual menu/widget type).
 *  @details Stores a pointer to an instance of MenuEntry::Type and delegates connection/initialization
 *           to Implementation::connect(typename MenuEntry::Type*).
 *
 *           @code
 *           class MyCustom : public CustomMenuHandler<MyCustom, RecentFileEntry>
 *           {
 *           public:
 *               void connect(RecentFilesMenu* m) {
 *                    // connect signals, set up initial state...
 *               }
 *           };
 *           @endcode
 *
 * @note The class stores a raw pointer to MenuEntry::Type; lifetime management is external. */

template<class Implementation, class MenuEntry>
class CustomMenuHandler
{
public:
    /*! @brief Constructor. Initializes internal p_menu pointer with nullptr. */
    CustomMenuHandler() :
        p_menu{nullptr}
    {}

    /*! @brief Stores the menu/widget pointer and calls \c Implementation::connect(...).
     *  @param menu Object of type \c MenuEntry::Type for handling/connection. */
    void connect(typename MenuEntry::Type* menu) {
        p_menu = menu;
        qDebug() << "Saving menu...";

        static_cast<Implementation*>(this)->connectImplementation(menu);
    }

protected:
    /*! @brief Access the connected menu/widget.
     *  @return Stored pointer (may be nullptr before connect()). */
    typename MenuEntry::Type* menu() { return p_menu; }

private:
    typename MenuEntry::Type* p_menu;
};

/*! @class GenericMenuEntryHandler draupnir/handlers/AbstractHandlers.h
 *  @brief Non-specialized template for menu entry handlers.
 *  @details This is the primary template for GenericMenuEntryHandler, serving as a catch-all for menu entry types that do
 *           not have a user-provided specialization. Its sole purpose is to trigger a compile-time error when instantiated
 *           with an unsupported menu entry type.
 *
 *           Attempting to instantiate this template directly will always result in a static_assert failure with a clear
 *           error message. You must provide a specialization for every menu entry type you intend to support.
 *
 *  @tparam Context - The context type (ignored in this base template).
 *  @tparam HandledEntry - The menu entry trait/class to be handled. */

template<class Context,class HandledEntry>
class GenericMenuEntryHandler
{
    template<class> static inline constexpr bool dependent_false_v = false;
public:
    /*! @brief Dummy constructor, triggers a compile-time error. Instantiating this template always results in a static_assert
     *         failure, indicating that a specialization for the given HandledEntry is missing. */
    GenericMenuEntryHandler() {
        static_assert(dependent_false_v<HandledEntry>,
                "GenericMenuEntryHandler template MUST be specialized for all handled entries.");
    }

    /*! @brief Dummy constructor, triggers a compile-time error. Instantiating this template always results in a static_assert
     *         failure, indicating that a specialization for the given HandledEntry is missing.
     *  @param Unused Reference to context (only required for interface compatibility).  */
    GenericMenuEntryHandler(Context&) {
        static_assert(dependent_false_v<HandledEntry>,
                "GenericMenuEntryHandler template MUST be specialized for all handled entries.");
    }
};

};

#endif // ABSTRACTHANDLERS_H
