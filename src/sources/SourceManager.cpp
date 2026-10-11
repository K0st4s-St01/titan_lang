#include "SourceManager.hpp"
#include <fstream>
namespace Titan {
namespace Sources {

Source::Source(std::string path) { this->path_ = std::filesystem::path(path); }

std::string Source::read_source() {
  std::ifstream reader(path_, std::ios::binary);
  if (!reader) {
    return "\0";
  }
  return std::string(std::istreambuf_iterator<char>(reader),
                     std::istreambuf_iterator<char>());
}

SourceManager::SourceManager() {}
size_t SourceManager::len() { return sources.size(); }
void SourceManager::add(std::string path) { sources.push_back(Source(path)); };
std::string SourceManager::get_source(size_t index) {
  return sources.at(index).read_source();
}
std::string SourceManager::get_name(size_t index){
  return this->sources.at(index).path_.filename().generic_string();
}
} // namespace Sources
} // namespace Titan
