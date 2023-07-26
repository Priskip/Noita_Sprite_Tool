#pragma once
#include <iostream>

#if PROJECT_DEBUG == 1
#define LOG(x) std::cout << x << std::endl
#elif PROJECT_DEBUG == 0
#define LOG(x)
#endif