#ifndef _TOKEN_TYPE_H_
#define _TOKEN_TYPE_H_

enum class TokenType {
    End,
    Delimiter,
    Number,
    String,
    Identifier,
    Dot,
    Comma,
    LeftParen, // ( 
    RightParen,  // )
    LeftBracket, // [
    RightBracket, // ]
    LeftBrace,  // {
    RightBrace, // }

    //四则运算 + - * /
    Add,
    Sub,
    Mul,
    Div,

    Increase,  // ++
    Decrease, // --
    Assign, // assignment 赋值 =
    AddAssign, // +=
    SubAssign, // -=
    DivAssign,
    MulAssign,
    ModAssign,
    AndAssign,
    OrAssign,
    XorAssign,
    Arrow, //  ->

    //
    Not, // !
    Equal,  // ==
    NotEqual, // !=
    Greater, // >
    Less, // <
    GreaterEqual, // >=
    LessEqual, // <=

    // |
    Or,
    LogicOr,

    // &
    And,
    LogicAnd,

    // #
    Mod, // %

    At,  // @

    Colon,  // :

    KeywordIf,
    KeywordElse,
    KeywordPrint,
    KeywordPrintend,
    KeywordExit,
    KeywordIn,
    KeywordType,
    KeywordAssign,
    KeywordClassdef,
    KeywordIs,
    KeywordThen,
    KeywordDo,
    KeywordPass,
    KeywordWhileDo,
    KeywordFunction,
    KeywordCall,
    KeywordImport,
    KeywordFuncBegin,
    KeywordFuncEnd,
    KeywordAssert,
    KeywordClassAssign,
    KeywordWhile,
    KeywordWhileEnd,
    KeywordReturn,
    KeywordTry,
    KeywordExcept,
    KeywordFinally,
    KeywordRaise,
    KeywordRaiseEnd,
    KeywordFrom,
    KeywordTo,
    KeywordEndFor,
    KeywordExtend,
    KeywordMethed,
    KeywordEndClass,
    KeywordCmd,
    KeywordBreak,
    KeywordListAssign,
    KeywordSetAssign,
    KeywordGlobalSet,
    KeywordFalse,
    KeywordTrue,
    KeywordNone,
    KeywordStackinit,
    KeywordPush,
    KeywordPop,
    KeywordModel,
    KeywordModelNew,
    KeywordClassInit,
    KeywordSelf,
    KeywordCallBegin,
    KeywordDelete,
};

#endif /* _TOKEN_TYPE_H_ */