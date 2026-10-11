#include "Lexer.hpp"

#include <cctype>
#include <cstddef>
#include <string>
#include <utility>

namespace Titan {

char Lexer::peek(std::size_t offset) {
  if (eof(offset)) {
    return '\0';
  }
  return source[index + offset];
}

char Lexer::advance() {
  if (eof()) {
    return '\0';
  }

  const char c = source[index++];
  if (c == '\n') {
    ++line;
    column = 1;
  } else {
    ++column;
  }
  return c;
}

bool Lexer::eof(std::size_t offset) {
  // Written this way to avoid overflowing index + offset.
  return index >= source.length() || offset >= source.length() - index;
}

void Lexer::skipWhitespaceAndComments() {
  while (!eof()) {
    if (peek() == ' ' || peek() == '\t' || peek() == '\r' || peek() == '\n') {
      advance();
      continue;
    }

    if (peek() == '/' && peek(1) == '/') {
      advance();
      advance();
      while (!eof() && peek() != '\n') {
        advance();
      }
      continue;
    }

    if (peek() == '/' && peek(1) == '*') {
      advance();
      advance();
      while (!eof() && !(peek() == '*' && peek(1) == '/')) {
        advance();
      }
      if (!eof()) {
        advance(); // '*'
        advance(); // '/'
      }
      // Unterminated comments reach EOF. Add a diagnostic here if the
      // lexer has a diagnostic API.
      continue;
    }

    break;
  }
}

bool Lexer::isHexNum() {
  return peek() == '0' && (peek(1) == 'x' || peek(1) == 'X');
}
bool Lexer::isBinNum() {
  return peek() == '0' && (peek(1) == 'b' || peek(1) == 'B');
}
bool Lexer::isDecNum() { return peek() >= '0' && peek() <= '9'; }

bool Lexer::consumeHex() {
  bool consumed = false;
  while (!eof()) {
    const char c = peek();
    if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
          (c >= 'A' && c <= 'F'))) {
      break;
    }
    advance();
    consumed = true;
  }
  return consumed;
}

bool Lexer::consumeBin() {
  bool consumed = false;
  while (!eof() && (peek() == '0' || peek() == '1')) {
    advance();
    consumed = true;
  }
  return consumed;
}

bool Lexer::consumeDec() {
  bool consumed = false;
  while (!eof() && peek() >= '0' && peek() <= '9') {
    advance();
    consumed = true;
  }
  return consumed;
}

Tokens::Token Lexer::nextHexNumber() {
  const auto start_loc = current_location();
  const std::size_t start_idx = index;
  advance(); // 0
  advance(); // x/X

  if (!consumeHex()) {
    return builder.nextInvalid(current_range(start_loc),
                               source_str(start_idx, index));
  }
  return builder.nextLiteral(current_range(start_loc),
                             source_str(start_idx, index),
                             Tokens::LiteralKind::NUMBER);
}

Tokens::Token Lexer::nextBinaryNumber() {
  const auto start_loc = current_location();
  const std::size_t start_idx = index;
  advance(); // 0
  advance(); // b/B

  if (!consumeBin()) {
    return builder.nextInvalid(current_range(start_loc),
                               source_str(start_idx, index));
  }
  return builder.nextLiteral(current_range(start_loc),
                             source_str(start_idx, index),
                             Tokens::LiteralKind::NUMBER);
}

Tokens::Token Lexer::nextDecimalNumber() {
  const auto start_loc = current_location();
  const std::size_t start_idx = index;
  consumeDec();

  bool is_float = false;
  if (peek() == '.' && peek(1) >= '0' && peek(1) <= '9') {
    is_float = true;
    advance(); // decimal point
    consumeDec();
  }

  // Optional decimal exponent: 1e3, 1.2E-4.
  if (peek() == 'e' || peek() == 'E') {
    const std::size_t exponent_start = index;
    advance();
    if (peek() == '+' || peek() == '-') {
      advance();
    }
    if (consumeDec()) {
      is_float = true;
    } else {
      // Do not accept an incomplete exponent as part of a valid literal.
      (void)exponent_start;
      return builder.nextInvalid(current_range(start_loc),
                                 source_str(start_idx, index));
    }
  }

  return builder.nextLiteral(current_range(start_loc),
                             source_str(start_idx, index),
                             is_float ? Tokens::LiteralKind::FLOAT_NUMBER
                                      : Tokens::LiteralKind::NUMBER);
}

Tokens::Token Lexer::nextNumberLiteral() {
  // Prefixes must be checked before decimal digits, because both begin with
  // '0'.
  if (isHexNum()) {
    return nextHexNumber();
  }
  if (isBinNum()) {
    return nextBinaryNumber();
  }
  if (isDecNum()) {
    return nextDecimalNumber();
  }

  const auto loc = current_location();
  return builder.nextInvalid(current_range(loc), source_str(index, index));
}

Tokens::Token Lexer::nextCharLiteral() {
  const auto start_loc = current_location();
  const std::size_t start_idx = index;
  if (peek() != '\'') {
    return builder.nextInvalid(current_range(start_loc),
                               source_str(start_idx, index));
  }

  advance(); // opening quote
  if (eof() || peek() == '\n' || peek() == '\r' || peek() == '\'') {
    if (peek() == '\'')
      advance();
    return builder.nextInvalid(current_range(start_loc),
                               source_str(start_idx, index));
  }

  if (peek() == '\\') {
    advance();
    if (eof()) {
      return builder.nextInvalid(current_range(start_loc),
                                 source_str(start_idx, index));
    }
    advance(); // escaped character; escape validity can be checked in a later
               // pass
  } else {
    advance(); // one source byte; UTF-8 character decoding is not implemented
               // here
  }

  if (peek() != '\'') {
    while (!eof() && peek() != '\'' && peek() != '\n' && peek() != '\r')
      advance();
    if (peek() == '\'')
      advance();
    return builder.nextInvalid(current_range(start_loc),
                               source_str(start_idx, index));
  }

  advance(); // closing quote
  return builder.nextLiteral(current_range(start_loc),
                             source_str(start_idx, index),
                             Tokens::LiteralKind::CHAR);
}

Tokens::Token Lexer::nextStrLiteral() {
  const auto start_loc = current_location();
  const std::size_t start_idx = index;
  if (peek() != '"') {
    return builder.nextInvalid(current_range(start_loc),
                               source_str(start_idx, index));
  }

  advance(); // opening quote
  while (!eof() && peek() != '"' && peek() != '\n' && peek() != '\r') {
    if (peek() == '\\') {
      advance();
      if (eof())
        break;
      advance(); // escaped character
    } else {
      advance();
    }
  }

  if (peek() != '"') {
    return builder.nextInvalid(current_range(start_loc),
                               source_str(start_idx, index));
  }
  advance(); // closing quote
  return builder.nextLiteral(current_range(start_loc),
                             source_str(start_idx, index),
                             Tokens::LiteralKind::STR);
}

Tokens::Token Lexer::nextLiteral() {
  const auto start_loc = current_location();
  const std::size_t start_idx = index;
  if (isHexNum() || isBinNum() || isDecNum()) {
    return nextNumberLiteral();
  }
  if (peek() == '"') {
    return nextStrLiteral();
  }
  if (peek() == '\'') {
    return nextCharLiteral();
  }
  if (!eof())
    advance();
  return builder.nextInvalid(current_range(start_loc),
                             source_str(start_idx, index));
}

Tokens::Token Lexer::nextPrimitive(std::string word) {
  // nextKeywordOrIdentifier() has already consumed the identifier before it
  // calls this helper. Primitive names contain only ASCII characters, so the
  // original source location is recoverable from the current column.
  const std::size_t word_length = word.size();
  const std::size_t start_idx =
      index >= word_length ? index - word_length : index;
  const auto start_loc = loc::SourceLocation(
      line, column >= word_length ? column - word_length : column);
  Tokens::PrimitiveKind kind;
  if (word == "VOID") {
    kind = Tokens::PrimitiveKind::VOID;

    return builder.nextPrimitive(current_range(start_loc), word, kind);
  }
  if (word == "BYTE") {
    kind = Tokens::PrimitiveKind::BYTE;

    return builder.nextPrimitive(current_range(start_loc), word, kind);
  }
  if (word == "I8") {
    kind = Tokens::PrimitiveKind::I8;

    return builder.nextPrimitive(current_range(start_loc), word, kind);
  }
  if (word == "I16") {
    kind = Tokens::PrimitiveKind::I16;

    return builder.nextPrimitive(current_range(start_loc), word, kind);
  }
  if (word == "I32") {
    kind = Tokens::PrimitiveKind::I32;

    return builder.nextPrimitive(current_range(start_loc), word, kind);
  }
  if (word == "I64") {
    kind = Tokens::PrimitiveKind::I64;

    return builder.nextPrimitive(current_range(start_loc), word, kind);
  }
  if (word == "I128") {
    kind = Tokens::PrimitiveKind::I128;

    return builder.nextPrimitive(current_range(start_loc), word, kind);
  }
  if (word == "U8") {
    kind = Tokens::PrimitiveKind::U8;

    return builder.nextPrimitive(current_range(start_loc), word, kind);
  }
  if (word == "U16") {
    kind = Tokens::PrimitiveKind::U16;

    return builder.nextPrimitive(current_range(start_loc), word, kind);
  }
  if (word == "U32") {
    kind = Tokens::PrimitiveKind::U32;

    return builder.nextPrimitive(current_range(start_loc), word, kind);
  }
  if (word == "U64") {
    kind = Tokens::PrimitiveKind::U64;

    return builder.nextPrimitive(current_range(start_loc), word, kind);
  }
  if (word == "U128") {
    kind = Tokens::PrimitiveKind::U128;

    return builder.nextPrimitive(current_range(start_loc), word, kind);
  }
  if (word == "F32") {
    kind = Tokens::PrimitiveKind::F32;

    return builder.nextPrimitive(current_range(start_loc), word, kind);
  }
  if (word == "F64") {
    kind = Tokens::PrimitiveKind::F64;

    return builder.nextPrimitive(current_range(start_loc), word, kind);
  }
  if (word == "F128") {
    kind = Tokens::PrimitiveKind::F128;

    return builder.nextPrimitive(current_range(start_loc), word, kind);
  }
  if (word == "CHAR") {
    kind = Tokens::PrimitiveKind::CHAR;

    return builder.nextPrimitive(current_range(start_loc), word, kind);
  }
  if (word == "BOOL") {
    kind = Tokens::PrimitiveKind::BOOL;

    return builder.nextPrimitive(current_range(start_loc), word, kind);
  }
  return builder.nextInvalid(current_range(start_loc),
                             source_str(start_idx, index));
}

Tokens::Token Lexer::nextKeywordOrIdentifier() {
  auto start_loc = current_location();
  std::size_t start_idx = index;
  std::string word;
  if (((peek() >= 'a' && peek() <= 'z') || (peek() >= 'A' && peek() <= 'Z')) ||
      peek() == '_') {
    advance();
    while (((peek() >= 'a' && peek() <= 'z') ||
            (peek() >= 'A' && peek() <= 'Z') ||
            (peek() >= '0' && peek() <= '9')) ||
           peek() == '_') {
      advance();
    }
    word = source_str(start_idx, index);

    if (word == "module")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::MODULE);
    if (word == "import")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::IMPORT);
    if (word == "struct")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::STRUCT);
    if (word == "enum")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::ENUM);
    if (word == "trait")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::TRAIT);
    if (word == "namespace")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::NAMESPACE);
    if (word == "extern")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::EXTERN);
    if (word == "const")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::CONST);
    if (word == "static")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::STATIC);
    if (word == "if")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::IF);
    if (word == "else")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::ELSE);
    if (word == "while")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::WHILE);
    if (word == "for")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::FOR);
    if (word == "switch")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::SWITCH);
    if (word == "case")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::CASE);
    if (word == "default")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::DEFAULT);
    if (word == "return")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::RETURN);
    if (word == "break")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::BREAK);
    if (word == "continue")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::CONTINUE);
    if (word == "fn")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::FN);
    if (word == "generic")
      return builder.nextKeyword(current_range(start_loc), word,
                                 Tokens::KeywordKind::GENERIC);
    Tokens::Token prim = nextPrimitive(word);
    if (prim.kind == Tokens::TokenKind::INVALID) {
      return builder.nextIdentifier(current_range(start_loc), word);
    } else {
      return prim;
    }
  }
  return builder.nextInvalid(current_range(start_loc), "");
}

Tokens::Token Lexer::nextOperator() {
  const auto start_loc = current_location();
  const std::size_t start_idx = index;

  if (eof()) {
    return builder.nextInvalid(current_range(start_loc), "");
  }

  // Match longest operators first (for example, >>= before >>).
  const auto matches = [this](const char *spelling) {
    const std::size_t length = std::char_traits<char>::length(spelling);
    return length <= source.size() - index &&
           source.compare(index, length, spelling) == 0;
  };

  const auto makeOperator = [this, start_loc, start_idx](
                                std::size_t length, Tokens::OperatorKind kind) {
    for (std::size_t i = 0; i < length; ++i) {
      advance();
    }
    return builder.nextOperator(current_range(start_loc),
                                source_str(start_idx, index), kind);
  };

  if (matches("<<="))
    return makeOperator(3, Tokens::OperatorKind::SHIFTLEFT_EQUAL);
  if (matches(">>="))
    return makeOperator(3, Tokens::OperatorKind::SHIFTRIGHT_EQUAL);

  if (matches("++"))
    return makeOperator(2, Tokens::OperatorKind::PLUSPLUS);
  if (matches("--"))
    return makeOperator(2, Tokens::OperatorKind::MINUSMINUS);
  if (matches("=="))
    return makeOperator(2, Tokens::OperatorKind::EQUALEQUAL);
  if (matches("!="))
    return makeOperator(2, Tokens::OperatorKind::BANGEQUAL);
  if (matches("<="))
    return makeOperator(2, Tokens::OperatorKind::LESSEQUAL);
  if (matches(">="))
    return makeOperator(2, Tokens::OperatorKind::GREATEREQUAL);
  if (matches("&&"))
    return makeOperator(2, Tokens::OperatorKind::ANDAND);
  if (matches("||"))
    return makeOperator(2, Tokens::OperatorKind::OROR);
  if (matches("+="))
    return makeOperator(2, Tokens::OperatorKind::PLUSEQUAL);
  if (matches("-="))
    return makeOperator(2, Tokens::OperatorKind::MINUSEQUAL);
  if (matches("*="))
    return makeOperator(2, Tokens::OperatorKind::STAREQUAL);
  if (matches("/="))
    return makeOperator(2, Tokens::OperatorKind::SLASHEQUAL);
  if (matches("%="))
    return makeOperator(2, Tokens::OperatorKind::PERCENTEQUAL);
  if (matches("^="))
    return makeOperator(2, Tokens::OperatorKind::CARET_EQUAL);
  if (matches("|="))
    return makeOperator(2, Tokens::OperatorKind::PIPE_EQUAL);
  if (matches("&="))
    return makeOperator(2, Tokens::OperatorKind::AMPERSANDEQUAL);
  if (matches("<<"))
    return makeOperator(2, Tokens::OperatorKind::SHIFTLEFT);
  if (matches(">>"))
    return makeOperator(2, Tokens::OperatorKind::SHIFTRIGHT);

  switch (peek()) {
  case '+':
    return makeOperator(1, Tokens::OperatorKind::PLUS);
  case '-':
    return makeOperator(1, Tokens::OperatorKind::MINUS);
  case '*':
    return makeOperator(1, Tokens::OperatorKind::STAR);
  case '/':
    return makeOperator(1, Tokens::OperatorKind::SLASH);
  case '%':
    return makeOperator(1, Tokens::OperatorKind::PERCENT);
  case '=':
    return makeOperator(1, Tokens::OperatorKind::EQUAL);
  case '!':
    return makeOperator(1, Tokens::OperatorKind::BANG);
  case '<':
    return makeOperator(1, Tokens::OperatorKind::LESS);
  case '>':
    return makeOperator(1, Tokens::OperatorKind::GREATER);
  case '&':
    return makeOperator(1, Tokens::OperatorKind::AMPERSAND);
  case '|':
    return makeOperator(1, Tokens::OperatorKind::PIPE);
  case '^':
    return makeOperator(1, Tokens::OperatorKind::CARET);
  case '~':
    return makeOperator(1, Tokens::OperatorKind::TILDE);
  case '?':
    return makeOperator(1, Tokens::OperatorKind::QUESTION);
  default:
    // Consume unrecognized bytes to ensure the lexer always makes progress.
    advance();
    return builder.nextInvalid(current_range(start_loc),
                               source_str(start_idx, index));
  }
}

Tokens::Token Lexer::nextPunctuation() {
  const auto start_loc = current_location();
  const std::size_t start_idx = index;

  if (eof()) {
    return builder.nextInvalid(current_range(start_loc), "");
  }

  const auto matches = [this](const char *spelling) {
    const std::size_t length = std::char_traits<char>::length(spelling);
    return length <= source.size() - index &&
           source.compare(index, length, spelling) == 0;
  };

  const auto makePunctuation = [this, start_loc,
                                start_idx](std::size_t length,
                                           Tokens::PunctuationKind kind) {
    for (std::size_t i = 0; i < length; ++i) {
      advance();
    }
    return builder.nextPunctuation(current_range(start_loc),
                                   source_str(start_idx, index), kind);
  };

  if (matches("..."))
    return makePunctuation(3, Tokens::PunctuationKind::ELLIPSIS);
  if (matches("::"))
    return makePunctuation(2, Tokens::PunctuationKind::COLONCOLON);
  if (matches("->"))
    return makePunctuation(2, Tokens::PunctuationKind::ARROW);

  switch (peek()) {
  case ',':
    return makePunctuation(1, Tokens::PunctuationKind::COMMA);
  case ':':
    return makePunctuation(1, Tokens::PunctuationKind::COLON);
  case ';':
    return makePunctuation(1, Tokens::PunctuationKind::SEMICOLON);
  case '(':
    return makePunctuation(1, Tokens::PunctuationKind::LEFTPAREN);
  case ')':
    return makePunctuation(1, Tokens::PunctuationKind::RIGHTPAREN);
  case '{':
    return makePunctuation(1, Tokens::PunctuationKind::LEFTBRACE);
  case '}':
    return makePunctuation(1, Tokens::PunctuationKind::RIGHTBRACE);
  case '[':
    return makePunctuation(1, Tokens::PunctuationKind::LEFTBRACKET);
  case ']':
    return makePunctuation(1, Tokens::PunctuationKind::RIGHTBRACKET);
  case '.':
    return makePunctuation(1, Tokens::PunctuationKind::DOT);
  default:
    advance();
    return builder.nextInvalid(current_range(start_loc),
                               source_str(start_idx, index));
  }
}

Tokens::Token Lexer::nextToken() {
  skipWhitespaceAndComments();

  if (eof()) {
    return builder.nextEof(current_range(current_location()));
  }

  const char c = peek();
  const char n = peek(1);

  if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
    return nextKeywordOrIdentifier();
  }

  if ((c >= '0' && c <= '9') || c == '\'' || c == '"') {
    return nextLiteral();
  }

  // Arrow belongs to PunctuationKind in the token model. Route it here before
  // treating '-' as an arithmetic operator.
  if (c == '-' && n == '>') {
    return nextPunctuation();
  }

  switch (c) {
  case ',':
  case ':':
  case ';':
  case '(':
  case ')':
  case '{':
  case '}':
  case '[':
  case ']':
  case '.':
    return nextPunctuation();
  default:
    return nextOperator();
  }
}

bool Lexer::match(char c) {
  if (eof() || peek() != c) {
    return false;
  }
  advance();
  return true;
}

loc::SourceLocation Lexer::current_location() {
  return loc::SourceLocation(line, column);
}

loc::SourceRange Lexer::current_range(loc::SourceLocation start) {
  return loc::SourceRange(start, current_location(), filename, file_index);
}

std::string Lexer::source_str(std::size_t start_idx, std::size_t end_idx) {
  if (start_idx > source.size()) {
    return {};
  }
  end_idx = end_idx > source.size() ? source.size() : end_idx;
  if (end_idx < start_idx) {
    return {};
  }
  return source.substr(start_idx, end_idx - start_idx);
}

} // namespace Titan
