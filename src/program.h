#pragma once

#include <vector>
#include <memory>
#include <string>
#include <tuple>

#include "source.h"
#include "util.h"

using namespace std;

// predef

struct Type;
struct Context;
struct Function;
struct Variable;

struct FunctionCall;
struct FunctionRef;
struct Assignment;
struct VariableRef;
struct Number;
struct String;

// structs

struct Expression {
  Source::Location location;
  virtual void print() = 0;
  virtual Type getType(Context &context) = 0;
};

struct Type {
  string_view name;
};

struct Context {
  vector<unique_ptr<Variable>> variables;
  vector<unique_ptr<Function>> functions;
  Context *parent = nullptr;

  Function *getFunction(string_view name, vector<Type>& argTypes);
  Variable *getVariable(string_view name);

  void print();
};

struct Function {
  string_view name;
  Type returnType;
  vector<unique_ptr<VariableRef>> arguments;
  vector<unique_ptr<Expression>> expressions;

  FunctionRef *definition;

  Context context;

  void print();
  Type getType(Context &context);
};

struct Variable {
  string_view name;
  Type type;

  Expression *definition;

  void print();
  Type getType(Context &context);
};

// expressions

struct FunctionCall : Expression {
  string_view name;
  Function *function = nullptr;
  vector<unique_ptr<Expression>> arguments;

  void print();
  Type getType(Context &context);
};

struct FunctionRef : Expression {
  Function *function;

  void print();
  Type getType(Context &context);
};

struct Assignment : Expression {
  Variable *var;
  unique_ptr<Expression> expression;

  void print();
  Type getType(Context &context);
};

struct VariableRef : Expression {
  Variable *variable;

  void print();
  Type getType(Context &context);
};

struct Number : Expression {
  long value;

  void print();
  Type getType(Context &context);
};

struct String : Expression {
  string_view value;

  void print();
  Type getType(Context &context);
};