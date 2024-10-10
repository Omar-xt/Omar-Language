#pragma once
#include <iostream>
#include <vector>
#include <map>

#include "variable_tracker.hpp"
#include "node_collection.hpp"
#include "_node_visitors.hpp"
#include "_transpiler.hpp"
#include "utils.hpp"
#include "print_utils.hpp"

void Error(std::string err)
{
    std::cout << err << std::endl;
}

struct Precedence
{
    std::map<std::string, int> table;

    Precedence()
    {
        table["endline"] = -11;
        // table["command"] = -11;
        table["Assignment"] = 1;
        table["Declare_var"] = 0;
        table["Add_OP"] = 2;
        table["Sub_OP"] = 2;
        table["Mul_OP"] = 4;
        table["Div_OP"] = 3;
    }

    int get_precedence(std::string &key)
    {
        return map_iterator(key, table);
    }
};

Node *node_tree_handler(std::vector<Node *> &nodes, VariableTracker &var_tracker)
{
    Precedence precedence;

    unsigned int current_node_index = 0;
    int higher_precedence_node_index = 0;
    int higher_node_precedence = 0;
    int current_node_presidence = -1;

    std::map<std::string, int> type_presidence;

    type_presidence["auto"] = 0;
    type_presidence["IntLiteral"] = 1;
    type_presidence["FloatLiteral"] = 2;

    std::string type_cheker;
    int current_type_presidence = 0;

    if (nodes.back()->type == "endline")
        nodes.pop_back();

    while (nodes.size() > 1)
    {
        higher_precedence_node_index = 0;
        higher_node_precedence = 0;

        for (unsigned int i = 0; i < nodes.size(); i++)
        {
            if (nodes[i]->visited)
                continue;

            current_node_presidence = precedence.get_precedence(nodes[i]->type);

            if (current_node_presidence < 0)
            {
                nodes[i]->visited = true;
                continue;
            }

            if (current_node_presidence > higher_node_precedence)
            {
                higher_node_precedence = current_node_presidence;
                higher_precedence_node_index = i;
            }
        }

        Node *node = nodes[higher_precedence_node_index];
        // print("get -> " + node->type);
        // print(std::to_string(nodes.size()));

        if (node->type == "Assignment")
        {
            if (nodes[higher_precedence_node_index - 1]->type == "Variable")
            {
                node->left = nodes[higher_precedence_node_index - 1];
                node->right = nodes[higher_precedence_node_index + 1];
                nodes.erase(nodes.begin() + (higher_precedence_node_index + 1));
                nodes.erase(nodes.begin() + (higher_precedence_node_index - 1));
                node->visited = true;

                if (type_cheker.size() == 0)
                {
                    if (node->right->type == "Variable")
                    {
                        type_cheker = var_tracker.get(node->right->var_name);

                        //--tracking
                        var_tracker.store(node->left->var_name, type_cheker);
                    }
                    else if (!node->right->binary_operator)
                    {
                        type_cheker = node->right->type;

                        //--tracking
                        var_tracker.store(node->left->var_name, type_cheker);
                    }
                }
            }
            else
            {
                Error("Invalid assignment expression");
                break;
            }
        }
        else if (node->binary_operator)
        {
            if (
                (nodes[higher_precedence_node_index - 1]->literal_type == true ||
                 nodes[higher_precedence_node_index - 1]->binary_operator == true ||
                 nodes[higher_precedence_node_index - 1]->type == "Variable") &&
                (nodes[higher_precedence_node_index + 1]->literal_type == true ||
                 nodes[higher_precedence_node_index + 1]->binary_operator == true ||
                 nodes[higher_precedence_node_index + 1]->type == "Variable"))
            {
                node->left = nodes[higher_precedence_node_index - 1];
                node->right = nodes[higher_precedence_node_index + 1];
                nodes.erase(nodes.begin() + higher_precedence_node_index + 1);
                nodes.erase(nodes.begin() + higher_precedence_node_index - 1);
                node->visited = true;

                                if ((node->left->type == node->right->type) && node->left->type == "StringLiteral")
                {
                    std::string new_str = node->left->value.substr(0, node->left->value.size() - 1);
                    new_str.append(" ");
                    new_str.append(node->right->value.substr(1, node->right->value.size() - 1));
                    Node *str_node = new StringLiteral(node->left->type, new_str);
                    nodes.erase(nodes.begin() + higher_precedence_node_index - 1);
                    nodes.insert(nodes.begin() + higher_precedence_node_index - 1, str_node);
                    continue;
                }

                if (!var_tracker.is_empty())
                {
                    if (node->left->type == "Variable" || node->right->type == "Variable")
                    {
                        print("finding");
                        print(var_tracker.get(node->right->var_name));
                        if (var_tracker.get(node->left->var_name) == "FloatLiteral")
                            type_cheker = "FloatLiteral";
                        else if (var_tracker.get(node->right->var_name) == "FloatLiteral")
                            type_cheker = "FloatLiteral";
                        else
                            type_cheker = "IntLiteral";
                    }
                }

                if (node->type == "Div_OP")
                    type_cheker = "FloatLiteral";

                if (node->left->type != "Variable" && type_cheker.size() == 0)
                {
                    if (node->left->type == node->right->type)
                        type_cheker = node->left->type;
                    else if (node->left->type == "FloatLiteral")
                        type_cheker = "FloatLiteral";
                    else if (node->right->type == "FloatLiteral")
                        type_cheker = "FloatLiteral";
                    else
                        type_cheker = "IntLiteral";
                }
            }
            else
            {
                if (node->type == "Add_OP")
                    Error("Can't add variable");
                if (node->type == "Sub_OP")
                    Error("Can't substract variable");
                break;
            }
        }
        else if (node->type == "Declare_var")
        {
            if (nodes[higher_precedence_node_index + 1]->type == "Assignment")
            {
                node->declare_type = "Initialize";
                node->var_type = type_cheker;
                node->operation = nodes[higher_precedence_node_index + 1];
                nodes.erase(nodes.begin() + higher_precedence_node_index + 1);
                node->visited = true;
            }
            else
            {
                Error("Cant assign operation");
                break;
            }
        }
    }

    print("assignment done");

    if (nodes.front()->type == "Declare_var")
        return nodes.front();

    Node *node = new Operation_node("Operations");
    node->operation = nodes.front();

    return node;
}

Node *handle_variable_declaration(std::vector<Node *> &nodes, VariableTracker &var_tracker)
{
    Precedence precedence;

    int visited_node_count = 0;
    unsigned int current_node_index = 0;
    int higher_precedence_node_index = 0;
    int higher_node_precedence = 0;
    int current_node_presidence = -1;

    std::map<std::string, int> type_presidence;

    type_presidence["auto"] = 0;
    type_presidence["IntLiteral"] = 1;
    type_presidence["FloatLiteral"] = 2;

    std::string type_cheker;
    int current_type_presidence = 0;

    while (true) // (visited_node_count != nodes.size() - 1)
    {
        higher_precedence_node_index = 0;
        higher_node_precedence = 0;
        for (unsigned int i = 0; i < nodes.size(); i++)
        {
            if (nodes[i]->visited)
            {
                visited_node_count += 1;
                continue;
            }

            current_node_presidence = precedence.get_precedence(nodes[i]->type);
            if (nodes[i]->binary_operator == true)
            {
                if (nodes[i]->type == "Div_OP")
                {
                    type_cheker = "FloatLiteral";
                    //-- menually setting higher precedence
                    current_type_presidence = 100;
                }
            }
            if (current_node_presidence < 0)
            {
                if (nodes[i]->literal_type)
                {
                    int p = type_presidence[nodes[i]->type];
                    if (p > current_type_presidence)
                    {
                        current_type_presidence = p;
                        type_cheker = nodes[i]->type;
                    }
                }

                nodes[i]->visited = true;
                visited_node_count += 1;
                continue;
            }
            if (current_node_presidence > higher_node_precedence)
            {
                higher_node_precedence = current_node_presidence;
                higher_precedence_node_index = i;
            }
        }

        if (visited_node_count == nodes.size() - 1)
        {
            break;
        }

        Node *node = nodes[higher_precedence_node_index];
        // print("get -> " + node->type);
        // print(std::to_string(higher_node_precedence));
        // print(std::to_string(current_node_presidence));

        if (node->type == "Assignment")
        {
            if (nodes[higher_precedence_node_index - 1]->type == "Variable")
            {
                node->left = nodes[higher_precedence_node_index - 1];
                node->right = nodes[higher_precedence_node_index + 1];
                nodes.erase(nodes.begin() + (higher_precedence_node_index + 1));
                nodes.erase(nodes.begin() + (higher_precedence_node_index - 1));
                node->visited = true;

                //--tracking
                var_tracker.store(node->left->var_name, node->right->type);
            }
            else
            {
                Error("Invalid assignment expression");
                break;
            }
        }
        if (node->binary_operator)
        {
            if (
                (nodes[higher_precedence_node_index - 1]->literal_type == true ||
                 nodes[higher_precedence_node_index - 1]->binary_operator == true ||
                 nodes[higher_precedence_node_index - 1]->type == "Variable") &&
                (nodes[higher_precedence_node_index + 1]->literal_type == true ||
                 nodes[higher_precedence_node_index + 1]->binary_operator == true ||
                 nodes[higher_precedence_node_index + 1]->type == "Variable"))
            {
                node->left = nodes[higher_precedence_node_index - 1];
                node->right = nodes[higher_precedence_node_index + 1];
                nodes.erase(nodes.begin() + higher_precedence_node_index + 1);
                nodes.erase(nodes.begin() + higher_precedence_node_index - 1);

                node->visited = true;

                if (!var_tracker.is_empty())
                {
                    if (node->left->type == "Variable" || node->right->type == "Variable")
                    {
                        if (var_tracker.get(node->left->var_name) == "FloatLiteral")
                            type_cheker = "FloatLiteral";
                        if (var_tracker.get(node->right->var_name) == "FloatLiteral")
                            type_cheker = "FloatLiteral";
                    }
                }
            }
            else
            {
                if (node->type == "Add_OP")
                    Error("Can't add variable");
                if (node->type == "Sub_OP")
                    Error("Can't substract variable");
                break;
            }
        }
        if (node->type == "endline")
        {
            node->visited = true;
            visited_node_count += 1;
            print("break due to endline in declare handeler");
            break;
        }
        if (node->type == "Declare_var")
        {
            if (nodes[higher_precedence_node_index + 1]->type == "Assignment")
            {
                node->declare_type = "Initialize";
                node->var_type = type_cheker;
                node->operation = nodes[higher_precedence_node_index + 1];
                nodes.erase(nodes.begin() + higher_precedence_node_index + 1);
                node->visited = true;
                // print("done...");
                break;
            }
            else
            {
                Error("Cant assign operation");
                break;
            }
        }
    }

    Node *declare_var = nodes[0];
    return declare_var;
    // declare_var_node_visitor(*declare_var);
}

Node *handle_builtin_function(std::vector<Node *> &nodes)
{
    Node *func;

    std::vector<Node *> code_block;

    unsigned int body_start_index = 0;

    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i]->type == "builtin_function")
        {
            func = nodes[i];
            nodes.erase(nodes.begin() + i);
        }

        if (nodes[i]->type == "start_parentheses")
        {
            body_start_index = i + 1;
            break;
        }
    }

    for (unsigned int i = body_start_index; i < nodes.size() - 1; i++)
    {
        if (nodes[i]->type == "newline")
            continue;
        else if (nodes[i]->binary_operator)
        {
            if (func->childrens.size() == 0)
            {
                print("operator error in btn function");
            }
            nodes[i]->left = func->childrens.back();
            nodes[i]->right = nodes[i + 1];
            func->childrens.pop_back();
            nodes.erase(nodes.begin() + i + 1);
        }
        func->childrens.push_back(nodes[i]);
    }

    print(std::to_string(func->childrens.size()));

    for (auto &child : func->childrens)
        print(child->type);

    return func;
}

void handle_function_declaration(std::vector<Node *> &nodes, VariableTracker &var_tracker)
{
    Node *func;

    std::vector<Node *> code_block;

    unsigned int body_start_index = 0;

    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i]->type == "Func")
        {
            func = nodes[i];
            nodes.erase(nodes.begin() + i);
        }
        if (nodes[i]->type == "Variable")
        {
            func->name = nodes[i]->var_name;
            nodes.erase(nodes.begin() + i);
        }
        if (nodes[i]->type == "start_curly")
        {
            body_start_index = i + 1;
            break;
        }
    }

    for (unsigned int i = body_start_index; i < nodes.size(); i++)
    {
        if (nodes[i]->type == "endline" && code_block.size() == 0)
        {
            continue;
        }

        code_block.push_back(nodes[i]);
        if (nodes[i]->type == "end_curly")
        {
            code_block.clear();
            break;
        }

        if (nodes[i]->type == "endline")
        {
            code_block.pop_back();
            Node *children;
            if (code_block[0]->type == "Declare_var")
                children = node_tree_handler(code_block, var_tracker);
            else if (code_block[0]->type == "builtin_function")
                children = handle_builtin_function(code_block);
            else if (code_block[0]->type == "NewLine")
                children = code_block[0];
            func->childrens.push_back(children);
            code_block.clear();
        }
    }

    // for (auto *child : func->childrens)
    //     std::cout << *child << std::endl;

    define_fnction_node_visitor(*func);
}
