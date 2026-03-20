#include "draupnir/utils/template_constructors.h"

class NonDefaultCtr
{
public:
    NonDefaultCtr(int) {}

private:
    NonDefaultCtr() = delete;
};

int main(int, char**)
{
    draupnir::utils::create_tuple_new<std::tuple<int*,NonDefaultCtr*>>();

    return 0;
}
