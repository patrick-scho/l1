#pragma once

#include <vector>
#include <memory>
#include <string>
#include <tuple>
#include <sstream>

#include "source.h"
#include "util.h"

using namespace std;

// predef

struct Type;
struct Context;
struct Function;
struct MetaFunction;
struct Variable;

struct FunctionCall;
struct FunctionRef;
struct Assignment;
struct VariableRef;
struct CCall;
struct Number;
struct String;

// structs

struct Expression {
  Source::Location location;
  bool is_meta = false;
  virtual void print(stringstream& str) = 0;
  virtual Type get_type(Context &context) = 0;
  virtual void to_c(stringstream& str, Context& context) = 0;
  virtual void to_meta(stringstream& str) = 0;
};

struct Type {
  string_view name;
};

struct Context {
  vector<unique_ptr<Variable>> variables;
  vector<unique_ptr<Function>> functions;
  vector<unique_ptr<MetaFunction>> meta_functions;
  Context *parent = nullptr;

  bool is_meta = false;
  bool is_meta_context();

  Function *get_function(string_view name, vector<Type>& argTypes, bool recursive = false);
  MetaFunction *get_meta_function(string_view name, vector<Type>& argTypes, bool recursive = false);
  Variable *get_variable(string_view name, bool recursive = false);

  void print(stringstream& str);
  void to_c(stringstream& str);
};

struct Function {
  string_view name;
  string appendix;
  Type returnType;
  vector<unique_ptr<VariableRef>> arguments;
  vector<unique_ptr<Expression>> expressions;

  FunctionRef *definition;

  Context context;

  void print(stringstream& str);
  Type get_type(Context &context);
  void to_c(stringstream& str, Context& context);
};

struct MetaFunction {
  string_view name;
  vector<unique_ptr<VariableRef>> arguments;
  vector<unique_ptr<Expression>> expressions;

  Source::Location location;

  Context context;

  void print(stringstream& str);
  void to_c(stringstream& str, Context& context);
};

struct Variable {
  string_view name;
  Type type;

  Expression *definition;

  void print(stringstream& str);
  Type get_type(Context &context);
};

// expressions

struct FunctionCall : Expression {
  string_view name;
  Function *function = nullptr;
  vector<unique_ptr<Expression>> arguments;

  void print(stringstream& str);
  Type get_type(Context &context);
  void to_c(stringstream& str, Context& context);
  void to_meta(stringstream& str);
};

struct FunctionRef : Expression {
  Function *function;

  void print(stringstream& str);
  Type get_type(Context &context);
  void to_c(stringstream& str, Context& context);
  void to_meta(stringstream& str);
};

struct MetaFunctionRef : Expression {
  MetaFunction *meta_function;

  void print(stringstream& str);
  Type get_type(Context &context);
  void to_c(stringstream& str, Context& context);
  void to_meta(stringstream& str);
};

struct Assignment : Expression {
  Variable *var;
  unique_ptr<Expression> expression;

  void print(stringstream& str);
  Type get_type(Context &context);
  void to_c(stringstream& str, Context& context);
  void to_meta(stringstream& str);
};

struct VariableRef : Expression {
  string_view name;
  Variable *variable;

  void print(stringstream& str);
  Type get_type(Context &context);
  void to_c(stringstream& str, Context& context);
  void to_meta(stringstream& str);
};

struct CCall : Expression {
  string_view value;

  void print(stringstream& str);
  Type get_type(Context &context);
  void to_c(stringstream& str, Context& context);
  void to_meta(stringstream& str);
};

struct Number : Expression {
  long value;

  void print(stringstream& str);
  Type get_type(Context &context);
  void to_c(stringstream& str, Context& context);
  void to_meta(stringstream& str);
};

struct String : Expression {
  string_view value;

  void print(stringstream& str);
  Type get_type(Context &context);
  void to_c(stringstream& str, Context& context);
  void to_meta(stringstream& str);
};