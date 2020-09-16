#pragma once

#include <string>
#include <vector>
#include <list>
#include <memory>

#include "source.h"

using namespace std;

struct Expression {
  virtual void print() = 0;
};

struct Variable;
struct Context;
struct FunctionCall;
struct Function;

struct Context {
  list<Variable*> variables;
  list<Function*> functions;
  Context* parent;

  //Function* getFunction(string name);
  //Variable* getVariable(string name);

  void print();
};

struct FunctionCall : Expression {
  string_view function;
  list<unique_ptr<Expression>> arguments;

  void print();
};

struct Function : Expression {
  string_view name;
  list<unique_ptr<Variable>> arguments;
  list<unique_ptr<Expression>> expressions;

  Context context;

  void print();
};

struct Assignment : Expression {
  string_view var;
  unique_ptr<Expression> expression;

  void print();
};

struct Variable : Expression {
  string_view name, type;

  void print();
};

struct Number : Expression {
  long value;

  void print();
};

struct String : Expression {
  string_view value;

  void print();
};
