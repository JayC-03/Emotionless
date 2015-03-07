#pragma once

#include <string>

extern bool main_quit;
extern bool main_break;
extern int main_break_steps;

void main_loop(std::string fn);
