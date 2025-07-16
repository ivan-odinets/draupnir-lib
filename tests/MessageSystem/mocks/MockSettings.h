#include "MessageSettingsInterface.h"

#include <QDebug>

#include "../../src/messages/core/DefaultMessageTraits.h"
#include "../../include/containers/fixed_map.h"

template<class... Messages>
class MockSettings final : public MessageSettingsInterface
{
public:
    ~MockSettings() final = default;

    bool contains(const QString& key) const final {
        return _containsImpl<Messages...>(key);
    }

    QVariant value(const QString& key, const QVariant& defaultValue = QVariant{}) {
        return _valueImpl<Messages...>(key,defaultValue);
    }

    void setValue(const QString& key, const QVariant& value) final {
        return _setValueImpl<Messages...>(key,value);
    }

    static constexpr MessageType types[] = { Messages::type... };

    fixed_map<
        types,
        QVariant
    > dummySettingsMap;

private:
    template<class First,class... Rest>
    bool _containsImpl(const QString& key) const {
        if (QString("notifications/").append(First::settingsKey) == key) {
            return true;
        }

        if constexpr (sizeof...(Rest) > 0) {
            return _containsImpl<Rest...>(key);
        } else {
            return false;
        }
    }

    template<class First,class... Rest>
    QVariant _valueImpl(const QString& key, const QVariant& defaultValue = QVariant()) {
        if (QString("notifications/").append(First::settingsKey) == key) {
            return dummySettingsMap[First::type];
        }

        if constexpr (sizeof...(Rest) > 0) {
            return _valueImpl<Rest...>(key,defaultValue);
        } else {
            return QVariant{};
        }
    }

    template<class First,class... Rest>
    void _setValueImpl(const QString& key, const QVariant& value) {
        if (QString("notifications/").append(First::settingsKey) == key) {
            dummySettingsMap[First::type] = value;
            return;
        }

        if constexpr (sizeof...(Rest) > 0) {
            _setValueImpl<Rest...>(key,value);
        }
    }
};
