#pragma once

#include <cstring>

#define SIZE 10000
extern volatile unsigned copies;
extern volatile unsigned moves;

struct X {
  X()
  {
    memset(data, 0, sizeof(data));
  }

  X(X const& rhs);
#if ALLOW_MOVE
  X(X&& rhs);
#endif

  int data[SIZE];
};

