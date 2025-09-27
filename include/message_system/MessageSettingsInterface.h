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

#ifndef MESSAGESETTINGSINTERFACE_H
#define MESSAGESETTINGSINTERFACE_H

#ifndef DRAUPNIR_MSGSYS_CUSTOM_SETTINGS
    #error("This file is only needed when having DRAUPNIR_MSGSYS_CUSTOM_SETTINGS enabled")
#endif // DRAUPNIR_MSGSYS_CUSTOM_SETTINGS

#include <QVariant>
#include <QString>

/*! @class MessageSettingsInterface draupnir-lib/include/messages/MessageSettingsInterface.h
 *  @brief Interface for settings objects used within the MessageSystem.
 *  @details When enabled, MessageSystem-related components use this interface to access the application's settings.
 *           To implement this interface one must implement following methods:
 *           - bool MessageSettingsInterface::contains(const QString& key);
 *           - QVariant value(const QString& key, const QVariant& defaultValue = QVariant{});
 *           - void setValue(const QString& key, const QVariant& value);
 *           These methods should work the same as analogues within QSettings / AppSettings.
 *
 *           In addition for convenient usage within MessageSystem-related objects this interface has a few helper methods.
 *
 * @note This class is available only when building with DRAUPNIR_MSGSYS_CUSTOM_SETTINGS macro. */

class MessageSettingsInterface
{
public:
    /*! @brief Virtual trivial destructor. */
    virtual ~MessageSettingsInterface() = default;

///@name Group of virtual methods to be implemented while subclassing the MessageSystemInterface
///@{
    /*! @brief Should check if a value is existing for a provided key. */
    virtual bool contains(const QString& key) const = 0;

    /*! @brief Should return value from the settings storage associated with provided key. */
    virtual QVariant value(const QString& key, const QVariant& defaultValue = QVariant{}) = 0;

    /*! @brief Should set value within the settings storage associated with provided key. */
    virtual void setValue(const QString& key, const QVariant& value) = 0;
///@}

///@name Templated enum-based access methods.
///@{
    /*! @brief Reads an enum value from config by key using a container with required static methods.
     *  @tparam Container - class that must define: enum Type and static Type fromConfigString(const QString&).
     *  @param key - configuration key.
     *  @return Enum value parsed from string or default-constructed if missing.
     * @note If the provided key has no section specified (e.g. gui/mainWindowSize) - this method writes keys to the Global
     *       section of config file. */
    template<class Container>
    typename Container::Type value(const QString& key) {
        const QString valueString = value(key,QString()).toString();
        return Container::fromConfigString(valueString);
    }

    /*! @brief Reads an enum value with a provided default.
     *  @tparam Container - class that must define: enum Type and static Type fromConfigString(const QString&).
     *  @param key - configuration key.
     *  @param defaultValue - value to use if key is missing or invalid.
     *  @return Enum value or defaultValue if missing.
     * @note If the provided key has no section specified (e.g. gui/mainWindowSize) - this method writes keys to the Global
     *       section of config file. */
    template<class Container>
    typename Container::Type value(const QString& key, const typename Container::Type defaultValue) {
        const QString valueString = value(key,QString()).toString();
        return (valueString.isEmpty()) ? defaultValue : Container::fromConfigString(valueString);
    }


    /*! @brief Writes an enum value to config by key using a container with required static methods. Does nothing if AppSettings::preserveConfig()
     *         is true.
     *  @tparam Container Class that must define: enum Type and static QString toConfigString(Type).
     *  @param key - configuration key.
     *  @param value - enum value to store (converted to string).
     * @note If the provided key has no section specified (e.g. gui/mainWindowSize) - this method writes keys to the Global
     *       section of config file. */
    template<class Container>
    void setValue(const QString& key, const typename Container::Type value) {
        setValue(key,Container::toConfigString(value));
    }
///@}
};

/*! @class MessageSettingsAdapter draupnir-lib/include/messages/MessageSettingsInterface.h
 *  @brief Template-based adapter implementing a MessageSystemInterface. To be used for example with QSettings or AppSettings objects.
 *  @tparam Settings class providing methods required for MessageSettingsInterface. */

template<class Settings>
class MessageSettingsAdapter final : public MessageSettingsInterface
{
public:
    /*! @brief Constructing MessageSettingsAdapter by using provided pointer to Settings object. */
    MessageSettingsAdapter(Settings* settings) :
        MessageSettingsInterface{},
        p_settings{settings}
    {
        Q_ASSERT_X(settings,"MessageSettingsAdapter::MessageSettingsAdapter",
                   "Provided pointer is nullptr.");
    }

    /*! @brief Trivial virtual destructor. */
    ~MessageSettingsAdapter() final = default;

    /*! @brief Checks if a value is existing for a provided key within instance of Settings. */
    bool contains(const QString& key) const final {
        return p_settings->contains(key);
    };

    /*! @brief Should return value from the Settings associated with provided key. */
    QVariant value(const QString& key, const QVariant& defaultValue = QVariant{}) final {
        return p_settings->value(key,defaultValue);
    }

    /*! @brief Should set value within the Settings associated with provided key. */
    void setValue(const QString& key, const QVariant& value) final {
        p_settings->setValue(key,value);
    }

private:
    Settings* p_settings;
};

#endif // MESSAGESETTINGSINTERFACE_H
