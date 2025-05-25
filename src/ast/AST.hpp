#pragma once
#include "ASTVisitor.hpp"
#include <vector>
#include <optional>
#include <string>
#include <memory>

// Clase base para todos los nodos del AST
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
    virtual int line() const = 0;
    virtual std::string type() const = 0;
};

// ------------------------------------------------------
// Nodos específicos del AST
// ------------------------------------------------------

class BinaryOpNode : public ASTNode {
public:
    std::string op;
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;
    int _line;
    std::string _type;

    BinaryOpNode(std::string op, std::shared_ptr<ASTNode> l, std::shared_ptr<ASTNode> r, int ln)
        : op(op), left(l), right(r), _line(ln), _type("") {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    int line() const override { return _line; }
    std::string type() const override { return _type; }
};

class UnaryOpNode : public ASTNode {
    public:
        std::string op;
        std::shared_ptr<ASTNode> operand;
        int _line;
        std::string _type;
    
        UnaryOpNode(const std::string& op, std::shared_ptr<ASTNode> operand, int line)
            : op(op), operand(operand), _line(line), _type("") {}
    
        void accept(ASTVisitor& visitor) override {
            visitor.visit(*this);
        }
    
        int line() const override { return _line; }
        std::string type() const override { return _type; }
    };

class BuiltInFunctionNode : public ASTNode {
public:
    std::string name;
    std::vector<std::shared_ptr<ASTNode>> args;
    int _line;
    std::string _type;

    BuiltInFunctionNode(const std::string& name, const std::vector<std::shared_ptr<ASTNode>>& args, int line)
        : name(name), args(args), _line(line), _type("") {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    int line() const override { return _line; }
    std::string type() const override { return _type; }
};

// ------------------------------------------------------

class FunctionCallNode : public ASTNode {
public:
    std::string funcName;
    std::vector<std::shared_ptr<ASTNode>> args;
    int _line;
    std::string _type;

    FunctionCallNode(std::string name, std::vector<std::shared_ptr<ASTNode>> args, int ln)
        : funcName(name), args(args), _line(ln), _type("") {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    int line() const override { return _line; }
    std::string type() const override { return _type; }
};

// ------------------------------------------------------

class LiteralNode : public ASTNode {
public:
    std::string value;
    std::string _type;
    int _line;

    LiteralNode(std::string val, std::string type, int ln)
        : value(val), _type(type), _line(ln) {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    int line() const override { return _line; }
    std::string type() const override { return _type; }
};

// ------------------------------------------------------

class BlockNode : public ASTNode {
public:
    std::vector<std::shared_ptr<ASTNode>> expressions;
    int _line;
    std::string _type;

    BlockNode(std::vector<std::shared_ptr<ASTNode>> exprs, int ln)
        : expressions(exprs), _line(ln), _type("") {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    int line() const override { return _line; }
    std::string type() const override { return _type; }
};

// ------------------------------------------------------

class VariableDeclarationNode : public ASTNode {
public:
    std::string name;
    std::string declaredType;
    std::shared_ptr<ASTNode> initializer;
    bool isMutable;
    int _line;
    std::string _type;

    VariableDeclarationNode(std::string name, std::string type, std::shared_ptr<ASTNode> init, bool isMut, int ln)
        : name(name), declaredType(type), initializer(init), isMutable(isMut), _line(ln), _type("") {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    int line() const override { return _line; }
    std::string type() const override { return _type; }
};

// ------------------------------------------------------

class IdentifierNode : public ASTNode {
public:
    std::string name;
    int _line;
    std::string _type;

    IdentifierNode(std::string name, int ln)
        : name(name), _line(ln), _type("") {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    int line() const override { return _line; }
    std::string type() const override { return _type; }
};

struct Parameter {
    std::string name;
    std::string type;
};

// Nodo para declaración de funciones
class FunctionDeclarationNode : public ASTNode {
public:
    std::string name;
    std::string returnType = ""; 
    std::shared_ptr<std::vector<Parameter>> params;
    std::shared_ptr<ASTNode> body;
    bool isInline;
    int _line;
    std::string _type;

    FunctionDeclarationNode(std::string name, 
                            std::shared_ptr<std::vector<Parameter>> params, std::shared_ptr<ASTNode> body, 
                            bool isInline, int ln)
        : name(name), params(params), 
          body(body), isInline(isInline), _line(ln), _type("") {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this); // Llama a visit(FunctionDeclarationNode&)
    }

    int line() const override { return _line; }
    std::string type() const override { return _type; }
};

struct LetDeclaration {
    std::string name;
    std::string declaredType; // Puede estar vacío
    std::shared_ptr<ASTNode> initializer;
};

class LetNode : public ASTNode {
public:
    std::shared_ptr<std::vector<LetDeclaration>> declarations;
    std::shared_ptr<ASTNode> body;
    int _line;
    std::string _type;

    LetNode(std::shared_ptr<std::vector<LetDeclaration>> decls, std::shared_ptr<ASTNode> body, int ln)
        : declarations(decls), body(body), _line(ln), _type("") {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    int line() const override { return _line; }
    std::string type() const override { return _type; }
};

class AssignmentNode : public ASTNode {
public:
    std::string name;
    std::shared_ptr<ASTNode> rhs;
    int _line;
    std::string _type;

    AssignmentNode(std::string name, std::shared_ptr<ASTNode> rhs, int ln)
        : name(name), rhs(rhs), _line(ln), _type("") {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    int line() const override { return _line; }
    std::string type() const override { return _type; }
};

struct IfBranch {
    std::shared_ptr<ASTNode> condition;
    std::shared_ptr<ASTNode> body;
};

class IfNode : public ASTNode {
public:
    std::shared_ptr<std::vector<IfBranch>> branches; // if + elifs
    std::shared_ptr<ASTNode> elseBody;
    int _line;
    std::string _type;

    IfNode(std::shared_ptr<std::vector<IfBranch>> branches, std::shared_ptr<ASTNode> elseBody, int ln)
        : branches(branches), elseBody(elseBody), _line(ln), _type("") {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    int line() const override { return _line; }
    std::string type() const override { return _type; }
};

class WhileNode : public ASTNode {
    public:
        std::shared_ptr<ASTNode> condition;
        std::shared_ptr<ASTNode> body;
        int _line;
        std::string _type;
    
        WhileNode(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> b, int ln)
            : condition(cond), body(b), _line(ln), _type("") {}
    
        void accept(ASTVisitor& visitor) override {
            visitor.visit(*this);
        }
    
        int line() const override { return _line; }
        std::string type() const override { return _type; }
    };
    
class ForNode : public ASTNode {
public:
    std::string varName;
    std::shared_ptr<ASTNode> iterable;
    std::shared_ptr<ASTNode> init_range;
    std::shared_ptr<ASTNode> end_range;
    std::shared_ptr<ASTNode> body;
    int _line;
    std::string _type;

    ForNode(const std::string& var, std::shared_ptr<ASTNode> init, std::shared_ptr<ASTNode> end, std::shared_ptr<ASTNode> b, int ln)
        : varName(var), init_range(init), end_range(end), body(b), _line(ln), _type("") {}

    void accept(ASTVisitor& visitor) override {
        visitor.visit(*this);
    }

    int line() const override { return _line; }
    std::string type() const override { return _type; }
};

class TypeDeclarationNode : public ASTNode {
public:
    std::string name;
    std::shared_ptr<std::vector<Parameter>> constructorParams;
    std::shared_ptr<std::vector<AttributeDeclaration>> attributes;
    std::shared_ptr<std::vector<MethodDeclaration>> methods;
    std::optional<std::string> baseType;            // Si hay herencia
    std::vector<std::shared_ptr<ASTNode>> baseArgs;                 // Argumentos para el padre
    int _line;

    TypeDeclarationNode(std::string name,
                        std::shared_ptr<std::vector<Parameter>> params,
                        std::shared_ptr<std::vector<AttributeDeclaration>> attrs,
                        std::shared_ptr<std::vector<MethodDeclaration>> methods,
                        std::optional<std::string> baseType,
                        std::vector<std::shared_ptr<ASTNode>> baseArgs,
                        int line)
        : name(std::move(name)), constructorParams(params),
          attributes(attrs), methods(methods),
          baseType(std::move(baseType)), baseArgs(std::move(baseArgs)),
          _line(line) {}

    void accept(ASTVisitor& v) override { v.visit(*this); }
    int line() const override { return _line; }
    std::string type() const override { return "Type"; }
};

class NewInstanceNode : public ASTNode {
public:
    std::string typeName;
    std::vector<std::shared_ptr<ASTNode>> args;
    int _line;
    std::string _type;

    NewInstanceNode(std::string typeName, std::vector<std::shared_ptr<ASTNode>> args, int line)
        : typeName(std::move(typeName)), args(std::move(args)), _line(line), _type("") {}

    void accept(ASTVisitor& v) override { v.visit(*this); }
    int line() const override { return _line; }
    std::string type() const override { return _type; }
};

struct AttributeDeclaration {
    std::string name;
    std::shared_ptr<ASTNode> initializer;

    AttributeDeclaration(std::string name, std::shared_ptr<ASTNode> init)
        : name(std::move(name)), initializer(init) {}
};

struct MethodDeclaration {
    std::string name;
    std::shared_ptr<std::vector<Parameter>> params;
    std::shared_ptr<ASTNode> body;
    std::string returnType;  

    MethodDeclaration(std::string name, std::shared_ptr<std::vector<Parameter>> params, std::shared_ptr<ASTNode> body, std::string ret = "")
        : name(std::move(name)), params(params), body(body), returnType(std::move(ret)) {}
};

class MethodCallNode : public ASTNode {
public:
    std::shared_ptr<ASTNode> object;
    std::string methodName;
    std::vector<std::shared_ptr<ASTNode>> args;
    int _line;
    std::string _type;

    MethodCallNode(std::shared_ptr<ASTNode> obj, std::string methodName, std::vector<std::shared_ptr<ASTNode>> args, int line)
        : object(obj), methodName(std::move(methodName)), args(std::move(args)), _line(line), _type("") {}

    void accept(ASTVisitor& v) override { v.visit(*this); }
    int line() const override { return _line; }
    std::string type() const override { return _type; }
};

