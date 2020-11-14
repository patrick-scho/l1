#pragma once

#include <sstream>

#include "parse.hpp"

using namespace std;

namespace codegen {
  void to_c(program::statement stmt, stringstream& sstr) {
    //CALL_STMT(to_c, stmt, sstr)
  }

  string to_c(program::program prg) {
    stringstream sstr;

    for (int i = 0; i < prg.main.statements.size(); i++) {
      to_c(prg.main.statements[i], sstr);
    }

    return sstr.str();
  }
}