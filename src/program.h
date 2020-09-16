#pragma once

#include <string>
#include <vector>
#include <list>

#include "source.h"

using namespace std;

class Variable;
class Context;
class FunctionCall;
class Function;
class Expression;

class Variable {
public:
  string_view name, type;

  void print();
};

class Context {
public:
  list<Variable> variables;
  list<Function> functions;
  Context* parent;

  Function* getFunction(string name);
  Variable* getVariable(string name);

  void print();
};

class FunctionCall {
public:
  string_view function;
  list<Expression> arguments;

  void print();
};

class Function {
public:
  string_view name;
  list<Variable> arguments;
  list<Expression> expressions;

  Context context;

  void print();
};

enum class ExpressionType {
  FnCall, FnDecl, Var, Number, String
};

class Expression {
public:
  ExpressionType type;
  FunctionCall functionCall;
  Function functionDeclaration;
  string_view variable;
  long number;
  string_view string;

  void print();
};
