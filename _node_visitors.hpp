#pragma once
#include <fstream>
#include <iostream>

#include "node_collection.hpp"
#include "_transpiler.hpp"

bool is_empty_file(std::fstream &file)
{
    file.seekp(0, std::ios::end);
    size_t size = file.tellg();
    return size == 0;
}

void import_lib_to_file(std::fstream &file)
{
    file << "#include <iostream>\n\n";
}

void prepare_file(std::fstream &file)
{
    if (is_empty_file(file))
        import_lib_to_file(file);
}

void new_line_node_visitor()
{
    std::fstream file;
    file.open("b.cpp", std::ios::app | std::ios::out);

    std::cout << "writing to file..." << std::endl;

    if (!file)
    {
        std::cout << "Error: Cannot open file " << std::endl;
        return;
    }

    file << "\n";
}

void binary_node_visitor(std::fstream &main_stream, Node *node, Transpiler &transpiler)
{
    if (node->left->binary_operator)
    {
        binary_node_visitor(main_stream, node->left, transpiler);
        // -- the bineary operator sign
        main_stream << transpiler.get_keyword(node->type);
    }
    else if (node->left->type == "Variable")
    {
        main_stream << node->left->var_name;
        // -- the bineary operator sign
        main_stream << transpiler.get_keyword(node->type);
    }
    else
    {
        main_stream << node->left->value;
        // -- the bineary operator sign
        main_stream << transpiler.get_keyword(node->type);
    }
    if (node->right->binary_operator)
    {
        binary_node_visitor(main_stream, node->right, transpiler);
    }
    else if (node->right->type == "Variable")
    {
        main_stream << node->right->var_name;
    }
    else
    {
        main_stream << node->right->value;
    }
}

void node_tree_visitor(Node &node)
{
    std::fstream file;
    file.open("b.cpp", std::ios::app | std::ios::out);

    std::cout << "writing to file..." << std::endl;

    if (!file)
    {
        std::cout << "Error: Cannot open file " << std::endl;
        return;
    }
    else
        prepare_file(file);

    std::string endCommand = ";\n";

    Transpiler transpiler;

    if (node.type == "Declare_var")
    {
        //-- type management --
        if (node.var_type.empty())
        {
            if (!node.operation->right->binary_operator)
                file << transpiler.get_keyword(node.operation->right->type);
        }
        else
            file << transpiler.get_keyword(node.var_type);

        print("dec done");
    }

    file << node.operation->left->var_name;
    file << transpiler.get_keyword(node.operation->type);
    if (node.operation->right->binary_operator)
    {
        // std::cout << node.operation->right->type << " : " << transpiler.get_keyword(node.operation->right->type) << std::endl;
        binary_node_visitor(file, node.operation->right, transpiler);
    }
    else if (node.operation->right->type == "Variable")
    {
        file << node.operation->right->var_name;
    }
    else
    {
        file << node.operation->right->value;
    }
    file << endCommand;
}

void declare_var_node_visitor(Node &declare_var)
{
    std::fstream file;
    file.open("b.cpp", std::ios::app | std::ios::out);

    std::cout << "writing to file..." << std::endl;

    if (!file)
    {
        std::cout << "Error: Cannot open file " << std::endl;
        return;
    }
    else
        prepare_file(file);

    std::string endCommand = ";\n";

    Transpiler transpiler;

    if (declare_var.declare_type == "Initialize")
    {
        // file << "auto ";
        if (declare_var.var_type.empty())
        {
            if (!declare_var.operation->right->binary_operator)
                file << transpiler.get_keyword(declare_var.operation->right->type);
        }
        else
            file << transpiler.get_keyword(declare_var.var_type);
        file << declare_var.operation->left->var_name;
        file << transpiler.get_keyword(declare_var.operation->type);
        if (declare_var.operation->right->binary_operator)
        {
            // std::cout << declare_var.operation->right->type << " : " << transpiler.get_keyword(declare_var.operation->right->type) << std::endl;
            binary_node_visitor(file, declare_var.operation->right, transpiler);
        }
        else if (declare_var.operation->right->type == "Variable")
        {
            file << declare_var.operation->right->var_name;
        }
        else
        {
            file << declare_var.operation->right->value;
        }
        file << endCommand;
    }
    else if (declare_var.declare_type == "Default")
    {
        file << declare_var.variable->var_name;
        file << endCommand;
    }
}

void btn_print_node_visitor(Node &func)
{
    std::fstream file;
    file.open("b.cpp", std::ios::app | std::ios::out);

    Transpiler transpiler;

    std::string endCommand = ";\n";
    std::string newLineCommand = " << \"\\n\"";

    std::cout << "writing to file..." << std::endl;

    if (!file)
    {
        std::cout << "Error: Cannot open file " << std::endl;
        return;
    }

    for (auto *child : func.childrens)
    {
        if (child->type == "Variable")
            file << "std::cout << " << child->var_name << newLineCommand << endCommand;
        else if (child->binary_operator)
        {
            file << "std::cout << ";
            binary_node_visitor(file, child, transpiler);
            file << newLineCommand << endCommand;
        }
        else
            file << "std::cout << " << child->value << newLineCommand << endCommand;
    }
}

void builtin_function_node_visitor(Node &func)
{
    if (func.name == "print")
        btn_print_node_visitor(func);
}

void define_fnction_node_visitor(Node &func)
{
    std::fstream file;
    file.open("b.cpp", std::ios::app | std::ios::out);

    std::cout << "writing to file..." << std::endl;

    if (!file)
    {
        std::cout << "Error: Cannot open file " << std::endl;
        return;
    }
    else
        prepare_file(file);

    std::string endCommand = ";\n";

    file << "int ";
    file << func.name;
    file << "()\n";
    file << "{\n";
    file.close();
    for (auto children : func.childrens)
    {
        if (children->type == "Declare_var")
            node_tree_visitor(*children);
        else if (children->type == "builtin_function")
            builtin_function_node_visitor(*children);
        else if (children->type == "NewLine")
            new_line_node_visitor();
    }
    file.open("b.cpp", std::ios::app | std::ios::out);
    file << "}";
}