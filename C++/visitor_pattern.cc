#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <cmath>
#include <functional>
#include <iomanip>
#include <typeinfo>

class Visitor;

// Step 1: Define the expression class and its child classes
class Expression {
public:
    virtual ~Expression() = default;
    virtual void accept(Visitor& visitor) = 0;
};


// Step 2: Define the visitor classes
class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit_constant(class Constant* constant) = 0;
    virtual void visit_op(class Op* op) = 0;
    virtual void visit_variable(class Variable* variable) = 0;
    virtual void visit_parentheses(class Parentheses* parentheses) = 0;
};

class Constant : public Expression {
public:
    explicit Constant(double x) : x(x) {}
    void accept(Visitor& visitor) override {
        visitor.visit_constant(this);
    }

    double x;
};

class Variable : public Expression {
public:
    explicit Variable(const std::string& name) : name(name) {}
    void accept(Visitor& visitor) override {
        visitor.visit_variable(this);
    }

    std::string name;
};

class Parentheses : public Expression {
public:
    explicit Parentheses(Expression* expr) : expr(expr) {}
    ~Parentheses() override {
        delete expr;
    }
    void accept(Visitor& visitor) override {
        visitor.visit_parentheses(this);
    }

    Expression* expr;
};

class Op : public Expression {
public:
    Op(char op, Expression* l, Expression* r) : op(op), l(l), r(r) {}
    ~Op() override {
        delete l;
        delete r;
    }
    void accept(Visitor& visitor) override {
        visitor.visit_op(this);
    }

    char op;
    Expression* l;
    Expression* r;
};

// Step 3: Implement the parser
Expression* parse(std::istringstream& iss);

Expression* parse(const std::string& t) {
    std::istringstream iss(t);
    return parse(iss);
}

Expression* parse(std::istringstream& iss) {
    int c = 0;
    std::string t;
    iss >> t;

    for (int i = t.length() - 1; i >= 0; --i) {
        if (t[i] == ')') {
            ++c;
        } else if (t[i] == '(') {
            --c;
        } else if (c == 0 && (t[i] == '+' || t[i] == '-')) {
            return new Op(t[i], parse(t.substr(0, i)), parse(t.substr(i + 1)));
        }
    }

    for (int i = t.length() - 1; i >= 0; --i) {
        if (t[i] == ')') {
            ++c;
        } else if (t[i] == '(') {
            --c;
        } else if (c == 0 && (t[i] == '*' || t[i] == '/')) {
            return new Op(t[i], parse(t.substr(0, i)), parse(t.substr(i + 1)));
        }
    }

    for (int i = t.length() - 1; i >= 0; --i) {
        if (t[i] == ')') {
            ++c;
        } else if (t[i] == '(') {
            --c;
        } else if (c == 0 && t[i] == '^') {
            return new Op(t[i], parse(t.substr(0, i)), parse(t.substr(i + 1)));
        }
    }

    if (t[0] == '(' && t.back() == ')') {
        return parse(t.substr(1, t.length() - 2));
    }

    return new Constant(std::stod(t));
}


// Step 0: Check if input string is a valid expression
bool is_valid_expression(const std::string& input_string) {
    if (input_string.empty() || input_string == " ") {
        return false;
    }
    try {
        std::istringstream iss(input_string);
        while (iss) {
            std::string token;
            iss >> token;
            if (!token.empty()) {
                // Convert to double and check for syntax errors
                std::stod(token);
            }
        }
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

// Step 4: Define ComputeVisitor and PrettyPrintVisitor
class ComputeVisitor : public Visitor {
public:
    ComputeVisitor(std::unordered_map<std::string, double>& variables) : variables(variables), result_(0) {}
    void visit_constant(Constant* constant) override {
        result_ = constant->x;
    }
    void visit_op(Op* op) override {
        op->l->accept(*this);
        double l = result_;

        op->r->accept(*this);
        double r = result_;

        static const std::unordered_map<char, std::function<double(double, double)>> operators = {
            {'^', [](double x, double y) { return std::pow(x, y); }},
            {'+', [](double x, double y) { return x + y; }},
            {'-', [](double x, double y) { return x - y; }},
            {'*', [](double x, double y) { return x * y; }},
            {'/', [](double x, double y) { return x / y; }},
        };

        auto it = operators.find(op->op);
        if (it != operators.end()) {
            result_ = it->second(l, r);
        } else {
            std::cout << "Invalid Operand " << op->op << std::endl;
        }
    }
    void visit_variable(Variable* variable) override {
        if (variables.find(variable->name) != variables.end()) {
            result_ = variables[variable->name];
        } else {
            std::cout << "Variable " << variable->name << " not defined." << std::endl;
            result_ = 0;
        }
    }
    void visit_parentheses(Parentheses* parentheses) override {
        parentheses->expr->accept(*this);
    }
    double result_;
private:
    std::unordered_map<std::string, double>& variables;
};

class PrettyPrintVisitor : public Visitor {
public:
    void visit_constant(Constant* constant) override {
        std::string numStr = std::to_string(constant->x);
        numStr = numStr.substr(0, numStr.find(".") + 3);
        result_ = "(" + numStr + ")";
    }
    void visit_op(Op* op) override {
        std::string temp;
        op->l->accept(*this);
        temp += result_;
        temp += op->op;
        op->r->accept(*this);
        result_ = "(" + temp + result_ + ")";
    }
    void visit_variable(Variable* variable) override {
        result_ = variable->name;
    }
    void visit_parentheses(Parentheses* parentheses) override {
        parentheses->expr->accept(*this);
    }
    std::string result_;
};

// Main function
int main() {
    std::string s = "3+5/2";
    std::unordered_map<std::string, double> variables;
    if (!is_valid_expression(s)) {
        std::cout << "Invalid Input" << std::endl;
        return 1;
    }
    Expression* e = parse(s);
    ComputeVisitor cmp_v(variables);
    e->accept(cmp_v);
    std::cout << "Result = " << cmp_v.result_ << std::endl;
    delete e;

    PrettyPrintVisitor pp_v;
    e = parse(s);
    e->accept(pp_v);
    std::cout << "PrettyPrint -> " << pp_v.result_ << std::endl;
    delete e;

    return 0;
}
