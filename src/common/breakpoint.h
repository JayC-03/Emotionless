#pragma once

#include <vector>

#include "common/common.h"

struct breakpoint_t
{
	u32 addr;
	bool enabled;
};

extern vector<breakpoint_t> breakpoints;
