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

#include "x.h"

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

static X make_X() { return X(); }

X rrvo_single()
{
  const bool param = true;
  trace t("rrvo_1");
  return make_X();
}

X rrvo_two()
{
  const bool param = true;
  trace t("rrvo_2");
  if(param)
    return make_X();
  else
    return make_X();
}

X rrvo_two_with_param(bool param)
{
  trace t("rrvo_two_with_param");
  if(param)
    return make_X();
  else
    return make_X();
}

X rrvo_with_exception_1(bool param)
{
  trace t("rrvo_with_exception_1");
  if(!param)
    throw std::exception();

  return make_X();
}

X rrvo_with_exception_2(bool param)
{
  trace t("rrvo_with_exception_2");
  if(param)
    return make_X();
  else
    throw std::exception();
}

X rrvo_with_exception_3()
{
  trace t("rrvo_with_exception_3");
  const bool param = true;
  if(param)
    return make_X();
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

X nrvo_single_with_exception_1a(bool param)
{
  trace t("nrvo_single_with_exception_1a");
  if(!param)
    throw std::exception();
  X a;
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

X nrvo_single_with_exception_2a(bool param)
{
  trace t("nrvo_single_with_exception_2a");
  if(param) {
    X a;
    return a;
  } else
    throw std::exception();
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

X nrvo_single_with_exception_3a()
{
  trace t("nrvo_single_with_exception_3a");
  const bool param = true;
  if(param) {
    X a;
    return a;
  } else
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

X nrvo_two_different_if_2()
{
  trace t("nrvo_two_different_if_2");
  const bool param = true;
  if(param) {
    X a;
    return a;
  } else {
    X b;
    return b;
  }
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

X nrvo_two_different_with_param_if_2(bool param)
{
  trace t("nrvo_two_different_with_param_if_2");
  if(param) {
    X a;
    return a;
  } else {
    X b;
    return b;
  }
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

X nrvo_urvo_mixed_static()
{
    trace t("nrvo_urvo_mixed_static");
    static const bool param = true;
    if (param)
        return X();
    X a;
    return a;
}

X nrvo_urvo_mixed_dynamic(bool param)
{
    trace t("nrvo_urvo_mixed_dynamic");
    if (param)
        return X();
    X a;
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

    CHECK_COPIES( X a = rrvo_single());
    CHECK_COPIES( X a = rrvo_two());
    CHECK_COPIES( X a = rrvo_two_with_param(true));
    CHECK_COPIES( X a = rrvo_with_exception_1(true));
    CHECK_COPIES( X a = rrvo_with_exception_2(true));
    CHECK_COPIES( X a = rrvo_with_exception_3());

    cerr << " ";

    CHECK_COPIES( X a = nrvo_single_1());
    CHECK_COPIES( X a = nrvo_single_2());
    CHECK_COPIES( X a = nrvo_single_with_exception_1(true));
    CHECK_COPIES( X a = nrvo_single_with_exception_1a(true));
    CHECK_COPIES( X a = nrvo_single_with_exception_2(true));
    CHECK_COPIES( X a = nrvo_single_with_exception_2a(true));
    CHECK_COPIES( X a = nrvo_single_with_exception_3());
    CHECK_COPIES( X a = nrvo_single_with_exception_3a());

    cerr << " ";

    CHECK_COPIES( X a = nrvo_two_different_tern());
    CHECK_COPIES( X a = nrvo_two_different_if());
    CHECK_COPIES( X a = nrvo_two_different_if_2());
    CHECK_COPIES( X a = nrvo_two_different_with_param_tern(true));
    CHECK_COPIES( X a = nrvo_two_different_with_param_if(true));
    CHECK_COPIES( X a = nrvo_two_different_with_param_if_2(true));
    CHECK_COPIES( X a = nrvo_two_equal_tern());
    CHECK_COPIES( X a = nrvo_two_equal_if());
    CHECK_COPIES( X a = nrvo_two_equal_with_param_tern(true));
    CHECK_COPIES( X a = nrvo_two_equal_with_param_if(true));

    cerr << " ";

    CHECK_COPIES( X a = nrvo_urvo_mixed_static());
    CHECK_COPIES( X a = nrvo_urvo_mixed_dynamic(true));
}

