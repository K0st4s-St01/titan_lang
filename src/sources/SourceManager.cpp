#include "SourceManager.hpp"
#include <fstream>
namespace Titan {
namespace Sources {

Source::Source(std::string path) { this->path_ = std::filesystem::path(path); }

std::optional<std::string> Source::read_source() {
  std::ifstream reader(path_, std::ios::binary);
  if (!reader) {
    return std::nullopt;
  }
  return std::string(std::istreambuf_iterator<char>(reader),
                     std::istreambuf_iterator<char>());
}

SourceManager::SourceManager() {}
size_t SourceManager::len() { return sources.size(); }
void SourceManager::add(std::string path) { sources.push_back(Source(path)); };
std::optional<std::string> SourceManager::get(size_t index) {
  return sources.at(index).read_source();
}
} // namespace Sources
} // namespace Titan
