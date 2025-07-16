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

#ifndef MESSAGESYSTEMINTERFACE_H
#define MESSAGESYSTEMINTERFACE_H

#include "MessageType.h"

#ifdef DRAUPNIR_MSGSYS_APP_SETTINGS
class AppSettings;
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS

#ifdef DRAUPNIR_MSGSYS_CUSTOM_SETTINGS
    #include "MessageSettingsInterface.h"
#endif // DRAUPNIR_MSGSYS_CUSTOM_SETTINGS

class MessageHandler;
class MessageUiBuilder;

/*! @class MessageSystemInterface draupnir-lib/include/messages/MessageSystemInterface.h
 *  @brief This is an interface class for a MessageSystemTemplate template class.
 *  @details This interface is used to simplify passing the objects of speific MessageSystemTemplate implementations at
 *           runtime.
 *
 * @see MessageSystemTemplate, MessageHanlder, MessageHandlerTemplate, MessageUiBuilder, MessageUiBuilderTemplate, Logger. */

class MessageSystemInterface
{
public:
    /*! @brief Trivial virtual destructor. */
    virtual ~MessageSystemInterface() = default;

#ifdef DRAUPNIR_MSGSYS_APP_SETTINGS
    /*! @brief This method should load settings for this MessageSystemInterface implementation from the provided AppSettings
     *         object.
     * @note This method is present only when having DRAUPNIR_MSGSYS_APP_SETTINGS marco enabled. */
    virtual void loadSettings(AppSettings* settings) = 0;
#endif // DRAUPNIR_MSGSYS_APP_SETTINGS

#ifdef DRAUPNIR_MSGSYS_CUSTOM_SETTINGS
    /*! @brief This method should load settings for this MessageSystemInterface implementation from the provided MessageSettingsInterface
     *         object.
     * @note This method is present only when having DRAUPNIR_MSGSYS_CUSTOM_SETTINGS marco enabled. */
    virtual void loadSettings(MessageSettingsInterface* settingsInterface) = 0;

    /*! @brief This is a helper method to simplify usage of QSettings / similar objects within MessageSystemInterface. This method creates
     *         MessageSettingsAdapter implementation by using specified Settings class and uses it as a settings stprage.
     *  @tparam Settings class of settings storage. This class should be usable as an argument for the MessageSettingsAdapter
     * @note This method is present only when having DRAUPNIR_MSGSYS_CUSTOM_SETTINGS marco enabled. */
    template<class SettingsImpl>
    void loadSettings(SettingsImpl* settingsImpl) {
        loadSettings(new MessageSettingsAdapter<SettingsImpl>{settingsImpl});
    }
#endif // DRAUPNIR_MSGSYS_CUSTOM_SETTINGS

    /*! @brief This method should return pointer to MessageHandler interface of this MessageSystem implementation. */
    virtual MessageHandler* handler() = 0;

    /*! @brief This method should return pointer to MessageUiBuilder interface of this MessageSystem implementation. */
    virtual MessageUiBuilder* uiBuilder() = 0;

    /*! @brief This method should return true if the specified type is known to this MessageSystem implementation. */
    virtual bool isTypeKnown(MessageType type) = 0;

protected:
    /*! @brief Constructor. Protected as this is an interface. */
    MessageSystemInterface() = default;
};

#endif // MESSAGESYSTEMINTERFACE_H
