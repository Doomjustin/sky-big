#include "sky_big/log.h"

#include <cstdlib>

int main(int argc, char* argv[])
{
    LOG_FATAL("this is a message from {}", "log");
    LOG_ERROR("this is a message from {}", "log");
    LOG_WARNING("waning message {}", "from LOG");
    LOG_DEBUG("debug message {}", "from LOG");
    LOG_INFO("info message {}", "from LOG");

    return EXIT_SUCCESS;
}