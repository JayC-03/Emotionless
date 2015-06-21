#include <cstdarg>

#include "common/log.h"

log_level log_filter;

//All you need for logging is slight upgrade of printf.
void log_print(std::string component, log_level level, std::string msg, ...)
{
    if(!(level & log_filter)) return;

    va_list args;
    va_start(args, msg.c_str());

    std::string level_str;
    switch(level)
    {
    case log_level::error:
    {
        level_str = "Error";
        break;
    }
    case log_level::warning:
    {
        level_str = "Warning";
        break;
    }
    case log_level::debug:
    {
        level_str = "Debug";
        break;
    }
    case log_level::verbose:
    {
        level_str = "Verbose";
        break;
    }
    }

    std::string final_msg = "[" + component + " | " + level_str + "] " + msg + "\n";

    vprintf(final_msg.c_str(), args);

    va_end(args);
}
