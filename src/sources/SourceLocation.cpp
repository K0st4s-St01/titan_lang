#include "SourceLocation.hpp"
#include <sstream>

std::string Titan::loc::SourceLocation::to_string() {
  std::ostringstream os;
  os << line << ":" << col;
  return os.str();
}

std::string Titan::loc::SourceRange::to_string() {
  std::ostringstream os;
  os << filename << " " << start.to_string() << "-" << end.to_string();
  return os.str();
}
