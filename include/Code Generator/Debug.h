
#include <iostream>

#define Log(...) do { std::cout << __VA_ARGS__ << std::endl; } while(0);
#define LogError(...) do { std::wcerr << __VA_ARGS__ << std::endl; } while(0);