#pragma once

#include "program.h"

using namespace std;

void test();

void func_call_to_c(FunctionCall& f, stringstream& str);
void func_to_c(Function& f, stringstream& str);
void expr_to_c(Expression& e, stringstream& str);