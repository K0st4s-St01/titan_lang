#include "tokens.hpp"
#include <sstream>
std::string Titan::Tokens::Token::to_string() {
  std::ostringstream os;
  os << range.to_string();
  os << " ";
  switch (kind) {
  case TokenKind::END_OF_FILE:
    os << "EOF";
    break;
  case TokenKind::INVALID:
    os << "INVALID" ; break;
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
    };
    break;
  case TokenKind::PUNCTUATION:
  case TokenKind::OPERATOR:
  case TokenKind::LITERAL:
  case TokenKind::PRIMITIVE_TYPE:
    break;
  }
}
