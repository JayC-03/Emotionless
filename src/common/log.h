#pragma once

#include <cstdio>
#include <string>

enum class log_level
{
  error,
  warning,
  debug,
  verbose
};

void log_print(std::string component, std::string msg, log_level level);

#define to_string std::to_string
