#include "ast.hpp"

namespace Ast {
    cantonese::Token Parser::getNextToken() {
        return CurTok = TokenList[++index];
    }

    int Parser::getTokPrecedence() {
        if (!isascii(CurTok.to_string().back())) {
            return -1;
        }

        int TokPrec = BinopPrecedence[CurTok.to_string().back()];
        if (TokPrec <= 0) return -1;
        return TokPrec;
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
    //    ::= numberexpr
    //    ::= stringexpr
    //    ::= parenexpr
    //    ::= identifierexpr
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

    // prototype
    // ::= id '|' id* '|'
    std::unique_ptr<PrototypeAST> Parser::ParsePrototype() {
        if (CurTok.mType != TokenType::Identifier) {
            return LogErrorP("Expected function name in prototype. ");
        }

        std::wstring FnName = CurTok.to_string();
        getNextToken(); // Skip the function name

        if (CurTok.mType != TokenType::Or) {
            return LogErrorP("Expected '|' in prototype. ");
        }
        getNextToken(); // Skip the '|'
        std::vector<std::wstring> ArgNames;
        cantonese::Token tk = getNextToken();

        while (tk.mType == TokenType::Identifier) {
            ArgNames.push_back(tk.to_string());
            tk = getNextToken();
        }

        if (CurTok.mType != TokenType::Or) {
            return LogErrorP("Expected '|' in prototype. ");
        }

        getNextToken(); // Skip the '|'

        return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
    }

    std::unique_ptr<ExprAST> Parser::ParseExpression() {
        // TODO
        return nullptr;
    }


    // identifierexpr
    //  ::= identifier
    //  ::= identifier 下 -> '|' expression* '|'
    std::unique_ptr<ExprAST> Parser::ParseIdentifierExpr() {
        std::wstring IdName = CurTok.to_string();
        getNextToken(); // Skip the identifier
        if (CurTok.mType != TokenType::KeywordCallBegin) {
            // Simple variable ref
            return std::make_unique<VariableExprAST>(IdName);
        }
        // Call
        getNextToken(); // Skip the KeywordCallBegin.
        std::vector<std::unique_ptr<ExprAST>> Args;

        return nullptr;
    }

    std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
        // TODO
        return nullptr;
    }
}