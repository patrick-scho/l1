#pragma once

#include <list>
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

struct FunctionCall;
struct FunctionRef;
struct Assignment;
struct Variable;
struct Number;
struct String;

// Visitor

struct Visitor {
  virtual void visit(FunctionCall*) = 0;
  virtual void visit(FunctionRef*) = 0;
  virtual void visit(Assignment*) = 0;
  virtual void visit(Variable*) = 0;
  virtual void visit(Number*) = 0;
  virtual void visit(String*) = 0;
};

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
  list<unique_ptr<Variable>> variables;
  list<unique_ptr<Function>> functions;
  Context *parent = nullptr;

  Function *getFunction(string_view name, list<Type>& argTypes);
  Variable *getVariable(string_view name);

  void print();
};

struct Function {
  string_view name;
  Type returnType;
  list<Variable *> arguments;
  list<unique_ptr<Expression>> expressions;

  FunctionRef *definition;

  Context context;

  void print();
  Type getType(Context &context);
};

// expressions

struct FunctionCall : Expression {
  string_view name;
  Function *function = nullptr;
  list<unique_ptr<Expression>> arguments;

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

struct Variable : Expression {
  string_view name;
  Type type;

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