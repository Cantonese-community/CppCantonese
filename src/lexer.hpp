#ifndef _CANTONESE_LEXER_H_
#define _CANTONESE_LEXER_H_

#include <cstdlib>
#include <deque>
#include <map>
#include <limits.h>

#include "cantonese.hpp"
#include "TokenType.hpp"


#define NEXT_CHAR mSource[mPosition + 1]

/// Current position
#define CURRENT_POS (&mSource[mPosition])
#define CURRENT_CHAR mSource[mPosition]
#define CURRENT_LINE mLine
#define CURRENT_TOKEN mCurrentToken

/// Set Current Token
#define TOKEN(K, S, len, line) { mCurrentToken.mType = K; \
mCurrentToken.mStart = S;  \
mCurrentToken.mLength = len; \
mCurrentToken.mLine = line; }

#define TOKEN_TYPE(K) mCurrentToken.mType = K;
#define TOKEN_LENGTH(l) {mCurrentToken.mLength = l;}
#define TOKEN_START mCurrentToken.mStart

// Get the next position
#define NEXT() {mPosition++;}

#define LEXER_UNKOWNCHAR(str) std::cout << "濑嘢 : Line " << mLine <<  "唔知"; std::cout<< "係咩: "

namespace cantonese {
    inline bool IsNumber(CAN_Char ch);
    inline bool IsAlpha(CAN_Char ch);
    inline bool IsCodeChar(CAN_Char ch);
    inline bool IsSpace(CAN_Char ch);

    static std::map<String, TokenType> CANKeywords = {
        {_CAN_C("如果"), TokenType::KeywordIf},
        {_CAN_C("唔系"), TokenType::KeywordElse},
        {_CAN_C("畀我睇下"), TokenType::KeywordPrint},
        {_CAN_C("点样先"), TokenType::KeywordPrintend},
        {_CAN_C("收工"), TokenType::KeywordExit},
        {_CAN_C("喺"), TokenType::KeywordIn},
        {_CAN_C("起底"), TokenType::KeywordType},
        {_CAN_C("讲嘢"), TokenType::KeywordAssign},
        {_CAN_C("系"), TokenType::KeywordIs},
        {_CAN_C("係"), TokenType::KeywordIs},
        {_CAN_C("嘅话"), TokenType::KeywordThen},
        {_CAN_C("掟个"), TokenType::KeywordRaise}
    };

    
    class Token {
        public:
            TokenType mType{TokenType::End};
            const CAN_Char *mStart{};
            CAN_UINT32 mLength{};
            CAN_UINT32 mLine{};

            std::wstring to_string() {
                CAN_Char tokeninfo[255] = {_CAN_C('\0')};
                // std::cout << (int)mType << " ";
                memcpy(tokeninfo, this->mStart, this->mLength * sizeof(CAN_Char));
                return tokeninfo;
            }

            TokenType getTokenType() {
                return mType;
            }

        Token(TokenType mType, const CAN_Char *mStart, CAN_UINT32 mLength, CAN_UINT32 mLine);
        Token() = default;
    };

    class Lexer {
    protected:
        bool mEof{false};
        std::deque<Token> mTokens;

        inline void GetNextToken();
        inline void SkipSpace();
        inline void SkipComment();
        inline void ParseIdentifier();
        inline void ParseString();
        inline void ParseNumber();

    public:
        Token mCurrentToken;

        Lexer(CAN_Char *mSource);
        ~Lexer();

        Token Read();
        TokenType ReadTokenType();
        TokenType PeekTokenType();
        void Consume();
        Token Peek(CAN_Integer i = 1);
        bool Match(TokenType tokenKind);

        CAN_Char *GetSource() {
            return mSource;
        }
    private:
        CAN_Char *mSource;
        CAN_UINT32 mPosition{0};
        CAN_UINT32 mLine{1};
    };
}

#endif /* _CANTONESE_LEXER_H_ */