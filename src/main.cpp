#include <iostream>

#include "base/Lexer.cpp"

int main()
{
  Lexer lexer(
      "int main() {\n"
      "  # This is a comment\n"
      "  float number = 3.14;\n"
      "}");
  auto tokens = lexer.tokenize();

  for (const auto &token : tokens)
  {
    std::cout << token.toJson().dump() << std::endl;
  }
  return 0;
}