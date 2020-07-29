#ifndef ITS_UNIT_TEST_HPP
#define ITS_UNIT_TEST_HPP

#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>

/* Macros for colors */
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED "\x1b[1;31m"
#define COLOR_RESET "\x1b[0m"

/**
 * Macro to execute a test without a check of the output
 * @param func: the tested function 
 * @param timeout_millis: the maximum time of execution in milli-seconds (optionnal)
 */
#define __ITS_GET_MACRO_TEST(_1, _2, NAME, ...) NAME
#define TEST(...)                                                 \
    __ITS_GET_MACRO_TEST(__VA_ARGS__, __ITS_TEST_2, __ITS_TEST_1) \
    (__VA_ARGS__)

/**
 * Macro to execute a test with a check of the output
 * @param func: the tested function 
 * @param file_output : the expected output file
 * @param timeout_millis: the maximum time of execution in milli-seconds (optionnal)
 */
#define __ITS_GET_MACRO_OTEST(_1, _2, _3, NAME, ...) NAME
#define OTEST(...)                                                 \
    __ITS_GET_MACRO_OTEST(__VA_ARGS__, __ITS_TEST_4, __ITS_TEST_3) \
    (__VA_ARGS__)

/**
 * Macro to execute a test without a check of the output
 * @param func: the tested function 
 */
#define __ITS_TEST_1(func)                                                                                       \
    do                                                                                                           \
    {                                                                                                            \
        std::string __current_test_name = #func;                                                                 \
        std::cout << "Check " << __current_test_name << " : ";                                                   \
        fflush(stdout);                                                                                          \
        __its_unit_test_cpp_running = true;                                                                      \
        __its_unit_test_cpp_load = std::thread(__its_unit_test_cpp_loadingEffect);                               \
        std::ofstream file("/dev/null");                                                                         \
        __its_unit_stream_buffer_cout = std::cout.rdbuf();                                                       \
        std::streambuf *__its_unit_stream_buffer_cerr = std::cerr.rdbuf();                                       \
        std::cout.rdbuf(file.rdbuf());                                                                           \
        std::cerr.rdbuf(file.rdbuf());                                                                           \
        auto start = std::chrono::steady_clock::now();                                                           \
        func();                                                                                                  \
        auto end = std::chrono::steady_clock::now();                                                             \
        std::cout.rdbuf(__its_unit_stream_buffer_cout);                                                          \
        std::cerr.rdbuf(__its_unit_stream_buffer_cerr);                                                          \
        unsigned long elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count(); \
        __its_unit_test_cpp_running = false;                                                                     \
        __its_unit_test_cpp_load.join();                                                                         \
        std::cout << COLOR_GREEN << "Success" << COLOR_RESET << " ("                                             \
                  << (unsigned int)elapsed_time / 1000 << "ms)" << std::endl;                                    \
        fflush(stdout);                                                                                          \
        file.close();                                                                                            \
    } while (0)

/**
 * Macro to execute a test without a check of the output
 * @param func: the tested function 
 * @param timeout_millis: the maximum time of execution in milli-seconds 
 */
#define __ITS_TEST_2(func, timeout_millis)                                                                                        \
    do                                                                                                                            \
    {                                                                                                                             \
        std::string __current_test_name = #func;                                                                                  \
        std::cout << "Check " << __current_test_name << " : ";                                                                    \
        fflush(stdout);                                                                                                           \
        __its_unit_test_cpp_running = true;                                                                                       \
        __its_unit_test_cpp_load = std::thread(__its_unit_test_cpp_loadingEffect);                                                \
        std::ofstream file("/dev/null");                                                                                          \
        __its_unit_stream_buffer_cout = std::cout.rdbuf();                                                                        \
        std::streambuf *__its_unit_stream_buffer_cerr = std::cerr.rdbuf();                                                        \
        std::cout.rdbuf(file.rdbuf());                                                                                            \
        std::cerr.rdbuf(file.rdbuf());                                                                                            \
        auto start = std::chrono::steady_clock::now();                                                                            \
        std::thread launch_func = std::thread([]() {func(); __its_unit_test_cpp_running= false; });                                                                          \
        __its_unit_test_cpp_timeout((timeout_millis), launch_func);                                                               \
        auto end = std::chrono::steady_clock::now();                                                                              \
        std::cout.rdbuf(__its_unit_stream_buffer_cout);                                                                           \
        std::cerr.rdbuf(__its_unit_stream_buffer_cerr);                                                                           \
        unsigned long elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();                  \
        std::cout << COLOR_GREEN << "Success " << COLOR_RESET << " (" << (unsigned int)elapsed_time / 1000 << "ms)" << std::endl; \
        fflush(stdout);                                                                                                           \
        file.close();                                                                                                             \
    } while (0)

/**
 * Macro to execute a test with a check of the output
 * @param func: the tested function 
 * @param file_output : the expected output file
 */
#define __ITS_TEST_3(func, file_output)                                                                          \
    do                                                                                                           \
    {                                                                                                            \
        std::string __current_test_name = #func;                                                                 \
        std::cout << "Check " << __current_test_name << " : ";                                                   \
        fflush(stdout);                                                                                          \
        __its_unit_test_cpp_running = true;                                                                      \
        __its_unit_test_cpp_load = std::thread(__its_unit_test_cpp_loadingEffect);                               \
        std::ofstream file(#func + std::string("_its_test.log"));                                                \
        __its_unit_stream_buffer_cout = std::cout.rdbuf();                                                       \
        std::streambuf *__its_unit_stream_buffer_cerr = std::cerr.rdbuf();                                       \
        std::cout.rdbuf(file.rdbuf());                                                                           \
        std::cerr.rdbuf(file.rdbuf());                                                                           \
        auto start = std::chrono::steady_clock::now();                                                           \
        func();                                                                                                  \
        auto end = std::chrono::steady_clock::now();                                                             \
        assert_file(file_output, #func + std::string("_its_test.log"));                                          \
        std::cout.rdbuf(__its_unit_stream_buffer_cout);                                                          \
        std::cerr.rdbuf(__its_unit_stream_buffer_cerr);                                                          \
        unsigned long elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count(); \
        __its_unit_test_cpp_running = false;                                                                     \
        __its_unit_test_cpp_load.join();                                                                         \
        std::cout << COLOR_GREEN << "Success" << COLOR_RESET << " ("                                             \
                  << (unsigned int)elapsed_time / 1000 << "ms)" << std::endl;                                    \
        fflush(stdout);                                                                                          \
        file.close();                                                                                            \
        remove((#func + std::string("_its_test.log")).c_str());                                                  \
    } while (0)

/**
 * Macro to execute a test with a check of the output
 * @param func: the tested function 
 * @param file_output : the expected output file
 * @param timeout_millis: the maximum time of execution in milli-seconds 
 */
#define __ITS_TEST_4(func, file_output, timeout_millis)                                                                           \
    do                                                                                                                            \
    {                                                                                                                             \
        std::string __current_test_name = #func;                                                                                  \
        std::cout << "Check " << __current_test_name << " : ";                                                                    \
        fflush(stdout);                                                                                                           \
        __its_unit_test_cpp_running = true;                                                                                       \
        __its_unit_test_cpp_load = std::thread(__its_unit_test_cpp_loadingEffect);                                                \
        std::ofstream file(#func + std::string("_its_test.log"));                                                                 \
        __its_unit_stream_buffer_cout = std::cout.rdbuf();                                                                        \
        std::streambuf *__its_unit_stream_buffer_cerr = std::cerr.rdbuf();                                                        \
        std::cout.rdbuf(file.rdbuf());                                                                                            \
        std::cerr.rdbuf(file.rdbuf());                                                                                            \
        auto start = std::chrono::steady_clock::now();                                                                            \
        std::thread launch_func = std::thread([]() {func(); __its_unit_test_cpp_running = false; });                                                                          \
        __its_unit_test_cpp_timeout((timeout_millis), launch_func);                                                               \
        auto end = std::chrono::steady_clock::now();                                                                              \
        std::cout.rdbuf(__its_unit_stream_buffer_cout);                                                                           \
        std::cerr.rdbuf(__its_unit_stream_buffer_cerr);                                                                           \
        unsigned long elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();                  \
        std::cout << COLOR_GREEN << "Success " << COLOR_RESET << " (" << (unsigned int)elapsed_time / 1000 << "ms)" << std::endl; \
        fflush(stdout);                                                                                                           \
        file.close();                                                                                                             \
        assert_file(file_output, #func + std::string("_its_test.log"));                                                           \
        remove((#func + std::string("_its_test.log")).c_str());                                                                   \
    } while (0)

/** 
 * Macro that test if the expression passed is true
 * @param expr: the expression tested
 */
#define assert(expr)                                                         \
    do                                                                       \
    {                                                                        \
        if (!(expr))                                                         \
        {                                                                    \
            std::ostream stream_out(__its_unit_stream_buffer_cout);          \
            __its_unit_test_cpp_running = false;                             \
            __its_unit_test_cpp_load.join();                                 \
            stream_out << COLOR_RED << "Failed " << COLOR_RESET << std::endl \
                       << "assertion failed : " << #expr << std::endl;       \
            stream_out.flush();                                              \
            exit(EXIT_FAILURE);                                              \
        }                                                                    \
    } while (0)

/** 
 * Macro that test if two files identical
 * @param first_file: the expected file
 * @param second_file : the tested file
 */
#define assert_file(first_file, second_file)                                                                          \
    do                                                                                                                \
    {                                                                                                                 \
        std::ostream stream_out(__its_unit_stream_buffer_cout);                                                       \
        bool equal = false;                                                                                           \
        std::ifstream f1(first_file, std::ifstream::binary | std::ifstream::ate);                                     \
        if (f1.fail())                                                                                                \
        {                                                                                                             \
            __its_unit_test_cpp_running = false;                                                                      \
            __its_unit_test_cpp_load.join();                                                                          \
            stream_out << COLOR_RED << "Failed " << COLOR_RESET << std::endl                                          \
                       << "Failure when opening the file " << first_file << "!" << std::endl;                         \
            stream_out.flush();                                                                                       \
            exit(EXIT_FAILURE);                                                                                       \
        }                                                                                                             \
        std::ifstream f2(second_file, std::ifstream::binary | std::ifstream::ate);                                    \
        if (f2.fail())                                                                                                \
        {                                                                                                             \
            __its_unit_test_cpp_running = false;                                                                      \
            __its_unit_test_cpp_load.join();                                                                          \
            stream_out << COLOR_RED << "Failed " << COLOR_RESET << std::endl                                          \
                       << "Failure when opening the file " << second_file << "!" << std::endl;                        \
            stream_out.flush();                                                                                       \
            exit(EXIT_FAILURE);                                                                                       \
        }                                                                                                             \
        if (f1.tellg() != f2.tellg())                                                                                 \
        {                                                                                                             \
            f1.seekg(0, std::ifstream::beg);                                                                          \
            f2.seekg(0, std::ifstream::beg);                                                                          \
            equal = std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),                                            \
                               std::istreambuf_iterator<char>(),                                                      \
                               std::istreambuf_iterator<char>(f2.rdbuf()));                                           \
        }                                                                                                             \
        f1.close();                                                                                                   \
        f2.close();                                                                                                   \
        if (!(equal))                                                                                                 \
        {                                                                                                             \
            __its_unit_test_cpp_running = false;                                                                      \
            __its_unit_test_cpp_load.join();                                                                          \
            stream_out << COLOR_RED << "Failed " << COLOR_RESET << std::endl                                          \
                       << "The files \"" << first_file "\"and \"" << second_file << "\" are different!" << std::endl; \
            stream_out.flush();                                                                                       \
            exit(EXIT_FAILURE);                                                                                       \
        }                                                                                                             \
    } while (0)

/* A boolean that indicate if the test is running */
static bool __its_unit_test_cpp_running;

/* The thread that display the loading effect */
static std::thread __its_unit_test_cpp_load;

/* A stream buffer to store the cout stream */
std::streambuf *__its_unit_stream_buffer_cout;

/**
 * While the test is running, create a waiting animation
 */
static void __its_unit_test_cpp_loadingEffect()
{
    std::ostream stream_out(__its_unit_stream_buffer_cout);
    int ind = 0;
    auto print_time = std::chrono::steady_clock::now();

    while (__its_unit_test_cpp_running)
    {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - print_time).count() > 300)
        {
            switch (ind)
            {
            case 0:
                stream_out << "   \b\b\b";
                break;
            case 1:
                stream_out << ".  \b\b\b";
                break;
            case 2:
                stream_out << ".. \b\b\b";
                break;
            case 3:
                stream_out << "...\b\b\b";
                break;
            }
            stream_out.flush();
            ind = (ind + 1) % 4;
            print_time = std::chrono::steady_clock::now();
        }
    }
    stream_out << "   \b\b\b";
    stream_out.flush();
}

/**
 * Stop the programm if the test is not finished after a certain time
 * @param time: the maximum time, in milliseconds
 * @param launch_func: the thread that execute the tested function
 */
static void __its_unit_test_cpp_timeout(float time, std::thread &launch_func)
{
    std::ostream stream_out(__its_unit_stream_buffer_cout);

    auto start_time = std::chrono::steady_clock::now();
    while (__its_unit_test_cpp_running)
        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count() > time)
        {
            __its_unit_test_cpp_running = false;
            stream_out << COLOR_RED << "Timeout (" << time << " ms)" << COLOR_RESET << std::endl;
            stream_out.flush();
            launch_func.detach();
            __its_unit_test_cpp_load.join();
            exit(EXIT_FAILURE);
        }
    launch_func.join();
    __its_unit_test_cpp_load.join();
}

#endif
