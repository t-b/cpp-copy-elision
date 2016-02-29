#include "x.h"

#include <iostream>

unsigned copies = 0;
unsigned moves = 0;

X::X(const X&)
{
    std::cout << std::flush;
    ++copies;
}

X::X(X&&)
{
    std::cout << std::flush;
    ++moves;
}
