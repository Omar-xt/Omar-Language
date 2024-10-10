#pragma once
#include <string>
#include <ostream>

struct Node
{
    std::string type = "Node";
    std::string value;
    std::string var_name;
    std::string var_type;
    std::string declare_type;

    std::string name;

    std::vector<Node *> childrens;

    bool visited = false;
    bool literal_type = false;

    bool seperator = false;
    bool binary_operator = false;

    Node *variable = nullptr;
    Node *operation = nullptr;
    Node *result = nullptr;
    Node *left = nullptr;
    Node *right = nullptr;

    std::string endCommand = ";";

    friend std::ostream &operator<<(std::ostream &out, Node const &node)
    {
        out << node.type << " -> ";
        if (node.value.size() > 0)
            out << node.value << '\n';
        if (node.var_type.size() > 0)
            out << node.var_type << '\n';
        if (node.var_name.size() > 0)
            out << node.var_name;

        return out;
    }
};

struct DeclareVar : public Node
{
    DeclareVar() {}
    DeclareVar(std::string Type)
    {
        type = Type;
    }
};

struct Variable : public Node
{
    Variable() {}
    Variable(std::string Type, std::string Name)
    {
        type = Type;
        var_name = Name;
    }
};

struct Assignment : public Node
{
    Assignment() {}
    Assignment(std::string Type, std::string Value)
    {
        type = Type;
        value = Value;
    }

    Assignment(std::string Type, Node *Left)
    {
        type = Type;
        left = Left;
    }
};

struct Bineary_Operator : public Node
{
    Bineary_Operator(std::string Type)
    {
        type = Type;
        binary_operator = true;
    }
};

struct IntLiteral : public Node
{
    IntLiteral(std::string Type, std::string Value)
    {
        type = Type;
        value = Value;
        literal_type = true;
    }
};

struct FloatLiteral : public Node
{
    FloatLiteral(std::string Type, std::string Value)
    {
        type = Type;
        value = Value;
        literal_type = true;
    }
};

struct StringLiteral : public Node
{
    StringLiteral(std::string Type, std::string Value)
    {
        type = Type;
        value = Value;
        literal_type = true;
    }
};

struct Command : public Node
{
    Command(std::string Type)
    {
        type = Type;
    }
};

struct Seperator : public Node
{
    Seperator(std::string Type, std::string Value)
    {
        type = Type;
        value = Value;
        seperator = true;
    }
};

struct Func_node : public Node
{
    Func_node(std::string Type)
    {
        type = Type;
    }
    Func_node(std::string Type, std::string Name)
    {
        type = Type;
        name = Name;
    }
};

struct Operation_node : public Node
{
    Operation_node(std::string Type)
    {
        type = Type;
    }
};

struct Name_node : public Node
{
    Name_node(std::string Name)
    {
        name = Name;
    }
};

struct _NULL : public Node
{
    _NULL()
    {
        type = "NULL";
    }
};