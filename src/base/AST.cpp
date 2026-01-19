#include <vector>
#include <string>

/**
 * A node is a fundamental unit in the Abstract Syntax Tree (AST).
 */
enum class NodeKind
{
  PROGRAM,
  EXPRESSION,
};

struct Node
{
  NodeKind kind;
};

/**
 * An expression is a construct that evaluates to a value.
 */
enum class ExpressionKind
{
  LITERAL,
  UNARY,
  BINARY,
};
struct Expression : public Node
{
  ExpressionKind expressionKind;
};

/**
 * Literal expressions represent fixed values in the source code.
 */
struct LiteralExpression : public Expression
{
  std::string value;
};

/**
 * Unary expressions represent operations with a single operand.
 */
enum class UnaryOperator
{
  NEGATE,
  NOT,
};
struct UnaryExpression : public Expression
{
  UnaryOperator op;
  Expression *operand;
};

/**
 * Binary expressions represent operations with two operands.
 */
enum class BinaryOperator
{
  ADD,
  SUBTRACT,
  MULTIPLY,
  DIVIDE,
};
struct BinaryExpression : public Expression
{
  BinaryOperator op;
  Expression *left;
  Expression *right;
};

/**
 * Program represents the hierarchical structure of the source code.
 */
struct Program : public Node
{
  std::vector<Expression> expressions;
};