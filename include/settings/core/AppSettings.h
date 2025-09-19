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

#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QDebug>
#include <QString>
#include <QVariant>

#include <QSettings>

/*! @class AppSettings
 *  @brief Wrapper class around QSettings providing sectioned access, optional config preservation, and enum support.
 *  @details AppSettings simplifies work with application configuration stored via QSettings:
 *           - Provides access by section (Core, Network, Files, GUI, Global);
 *           - Allows "preserve mode" (if enabled, settings are never written to disk);
 *           - Offers templated methods for seamless enum <-> string conversion for config keys;
 *
 *           To use enum types with AppSettings template methods, your type must be a class or struct (Container) that
 *           defines:
 *           - enum Type;
 *           - static Type fromConfigString(const QString&);
 *           - static QString toConfigString(Type); */

class AppSettings
{
public:
    /*! @brief Defines configuration sections (used as key prefixes). Global: keys without a specific section. Core, Network,
     *         Files, GUI: map to corresponding QSettings sections/prefixes. */
    enum Section {
        Global,    /*!< @brief This corresponds to the default section of the config file: everything which is not fitting
                               to the categories below. */
        Core,      /*!< @brief This corresponds to the [core] section of config file. */        
        Network,   /*!< @brief This corresponds to the [network] section of config file. */
        Files,     /*!< @brief This corresponds to the [files] section of config file. */
        GUI,       /*!< @brief This corresponds to the [gui] section of config file. */
    };

    AppSettings(QSettings* settings) :
        p_settings{settings}
    {}

    /*! @brief Default constructor.
     *  @details Initializes QSettings and sets preserveConfig to false. */
    AppSettings();

    /*! @brief Destructor.
     *  @details Synchronizes (flushes) changes and deletes internal QSettings instance. */
    ~AppSettings();

    /*! @brief Enable or disable preservation mode (no writing to config file).
     *  @param arg - if true, changes will not be persisted to disk. */
    void setPreserveConfig(bool arg) { m_preserveConfig = arg; }

    /*! @brief Returns true if preservation mode is enabled. */
    bool preserveConfig() const { return m_preserveConfig; }

    /*! @brief Checks if a value exists by key (optionally in a given section).
     * @param key - configuration key (optionally with section prefix).
     * @return True if the value exists.
     * @note If the provided key has no section specified (e.g. gui/mainWindowSize) - this method checks keys in the Global
     *       section of config file. */
    bool contains(const QString& key) const;

    /*! @brief Checks if a value exists in the specified section.
     *  @param section - section to look in.
     *  @param key - key (relative to section).
     *  @return True if the value exists. */
    bool contains(Section section, const QString& key) const;

    /*! @brief Removes a value from config by key (if not preserving). Does nothing if AppSettings::preserveConfig() is true.
     *  @param key - configuration key (optionally with section prefix).
     * @note If the provided key has no section specified (e.g. gui/mainWindowSize) - this method removes the key from the
     *       Global section of config file. */
    void remove(const QString& key);

    /*! @brief Removes a value from a specific section (if not preserving). Does nothing if AppSettings::preserveConfig() is true.
     *  @param section - section to remove from.
     *  @param key - key to remove. */
    void remove(Section section, const QString& key);

///@name Generic QVariant-based access methods
///@{

    /*! @brief Reads a QVariant from config by key, with optional default.
     *  @param key - configuration key.
     *  @param defaultValue - value to return if key is missing.
     *  @return The value (or defaultValue if missing).
     * @note If the provided key has no section specified (e.g. gui/mainWindowSize) - this method reads keys from the Global
     *       section of config file. */
    QVariant value(const QString& key,const QVariant& defaultValue = QVariant()) const;

    /*! @brief Reads a QVariant from a specific section.
     *  @param section - section to look in.
     *  @param key - key to read.
     *  @param defaultValue - value to return if key is missing.
     *  @return The value (or defaultValue if missing). */
    QVariant value(Section section, const QString& key,const QVariant& defaultValue = QVariant()) const;

    /*! @brief Saves value represented by key in opened QSettings object. If preserveConfig == true, this method does nothing.
     * @note If the provided key has no section specified (e.g. gui/mainWindowSize) - this method writes keys to the Global
     *       section of config file. */

    /*! @brief Writes a QVariant value to config by key (if not preserving). Does nothing if AppSettings::preserveConfig() is true.
     *  @param key - key to write.
     *  @param value - value to store. */
    void setValue(const QString& key,const QVariant& value);

    /*! @brief Writes a QVariant value to config in a specific section (if not preserving). Does nothing if AppSettings::preserveConfig()
     *         is true.
     *  @param section - section to write in.
     *  @param key - key to write.
     *  @param value - value to store. */
    void setValue(Section section, const QString& key,const QVariant& value);
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
        const QString valueString = AppSettings::value(key,QString()).toString();
        return Container::fromConfigString(valueString);
    }

    /*! @brief Reads an enum value from a specific section.
     *  @tparam Container - class that must define: enum Type and static Type fromConfigString(const QString&).
     *  @param section - section to look in.
     *  @param key - key to read.
     *  @return Enum value parsed from string or default-constructed if missing. */
    template<class Container>
    typename Container::Type value(Section section, const QString& key) {
        return AppSettings::value<Container>(_sectionToString(section) + key);
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
        const QString valueString = AppSettings::value(key,QString()).toString();
        return (valueString.isEmpty()) ? defaultValue : Container::fromConfigString(valueString);
    }

    /*! @brief Reads an enum value from a specific section with default.
     *  @tparam Container - class that must define: enum Type and static Type fromConfigString(const QString&).
     *  @param section - section to look in.
     *  @param key - key to read.
     *  @param defaultValue - value to use if key is missing or invalid.
     *  @return Enum value or defaultValue if missing. */
    template<class Container>
    typename Container::Type value(Section section, const QString& key, const typename Container::Type defaultValue) {
        return AppSettings::value<Container>(_sectionToString(section) + key,defaultValue);
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
        AppSettings::setValue(key,Container::toConfigString(value));
    }

    /*! @brief Writes an enum value to config in a specific section. Does nothing if AppSettings::preserveConfig()
     *         is true.
     *  @tparam Container Class that must define: enum Type and static QString toConfigString(Type).
     *  @param section - section to write in.
     *  @param key - key to write.
     *  @param value - enum value to store (converted to string). */
    template<class Container>
    void setValue(Section section, const QString& key, const typename Container::Type value) {
        AppSettings::setValue<Container>(_sectionToString(section)+key,value);
    }
///@}

//private:
    Q_DISABLE_COPY(AppSettings);
    bool        m_preserveConfig;

    QSettings*  p_settings;

    /*! @brief Converts a Section enum to its string prefix for use in config keys.
     *  @param section - section to convert.
     *  @return QString prefix for the section. */
    static QString _sectionToString(Section section);
};

#endif // APPSETTINGS_H
