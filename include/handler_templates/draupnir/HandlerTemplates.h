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

#ifndef HANDLERTEMPLATES_H
#define HANDLERTEMPLATES_H

/*! @file draupnir/HandlerTemplates.h
 *  @ingroup HandlerTemplates
 *  @brief This file is a general include to use the @ref HandlerTemplates module. */

// General things
#include "draupnir/handlers/templates/GenericMenuEntriesHandler.h"   // IWYU pragma: keep
#include "draupnir/handlers/templates/GenericMenuEntryHandler.h"     // IWYU pragma: keep

// File Menu handlers
#include "draupnir/handlers/file_menu/FileMenuEntriesHandler.h"      // IWYU pragma: keep
#include "draupnir/handlers/file_menu/FileNewEntryHandler.h"         // IWYU pragma: keep
#include "draupnir/handlers/file_menu/FileOpenEntryHandler.h"        // IWYU pragma: keep
#include "draupnir/handlers/file_menu/FileCloseEntryHandler.h"       // IWYU pragma: keep
#include "draupnir/handlers/file_menu/FileExitEntryHandler.h"        // IWYU pragma: keep
#include "draupnir/handlers/file_menu/FileRecentEntryHandler.h"      // IWYU pragma: keep
#include "draupnir/handlers/file_menu/FileSaveAsEntryHandler.h"      // IWYU pragma: keep
#include "draupnir/handlers/file_menu/FileSaveEntryHandler.h"        // IWYU pragma: keep

// Settings Menu handlers
#include "draupnir/handlers/settings_menu/SettingsMenuEntriesHandler.h"        // IWYU pragma: keep
#include "draupnir/handlers/settings_menu/SettingsCheckableEntryHandler.h"     // IWYU pragma: keep
#include "draupnir/handlers/settings_menu/MinimizeOnCloseEntryHandler.h"       // IWYU pragma: keep
#include "draupnir/handlers/settings_menu/MinimizeToTrayEntryHandler.h"        // IWYU pragma: keep
#include "draupnir/handlers/settings_menu/StartHiddenEntryHandler.h"           // IWYU pragma: keep

// Help Menu handlers
#include "draupnir/handlers/help_menu/HelpMenuEntriesHandler.h"           // IWYU pragma: keep
#include "draupnir/handlers/help_menu/AboutAppEntryHandler.h"             // IWYU pragma: keep
#include "draupnir/handlers/help_menu/AboutDraupnirLibEntryHandler.h"     // IWYU pragma: keep
#include "draupnir/handlers/help_menu/AboutQtEntryHandler.h"              // IWYU pragma: keep
#include "draupnir/handlers/help_menu/HelpEntryHandler.h"                 // IWYU pragma: keep

#endif // HANDLERTEMPLATES_H
