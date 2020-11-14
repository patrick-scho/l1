#include <iostream>
#include <string>
#include <vector>

#include "magic_enum.hpp"

#include "lex.hpp"
#include "parse.hpp"
#include "program.hpp"
#include "program_print.hpp"
#include "codegen.hpp"
#include "util.hpp"
#include "view.hpp"

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
- Example with opt return
*/

/*
Notes:
- end token
- no spaces?
*/

int main(int argc, char **argv) {
  if (argc != 2)
    return 1;

  try {
    string filename = argv[1];
    u32string file = util::read_file(filename);

    view::view source(filename, &file);

    //cout << "\nLexing\n\n";

    vector<lex::token> tokens = lex::lex(source);

    // for (lex::token t : tokens) {
    //   cout.width(20); cout << std::left;
    //   cout << magic_enum::enum_name(t.type);
    //   cout.width(10);
    //   cout << t.view.to_str();
    //   cout << "(";
    //   cout.width(3); cout << std::right;
    //   cout << t.view.get_loc().line;
    //   cout << ", ";
    //   cout.width(3);
    //   cout << t.view.get_loc().column;
    //   cout << ')' << endl;
    // }

    cout << "\n\nParsing\n\n";

    program::program prg = parse::parse(tokens);
    program::print_program(prg);

    cout << "\n\nGenerating C Code\n\n";

    string c = codegen::to_c(prg);

    cout << c;

    cout << "\n\nDone\n";
  } catch (std::runtime_error &e) {
    cout << e.what() << endl;
    return 1;
  }

  return 0;
}
