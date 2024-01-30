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

class Op(Expression):
    def __init__(self, op, l, r):
        self.op = op
        self.l = l
        self.r = r
        Expression.register_loader("Op", Op.load)

    def accept(self, visitor):
        visitor.visit(self)

    @staticmethod
    def load(s):
        op = s.read(1)
        l = Expression.load(s)
        r = Expression.load(s)
        return Op(op, l, r)

# Step 2: Define the Visitor interface
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

# Step 2: Define the Visitor interface
class Visitor(ABC):
    @abstractmethod
    def visit_constant(self, constant:Constant) -> None:
        pass

    @abstractmethod
    def visit_op(self, op:Op) -> None:
        pass
    
class ComputeVisitor(Visitor):

    def __init__(self) -> None:
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
        ast.parse(input_string)
        return True
    except SyntaxError:
        return False



# test expression
s = "16/4+4*3-3+1"
result = is_valid_expression(s)

# if not true end execution
if not result:
    exit()

# else continue execution 
e = parse(s)
cmp_v = ComputeVisitor()
e.accept(cmp_v)
print("Result =",cmp_v.result_)

pp_v = PrettyPrintVisitor()
e.accept(pp_v)
print("PrettyPrint ->", pp_v.result_)