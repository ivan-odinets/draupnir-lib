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

#include "draupnir/core/AppSettings.h"

#include <QDebug>
#include <QSettings>

static const QLatin1String globalSection_settingsKey{ ""        };
static const QLatin1String coreSection_settingsKey{   "core/"   };
static const QLatin1String networkSection_settingsKey{"network/"};
static const QLatin1String filesSection_settingsKey{  "files/"  };
static const QLatin1String guiSection_settingsKey{    "gui/"    };

AppSettings::AppSettings() :
    m_preserveConfig{false},
    p_settings{nullptr}
{
    QSettings::setDefaultFormat(QSettings::NativeFormat);
    p_settings = new QSettings;
}

AppSettings::~AppSettings()
{
    p_settings->sync();
    delete p_settings;
}

bool AppSettings::contains(const QString& key) const
{
    return p_settings->contains(key);
}

bool AppSettings::contains(Section section, const QString& key) const
{
    return contains(_sectionToString(section) + key);
}

void AppSettings::remove(const QString& key)
{
    if (m_preserveConfig)
        return;

    p_settings->remove(key);
}

void AppSettings::remove(Section section, const QString& key)
{
    remove(_sectionToString(section) + key);
}

QVariant AppSettings::value(const QString& key,const QVariant& defaultValue) const
{
    return p_settings->value(key,defaultValue);
}

QVariant AppSettings::value(Section section, const QString& key,const QVariant& defaultValue) const
{
    return value(_sectionToString(section) + key, defaultValue);
}

void AppSettings::setValue(const QString& key,const QVariant& value)
{
    if (m_preserveConfig)
        return;

    p_settings->setValue(key,value);
}

void AppSettings::setValue(Section section, const QString& key,const QVariant& value)
{
    setValue(_sectionToString(section) + key, value);
}

QString AppSettings::_sectionToString(AppSettings::Section section)
{
    switch (section) {
    case Global:
        return globalSection_settingsKey;
    case Core:
        return coreSection_settingsKey;
    case Network:
        return networkSection_settingsKey;
    case Files:
        return filesSection_settingsKey;
    case GUI:
        return guiSection_settingsKey;
    }
    Q_ASSERT_X(false,"AppSettings::_sectionToString",
               "All values from the AppSettings::Section enum must be handled within this method.");
    return QString();
}
