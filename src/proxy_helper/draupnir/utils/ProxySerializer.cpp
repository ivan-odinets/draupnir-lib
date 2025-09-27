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

#include "draupnir/utils/ProxySerializer.h"

#include <QRegularExpression>

namespace Draupnir::Proxy
{

bool ProxySerializer::isValidProxy(const QString& input)
{
    static const QRegularExpression proxyPattern("^(http|socks5)://[^:]+:" // Match the protocol + host
                                      "([1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5]|[1-9]\\d{0,3})$"); // Match port 1 - 65535
    return proxyPattern.match(input).hasMatch();
}

QNetworkProxy ProxySerializer::proxyFromString(QString inputString)
{
    QNetworkProxy result;

    if (inputString.isEmpty()) {
        result.setType(QNetworkProxy::NoProxy);
        return result;
    }

    //Get proxy type
    static const QRegularExpression protocolRegExp("^.*(?=://)");
    const QRegularExpressionMatch protocolMatch = protocolRegExp.match(inputString);
    if (!protocolMatch.hasMatch()) { //No protocol specified
        // qWarning() << QString("Error parsing proxy protocol!");
        return QNetworkProxy();
    }

    const QString protocol = protocolMatch.captured();
    if (protocol == QStringLiteral("http")) {
        result.setType(QNetworkProxy::HttpProxy);
        inputString.remove(QRegExp("^http://"));
    } else if (protocol == QStringLiteral("socks5")) {
        result.setType(QNetworkProxy::Socks5Proxy);
        inputString.remove(QRegExp("^socks5://"));
    } else {
        // qWarning() << QString("Unknown proxy protocol %1. Disabling proxy.").arg(protocol);
        return QNetworkProxy();
    }

    //Get proxy server
    static const QRegularExpression hostnameRegExp("^[^:]+(?=:)");
    const QRegularExpressionMatch hostnameMatch = hostnameRegExp.match(inputString);
    if (!hostnameMatch.hasMatch()) {
        //Probably some syntax error, return sth
        // qWarning() << "Error parsing proxy host. Disabling proxy...";
        return QNetworkProxy();
    }

    result.setHostName(hostnameMatch.captured());
    inputString.remove(hostnameRegExp);
    if (!inputString.startsWith(":")) {
        //Probably no port specified, return sth
        // qWarning() << "Error parsing proxy port. Disabling proxy...";
        return QNetworkProxy();
    }

    inputString.remove(QRegExp("^:"));
    bool ok = false;
    const qint32 proxyPort = inputString.toInt(&ok);
    if (!ok || proxyPort <= 0 || proxyPort > 65535) {
        // qWarning() << "Another error parsing proxy port. Disabling proxy...";
        return QNetworkProxy();
    }

    result.setPort(proxyPort);

    return result;
}

QString ProxySerializer::stringFromProxy(const QNetworkProxy& proxy)
{
    QString proxyType;
    switch (proxy.type()) {
    case QNetworkProxy::HttpProxy:
    case QNetworkProxy::HttpCachingProxy:
        proxyType = "http";
        break;
    case QNetworkProxy::Socks5Proxy:
        proxyType = "socks5";
        break;
    case QNetworkProxy::DefaultProxy:
    case QNetworkProxy::FtpCachingProxy:
    case QNetworkProxy::NoProxy:
        return QString();
    }

    return QString("%1://%2:%3")
            .arg(proxyType)
            .arg(proxy.hostName())
            .arg(proxy.port());
}

}; // namespace Draupnir::Proxy
