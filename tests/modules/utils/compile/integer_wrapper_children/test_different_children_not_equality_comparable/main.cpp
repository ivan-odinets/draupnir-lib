#include "draupnir/utils/integer_wrapper.h"

DEFINE_WRAPPED_INTEGER(FirstChild,int)
DEFINE_WRAPPED_INTEGER(SecondChild,int)

int main(int, char**)
{
    FirstChild one{42};
    SecondChild two{92};

    bool _ = one == two;

    return 0;
}
