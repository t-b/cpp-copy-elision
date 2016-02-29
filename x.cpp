#include "x.h"

#include <iostream>

volatile unsigned copies = 0;
volatile unsigned moves = 0;

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
