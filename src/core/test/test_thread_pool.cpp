#include "sky_big/thread_pool.h"

#include <cstdlib>
#include <iostream>
#include <thread>

using namespace sb;

void function()
{
    std::cout << "function" << std::endl;
}

int main(int argc, char* argv[])
{
    ThreadPool thread_pool{ 4 };

    thread_pool.submit(function);

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);

    std::cout << "Stopping thread pool" << std::endl;
    thread_pool.stop();
    return EXIT_SUCCESS;
}