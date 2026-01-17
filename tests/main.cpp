#include <iostream>

#include "snaps.hpp"

int add(int a, int b)
{
  return a + b;
}

void test_addition()
{
  int result = add(2, 3);
  std::string actual_output = std::to_string(result);
  assert_snap("addition", actual_output);
}

int main()
{
  test_addition();
  return 0;
}