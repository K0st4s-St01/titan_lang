#include "sources/SourceManager.hpp"

int main(int argc, char **argv) {
  Titan::Sources::SourceManager sm;
  for (int i = 0; i < argc; i++)
    sm.add(argv[i]);
  return 0;
}
