#include <functional>

#define ASSERT(exp) ((!exp) ? throw "Error" : (void*)0)
#define ECXEPTION(_CHECK_, _RESPONSE_) try { _CHECK_; } catch(...) { _RESPONSE_; }

#include "../../src/function.hpp"

#include "../../cpputest/include/CppUTest/TestHarness.h"

// ============================= / Testing / ==================================

TEST_GROUP(Function)
{
  
};

TEST(Function, correctWorkingFunctionConstructors)
{
  // Constructor
  function<int (int)> foo1([](int i)
  {
    return i+1;
  });

  ECXEPTION(CHECK_EQUAL(3, foo1(2)), CHECK(false));

  // Copy constructor
  function<int (int)> foo2(foo1);

  ECXEPTION(CHECK_EQUAL(1, foo2(0)), CHECK(false));
  ECXEPTION(CHECK_EQUAL(5, foo1(4)), CHECK(false));

  // Default constructor
  function<int (int)> foo3;

  ECXEPTION(foo3(0); CHECK(false), CHECK(true));
}

TEST(Function, correctWorkingAssigment)
{
  function<int (int)> foo1([](int i)
  {
    return i+5;
  });

  CHECK_EQUAL(0, foo1(-5));

  function<int (int)> foo2 = foo1;

  CHECK_EQUAL(5, foo2(0));
  CHECK_EQUAL(9, foo1(4));
}

TEST(Function, bindingFunction)
{
  struct test
  {
    int foo(int i)
    {
      return i+3;
    }
  } obj;

  function<int (int)> lambda_whithout_capture([](int i) { return i+1; });
  function<int (int)> lambda_whith_capture([&](int i) { return i+2; });
  function<int (int)> bind_member_function(std::bind(&test::foo, &obj, std::placeholders::_1));
  function<int (int)> lambda_member_function([&](int i) { return obj.foo(i); });

  CHECK_EQUAL(-4, lambda_whithout_capture(-5));
  CHECK_EQUAL(-3, lambda_whith_capture(-5));
  CHECK_EQUAL(-2, bind_member_function(-5));
  CHECK_EQUAL(-2, lambda_member_function(-5));
}
