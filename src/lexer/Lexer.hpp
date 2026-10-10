#pragma once

#include "../tokens/tokens.hpp"

namespace Titan {
class Lexer {
private:
  Tokens::TokenBuilder builder;
  std::string source;

  char peek(std::size_t offset = 0);
  char advance();
  bool match(char c);

  Tokens::Token nextLiteral();
  Tokens::Token nextPrimitive();
  Tokens::Token nextKeyword();
  Tokens::Token nextOperator();
  Tokens::Token nextPunctuation();

  std::size_t index = 0;
  std::size_t line = 1, column = 1;

  loc::SourceLocation current_location();

public:
  Lexer(Tokens::TokenBuilder builder, std::string source)
      : builder(std::move(builder)), source(std::move(source)) {}
  Tokens::Token nextToken();
};

} // namespace Titan
