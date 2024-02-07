from abc import ABC, abstractmethod
import ast

#Step 1: Define the expression class and it's child classes
class Expression(ABC):
    def accept(self, visitor):
        pass

    loaders = {}

    @staticmethod
    def register_loader(key, loader):
        Expression.loaders[key] = loader

    @staticmethod
    def load(s):
        key = s.read(1)
        loader = Expression.loaders.get(key)
        if loader is None:
            raise ValueError(f"Found unexpected token {key}")
        return loader(s)

class Constant(Expression):
    def __init__(self, x):
        self.x = x
        Expression.register_loader("Constant", Constant.load)

    def accept(self, visitor):
        visitor.visit_constant(self)

    @staticmethod
    def load(s):
        x = float(s.read().split()[0])
        return Constant(x)

class Variable(Expression):
    def __init__(self, name):
        self.name = name
        Expression.register_loader("Variable", Variable.load)

    def accept(self, visitor):
        visitor.visit_variable(self)

    @staticmethod
    def load(s):
        name = s.read().split()[0]
        return Variable(name)

class Parentheses(Expression):
    def __init__(self, expr):
        self.expr = expr
        Expression.register_loader("Parentheses", Parentheses.load)

    def accept(self, visitor):
        visitor.visit_parentheses(self)

    @staticmethod
    def load(s):
        expr = Expression.load(s)
        return Parentheses(expr)

class Op(Expression):
    def __init__(self, op, l, r):
        self.op = op
        self.l = l
        self.r = r
        Expression.register_loader("Op", Op.load)

    def accept(self, visitor):
        visitor.visit_op(self)

    @staticmethod
    def load(s):
        op = s.read(1)
        l = Expression.load(s)
        r = Expression.load(s)
        return Op(op, l, r)

class Visitor(ABC):
    @abstractmethod
    def visit_constant(self, constant:Constant) -> None:
        pass

    @abstractmethod
    def visit_op(self, op:Op) -> None:
        pass
        
    @abstractmethod
    def visit_variable(self, variable:Variable) -> None:
        pass
    
    @abstractmethod
    def visit_parentheses(self, parentheses:Parentheses) -> None:
        pass
class ComputeVisitor(Visitor):

    def __init__(self) -> None:
        self.variables = variables or {}
        self.result_ = 0

    def visit_constant(self, constant):
        self.result_ = constant.x

    def visit_op(self, op):
        op.l.accept(self)
        l = self.result_

        op.r.accept(self)
        r = self.result_

        operators = {
            '^': lambda x, y: x ** y,
            '+': lambda x, y: x + y,
            '-': lambda x, y: x - y,
            '*': lambda x, y: x * y,
            '/': lambda x, y: x // y,
        }

        if op.op in operators:
            self.result_ = operators[op.op](l, r)

        else:
            print("Invalid Operand", op.op)

    def visit_variable(self, variable):
        if variable.name in self.variables:
            self.result_ = self.variables[variable.name]
        else:
            print(f"Variable {variable.name} not defined.")
            self.result_ = 0

    def visit_parentheses(self, parentheses):
        parentheses.expr.accept(self)

class PrettyPrintVisitor(Visitor):
    def __init__(self) -> None:
        self.result_ = ""

    def visit_constant(self, constant):
        self.result_ = "(" + str(constant.x) + ")"

    def visit_op(self, op):
        result = []
        result.append("(")
        op.l.accept(self)
        result.append(self.result_)
        result.append(op.op)

        op.r.accept(self)
        result.append(self.result_)

        result.append(")")
        self.result_ = "".join(result)


# Step 3: Implement the parser
def parse(t: str) -> Expression:
    for i in range(len(t) - 1, -1, -1):
        if t[i] in ['+', '-']:
            return Op(t[i], parse(t[:i]), parse(t[i + 1:]))
            
    for i in range(len(t) - 1, -1, -1):
        if t[i] in ['*', '/']:
            return Op(t[i], parse(t[:i]), parse(t[i + 1:]))
            
    for i in range(len(t) - 1, -1, -1):
        if t[i] == '^':
            return Op(t[i], parse(t[:i]), parse(t[i + 1:]))
    
    return Constant(int(t))


# Step 0: Check if input string is a valid expression
def is_valid_expression(input_string):
    try:
        # Use ast.parse to parse the input string
        if not input_string or input_string == " ":
            return False
        ast.parse(input_string)
        return True
    except SyntaxError:
        return False
    
def main(s):
    # test expression
    # s = "16/4+4*3-3+1"
    is_valid = is_valid_expression(s)

    # if not true end execution
    if not is_valid:
        return "Invalid Input"

    result = {}
    # else continue execution 
    e = parse(s)
    cmp_v = ComputeVisitor()
    e.accept(cmp_v)
    print("Result =",cmp_v.result_)
    result['result'] = cmp_v.result_

    pp_v = PrettyPrintVisitor()
    e.accept(pp_v)
    print("PrettyPrint ->", pp_v.result_)
    result['pretty_print'] = pp_v.result_

    return result


if __name__ == "__main__":
    main()

