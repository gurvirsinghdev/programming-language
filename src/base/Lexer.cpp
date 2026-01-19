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
  FLOAT,
  KEYWORD,

  LPAREN,
  RPAREN,
  LBRACE,
  RBRACE,
  LBRACKET,
  RBRACKET,
  SEMICOLON,
};

/**
 * A structure to hold token data, and facilitate debugging
 * as it can be easily converted to a JSON representation.
 */
struct PotentiallyDebugableToken
{
  TokenType type;
  std::string lexeme;

  /**
   * Diagnostic information for error reporting.
   */
  std::size_t lineNumber;
  std::size_t columnStart;
};

class Token
{
private:
  PotentiallyDebugableToken tokenData;

public:
  Token(TokenType t, const std::string &l, std::size_t ln, std::size_t cs) : tokenData{t, l, ln, cs} {};
  ~Token() {};

  /**
   * Convert the token to a JSON representation.
   */
  nlohmann::json toJson() const
  {
    return nlohmann::json{
        {"type", static_cast<int>(tokenData.type)},
        {"lexeme", tokenData.lexeme},
        {"lineNumber", tokenData.lineNumber},
        {"columnStart", tokenData.columnStart},
    };
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
  std::size_t columnStart = 0;
  std::size_t lexerPosition = 0;

  /**
   * Reserved keywords in the language.
   */
  std::vector<std::string> keywords = {
      "int",
      "float",
  };

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
    columnStart++;
    return previousChar;
  }

  /**
   * Skips over a new-line character, updating line and column counters.
   */
  void skipNewLine()
  {
    advance();
    lineNumber++;
    columnStart = 0;
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
      if (c == '\n')
        skipNewLine();
      else if (c == ' ' || c == '\t')
        advance();
      else if (c == '#')
        while (currentChar() != '\n' && !isAtEnd())
          advance();
      else
        break;
    }
  }

  /**
   * Builds a token from the current lexer position.
   */
  Token buildToken(TokenType type, const std::string &lexeme, std::size_t lexemeColumnStart)
  {
    Token token(type, lexeme, lineNumber, lexemeColumnStart);
    return token;
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
       * Parses identifiers and keywords.
       */
      if (std::isalpha(currentChar()))
      {
        std::string lexeme;
        std::size_t lexemeColumnStart = columnStart;
        while (std::isalpha(currentChar()))
          lexeme += advance();

        TokenType type =
            keywords.end() != std::find(keywords.begin(), keywords.end(), lexeme)
                ? TokenType::KEYWORD
                : TokenType::IDENTIFIER;
        tokens.push_back(std::move(buildToken(type, lexeme, lexemeColumnStart)));

        continue;
      }

      /**
       * Parses nemeric literals like integers and floats.
       */
      if (std::isdigit(currentChar()))
      {
        std::string lexeme;
        std::size_t lexemeColumnStart = columnStart;
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

        TokenType type = hasDeciamalPoint ? TokenType::FLOAT : TokenType::INTEGER;
        tokens.push_back(std::move(buildToken(type, lexeme, lexemeColumnStart)));
        continue;
      }

      /**
       * Single-character tokens.
       */
      switch (currentChar())
      {
      case '(':
        tokens.push_back(std::move(buildToken(TokenType::LPAREN, "(", columnStart)));
        break;
      case ')':
        tokens.push_back(std::move(buildToken(TokenType::RPAREN, ")", columnStart)));
        break;
      case '{':
        tokens.push_back(std::move(buildToken(TokenType::LBRACE, "{", columnStart)));
        break;
      case '}':
        tokens.push_back(std::move(buildToken(TokenType::RBRACE, "}", columnStart)));
        break;
      case '[':
        tokens.push_back(std::move(buildToken(TokenType::LBRACKET, "[", columnStart)));
        break;
      case ']':
        tokens.push_back(std::move(buildToken(TokenType::RBRACKET, "]", columnStart)));
        break;
      case ';':
        tokens.push_back(std::move(buildToken(TokenType::SEMICOLON, ";", columnStart)));
        break;
      }

      advance();
    }

    return tokens;
  }
};