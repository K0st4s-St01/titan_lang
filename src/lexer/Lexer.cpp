#include "Lexer.hpp"
namespace Titan {
char Lexer::peek(std::size_t offset) {
  if (!eof(offset)) {
    return source[index];
  } else {
    return '\0';
  }
}
char Lexer::advance() {
  char c = peek();
  index++;
  if (c == '\n') {
    line++;
    column = 1;
  } else {
    column += 1;
  }
  return c;
}
bool Lexer::eof(std::size_t offset) {
  return offset + index >= source.length();
}

void Lexer::skipWhitespaceAndComments() {
  while (!eof()) {
    while (peek() == '\t' || peek() == '\r' || peek() == '\n' ||
           peek() == ' ') {
      advance();
    }
    if (peek() == '/' && peek(1) == '/') {
      advance();
      advance();
      while (peek() != '\n') {
        advance();
      }
      advance();
    }
    if (peek() == '/' && peek(1) == '*') {
      advance();
      advance();
      while (peek() == '*' && peek(1) == '/') {
        advance();
      }
      advance();
      advance();
    }
    if (peek() == ' ' || peek() == '\r' || peek() == '\n' || peek() == ' ') {
      continue;
    } else
      break;
  }
}
bool Lexer::isHexNum() { return peek() == '0' && peek(1) == 'x'; }
bool Lexer::isBinNum() { return peek() == '0' && peek(1) == 'b'; }
bool Lexer::isDecNum() {
  return std::isdigit(static_cast<unsigned char>(peek()));
}
bool Lexer::consumeHex() {
  int i = 0;
  while (std::isxdigit(static_cast<unsigned char>(peek())) && !eof()) {
    advance();
    i++;
  }
  return i;
}

bool Lexer::consumeBin() {
  int i = 0;
  while ((peek() == '0' || peek() == '1') && !eof()) {
    advance();
    i++;
  }
  return i;
}

bool Lexer::consumeDec() {
  int i = 0;
  while (std::isdigit(static_cast<unsigned char>(peek())) && !eof()) {
    advance();
    i++;
  }
  return i;
}
Tokens::Token Lexer::nextHexNumber() {
  auto start_loc = current_location();
  std::size_t start_idx = index;
  if (isHexNum()) {
    advance();
    advance();
    consumeHex();

    if (peek() == '.') {
      if (consumeHex())
        return builder.nextLiteral(current_range(start_loc),
                                   source_str(start_idx, index),
                                   Tokens::LiteralKind::FLOAT_NUMBER);
      else
        return builder.nextInvalid(current_range(start_loc),
                                   source_str(start_idx, index));
    } else {
      if (consumeHex())
        return builder.nextLiteral(current_range(start_loc),
                                   source_str(start_idx, index),
                                   Tokens::LiteralKind::NUMBER);
    }
  }
  return builder.nextInvalid(current_range(start_loc),
                             source_str(start_idx, index));
}

Tokens::Token Lexer::nextBinaryNumber() {
  auto start_loc = current_location();
  std::size_t start_idx = index;
  if (isBinNum()) {
    advance();
    advance();
    consumeBin();

    if (peek() == '.') {
      if (consumeBin())
        return builder.nextLiteral(current_range(start_loc),
                                   source_str(start_idx, index),
                                   Tokens::LiteralKind::FLOAT_NUMBER);
      else
        return builder.nextInvalid(current_range(start_loc),
                                   source_str(start_idx, index));
    } else {
      if (consumeBin())
        return builder.nextLiteral(current_range(start_loc),
                                   source_str(start_idx, index),
                                   Tokens::LiteralKind::NUMBER);
    }
  }
  return builder.nextInvalid(current_range(start_loc),
                             source_str(start_idx, index));
}

Tokens::Token Lexer::nextDecimalNumber() {
  auto start_loc = current_location();
  std::size_t start_idx = index;
  if (isDecNum()) {
    advance();
    advance();
    consumeDec();

    if (peek() == '.') {
      if (consumeDec())
        return builder.nextLiteral(current_range(start_loc),
                                   source_str(start_idx, index),
                                   Tokens::LiteralKind::FLOAT_NUMBER);
      else
        return builder.nextInvalid(current_range(start_loc),
                                   source_str(start_idx, index));
    } else {
      if (consumeDec())
        return builder.nextLiteral(current_range(start_loc),
                                   source_str(start_idx, index),
                                   Tokens::LiteralKind::NUMBER);
    }
  }
  return builder.nextInvalid(current_range(start_loc),
                             source_str(start_idx, index));
}
Tokens::Token Lexer::nextNumberLiteral() {
  if (isDecNum()) {
    return nextDecimalNumber();
  }
  if (isHexNum()) {
    return nextHexNumber();
  }
  if (isBinNum()) {
    return nextBinaryNumber();
  }
  return builder.nextInvalid(current_range(current_location()),
                             source_str(index, index));
}
Tokens::Token Lexer::nextCharLiteral() {
  auto start_loc = current_location();
  std::size_t start_idx = index;
  if (peek() == '\'') {
    while ((std::isalnum(peek()) || peek('\\')) && !eof()) {
      advance();
    }
    if (peek() == '\'') {
      advance();
      return builder.nextLiteral(current_range(start_loc),
                                 source_str(start_idx, index),
                                 Tokens::LiteralKind::CHAR);
    } else {
      return builder.nextInvalid(current_range(start_loc),
                                 source_str(start_idx, index));
    }
  }
  return builder.nextInvalid(current_range(start_loc),
                             source_str(start_idx, index));
}

Tokens::Token Lexer::nextStrLiteral() {
  {
    auto start_loc = current_location();
    std::size_t start_idx = index;
    if (peek() == '\"') {
      while ((std::isalnum(peek()) || peek('\\')) && !eof()) {
        advance();
      }
      if (peek() == '\"') {
        advance();
        return builder.nextLiteral(current_range(start_loc),
                                   source_str(start_idx, index),
                                   Tokens::LiteralKind::STR);
      } else {
        return builder.nextInvalid(current_range(start_loc),
                                   source_str(start_idx, index));
      }
    }
    return builder.nextInvalid(current_range(start_loc),
                               source_str(start_idx, index));
  }
}



} // namespace Titan
