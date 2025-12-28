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

#ifndef FILEMANAGERVALIDATOR_H
#define FILEMANAGERVALIDATOR_H

#include <type_traits>

#include <QString>
#include <QFileInfo>

namespace Draupnir::Handlers
{

/*! @class FileManagerValidator draupnir/utils/FileManagerValidator.h
 *  @ingroup HandlerTemplates
 *  @brief SFINAE-based trait collection for checking file manager interface compliance.
 *
 *  @details Provides a set of compile-time traits (via SFINAE and std::void_t) to verify that a candidate file manager class
 *           implements all required methods for integration with the file menu system. This allows template code to enforce
 *           interface contracts, producing clear compile-time errors when methods are missing or have the wrong signature.
 *
 *           - All trait structs are used as follows: FileManagerValidator::has_newFile<YourClass>::value, etc.
 *           - Each trait checks for the *presence* and *signature* of the required method.
 *           - Traits are grouped by logical functionality: file management, status reporting, UI helpers.
 *
 * @note This class is intended to be used only at compile time; it contains no data or runtime logic.
 *
 * @todo Maybe it make sense to convert this class to a pure namespace? */

class FileManagerValidator
{
public:
///@name Group of structs for checking presense of file management methods (new, open, save, etc)
///@{

    /*! @brief Checks for newFile() method with signature void newFile(). */
    template<class, class = std::void_t<>>
    struct has_newFile : std::false_type {};

    template<class Class>
    struct has_newFile<
        Class,
        std::void_t<decltype(
            std::is_same_v<void,decltype(std::declval<Class>().newFile())>
        )>
    > : std::true_type {};

    /*! @brief Checks for openFile(const QFileInfo&) method returning void. */
    template<class, class = std::void_t<>>
    struct has_openFile : std::false_type {};

    template<class Class>
    struct has_openFile<
        Class,
        std::void_t<decltype(
            std::is_same_v<void,decltype(std::declval<Class>().openFile(std::declval<const QFileInfo&>()))>
        )>
    > : std::true_type {};

    /*! @brief Checks for openFiles(const QStringList&) method returning void. */
    template<class, class = std::void_t<>>
    struct has_openFiles : std::false_type {};

    template<class Class>
    struct has_openFiles<
        Class,
        std::void_t<decltype(
            std::is_same_v<void,decltype(std::declval<Class>().openFiles(std::declval<const QStringList&>()))>
        )>
    > : std::true_type {};

    /*! @brief Checks for saveCurrentFile() method with signature void saveCurrentFile(). */
    template<class Class, class = std::void_t<>>
    struct has_saveCurrentFile : std::false_type {};

    template<class Class>
    struct has_saveCurrentFile<
        Class,
        std::void_t<decltype(
            std::is_same_v<void,decltype(std::declval<Class>().saveCurrentFile())>
        )>
    > : std::true_type {};

    /*! @brief Checks for closeCurrentFile() method with signature void closeCurrentFile(). */
    template<class, class = std::void_t<>>
    struct has_closeCurrentFile : std::false_type {};

    template<class Class>
    struct has_closeCurrentFile<
        Class,
        std::void_t<decltype(
            std::is_same_v<void,decltype(std::declval<Class>().closeCurrentFile())>
        )>
    > : std::true_type {};
///@}

///@name Group of structs checking presense of status reporting methods (is sth opened, is everything saved, etc.)
///@{
    /*! @brief Checks for hasNothingOpened() method with signature bool hasNothingOpened(). */
    template<class, class = std::void_t<>>
    struct has_hasNothingOpened : std::false_type {};

    template<class Class>
    struct has_hasNothingOpened<
        Class,
        std::void_t<decltype(
            std::is_same_v<bool,decltype(std::declval<Class>().hasNothingOpened())>
        )>
    > : std::true_type {};

    /*! @brief Checks for isCurrentFileSaved() method with signature bool isCurrentFileSaved(). */
    template<class, class = std::void_t<>>
    struct has_isCurrentFileSaved : std::false_type {};

    template<class Class>
    struct has_isCurrentFileSaved<
        Class,
        std::void_t<decltype(
            std::is_same_v<bool,decltype(std::declval<Class>().isCurrentFileSaved())>
        )>
    > : std::true_type {};
///@}

///@name Group of structs for checking presense of UI-related methods (e.g. titles of dialogs)
///@{

    /*! @brief Checks for openDialogTitle() method with signature QString openDialogTitle(). */
    template<class, class = std::void_t<>>
    struct has_openDialogTitle : std::false_type {};

    template<class Class>
    struct has_openDialogTitle<
        Class,
        std::void_t<decltype(
            std::is_same_v<QString,decltype(std::declval<Class>().openDialogTitle())>
        )>
    > : std::true_type {};

    /*! @brief Checks for saveDialogTitle() method with signature QString saveDialogTitle(). */
    template<class, class = std::void_t<>>
    struct has_saveDialogTitle : std::false_type {};

    template<class Class>
    struct has_saveDialogTitle<
        Class,
        std::void_t<decltype(
            std::is_same_v<QString,decltype(std::declval<Class>().saveDialogTitle())>
        )>
    > : std::true_type {};

    /*! @brief Checks for supportedExtensions() method with signature QString supportedExtensions(). */
    template<class, class = std::void_t<>>
    struct has_supportedExtensions : std::false_type {};

    template<class Class>
    struct has_supportedExtensions<
        Class,
        std::void_t<decltype(
            std::is_same_v<QString,decltype(std::declval<Class>().supportedExtensions())>
        )>
    > : std::true_type {};
///@}
};

};

#endif // FILEMANAGERVALIDATOR_H
