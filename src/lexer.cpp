#include <iostream>
#include <cstring>
#include "lexer.hpp"

namespace cantonese {

    bool IsSpace(CAN_Char ch) { return ch == _CAN_C(' ') || ch == _CAN_C('\r') || ch == _CAN_C('\n');}
    bool IsNumber(CAN_Char ch) { return ch >= _CAN_C('0') && ch <= _CAN_C('9'); }
    bool IsAlpha(CAN_Char ch) { return (ch >= _CAN_C('a') && ch <= _CAN_C('z')) || (ch >= _CAN_C('A') && ch <= _CAN_C('Z')); }
    bool IsCodeChar(CAN_Char ch) { return IsAlpha(ch) || ch == _CAN_C('_') || (ch >= _CAN_C('\u4e00') && ch <= _CAN_C('\u9fa5')); }

    // 读取一个Token
    void Lexer::GetNextToken() {
        if (mEof)
            return;
        // 跳过空格
        SkipSpace();
        switch (CURRENT_CHAR) {
            case _CAN_C(';'):
                TOKEN(TokenType::Delimiter, CURRENT_POS, 1, CURRENT_LINE); 
                break;
            case _CAN_C('('):
                TOKEN(TokenType::LeftParen, CURRENT_POS, 1, CURRENT_LINE);
                break;
            case _CAN_C(')'):
                TOKEN(TokenType::RightParen, CURRENT_POS, 1, CURRENT_LINE);
                break;
            case _CAN_C('['):
                TOKEN(TokenType::LeftBracket, CURRENT_POS, 1, CURRENT_LINE);
                break;
            case _CAN_C(']'):
                TOKEN(TokenType::RightBracket, CURRENT_POS, 1, CURRENT_LINE);
                break;
            case _CAN_C('{'):
                TOKEN(TokenType::LeftBrace, CURRENT_POS, 1, CURRENT_LINE);
                break;
            case _CAN_C('}'):
                TOKEN(TokenType::RightBrace, CURRENT_POS, 1, CURRENT_LINE);
                break;
            case _CAN_C('.'):
                TOKEN(TokenType::Dot, CURRENT_POS, 1, CURRENT_LINE);
                break;
            case _CAN_C('|'):
                TOKEN(TokenType::Or, CURRENT_POS, 1, CURRENT_CHAR);
                break;
            case _CAN_C('!'):
                if (NEXT_CHAR == _CAN_C('=')) {
                    TOKEN(TokenType::NotEqual, CURRENT_POS, 2, CURRENT_LINE);  // != not equal 不相等
                    NEXT();
                } else {
                    TOKEN(TokenType::Not, CURRENT_POS, 1, CURRENT_LINE);  // !
                }
                break;
            case _CAN_C('+'):
                if (NEXT_CHAR == _CAN_C('+')) {
                    TOKEN(TokenType::Increase, CURRENT_POS, 2, CURRENT_LINE); // ++
                    NEXT();
                } else if (NEXT_CHAR == _CAN_C('=')) {
                    TOKEN(TokenType::AddAssign, CURRENT_POS, 2, CURRENT_LINE); // +=
                    NEXT();
                } else {
                    TOKEN(TokenType::Add, CURRENT_POS, 1, CURRENT_LINE); // +
                }
                break;
            case _CAN_C('='):
                if (NEXT_CHAR == _CAN_C('=')) {
                    TOKEN(TokenType::Equal, CURRENT_POS, 2, CURRENT_LINE); // ==
                    NEXT();
                } else {
                    TOKEN(TokenType::Assign, CURRENT_POS, 1, CURRENT_LINE); // =
                }
                break;
            case _CAN_C('-'):
                if (NEXT_CHAR == _CAN_C('-')) { // --
                    TOKEN(TokenType::Decrease, CURRENT_POS, 2, CURRENT_LINE); // 自减
                    NEXT();
                } else if (NEXT_CHAR == _CAN_C('=')) { // -=
                    TOKEN(TokenType::SubAssign, CURRENT_POS, 2, CURRENT_LINE);
                    NEXT();
                } else if (NEXT_CHAR == _CAN_C('>')) { // -> Arrow 箭头
                    TOKEN(TokenType::Arrow, CURRENT_POS, 2, CURRENT_LINE);
                    NEXT();
                } else {
                    TOKEN(TokenType::Sub, CURRENT_POS, 1, CURRENT_LINE); // -
                }
                break;
            case _CAN_C('*'):
                TOKEN(TokenType::Mul, CURRENT_POS, 1, CURRENT_LINE);
                break;
            case _CAN_C(','):
                TOKEN(TokenType::Comma, CURRENT_POS, 1, CURRENT_LINE);
                break;
            
            case _CAN_C('/'):
                if (NEXT_CHAR == _CAN_C('/') || NEXT_CHAR == _CAN_C('*')) {
                    SkipComment();
                    return GetNextToken();
                } else {
                    TOKEN(TokenType::Div, CURRENT_POS, 1, CURRENT_LINE);
                }
                break;
            case _CAN_C('\0'):
                mEof = true;
                TOKEN(TokenType::End, CURRENT_POS, 0, CURRENT_LINE);
                return;
            default:
                if (IsCodeChar(CURRENT_CHAR) || IsAlpha(CURRENT_CHAR)) {
                    ParseIdentifier();
                } else if (IsNumber(CURRENT_CHAR)) {
                    ParseNumber();
                } else if (CURRENT_CHAR == _CAN_C('"')) {
                    ParseString();
                } else {
                    LEXER_UNKOWNCHAR(CURRENT_CHAR);
                    wprintf(L"%S\n", &CURRENT_CHAR);
                    NEXT();
                }
                return;
        }
        NEXT();
    }

    void Lexer::SkipComment() {
        do {
            NEXT();
        } while (CURRENT_CHAR == _CAN_C('\n'));
        CURRENT_LINE++;
    }

    void Lexer::SkipSpace() {
        while (IsSpace(CURRENT_CHAR)){
            if (CURRENT_CHAR == _CAN_C('\n')) CURRENT_LINE++;
            NEXT();
        }
    }

    bool Lexer::Match(TokenType tokenKind) {
        Token token = Peek(1);
        if (token.mType == tokenKind) {
            mTokens.pop_front();
            return true;
        }
        return false;
    }

    Token Lexer::Peek(CAN_Integer i) {
        while (mTokens.size() < i) {
            if (mEof)
                return CURRENT_TOKEN;
            GetNextToken();
            mTokens.push_back(CURRENT_TOKEN);
        }
        return mTokens[i - 1];
    }

    void Lexer::ParseNumber() {
        TOKEN(TokenType::Number, CURRENT_POS, 0, CURRENT_LINE);
        do {
            NEXT();
        } while (IsNumber(CURRENT_CHAR) || CURRENT_CHAR == _CAN_C('.'));
        TOKEN_LENGTH((CAN_UINT32)(CURRENT_POS - TOKEN_START));
    }

    void Lexer::ParseString() {
        TOKEN(TokenType::String, CURRENT_POS, 0, CURRENT_LINE);
        do {
            NEXT();
            if (CURRENT_CHAR == _CAN_C('\n')) CURRENT_LINE++;
        } while (CURRENT_CHAR != _CAN_C('"'));
        NEXT();
        TOKEN_LENGTH((CAN_UINT32)(CURRENT_POS - TOKEN_START));
    }
    
    void Lexer::ParseIdentifier() {
        TOKEN(TokenType::Identifier, CURRENT_POS, 0, CURRENT_LINE);
        do {
            NEXT();
        } while (IsCodeChar(CURRENT_CHAR) || IsNumber(CURRENT_CHAR));
        TOKEN_LENGTH((CAN_UINT32)(CURRENT_POS - TOKEN_START));
        CAN_Char identifierBuffer[255] = {_CAN_C('\0')}; // init the buffer with the end char '\0'
        memcpy(identifierBuffer, mCurrentToken.mStart, mCurrentToken.mLength * sizeof(CAN_Char));
        TokenType tok = CANKeywords[identifierBuffer];
        if (tok != TokenType::End) {
            TOKEN_TYPE(tok);
        }
    }

    Lexer::~Lexer() = default;

    Token Lexer::Read() {
        if (!mTokens.empty()) {
            Token token = mTokens.front();
            mTokens.pop_front();
            return token;
        }
        GetNextToken();
        return CURRENT_TOKEN;
    }

    Lexer::Lexer(CAN_Char *mSource) : mSource(mSource) {}

    TokenType Lexer::ReadTokenType() {
        GetNextToken();
        return CURRENT_TOKEN.mType;
    }

    TokenType Lexer::PeekTokenType() {
        if (mTokens.empty()) {
            GetNextToken();
            mTokens.push_back(mCurrentToken);
        }
        return mTokens[0].mType;
    }

    void Lexer::Consume() {
        if (!mTokens.empty())
            mTokens.pop_front();
        else
            GetNextToken();
    }

    Token::Token(TokenType mType, const CAN_Char *mStart, CAN_UINT32 mLength, CAN_UINT32 mLine) : 
        mType(mType),
        mStart(mStart),
        mLength(mLength),
        mLine(mLine) {}
}
