CONFIG += c++20

TEMPLATE = subdirs
CONFIG += ordered warn_on qt debug_and_release

SUBDIRS += \
    MessageSystem/MessageSystemTests.pro \
    ProxyHelperTest/ProxyHelperTest.pro \
    fixed_tuple_map \
    fixed_map
