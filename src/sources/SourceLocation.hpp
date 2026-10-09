#pragma once

#include <string>
namespace Titan {
namespace loc {

struct SourceLocation {
  SourceLocation(int line, int col) : line(line), col(col) {}
  int line, col;
  std::string to_string();
};

struct SourceRange {
  SourceRange(SourceLocation start, SourceLocation end, std::string filename,
              std::size_t file_index)
      : start(start), end(end), filename(std::move(filename)),
        file_index(file_index) {}
  SourceLocation start, end;
  std::string filename;
  std::size_t file_index;
  std::string to_string();
};
} // namespace loc
} // namespace Titan
