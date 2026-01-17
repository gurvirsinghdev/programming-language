#include <gtest/gtest.h>

#include "snaps.hpp"

int add(int a, int b) { return a + b; }
int substract(int a, int b) { return a - b; }

TEST(Math, Addition)
{
  std::string out = std::to_string(add(2, 3));
  validate_output("addition", out);
}

TEST(Math, Substraction)
{
  std::string out = std::to_string(substract(5, 3));
  validate_output("substraction", out);
}