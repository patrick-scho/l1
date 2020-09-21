#include "program.h"

#include <vector>

Type Function::get_type(Context &context) {
  static int stackDepth = 0;
  if (stackDepth > 10)
    throw ERROR(definition->location, "Return Type Inference too deeply nested!");
  if (returnType.name.empty()) {
    stackDepth++;
    if (expressions.empty())
      throw ERROR(definition->location, "Return Type cannot be inferred");
    return expressions.back()->get_type(context);
  } else {
    stackDepth = 0;
    return returnType;
  }
}

Type Variable::get_type(Context &context) {
  if (type.name.empty()) {
    type = definition->get_type(context);
  }
  return type;
}


Type FunctionCall::get_type(Context &context) {
  if (function == nullptr) {
    vector<Type> argTypes;
    for (auto& e: arguments)
      argTypes.push_back(e->get_type(context));
    function = context.get_function(name, argTypes, true);
  }
  if (function == nullptr) {
    throw ERROR(location, "Calling undefined Function {}", name);
  }
  return function->get_type(context);
}
Type FunctionRef::get_type(Context &context) {
  return function->get_type(context);
}
Type MetaFunctionRef::get_type(Context &context) {
  return Type{"void"};
}
Type Assignment::get_type(Context &context) {
  return expression->get_type(context);
}
Type VariableRef::get_type(Context &context) { return variable->get_type(context); }
Type CCall::get_type(Context &context) { return Type{"void"}; }
Type Number::get_type(Context &context) { return Type{"i64"}; }
Type String::get_type(Context &context) { return Type{"string"}; }