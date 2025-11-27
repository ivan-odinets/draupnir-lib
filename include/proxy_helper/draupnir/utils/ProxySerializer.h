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

#ifndef PROXYSERIALIZER_H
#define PROXYSERIALIZER_H

#include <QNetworkProxy>

namespace Draupnir::Proxy
{

/*! @class ProxySerializer draupnir/utils/ProxySerializer.h
 *  @ingroup ProxyHelper
 *  @brief Static utility class for conversion between `QNetworkProxy` objects and their `QString` representations.
 *
 *  @details Supports parsing and formatting of HTTP and SOCKS5 proxy URLs in string form. Proxy authentication is
 *           currently not supported. */

class ProxySerializer
{
public:
    /*! @brief Checks if the given string is a valid proxy specification.
     *  @param input - proxy string to validate.
     *  @return True - if the string is a valid, supported proxy URL; false otherwise.
     *  @details Recognizes strings of the form: "<protocol>://host:port" where host is a non-empty hostname or IP address
     *           and port is in the range 1-65535. */
    static bool isValidProxy(const QString& input);

    /*! @brief Parses a string and returns the corresponding QNetworkProxy object.
     *  @param input - proxy string in the format "protocol://host:port".
     *  @return QNetworkProxy - object corresponding to the input string, or an invalid proxy on error.
     *  @details Supports HTTP and SOCKS5 proxies. If the input string is invalid or an unsupported protocol is given,
     *           returns a default-constructed (invalid) QNetworkProxy. */
    static QNetworkProxy proxyFromString(QString input);

    /*! @brief Converts supported proxies (HTTP, SOCKS5) into the form "protocol://host:port".
     *  @param proxy - QNetworkProxy object to serialize.
     *  @return QString representation of the proxy, or an empty string if not supported.
     *  @details Returns an empty string for unsupported proxy types or if the proxy is invalid. */
    static QString stringFromProxy(const QNetworkProxy& proxy);
};

}; // namespace Draupnir::Proxy

#endif // PROXYSERIALIZER_H
