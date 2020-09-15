#ifndef PROGRAM__H
#define PROGRAM__H

#include "source.h"

typedef struct Variable Variable;
typedef struct Context Context;
typedef struct FunctionCall FunctionCall;
typedef struct Expression Expression;
typedef struct Function Function;

typedef struct Variable {
  char* name;
  char* type;
} Variable;

Variable* Variable_new(Source name, Source type);
void Variable_free(Variable* variable);

typedef struct Context {
  Variable** variables;
  Function** functions;
  Context* parent;
} Context;

Context* Context_new();
Variable* Context_get_variable(Context* context, Source name);
Function* Context_get_function(Context* context, Source name);
void Context_free(Context* context);

typedef struct FunctionCall {
  char* function;
  Expression** arguments;
} FunctionCall;

FunctionCall* FunctionCall_new(Source name);
void FunctionCall_free(FunctionCall* functionCall);

typedef enum ExpressionType {
  EXPR_FN_CALL, EXPR_FN_DECL, EXPR_VAR, EXPR_NUMBER, EXPR_STRING//, EXPR_OP
} ExpressionType;

typedef struct Expression {
  ExpressionType type;
  union {
    FunctionCall* functionCall;
    Function* functionDeclaration;
    char* variable;
    long number;
    char* string;
  };
} Expression;

Expression* Expression_new();
void Expression_set_string(Expression* expression, Source str);
void Expression_free(Expression* expression);

typedef struct Function {
  Context* context;

  char* name;

  Variable** arguments;

  Expression** expressions;
} Function;

Function* Function_new(Source name);
void Function_free(Function* function);

void Variable_print(Variable* variable);
void FunctionCall_print(FunctionCall* functionCall);
void Expression_print(Expression* expression);
void Function_print(Function* function);

#endif
