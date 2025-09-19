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

#ifndef MESSAGEDISPLAYWIDGET_H
#define MESSAGEDISPLAYWIDGET_H

#include <QWidget>
#include <QSize>

class QLabel;

class Message;

/*! @class MessageDisplayWidget draupnir-lib/src/messages/widgets/MessageDisplayWidget.h
 *  @brief A lightweight QWidget used to visually display the contents of a single Message.
 *  @details This widget is designed to present a `Message` object in a user-friendly form using labels for the brief description,
 *           detailed explanation, timestamp, and an icon.
 *
 *           The layout consists of:
 *           - A left-aligned icon
 *           - A right-aligned `QFormLayout` with labels for each field
 *
 *           This widget supports dynamic retranslation via `QEvent::LanguageChange` and allows programmatic clearing or updating
 *           of the displayed content.
 *
 * @see Message. */

class MessageDisplayWidget final : public QWidget
{
    Q_OBJECT
public:
    /*! @brief Default constructor. */
    explicit MessageDisplayWidget(QWidget *parent = nullptr);

    /*! @brief Destructor. */
    ~MessageDisplayWidget() final {}

    /*! @brief Clearing the widget state to the default one. */
    void clear();

    /*! @brief Show Message object within this widget.
     *  @param Message* - pointer to the Message object. Can not be nullptr. */
    void showMessage(Message* message);

protected:
    /*! @brief Reimplementation of this method required for retranslation of this dialog. */
    void changeEvent(QEvent* event) final;

private:
    void _retranslateUi();

    QSize m_iconSize;

    QLabel*   w_messageBriefNameLabel;
    QLabel*   w_messageBriefLabel;
    QLabel*   w_messageWhatNameLabel;
    QLabel*   w_messageWhatLabel;
    QLabel*   w_messageDateTimeNameLabel;
    QLabel*   w_messageDateTimeLabel;
    QLabel*   w_messageIconLabel;
};

#endif // MESSAGEDISPLAYWIDGET_H
