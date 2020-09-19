#pragma once

#include <string>
#include <vector>
#include <list>
#include <memory>

#include "source.h"
#include "util.h"

using namespace std;

struct Context;

struct FunctionCall;
struct Function;
struct Assignment;
struct Variable;
struct Number;
struct String;
struct Type;

struct Expression {
  Source::Location location;
  virtual void print() = 0;
  virtual Type getType(Context& context) = 0;
};

struct Context {
  list<Variable*> variables;
  list<Function*> functions;
  Context* parent = nullptr;

  Function* getFunction(Expression* expr, string_view name);
  Variable* getVariable(Expression* expr, string_view name);

  void print();
  Type getType(Context& context);
};

struct Type {
  string_view name;
};

struct FunctionCall : Expression {
  string_view function;
  list<unique_ptr<Expression>> arguments;

  void print();
  Type getType(Context& context);
};

struct Function : Expression {
  string_view name;
  Type returnType;
  list<unique_ptr<Variable>> arguments;
  list<unique_ptr<Expression>> expressions;

  Context context;

  void print();
  Type getType(Context& context);
};

struct Assignment : Expression {
  unique_ptr<Variable> var;
  unique_ptr<Expression> expression;

  void print();
  Type getType(Context& context);
};

struct Variable : Expression {
  string_view name;
  Type type;

  void print();
  Type getType(Context& context);
};

struct Number : Expression {
  long value;

  void print();
  Type getType(Context& context);
};

struct String : Expression {
  string_view value;

  void print();
  Type getType(Context& context);
};