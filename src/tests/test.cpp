#include <cassert>
#include <iostream>
#include <string>

static int times_of_backspace(const char* c)
{
    int times = 1;

    while ((static_cast<unsigned char>(*c) & 0b11000000) == 0b10000000)
    {
        ++times;
        --c;
    }

    return times;
}

namespace test
{
void test_times_of_backspace()
{
    std::string str{"你好"};
    std::cout << times_of_backspace(str.c_str() + str.size() - 1) << std::endl;
    assert(times_of_backspace(str.c_str() + str.size() - 1) == 3);

    std::string str2{"abc"};
    std::cout << times_of_backspace(str2.c_str() + str2.size() - 1) << std::endl;
    assert(times_of_backspace(str2.c_str() + str2.size() - 1) == 1);
}
} // namespace test

int main()
{
    // test::test_times_of_backspace();
}