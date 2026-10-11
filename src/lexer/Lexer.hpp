#pragma once

#include "../tokens/tokens.hpp"
#include <cstddef>
#include <string>
#include <utility>

namespace Titan {
class Lexer {
private:
  Tokens::TokenBuilder builder;
  std::string source;

  char peek(std::size_t offset = 0);
  char advance();
  bool match(char c);
  bool eof(std::size_t offset = 0);
  void skipWhitespaceAndComments();
  bool isHexNum();
  bool isBinNum();
  bool isDecNum();
  bool consumeHex();
  bool consumeBin();
  bool consumeDec();
  Tokens::Token nextHexNumber();
  Tokens::Token nextBinaryNumber();
  Tokens::Token nextDecimalNumber();
  Tokens::Token nextNumberLiteral();
  Tokens::Token nextCharLiteral();
  Tokens::Token nextStrLiteral();
  Tokens::Token nextLiteral();
  Tokens::Token nextPrimitive(std::string word);
  Tokens::Token nextKeywordOrIdentifier();
  Tokens::Token nextOperator();
  Tokens::Token nextPunctuation();

  std::size_t index = 0;
  std::size_t line = 1;
  std::size_t column = 1;

  loc::SourceLocation current_location();
  loc::SourceRange current_range(loc::SourceLocation start);
  std::string source_str(std::size_t start_idx, std::size_t end_idx);

  std::string filename;
  std::size_t file_index = 0;

public:
  Lexer(Tokens::TokenBuilder builder, std::string source)
      : builder(std::move(builder)), source(std::move(source)) {}

  Tokens::Token nextToken();
};
} // namespace Titan
