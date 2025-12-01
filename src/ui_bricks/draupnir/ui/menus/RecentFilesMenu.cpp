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

#include "draupnir/ui_bricks/ui/menus/RecentFilesMenu.h"

#include <QDebug>
#include <QEvent>

namespace Draupnir::Ui {

RecentFilesMenu::RecentFilesMenu(QWidget* parent) :
    QMenu{parent},
    w_recentFilesActions{new QActionGroup{this}},
    w_recentFilesSeparator{nullptr},
    w_clearRecentAction{new QAction{this}}
{
    _setupUi();
}

RecentFilesMenu::RecentFilesMenu(const QString& title, QWidget* parent) :
    QMenu{title,parent},
    w_recentFilesActions{new QActionGroup{this}},
    w_recentFilesSeparator{nullptr},
    w_clearRecentAction{new QAction{this}}
{
    _setupUi();
}

QAction* RecentFilesMenu::getFileAction(const QFileInfo& fileInfo)
{
    for (auto action : w_recentFilesActions->actions()) {
        if (action->data().value<QFileInfo>() == fileInfo)
            return action;
    }

    return nullptr;
}

void RecentFilesMenu::loadRecentFiles(const QStringList& filePathsList)
{
    reset();
    for (const QString& filePath : filePathsList) {
        const QFileInfo fileInfo{filePath};
        addRecentAction(fileInfo);
        m_recentFiles.append(std::move(fileInfo));
    }
}

void RecentFilesMenu::loadRecentFiles(const QList<QFileInfo>& fileInfoList)
{
    reset();
    for (const QFileInfo& fileInfo : fileInfoList) {
        addRecentAction(fileInfo);
    }
    m_recentFiles = fileInfoList;
}

void RecentFilesMenu::loadRecentFiles(QList<QFileInfo>&& fileInfoList)
{
    reset();
    for (const QFileInfo& fileInfo : fileInfoList) {
        addRecentAction(fileInfo);
    }
    m_recentFiles = std::move(fileInfoList);
}

void RecentFilesMenu::reset()
{
    const QList<QAction*> actions = w_recentFilesActions->actions();
    for (auto action : actions) {
        removeAction(action);
        w_recentFilesActions->removeAction(action);
        action->deleteLater();
    }

    m_recentFiles.clear();
}

QStringList RecentFilesMenu::recentFilesPathsList() const
{
    QStringList result;
    for (const QFileInfo& fileInfo : m_recentFiles)
        result.append(fileInfo.absoluteFilePath());

    return result;
}

void RecentFilesMenu::addRecentAction(const QFileInfo& fileInfo)
{
    QAction* recentFileAction = new QAction(fileInfo.fileName());
    recentFileAction->setData(QVariant::fromValue(fileInfo));
    recentFileAction->setToolTip(fileInfo.filePath());

    insertAction(w_recentFilesSeparator,recentFileAction);
    w_recentFilesActions->addAction(recentFileAction);
    m_recentFiles.append(fileInfo);
}

void RecentFilesMenu::updateRecentAction(const QFileInfo& oldFileInfo, const QFileInfo& newFileInfo)
{
    QAction* action = getFileAction(oldFileInfo);
    Q_ASSERT_X(action, "RecentFilesMenu::updateRecentAction",
               "Provided oldFileInfo is not mathcing to any of the QActions within this RecentFilesMenu");
    Q_ASSERT_X(m_recentFiles.contains(oldFileInfo), "RecentFilesMenu::updateRecentAction",
               "Provided oldFileInfo is not contained within the QList<QFileInfo> m_recentFiles");

    updateRecentAction(action, newFileInfo);
    m_recentFiles.replace(
        m_recentFiles.indexOf(oldFileInfo), newFileInfo
    );
}

void RecentFilesMenu::updateRecentAction(QAction* action, const QFileInfo& fileInfo)
{
    Q_ASSERT_X(w_recentFilesActions->actions().contains(action),"RecentFilesMenu::removeRecentAction",
               "Provided QAction is not from this RecentFilesMenu.");

    action->setText(fileInfo.fileName());
    action->setData(QVariant::fromValue(fileInfo));
}

void RecentFilesMenu::removeRecentAction(QAction* action)
{
    Q_ASSERT_X(w_recentFilesActions->actions().contains(action),"RecentFilesMenu::removeRecentAction",
               "Provided QAction is not from this RecentFilesMenu.");

    removeAction(action);
    w_recentFilesActions->removeAction(action);
    action->deleteLater();
}

void RecentFilesMenu::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        _retranslateUi();
    }

    QMenu::changeEvent(event);
}

void RecentFilesMenu::_onRecentFileSelected(QAction* action)
{
    Q_ASSERT_X(sender(), "RecentFilesMenu::_onRecentFileSelected",
               "This method should be called as slot.");
    Q_ASSERT_X(action, "RecentFilesMenu::_onFecentFileSelected",
               "Provided QAction can not be nullptr");

    emit recentFileSelected(action->data().value<QFileInfo>());
}

void RecentFilesMenu::_onRecentClearRequested()
{
    reset();
    emit recentFilesMenuCleared();
}

void RecentFilesMenu::_setupUi()
{
    connect(w_recentFilesActions, &QActionGroup::triggered,
            this,                 &RecentFilesMenu::_onRecentFileSelected);


    connect(w_clearRecentAction,  &QAction::triggered,
            this,                 &RecentFilesMenu::_onRecentClearRequested);
    addAction(w_clearRecentAction);

    w_recentFilesSeparator = addSeparator();
    _retranslateUi();
}

void RecentFilesMenu::_retranslateUi()
{
    w_clearRecentAction->setText(tr("Clear"));
}

} // namespace Draupnir::Ui
