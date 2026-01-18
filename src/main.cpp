#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "base/Lexer.cpp"

/**
 * Function to read the give source file and
 * return its contents as a string.
 */
std::string readSourceFile(const std::string &filePath)
{
  std::ifstream fileStream(filePath);
  if (!fileStream.is_open())
    throw std::runtime_error("Could not open file: " + filePath);

  std::stringstream buffer;
  buffer << fileStream.rdbuf();
  return buffer.str();
}

int main(int argc, char *argv[])
{
  /**
   * Asserting correct number of arguments are provided.
   */
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <source-file-path>" << std::endl;
    return 1;
  }

  /**
   * Reading the source file and tokenizing its contents.
   */
  std::string sourceFilePath = argv[1];
  std::string sourceCode = readSourceFile(sourceFilePath);
  Lexer lexer(sourceCode);
  std::vector<Token> tokens = lexer.tokenize();

  /**
   * Outputting the tokens as JSON.
   */
  for (const auto &token : tokens)
    std::cout << token.toJson().dump() << std::endl;
  return 0;
}