#include "draupnir/utils/integer_wrapper.h"

DEFINE_WRAPPED_INTEGER(FirstChild,int)
DEFINE_WRAPPED_INTEGER(SecondChild,int)

int main(int, char**)
{
    FirstChild three = FirstChild{42} + SecondChild{42};

    return 0;
}
