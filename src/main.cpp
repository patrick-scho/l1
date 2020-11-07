#include <string>
#include <vector>
#include <iostream>

#include "lex.hpp"
#include "parse.hpp"
#include "view.hpp"
#include "program.hpp"
#include "util.hpp"

using namespace std;

/*
Roadmap:
- when types?
- TCC
- Meta
- c_type/c_name
- Syntax
- Operators
- if/for
- include
- import/export/using
- Standard Types
- Types
- ZPL/tbox
- Git Submodules
- Arbitrary Precision Integers
- Conversion
*/

int main(int argc, char **argv) {
  string filename = argv[1];
  u32string file = util::read_file(filename);

  view::view source(filename, &file);

  vector<lex::token> tokens;
  
  try {
    tokens = lex::lex(source);
  }
  catch (std::runtime_error &e) {
    cout << e.what() << endl;
    return 1;
  }

  // for (lex::token t: tokens) {
  //   switch (t.type) {
  //   case lex::token_type::symbol:
  //     cout << "Symbol: " << t.s << "\t(" << t.loc.index << ')' << endl;
  //     break;
  //   case lex::token_type::identifier:
  //     cout << "Identifier: " << t.s << "\t(" << t.loc.index << ')' << endl;
  //     break;
  //   case lex::token_type::integer:
  //     cout << "Integer: " << t.i << "\t(" << t.loc.index << ')' << endl;
  //     break;
  //   case lex::token_type::decimal:
  //     cout << "Decimal: " << t.d << "\t(" << t.loc.index << ')' << endl;
  //     break;
  //   case lex::token_type::string:
  //     cout << "String: " << t.s << "\t(" << t.loc.index << ')' << endl;
  //     break;
  //   }
  // }

  program::function main = parse::parse(tokens);

  return 0;
}
