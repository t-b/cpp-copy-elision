// Based on http://cpp-next.com/archive/2009/08/want-speed-pass-by-value/comment-page-1/#comment-48
// although it was completely rewritten
#if __DMC__
  #include <iostream.h>
  #include <exception>
#else
  #include <stdexcept>
  #include <iostream>
  using namespace std;
#endif

#include <cstring>

#define SIZE 10000
unsigned copies = 0;
unsigned moves  = 0;

struct X {
  X()
  {
    memset(data, 0, sizeof(data));
  }

  X(X const& rhs)
  {
    ++copies;
  }

#if ALLOW_MOVE
  X(X&& rhs)
  {
    ++moves;
  }
#endif

  int data[SIZE];
};

#define CHECK_COPIES( stmt )                       \
{                                                  \
  unsigned const old_copies = copies;              \
  unsigned const old_moves  = moves;               \
  try                                              \
  {                                                \
      stmt;                                        \
  }                                                \
  catch(...)                                       \
  {}                                               \
                                                   \
  unsigned const n = copies - old_copies;          \
  unsigned const m = moves - old_moves;            \
                                                   \
  if(n > 0)                                        \
  {                                                \
    if(m == 0)                                     \
      cerr << "c ";                                \
    else                                           \
      cerr << "unknown state";                     \
  }                                                \
  else                                             \
  {                                                \
    if(m == 0)                                     \
      cerr << "e ";                                \
    else                                           \
      cerr << "m ";                                \
  }                                                \
}                                                  \

struct trace {
  trace(char const* name)
  :
  name(name)
  {
    cout << name  << endl;
  }

  char const* name;
};

X urvo_single()
{
  const bool param = true;
  trace t("urvo_1");
  return X();
}

X urvo_two()
{
  const bool param = true;
  trace t("urvo_2");
  if(param)
    return X();
  else
    return X();
}

X urvo_two_with_param(bool param)
{
  trace t("urvo_two_with_param");
  if(param)
    return X();
  else
    return X();
}

X urvo_with_exception_1(bool param)
{
  trace t("urvo_with_exception_1");
  if(!param)
    throw std::exception();

  return X();
}

X urvo_with_exception_2(bool param)
{
  trace t("urvo_with_exception_2");
  if(param)
    return X();
  else
    throw std::exception();
}

X urvo_with_exception_3()
{
  trace t("urvo_with_exception_3");
  const bool param = true;
  if(param)
    return X();
  else
    throw std::exception();
}

X nrvo_single_1()
{
  trace t("nrvo_single_1");
  X a;
  return a;
}

X nrvo_single_2()
{
  trace t("nrvo_single_2");
  {
    X a;
    return a;
  }
}

X nrvo_single_with_exception_1(bool param)
{
  trace t("nrvo_single_with_exception_1");
  X a;
  if(!param)
    throw std::exception();

  return a;
}

X nrvo_single_with_exception_2(bool param)
{
  trace t("nrvo_single_with_exception_2");
  X a;
  if(param)
    return a;
  else
    throw std::exception();

   // Silence compilation error, does not count as an additional
   // return statement as it is unreachable code
  return a;
}

X nrvo_single_with_exception_3()
{
  trace t("nrvo_single_with_exception_3");
  const bool param = true;
  X a;
  if(param)
    return a;
  else
    throw std::exception();
}
X nrvo_two_different_tern()
{
  trace t("nrvo_two_different_tern");
  const bool param = true;
  X a, b;
  return param ? a : b;
}

X nrvo_two_different_if()
{
  trace t("nrvo_two_different_if");
  const bool param = true;
  X a, b;
  if(param)
    return a;
  else
    return b;
}

X nrvo_two_different_with_param_tern(bool param)
{
  trace t("nrvo_two_different_with_param_tern");
  X a, b;
  return param ? a : b;
}

X nrvo_two_different_with_param_if(bool param)
{
  trace t("nrvo_two_different_with_param_if");
  X a, b;
  if(param)
    return a;
  else
    return b;
}

X nrvo_two_equal_tern()
{
  trace t("nrvo_two_equal_tern");
  const bool param = true;
  X a;
  return param ? a : a;
}

X nrvo_two_equal_if()
{
  trace t("nrvo_two_equal_if");
  const bool param = true;
  X a;
  if(param)
    return a;
  else
    return a;
}

X nrvo_two_equal_with_param_tern(bool param)
{
  trace t("nrvo_two_equal_with_param_tern");
  X a;
  return param ? a : a;
}

X nrvo_two_equal_with_param_if(bool param)
{
  trace t("nrvo_two_equal_with_param_if");
  X a;
  if(param)
    return a;
  else
    return a;
}

int main()
{
    CHECK_COPIES( X a = urvo_single());
    CHECK_COPIES( X a = urvo_two());
    CHECK_COPIES( X a = urvo_two_with_param(true));
    CHECK_COPIES( X a = urvo_with_exception_1(true));
    CHECK_COPIES( X a = urvo_with_exception_2(true));
    CHECK_COPIES( X a = urvo_with_exception_3());

    cerr << " ";

    CHECK_COPIES( X a = nrvo_single_1());
    CHECK_COPIES( X a = nrvo_single_2());
    CHECK_COPIES( X a = nrvo_single_with_exception_1(true));
    CHECK_COPIES( X a = nrvo_single_with_exception_2(true));
    CHECK_COPIES( X a = nrvo_single_with_exception_3());

    cerr << " ";

    CHECK_COPIES( X a = nrvo_two_different_tern());
    CHECK_COPIES( X a = nrvo_two_different_if());
    CHECK_COPIES( X a = nrvo_two_different_with_param_tern(true));
    CHECK_COPIES( X a = nrvo_two_different_with_param_if(true));
    CHECK_COPIES( X a = nrvo_two_equal_tern());
    CHECK_COPIES( X a = nrvo_two_equal_if());
    CHECK_COPIES( X a = nrvo_two_equal_with_param_tern(true));
    CHECK_COPIES( X a = nrvo_two_equal_with_param_if(true));
}

