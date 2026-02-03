#ifndef TYPE_DETECTORS_H
#define TYPE_DETECTORS_H

#include <type_traits>

namespace draupnir::utils
{

template<typename T>
struct is_integer : std::bool_constant<
    std::is_same_v<T,short> || std::is_same_v<T,unsigned short> ||
    std::is_same_v<T,int> || std::is_same_v<T,unsigned int> ||
    std::is_same_v<T,long> || std::is_same_v<T,unsigned long> ||
    std::is_same_v<T,long long> || std::is_same_v<T,unsigned long long>
> {};

};

#endif // TYPE_DETECTORS_H
