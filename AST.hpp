#pragma once
#include <iostream>
#include <fstream>
#include <vector>

#include "tokenizer.hpp"
#include "utils.hpp"
#include "print_utils.hpp"
#include "node_collection.hpp"
#include "_node_visitors.hpp"
#include "handler.hpp"
#include "_transpiler.hpp"
#include "variable_tracker.hpp"

Node *get_node(Token const &token)
{
    if (token.type == "IntLiteral")
        return new IntLiteral(token.type, token.value);
    else if (token.type == "FloatLiteral")
        return new FloatLiteral(token.type, token.value);
    else if (token.type == "StringLiteral")
        return new StringLiteral(token.type, token.value);
    else if (token.type == "Assignment")
        return new Assignment(token.type, token.value);
    else if (token.type == "Declare_var")
        return new DeclareVar(token.type);
    else if (token.type == "unknown")
        return new Variable("Variable", token.value);
    else if (token.type == "Variable")
        return new Variable(token.type, token.value);
    else if (token.type == "Add_OP")
        return new Bineary_Operator(token.type);
    else if (token.type == "Sub_OP")
        return new Bineary_Operator(token.type);
    else if (token.type == "Mul_OP")
        return new Bineary_Operator(token.type);
    else if (token.type == "Div_OP")
        return new Bineary_Operator(token.type);
    else if (token.type == "Func")
        return new Func_node(token.type);
    else if (token.type == "builtin_function")
        return new Func_node(token.type, token.value);
    else if (token.type == "start_curly")
        return new Seperator(token.type, token.value);
    else if (token.type == "end_curly")
        return new Seperator(token.type, token.value);
    else if (token.type == "start_parentheses")
        return new Seperator(token.type, token.value);
    else if (token.type == "end_parentheses")
        return new Seperator(token.type, token.value);
    else if (token.type == "command")
        return new Command(token.value);
    else if (token.type == "NewLine")
        return new Command(token.type);

    return new _NULL();
}

class AST
{
public:
    unsigned int current_ind = 0;
    std::vector<Token> code_block_of_line;

    bool pursing_function = false;
    unsigned int seperator_count = 0;

    std::vector<Node *> nodes;

    Transpiler transpiler;
    VariableTracker var_tracker;

public:
    void make_ast(std::vector<Token> &token_list)
    {
        for (unsigned int i = 0; i < token_list.size(); i++)
        {
            code_block_of_line.push_back(token_list[i]);
            if (token_list[i].value == "endline")
            {
                if (code_block_of_line.size() == 0)
                    return;
                make_node_tree(code_block_of_line);
                code_block_of_line.clear();
                continue;
            }
        }
    }

    void make_node_tree(std::vector<Token> &tokens)
    {
        // print(tokens);

        for (Token const &token : tokens)
        {
            Node *node = get_node(token);
            nodes.push_back(node);
        }

        // for (int i = 0; i < nodes.size(); i++)
        //     print(nodes[i]->type);

        if (pursing_function)
        {
            for (Node *node : nodes)
            {
                if (node->type == "end_curly")
                {
                    pursing_function = false;
                    seperator_count = 0;
                }
            }
            if (pursing_function)
                return;
            handle_function_declaration(nodes, var_tracker);
            nodes.clear();
            return;
        }

        if (nodes.front()->type == "Declare_var")
        {
            // Node *declare_var = handle_variable_declaration(nodes, var_tracker);
            // declare_var_node_visitor(*declare_var);
            // nodes.clear();

            Node *node = node_tree_handler(nodes, var_tracker);
            print("calling declare_var");
            node_tree_visitor(*node);
            nodes.clear();
        }
        else if (nodes.front()->type == "NewLine")
        {
            new_line_node_visitor();
        }
        else if (nodes.front()->type == "Func")
        {
            pursing_function = true;
            seperator_count += 1;
            for (Node *node : nodes)
            {
                if (node->type == "end_curly")
                {
                    pursing_function = false;
                    seperator_count = 0;
                }
            }
            return;
        }

        if (nodes.size())
            nodes.clear();
    }
};
