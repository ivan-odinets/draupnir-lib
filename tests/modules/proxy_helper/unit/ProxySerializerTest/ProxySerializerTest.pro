TEST_NAME = $$basename(PWD)
include(../../../../common/TestConfig.pri)

QT += network

include(../../../../../modules/ProxyHelper.pri)

SOURCES +=  \
    ProxySerializerTest.cpp
