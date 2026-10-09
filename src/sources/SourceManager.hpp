#pragma once
#include <filesystem>
#include <optional>
#include <vector>

namespace Titan {
namespace Sources {
struct Source {
  std::filesystem::path path_;
  Source(std::string path);
  std::optional<std::string> read_source();
};

class SourceManager {
private:
  std::vector<Source> sources;

public:
  SourceManager();
  ~SourceManager() = default;
  size_t len();
  void add(std::string path);
  std::optional<std::string> get(size_t index);
};
} // namespace Sources
} // namespace Titan
