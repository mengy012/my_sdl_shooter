#include <cassert>
#include <chrono>
#include <fstream>
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

namespace time
{
using s_clock = std::chrono::steady_clock;
using s_time_point = s_clock::time_point;
using float_secends = std::chrono::duration<float>;

} // namespace time

void test_construct_duration()
{

    time::s_time_point start = time::s_clock::now();
    std::this_thread::sleep_for(time::float_secends(1.5f));
    time::s_time_point end = time::s_clock::now();

    std::cout << std::fixed << time::float_secends(end - start).count() << std::endl;
}

void test_file_stream(std::string file_name)
{
    std::fstream fs(file_name, std::ios::app | std::ios::in);
    if (fs)
    {
        fs << "hello world" << std::endl;
    }
    fs.seekg(0, std::ios_base::end);
    std::streamsize size = fs.tellg();
    std::cout << "file size: " << size << std::endl;

    std::string line(32, '\0');
    fs.seekg(0, std::ios_base::beg);
    fs.getline(line.data(), line.size());
    std::cout << line << std::endl;
}

void test_string_substr()
{
    std::string line("name: Alice, score: 129920");
    std::streamsize pos = line.find_first_of(' ', 0);
    std::streamsize pos1 = line.find_first_of(',', 0);
    std::string name(line.substr(pos + 1, pos1 - pos - 1));

    pos = line.find_first_of(' ', pos1 + 2);
    std::string score(line.substr(pos + 1));

    std::cout << "name: " << name << " " << "score: " << score << std::endl;
}

} // namespace test

int main()
{
    // test::test_times_of_backspace();
    // test::test_count_utf8_characters();
    // test::test_construct_duration();
    // test::test_file_stream("test.txt");
    test::test_string_substr();
}