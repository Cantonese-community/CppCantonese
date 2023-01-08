#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <ctype.h>
#include <clocale>
#include "ast.hpp"

using namespace cantonese;
using namespace std;


void args_parse(std::string str);
void _help();
bool readFile(const char* filename, wstring &content);

enum msg
{
    help,
    version,
    default_
};

bool readFile(const char* filename, wstring& content) {
    wchar_t linex[4096];

    FILE* file;
    fopen_s(&file, filename, "rt+,ccs=UTF-8");
    if (file == NULL) {
        puts("打唔开你份文件!");
        return false;
    }
    while(fgetws(linex, 4096, file))
    {
        content += wstring(linex);
    }
    fclose(file);
    return true;
}

void args_parse(std::string str) {
    msg temp = default_;
    if (str == "-h" || str == "-H" || str == "-help") temp = help;
    else if (str == "-v" || str == "-V" || str == "-version") temp = version;
    switch (temp)
    {
    case help:
        _help();
        break;
    case version:
        std::cout<<"Cantonese (Core) version 1.0.0"<<std::endl;
        break;
    default:
        const char* filename = str.c_str();
        std::wstring code;
        bool result = readFile(filename, code);
        if(result == false) break; 
        auto *source = const_cast<CAN_Char *>(code.c_str());
        vector<Token> TokenList;
        cantonese::Lexer lexer(source);
        Token tk = lexer.Read();
        TokenList.push_back(tk);
        while (tk.mType != TokenType::End) {
            tk = lexer.Read();
            TokenList.push_back(tk);
        }

        Ast::Parser parser(TokenList);
        parser.parse();
        break;
    }
}

void _help() {
    std::string info = "usage:\n -h   : use help \n -v   : print the version\n -e   : exec a statement ";
    std::cout<<info<<std::endl;
}

static void repl() {
    char line[1024];
    for (;;) {
        wprintf(L">");
        if (!fgets(line, sizeof(line), stdin)) {
            wprintf(L"\n");
            break;
        }
    }
}


int main(int argc, char* argv[]) {
    /* Only for mingw */
    /* TODO: test under VS */
    std::setlocale(LC_ALL, ".936");
    std::ios_base::sync_with_stdio(false);

    if (argc == 0) {
        repl();
    }

    for (int i = 0; i < argc; i++) {
        if (i != 0) {args_parse(argv[i]);}
    }

    return 0;
}