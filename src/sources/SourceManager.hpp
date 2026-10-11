#pragma once
#include <filesystem>
#include <optional>
#include <vector>

namespace Titan {
namespace Sources {
struct Source {
  std::filesystem::path path_;
  Source(std::string path);
  std::string read_source();
};

class SourceManager {
private:
  std::vector<Source> sources;

public:
  SourceManager();
  ~SourceManager() = default;
  size_t len();
  void add(std::string path);
  std::string get_source(size_t index);
  std::string get_name(size_t index);
};
} // namespace Sources
} // namespace Titan
