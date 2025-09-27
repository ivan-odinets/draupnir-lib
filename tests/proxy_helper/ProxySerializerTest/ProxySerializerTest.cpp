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

#include <QtTest>

#include "draupnir/utils/ProxySerializer.h"

/*! @class ProxySerializerTest tests/proxy_helper/ProxySerializerTest/ProxySerializerTest.cpp
 *  @brief Test class for the ProxySerializer static object. */

class ProxySerializerTest final : public QObject
{
    Q_OBJECT

public:
    ProxySerializerTest() = default;
    ~ProxySerializerTest() final = default;

    /*! @brief Static QVector with dataset of valid QNetworkProxy objects and their QString representations. */
    static const QVector<QPair<QString,QNetworkProxy> > validDatasetArray;

    /*! @brief Static QVector with dataset of invalid QNetworkProxy objects and their QString representations. */
    static const QVector<QPair<QString,QNetworkProxy> > invalidDatasetArray;

private slots:
    /*! @brief Tests ProxyHelper::isValidProxy method. */
    void testIsValidProxy() {
        // Confirm that valid proxies are indeed valid:
        for (auto data : validDatasetArray) {
            if (!Draupnir::Proxy::ProxySerializer::isValidProxy(data.first)) {
                qWarning() << "Current data: "<<data;
            }
            QVERIFY(Draupnir::Proxy::ProxySerializer::isValidProxy(data.first));
        }

        // Confirm that invalid proxies are indeed invalid:
        for (auto data : invalidDatasetArray) {
            if (Draupnir::Proxy::ProxySerializer::isValidProxy(data.first)) {
                qWarning() << "Current data: "<<data;
            }
            QVERIFY(!Draupnir::Proxy::ProxySerializer::isValidProxy(data.first));
        }
    }

    /*! @brief Tests ProxyHelper::proxyFromString method. */
    void testProxyFromString() {
        // Confirm that valid proxies are indeed valid:
        for (auto data : validDatasetArray) {
            const QNetworkProxy proxy = Draupnir::Proxy::ProxySerializer::proxyFromString(data.first);
            if (proxy != data.second) {
                qWarning() << "Current data: "<<data;
            }
            QVERIFY(proxy == data.second);
        }

        // Confirm that invalid proxies are indeed invalid:
        for (auto data : invalidDatasetArray) {
            const QNetworkProxy proxy = Draupnir::Proxy::ProxySerializer::proxyFromString(data.first);
            if (proxy != data.second) {
                qWarning() << "Current data: "<<data;
            }
            QVERIFY(proxy == data.second);
        }
    }

    /*! @brief Tests ProxyHelper::stringFromProxy method. */
    void testStringFromProxy() {
        // Confirm that valid proxies are indeed valid:
        for (auto data : validDatasetArray) {
            const QString proxyString = Draupnir::Proxy::ProxySerializer::stringFromProxy(data.second);
            if (proxyString != data.first) {
                qWarning() << "Current data: "<<data;
            }
            QVERIFY(proxyString == data.first);
        }

        // Confirm that invalid proxies are indeed invalid:
        for (auto data : invalidDatasetArray) {
            const QString proxyString = Draupnir::Proxy::ProxySerializer::stringFromProxy(data.second);
            if (proxyString != QString()) {
                qWarning() << "Current data: "<<data;
            }
            QVERIFY(proxyString.isEmpty());
        }
    }
};

const QVector<QPair<QString,QNetworkProxy> > ProxySerializerTest::validDatasetArray = {
    // Http proxy
    { "http://localhost:65535", QNetworkProxy{QNetworkProxy::HttpProxy, "localhost", 65535} },
    { "http://localhost:60000", QNetworkProxy{QNetworkProxy::HttpProxy, "localhost", 60000} },
    { "http://localhost:30000", QNetworkProxy{QNetworkProxy::HttpProxy, "localhost", 30000} },
    { "http://localhost:20000", QNetworkProxy{QNetworkProxy::HttpProxy, "localhost", 20000} },
    { "http://localhost:10000", QNetworkProxy{QNetworkProxy::HttpProxy, "localhost", 10000} },
    { "http://localhost:1000",  QNetworkProxy{QNetworkProxy::HttpProxy, "localhost", 1000} },
    { "http://localhost:100",   QNetworkProxy{QNetworkProxy::HttpProxy, "localhost", 100} },
    { "http://localhost:10",    QNetworkProxy{QNetworkProxy::HttpProxy, "localhost", 10} },
    { "http://1.1.1.1:80",      QNetworkProxy{QNetworkProxy::HttpProxy, "1.1.1.1", 80} },
    { "http://o:80",            QNetworkProxy{QNetworkProxy::HttpProxy, "o", 80} },

    // Socks5 proxy
    { "socks5://localhost:9050", QNetworkProxy{QNetworkProxy::Socks5Proxy, "localhost", 9050} },
    { "socks5://127.0.0.1:9050", QNetworkProxy{QNetworkProxy::Socks5Proxy, "127.0.0.1", 9050} }
};

const QVector<QPair<QString,QNetworkProxy> > ProxySerializerTest::invalidDatasetArray = {
    // Unsopported schemas
    { "https://localhost:10000",  QNetworkProxy{} },
    { "blah://localhost:10000",   QNetworkProxy{} },

    // Mistypes & wrong ports
    { "http://localhost:65536",   QNetworkProxy{} },
    { "http:/localhost:1000000",  QNetworkProxy{} },
    { "http:/localhost:70000",    QNetworkProxy{} },
    { "http://localhost:0",       QNetworkProxy{} },
    { "http://localhost:-100",    QNetworkProxy{} },
    { "http:/localhost:10000",    QNetworkProxy{} },
    { "http://10000",             QNetworkProxy{} },
    { "http//localhost:10000",    QNetworkProxy{} },
    { "http//localhost:",         QNetworkProxy{} },
    { "http://:111",              QNetworkProxy{} },
    { "http://::111",             QNetworkProxy{} },

    // Random data
    { "some random string",       QNetworkProxy{} },
};

QTEST_APPLESS_MAIN(ProxySerializerTest)

#include "ProxySerializerTest.moc"
