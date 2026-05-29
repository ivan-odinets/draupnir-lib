#ifndef COMPLEXVALUESETTINGTRAIT_H
#define COMPLEXVALUESETTINGTRAIT_H

#include <QString>

/*! @class ComplexValue draupnir-test/traits/settings/ComplexValueSettingTrait.h
 *  @ingroup SettingsRegistryTests
 *  @brief Some complex value to store in a custom way. */

struct ComplexValue
{
    int firstInteger;
    int secondInteger;

    bool operator==(const ComplexValue& other) const = default;
    auto operator<=>(const ComplexValue& other) const = default;
};

/*! @class ComplexValueSettingTrait draupnir-test/traits/settings/ComplexValueSettingTrait.h
 *  @ingroup SettingsRegistryTests  */

class ComplexValueSettingTrait
{
public:
    using Value = ComplexValue;
    static QString key() { return "RandomComplexThing"; }
    static ComplexValue defaultValue() { return ComplexValue{0,0}; }
};

#include "draupnir/settings_registry/utils/ValueSerializerTemplate.h"

namespace Draupnir::Settings
{

template<>
class ValueSerializerTemplate<ComplexValue>
{
public:
    static QVariant toQVariant(const ComplexValue& value) {
        return QVariant{QString{"%1,%2"}.arg(value.firstInteger).arg(value.secondInteger)};
    }

    static std::optional<ComplexValue> fromQVariant(const QVariant& value) {
        if (!value.canConvert<QString>())
            return std::nullopt;

        const QStringList stringParts = value.toString().split(',');
        if (stringParts.count() != 2)
            return std::nullopt;

        bool ok = false;

        const int firstNumber = stringParts.at(0).toInt(&ok);
        if (!ok)
            return std::nullopt;

        const int secondNumber = stringParts.at(1).toInt(&ok);
        if (!ok)
            return std::nullopt;

        return std::optional<ComplexValue>{ComplexValue{firstNumber, secondNumber}};
    }
};

}; // namespace Draupnir::Settings

#endif // COMPLEXVALUESETTINGTRAIT_H
