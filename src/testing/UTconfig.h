#pragma once

#include <iostream>
#include <cassert>

#define TEST_CASE(C) { if (!(C)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; else { std::cout << "Test case on line " << __LINE__ << " passed successfully!\n"; ++COUNT; } }
#define __check_result_no_return__(X, Y) { assert(X == Y); }
#define __check_result__(X, Y) { assert(X == Y); return true; }
#define __send_console_msg__(VAR) { std::cerr << VAR << ' '; }
#define __debug_output_msg__ 0