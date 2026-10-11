#pragma once

#include "../sources/SourceLocation.hpp"
#include <optional>
namespace Titan {
namespace Tokens {
enum class TokenKind {
  END_OF_FILE,
  INVALID,
  KEYWORD,
  PUNCTUATION,
  OPERATOR,
  LITERAL,
  PRIMITIVE_TYPE,
  IDENTIFIER
};

enum class KeywordKind {
  MODULE,
  IMPORT,
  STRUCT,
  ENUM,
  TRAIT,
  NAMESPACE,
  EXTERN,
  CONST,
  STATIC,

  IF,
  ELSE,
  WHILE,
  FOR,
  SWITCH,
  CASE,
  DEFAULT,

  RETURN,
  BREAK,
  CONTINUE,
  FN,
  GENERIC,
};

enum class PunctuationKind {
  COMMA,
  COLON,
  SEMICOLON,
  COLONCOLON,
  ARROW,
  LEFTPAREN,
  RIGHTPAREN,
  LEFTBRACE,
  RIGHTBRACE,
  LEFTBRACKET,
  RIGHTBRACKET,
  DOT,
  ELLIPSIS,

};

enum class OperatorKind {

  ARROW,
  PLUS,
  MINUS,
  STAR,
  SLASH,
  PERCENT,
  EQUAL,
  EQUALEQUAL,
  BANG,
  BANGEQUAL,
  LESS,
  LESSEQUAL,
  GREATER,
  GREATEREQUAL,
  AMPERSAND,
  PIPE,
  CARET,
  TILDE,
  SHIFTLEFT,
  SHIFTRIGHT,
  ANDAND,
  OROR,
  PLUSPLUS,
  MINUSMINUS,
  PLUSEQUAL,
  MINUSEQUAL,
  STAREQUAL,
  SLASHEQUAL,
  PERCENTEQUAL,
  CARET_EQUAL,
  PIPE_EQUAL,
  AMPERSANDEQUAL,
  SHIFTLEFT_EQUAL,
  SHIFTRIGHT_EQUAL,
  QUESTION,
};

enum class LiteralKind {
  NUMBER,
  FLOAT_NUMBER,
  CHAR,
  STR,
};

enum class PrimitiveKind {
  VOID,
  BYTE,
  I8,
  I16,
  I32,
  I64,
  I128,
  U8,
  U16,
  U32,
  U64,
  U128,
  F32,
  F64,
  F128,
  CHAR,
  BOOL,
};
struct Token {
  Token(TokenKind kind, Titan::loc::SourceRange range, std::string lexeme,
        std::optional<LiteralKind> literal_kind,
        std::optional<PrimitiveKind> primitive_kind,
        std::optional<KeywordKind> keyword_kind,
        std::optional<OperatorKind> operator_kind,
        std::optional<PunctuationKind> punctuation_kind)
      : kind(kind), range(std::move(range)), lexeme(std::move(lexeme)),
        literal_kind(literal_kind), primitive_kind(primitive_kind),
        keyword_kind(keyword_kind), operator_kind(operator_kind),
        punctuation_kind(punctuation_kind) {}
  TokenKind kind;
  Titan::loc::SourceRange range;
  std::string lexeme;
  std::optional<LiteralKind> literal_kind;
  std::optional<PrimitiveKind> primitive_kind;
  std::optional<KeywordKind> keyword_kind;
  std::optional<OperatorKind> operator_kind;
  std::optional<PunctuationKind> punctuation_kind;

  std::string to_string();
};
class TokenBuilder {
private:
  std::string filename;

public:
  explicit TokenBuilder(std::string filename) : filename(std::move(filename)) {}
  Token nextKeyword(Titan::loc::SourceRange range, std::string lexeme,
                    KeywordKind kind);
  Token nextLiteral(Titan::loc::SourceRange range, std::string lexeme,
                    LiteralKind kind);
  Token nextPrimitive(Titan::loc::SourceRange range, std::string lexeme,
                      PrimitiveKind kind);
  Token nextOperator(Titan::loc::SourceRange range, std::string lexeme,
                     OperatorKind kind);
  Token nextPunctuation(Titan::loc::SourceRange range, std::string lexeme,
                        PunctuationKind kind);
  Token nextInvalid(Titan::loc::SourceRange range, std::string lexeme);
  Token nextIdentifier(loc::SourceRange range,std::string lexme);
  Token nextEof(loc::SourceRange);
};
} // namespace Tokens
} // namespace Titan
