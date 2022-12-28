#include "ast.hpp"

namespace Ast {
    cantonese::Token Parser::getNextToken() {
        return CurTok = TokenList[++index];
    }

    int Parser::getTokPrecedence() {
        // TODO
        return 0;
    }

    std::unique_ptr<ExprAST> Parser::LogError(const char* Str) {
        fprintf(stderr, "Error: %s\n", Str);
        return nullptr;
    }

    std::unique_ptr<PrototypeAST> Parser::LogErrorP(const char *Str) {
        LogError(Str);
        return nullptr;
    }

    // stringexpr ::= string
    std::unique_ptr<ExprAST> Parser::ParseStringExpr() {
        auto Result = std::make_unique<StringExprAST>(CurTok.to_string());
        getNextToken(); // Skip the string
        return std::move(Result);
    }

    // numberexpr ::= number
    std::unique_ptr<ExprAST> Parser::ParseNumberExpr() {
        // wstring to number
        int NumVal = stod(CurTok.to_string());
        auto Result = std::make_unique<NumberExprAST>(NumVal);
        getNextToken(); // Skip the number
        return std::move(Result);
    }

    // parenexpr ::= '(' expression ')'
    std::unique_ptr<ExprAST> Parser::ParseParenExpr() {
        getNextToken(); // Skip '('
        auto v = ParseExpression();
        if (!v) return nullptr;
        if (CurTok.getTokenType() != TokenType::RightParen) {
            return LogError("excepted ')");
        }
        getNextToken(); // Skip ')'
        return v;
    }

    //  primary
    //  ::= numberexpr
    //  ::= stringexpr
    //  ::= parenexpr
    std::unique_ptr<ExprAST> Parser::ParsePrimary() {
        switch (CurTok.mType) {
        default:
            return LogError("unknown token when expecting an expression");
        case TokenType::Identifier:
            return ParseIdentifierExpr();
        case TokenType::Number:
            return ParseNumberExpr();
        case TokenType::String:
            return ParseStringExpr();
        case TokenType::LeftParen:
            return ParseParenExpr();
        }
    }

    std::unique_ptr<ExprAST> Parser::ParseExpression() {
        // TODO
        return nullptr;
    }

    std::unique_ptr<ExprAST> Parser::ParseIdentifierExpr() {
        // TODO
        return nullptr;
    }

    std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
        // TODO
        return nullptr;
    }
}