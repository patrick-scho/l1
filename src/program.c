#include "program.h"

#include <stdio.h>

#include "array.h"
#include "alloc.h"
#include "source.h"
#include "util.h"


Variable* Variable_new(Source name, Source type) {
  Variable* result = a_malloc(sizeof(Variable));
  copy_str(&result->name, name);
  copy_str(&result->type, type);
  return result;
}
void Variable_free(Variable* variable) {
  a_free(variable->name);
  a_free(variable->type);

  a_free(variable);
}


Context* Context_new() {
  Context* result = a_malloc(sizeof(Context));
  result->functions = array_new(sizeof(Function*), 1);
  result->variables = array_new(sizeof(Variable*), 1);
  return result;
}
Variable* Context_get_variable(Context* context, Source name) {
  for (int i = 0; i < array_get_length(context->variables); i++) {
    Variable* var = array_get(context->variables, i);
    if (source_cmp(name, var->name))
      return var;
  }
  Source type = { .data="i32", .index=0, .size=3 };
  Variable* var = Variable_new(name, type);
  context->variables = array_add(context->variables, var);
  return var;
}
Function* Context_get_function(Context* context, Source name) {
  for (int i = 0; i < array_get_length(context->functions); i++) {
    Function* fn = array_get(context->functions, i);
    if (source_cmp(name, fn->name))
      return fn;
  }
  return NULL;
}
void Context_free(Context* context) {
  ARRAY_FREE(context->functions, Function)
  ARRAY_FREE(context->variables, Variable)
  array_free(context->functions);
  array_free(context->variables);

  a_free(context);
}


FunctionCall* FunctionCall_new(Source name) {
  FunctionCall* result = a_malloc(sizeof(FunctionCall));
  copy_str(&result->function, name);
  result->arguments = array_new(sizeof(Expression*), 1);
  return result;
}
void FunctionCall_free(FunctionCall* functionCall) {
  a_free(functionCall->function);
  ARRAY_FREE(functionCall->arguments, Expression)
  array_free(functionCall->arguments);

  a_free(functionCall);
}


Expression* Expression_new() {
  Expression* result = a_malloc(sizeof(Expression));
  result->string = NULL;
  return result;
}
void Expression_set_string(Expression* expression, Source str) {
  if (expression->type == EXPR_STRING && expression->string != NULL) {
    a_free(expression->string);
  }
  expression->type = EXPR_STRING;
  copy_str(&(expression->string), str);
}
void Expression_free(Expression* expression) {
  switch (expression->type) {
  case EXPR_STRING:
    a_free(expression->string); break;
  case EXPR_FN_CALL:
    FunctionCall_free(expression->functionCall); break;
  case EXPR_FN_DECL:
    Function_free(expression->functionDeclaration); break;
  case EXPR_VAR:
    a_free(expression->variable);
  }

  a_free(expression);
}


Function* Function_new(Source name) {
  Function* result = a_malloc(sizeof(Function));
  copy_str(&(result->name), name);
  result->context = Context_new();
  result->arguments = array_new(sizeof(Variable*), 1);
  result->expressions = array_new(sizeof(Expression*), 1);
  return result;
}
void Function_free(Function* function) {
  a_free(function->name);
  Context_free(function->context);
  ARRAY_FREE(function->arguments, Variable)
  ARRAY_FREE(function->expressions, Expression)
  array_free(function->arguments);
  array_free(function->expressions);

  a_free(function);
}


void Variable_print(Variable* variable) {
  printf("%s(%s)", variable->name, variable->type);
}
void FunctionCall_print(FunctionCall* functionCall) {
  printf("%s(", functionCall->function);
  for (int i = 0; i < array_get_length(functionCall->arguments); i++) {
    Expression_print(array_get(functionCall->arguments, i));
  }
  printf(")");
}
void Expression_print(Expression* expression) {
  switch (expression->type) {
  case EXPR_FN_CALL:
    FunctionCall_print(expression->functionCall); break;
  case EXPR_FN_DECL:
    Function_print(expression->functionDeclaration); break;
  case EXPR_NUMBER:
    printf("%l", expression->number); break;
  case EXPR_STRING:
    printf("%s", expression->string); break;
  case EXPR_VAR:
    printf("%s", expression->variable); break;
  }
}
void Function_print(Function* function) {
  printf("fn %s(", function->name);
  for (int i = 0; i < array_get_length(function->arguments); i++) {
    Variable_print(array_get(function->arguments, i));
  }
  printf(") {\n");
  for (int i = 0; i < array_get_length(function->expressions); i++) {
    Expression_print(array_get(function->expressions, i));
    printf("\n");
  }
  printf("}\n");
}