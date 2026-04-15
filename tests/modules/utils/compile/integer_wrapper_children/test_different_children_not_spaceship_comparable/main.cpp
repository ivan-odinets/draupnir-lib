#include "draupnir/utils/integer_wrapper.h"

DEFINE_WRAPPED_INTEGER(FirstChild,int)
DEFINE_WRAPPED_INTEGER(SecondChild,int)

int main(int, char**)
{
    FirstChild one{42};
    SecondChild two{92};

    auto _ = one <=> two;

    auto a = one < two;
    auto b = one > two;
    auto c = one <= two;
    auto d = one >= two;

    return 0;
}
