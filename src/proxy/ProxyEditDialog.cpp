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

#include "ProxyEditDialog.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>

#include "ProxyEditWidget.h"

ProxyEditDialog::ProxyEditDialog(QWidget* parent) :
    QDialog{parent},
    w_proxyEditWidget{new ProxyEditWidget}
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(w_proxyEditWidget);

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    connect(buttons,&QDialogButtonBox::accepted,this,&ProxyEditDialog::accept);
    connect(buttons,&QDialogButtonBox::rejected,this,&ProxyEditDialog::reject);

    mainLayout->addWidget(buttons);
    setLayout(mainLayout);
}

void ProxyEditDialog::setProxy(const QNetworkProxy& proxy)
{
    w_proxyEditWidget->setProxy(proxy);
}

QNetworkProxy ProxyEditDialog::proxy() const
{
    return w_proxyEditWidget->proxy();
}
