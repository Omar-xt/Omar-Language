#include <iostream>
#include <fstream>
#include <vector>

#include "my_lexer.hpp"
#include "print_utils.hpp"
#include "AST.hpp"

#include "compiler.hpp"

using namespace std;

int main()
{
    Lexer lexer;
    vector<string> list;
    vector<Token> token_list;

    string filename = "b.omer";

    string line;
    fstream file;
    file.open("b.cpp", ios::trunc | ios::out);
    file.close();
    file.open(filename);

    while (getline(file, line))
    {
        if (line == "")
        {
            list.push_back("\n");
            list.push_back("endline");
            continue;
        }
        split(line, ' ', list);
    }

    // for (int i = 0; i < list.size(); i++)
    //     print(list[i]);

    lexer.lex_token(list, token_list);

    print(token_list);

    AST ast;

    ast.make_ast(token_list);

    // compile_and_run(filename);
}