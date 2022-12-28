#ifndef _AST_H_
#define _AST_H_

#include <memory>
#include <vector>
#include <string>
#include <map>

#include "cantonese.hpp"
#include "lexer.hpp"

namespace Ast {
    // Base class for all expression nodes.
    class ExprAST {
        public:
            virtual ~ExprAST() = default;
    };

    class NumberExprAST : public ExprAST {
        double Val;

        public:
            NumberExprAST(double Val) : Val(Val) {}
    };

    class StringExprAST : public ExprAST {
        std::wstring Val;

        public:
            StringExprAST(std::wstring Val) : Val(Val) {}
    };

    class VariableExprAST : public ExprAST {
        std::string Name;

        public:
            VariableExprAST(const std::string& Name) : Name(Name) {}
    };

    class BinaryExprAST : public ExprAST {
        char Op;
        std::unique_ptr<ExprAST> LHS;
        std::unique_ptr<ExprAST> RHS;
        public:
            BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS) :
            Op(Op),
            LHS(std::move(LHS)),
            RHS(std::move(RHS))    
            {}
    };

    class CallExprAST : public ExprAST {
        std::string Callee;
        std::vector<std::unique_ptr<ExprAST>> Args;
        public:
            CallExprAST(const std::string& Callee, std::vector<std::unique_ptr<ExprAST>> Args) :
            Callee(Callee), Args(std::move(Args)) {}
    };

    class PrototypeAST {
        std::string Name;
        std::vector<std::string> Args;
        public:
            PrototypeAST(const std::string& name, std::vector<std::string> Args) :
                Name(name),
                Args(std::move(Args)) {}
            const std::string& getName() const {return Name;}
    };

    class FunctionAST {
        std::unique_ptr<PrototypeAST> Proto;
        std::unique_ptr<ExprAST> Body;

        public:
            FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                    std::unique_ptr<ExprAST> Body) :
                Proto(std::move(Proto)),
                Body(std::move(Body)) {}
    };

    class Parser {
        private:
            int index;
        public:
            // The token list
            std::vector<cantonese::Token> TokenList;
            // The current token the parser is looking at.
            cantonese::Token CurTok;
            // Get the next token and update the CurTok.
            cantonese::Token getNextToken();
            // Get the precedence of the pending binary operator token.
            int getTokPrecedence();
            // For error handling.
            static std::unique_ptr<ExprAST> LogError(const char* Str);
            static std::unique_ptr<PrototypeAST> LogErrorP(const char* Str);

            // For BNF parse
            std::unique_ptr<ExprAST> ParseExpression();
            std::unique_ptr<ExprAST> ParseNumberExpr();
            std::unique_ptr<ExprAST> ParseStringExpr();
            std::unique_ptr<ExprAST> ParseParenExpr();
            std::unique_ptr<ExprAST> ParseIdentifierExpr();
            std::unique_ptr<ExprAST> ParsePrimary();
            std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);

            std::unique_ptr<CallExprAST> ParsePrintExpr();
            
            std::unique_ptr<PrototypeAST> ParsePrototype();
            std::unique_ptr<FunctionAST> ParseFuncDefinition();
            std::unique_ptr<FunctionAST> ParseTopLevelExpr();

            std::map<char, int> BinopPrecedence;

            Parser(std::vector<cantonese::Token> TokenList) {
                // Init the index for TokenList.
                index = 0;
                // init the precedence map.
                this->BinopPrecedence = {
                    {'<', 10},
                    {'+', 20},
                    {'-', 20},
                    {'*', 40}
                };
            }
    };
}

#endif /* _AST_H_*/