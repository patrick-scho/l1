#pragma once

#include "program.h"

using namespace std;

void compile(const string& filename, const string& src);

void run(const string& filename);

string get_c_type(Type type);