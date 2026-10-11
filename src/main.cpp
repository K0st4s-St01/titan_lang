#include "lexer/Lexer.hpp"
#include "sources/SourceManager.hpp"
#include <iostream>

int main(int argc, char **argv) {
  Titan::Sources::SourceManager sm;
  for (int i = 1; i < argc; i++)
    sm.add(argv[i]);

  for (int i = 0; i < sm.len(); i++) {
    Titan::Lexer lexer = Titan::Lexer(
        Titan::Tokens::TokenBuilder(sm.get_name(i)), sm.get_source(i));
    while(!lexer.eof()){
      std::cout<<lexer.nextToken().to_string()<<"\n";
    }
  }
  return 0;
}
