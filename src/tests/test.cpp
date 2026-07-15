#include <cassert>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

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

static int count_utf8_characters(const std::string& str)
{
    int count{};
    if (str.empty())
    {
        return count;
    }
    int utf8_count{};
    for (const unsigned char c : str)
    {
        /*
        if ((c & 0b11000000) == 0b11000000)
        {
            ++utf8_count;
        }
        else if (!(c & 0b10000000))
        {
            ++count;
        }
        */

        if ((c >> 6) == 0b11)
        {
            ++utf8_count;
        }
        else if (!(c >> 7))
        {
            ++count;
        }
    }
    count += utf8_count;
    return count;
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

void test_count_utf8_characters()
{
    std::string str{"你好_世界_2b"};
    std::cout << count_utf8_characters(str) << std::endl;
    // assert(count_utf8_characters(str) == 2);
}

void test_construct_duration()
{
    using s_clock = std::chrono::steady_clock;
    using s_time_point = s_clock::time_point;

    s_time_point start = s_clock::now();
    std::this_thread::sleep_for(std::chrono::duration<float>(1.5f));
    s_time_point end = s_clock::now();

    std::cout << std::fixed << std::chrono::duration<float>(end - start).count() << std::endl;
}

} // namespace test

int main()
{
    // test::test_times_of_backspace();
    test::test_count_utf8_characters();
    test::test_construct_duration();
}