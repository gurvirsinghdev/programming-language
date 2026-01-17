#include <string>
#include <vector>
#include <iostream>

#include <nlohmann/json.hpp>

/**
 * A token is the smallest unit of meaningful data in source code,
 * such as keywords, identifiers, operators, and literals.
 */
enum class TokenType
{
  IDENTIFIER,
  INTEGER,
  FLOAT
};

/**
 * A structure to hold token data, and facilitate debugging
 * as it can be easily converted to a JSON representation.
 */
struct PotentiallyDebugableToken
{
  TokenType type;
  std::string lexeme;
};

class Token
{
private:
  PotentiallyDebugableToken tokenData;

public:
  Token(TokenType t, const std::string &l) : tokenData{t, l} {};
  ~Token() {};

  /**
   * Convert the token to a JSON representation.
   */
  nlohmann::json toJson() const
  {
    return nlohmann::json{
        {"type", static_cast<int>(tokenData.type)},
        {"lexeme", tokenData.lexeme}};
  };
};

/**
 * Lexer is responsible for tokenizing the input source code into
 * a sequence of tokens that can be further processed by the parser.
 */
class Lexer
{
private:
  std::string sourceCode;

  /**
   * Diagnostic information for error reporting.
   */
  std::size_t lineNumber = 1;
  std::size_t columnNumber = 0;
  std::size_t lexerPosition = 0;

  /**
   * Returns true if the lexer has reached the end of the source code.
   */
  bool isAtEnd() const
  {
    return lexerPosition >= sourceCode.length();
  }

  /**
   * Returns the current character without advancing the lexer position.
   */
  char currentChar() const
  {
    if (isAtEnd())
      return '\0';
    else
      return sourceCode[lexerPosition];
  }

  /**
   * Advances the lexer position by one character.
   */
  char advance()
  {
    char previousChar = currentChar();
    lexerPosition++;
    return previousChar;
  }

  /**
   * Skips over ignoreable sequences such as whitespace and comments.
   */
  void skipIgnoreableSequence()
  {
    /**
     * Loops over the source code, and keeps advancing until
     * a non-ignoreable character is found.
     */
    while (!isAtEnd())
    {
      char c = currentChar();
      if (c == ' ' || c == '\t')
        advance();
      if (c == '#')
        while (currentChar() != '\n' && !isAtEnd())
          advance();
      else
        break;
    }
  }

public:
  Lexer(const std::string &s) : sourceCode(s) {};
  ~Lexer() {};

  /**
   * Tokenizes the source code and returns a vector of tokens.
   */
  std::vector<Token> tokenize()
  {
    /**
     * Initialize an empty vector to hold the tokens.
     */
    std::vector<Token> tokens;

    /**
     * Looping through the source code to identify and extract tokens.
     */
    while (!isAtEnd())
    {
      skipIgnoreableSequence();

      /**
       * Parses identifiers and creates corresponding tokens.
       */
      if (std::isalpha(currentChar()))
      {
        std::string lexeme;
        while (std::isalpha(currentChar()))
          lexeme += advance();
        tokens.emplace_back(TokenType::IDENTIFIER, lexeme);
        continue;
      }

      /**
       * Parses nemeric literals like integers and floats.
       */
      if (std::isdigit(currentChar()))
      {
        std::string lexeme;
        bool hasDeciamalPoint = false;
        while (std::isdigit(currentChar()) || currentChar() == '.')
        {
          if (currentChar() == '.')
          {
            if (hasDeciamalPoint)
              // TODO: Replace with proper error diagnostic mechanism.
              throw std::runtime_error("Invalid numeric literal with multiple decimal points.");

            hasDeciamalPoint = true;
          }
          lexeme += advance();
        }
        tokens.emplace_back(hasDeciamalPoint ? TokenType::FLOAT : TokenType::INTEGER, lexeme);
        continue;
      }

      advance();
    }

    return tokens;
  }
};