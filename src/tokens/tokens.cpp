#include "tokens.hpp"
#include <sstream>
namespace Titan {
namespace Tokens {
std::string Titan::Tokens::Token::to_string() {
  std::ostringstream os;
  os << range.to_string();
  os << " ";
  switch (kind) {
  case TokenKind::END_OF_FILE:
    os << "EOF";
    break;
  case TokenKind::INVALID:
    os << "INVALID";
    break;
  case TokenKind::KEYWORD:
    os << "KW ";
    switch (keyword_kind.value()) {
    case KeywordKind::MODULE:
      os << "MODULE";
      break;
    case KeywordKind::IMPORT:
      os << "IMPORT";
      break;
    case KeywordKind::STRUCT:
      os << "STRUCT";
      break;
    case KeywordKind::ENUM:
      os << "ENUM";
      break;
    case KeywordKind::TRAIT:
      os << "TRAIT";
      break;
    case KeywordKind::NAMESPACE:
      os << "NAMESPACE";
      break;
    case KeywordKind::EXTERN:
      os << "EXTERN";
      break;
    case KeywordKind::CONST:
      os << "CONST";
      break;
    case KeywordKind::STATIC:
      os << "STATIC";
      break;
    case KeywordKind::IF:
      os << "IF";
      break;
    case KeywordKind::ELSE:
      os << "ELSE";
      break;
    case KeywordKind::WHILE:
      os << "WHILE";
      break;
    case KeywordKind::FOR:
      os << "FOR";
      break;
    case KeywordKind::SWITCH:
      os << "SWITCH";
      break;
    case KeywordKind::CASE:
      os << "CASE";
      break;
    case KeywordKind::DEFAULT:
      os << "DEFAULT";
      break;
    case KeywordKind::RETURN:
      os << "RETURN";
      break;
    case KeywordKind::BREAK:
      os << "BREAK";
      break;
    case KeywordKind::CONTINUE:
      os << "CONTINUE";
      break;
    case KeywordKind::FN:
      os << "FN";
      break;
    case KeywordKind::GENERIC:
      os << "GENERIC";
      break;
    }
    break;
  case TokenKind::PUNCTUATION:
    switch (punctuation_kind.value()) {

    case PunctuationKind::COMMA:
      os << "COMMA";
      break;
    case PunctuationKind::COLON:
      os << "COLON";
      break;
    case PunctuationKind::SEMICOLON:
      os << "SEMICOLON";
      break;
    case PunctuationKind::COLONCOLON:
      os << "COLONCOLON";
      break;
    case PunctuationKind::ARROW:
      os << "ARROW";
      break;
    case PunctuationKind::LEFTPAREN:
      os << "LEFTPAREN";
      break;
    case PunctuationKind::RIGHTPAREN:
      os << "RIGHTPAREN";
      break;
    case PunctuationKind::LEFTBRACE:
      os << "LEFTBRACE";
      break;
    case PunctuationKind::RIGHTBRACE:
      os << "RIGHTBRACE";
      break;
    case PunctuationKind::LEFTBRACKET:
      os << "LEFTBRACKET";
      break;
    case PunctuationKind::RIGHTBRACKET:
      os << "RIGHTBRACKET";
      break;
    case PunctuationKind::DOT:
      os << "DOT";
      break;
    case PunctuationKind::ELLIPSIS:
      os << "ELLIPSIS";
      break;
    }
    break;
  case TokenKind::OPERATOR:
    switch (operator_kind.value()) {
    case OperatorKind::ARROW:
      os << "ARROW";
      break;
    case OperatorKind::PLUS:
      os << "PLUS";
      break;
    case OperatorKind::MINUS:
      os << "MINUS";
      break;
    case OperatorKind::STAR:
      os << "STAR";
      break;
    case OperatorKind::SLASH:
      os << "SLASH";
      break;
    case OperatorKind::PERCENT:
      os << "PERCENT";
      break;
    case OperatorKind::EQUAL:
      os << "EQUAL";
      break;
    case OperatorKind::EQUALEQUAL:
      os << "EQUALEQUAL";
      break;
    case OperatorKind::BANG:
      os << "BANG";
      break;
    case OperatorKind::BANGEQUAL:
      os << "BANGEQUAL";
      break;
    case OperatorKind::LESS:
      os << "LESS";
      break;
    case OperatorKind::LESSEQUAL:
      os << "LESSEQUAL";
      break;
    case OperatorKind::GREATER:
      os << "GREATER";
      break;
    case OperatorKind::GREATEREQUAL:
      os << "GREATEREQUAL";
      break;
    case OperatorKind::AMPERSAND:
      os << "AMPERSAND";
      break;
    case OperatorKind::PIPE:
      os << "PIPE";
      break;
    case OperatorKind::CARET:
      os << "CARET";
      break;
    case OperatorKind::TILDE:
      os << "TILDE";
      break;
    case OperatorKind::SHIFTLEFT:
      os << "SHIFTLEFT";
      break;
    case OperatorKind::SHIFTRIGHT:
      os << "SHIFTRIGHT";
      break;
    case OperatorKind::ANDAND:
      os << "ANDAND";
      break;
    case OperatorKind::OROR:
      os << "OROR";
      break;
    case OperatorKind::PLUSPLUS:
      os << "PLUSPLUS";
      break;
    case OperatorKind::MINUSMINUS:
      os << "MINUSMINUS";
      break;
    case OperatorKind::PLUSEQUAL:
      os << "PLUSEQUAL";
      break;
    case OperatorKind::MINUSEQUAL:
      os << "MINUSEQUAL";
      break;
    case OperatorKind::STAREQUAL:
      os << "STAREQUAL";
      break;
    case OperatorKind::SLASHEQUAL:
      os << "SLASHEQUAL";
      break;
    case OperatorKind::PERCENTEQUAL:
      os << "PERCENTEQUAL";
      break;
    case OperatorKind::CARET_EQUAL:
      os << "CARET_EQUAL";
      break;
    case OperatorKind::PIPE_EQUAL:
      os << "PIPE_EQUAL";
      break;
    case OperatorKind::AMPERSANDEQUAL:
      os << "AMPERSANDEQUAL";
      break;
    case OperatorKind::SHIFTLEFT_EQUAL:
      os << "SHIFTLEFT_EQUAL";
      break;
    case OperatorKind::SHIFTRIGHT_EQUAL:
      os << "SHIFTRIGHT_EQUAL";
      break;
    case OperatorKind::QUESTION:
      os << "QUESTION";
      break;
    }
    break;
  case TokenKind::LITERAL:
    switch (literal_kind.value()) {
    case LiteralKind::NUMBER:
      os << "NUMBER";
      break;
    case LiteralKind::FLOAT_NUMBER:
      os << "FLOAT_NUMBER";
      break;
    case LiteralKind::CHAR:
      os << "CHAR";
      break;
    case LiteralKind::STR:
      os << "STR";
      break;
    }
  case TokenKind::PRIMITIVE_TYPE:
    switch (primitive_kind.value()) {

    case PrimitiveKind::BYTE:
      os << "BYTE";
      break;
    case PrimitiveKind::I16:
      os << "I16";
      break;
    case PrimitiveKind::I32:
      os << "I32";
      break;
    case PrimitiveKind::I64:
      os << "I64";
      break;
    case PrimitiveKind::I128:
      os << "I128";
      break;
    case PrimitiveKind::U32:
      os << "U32";
      break;
    case PrimitiveKind::U64:
      os << "U64";
      break;
    case PrimitiveKind::U128:
      os << "U128";
      break;
    case PrimitiveKind::CHAR:
      os << "CHAR";
      break;
    case PrimitiveKind::BOOL:
      os << "BOOL";
      break;
    }
    break;
  }
  return os.str();
}

Token TokenBuilder::nextKeyword(Titan::loc::SourceRange range,
                                std::string lexeme, KeywordKind kind) {
  return Token(TokenKind::KEYWORD, range, std::move(lexeme), std::nullopt,
               std::nullopt, kind, std::nullopt, std::nullopt);
}

Token TokenBuilder::nextLiteral(Titan::loc::SourceRange range,
                                std::string lexeme, LiteralKind kind) {
  return Token(TokenKind::LITERAL, range, std::move(lexeme), kind, std::nullopt,
               std::nullopt, std::nullopt, std::nullopt);
}
Token TokenBuilder::nextPrimitive(Titan::loc::SourceRange range,
                                  std::string lexeme, PrimitiveKind kind) {
  return Token(TokenKind::PRIMITIVE_TYPE, range, std::move(lexeme),
               std::nullopt, kind, std::nullopt, std::nullopt, std::nullopt);
}
Token TokenBuilder::nextOperator(Titan::loc::SourceRange range,
                                 std::string lexeme, OperatorKind kind) {
  return Token(TokenKind::OPERATOR, range, std::move(lexeme), std::nullopt,
               std::nullopt, std::nullopt, kind, std::nullopt);
}
Token TokenBuilder::nextPunctuation(Titan::loc::SourceRange range,
                                    std::string lexeme, PunctuationKind kind) {
  return Token(TokenKind::PUNCTUATION, range, std::move(lexeme), std::nullopt,
               std::nullopt, std::nullopt, std::nullopt, kind);
}
Token TokenBuilder::nextInvalid(Titan::loc::SourceRange range,
                                std::string lexeme) {
  return Token(TokenKind::INVALID, range, std::move(lexeme), std::nullopt,
               std::nullopt, std::nullopt, std::nullopt, std::nullopt);
}
Token TokenBuilder::nextEof(loc::SourceRange range) {
  return Token(TokenKind::END_OF_FILE, range, "<EOF>", std::nullopt,
               std::nullopt, std::nullopt, std::nullopt, std::nullopt);
}
} // namespace Tokens
} // namespace Titan
