#pragma once
#include "../util.h"

enum Token_Type
{
	TOKEN_END_OF_STREAM = 0,
	TOKEN_UNKNOWN = 1,

	// Symbols
	TOKEN_SYMBOL_NOT = '!',					// 33
	TOKEN_SYMBOL_QUOTE = '"',				// 34
	TOKEN_SYMBOL_POUND = '#',				// 35
	TOKEN_SYMBOL_DOLLAR = '$',				// 36
	TOKEN_SYMBOL_MOD = '%',					// 37
	TOKEN_SYMBOL_AND = '&',					// 38
	TOKEN_SYMBOL_OPEN_PAREN = '(',			// 40
	TOKEN_SYMBOL_CLOSE_PAREN = ')',			// 41
	TOKEN_SYMBOL_TIMES = '*',				// 42
	TOKEN_SYMBOL_PLUS = '+',				// 43
	TOKEN_SYMBOL_COMMA = ',',				// 44
	TOKEN_SYMBOL_MINUS = '-',				// 45
	TOKEN_SYMBOL_DOT = '.',					// 46
	TOKEN_SYMBOL_DIV = '/',					// 47
	TOKEN_SYMBOL_COLON = ':',				// 58
	TOKEN_SYMBOL_SEMICOLON = ';',			// 59
	TOKEN_SYMBOL_LESS = '<',				// 60
	TOKEN_SYMBOL_EQUAL = '=',				// 61
	TOKEN_SYMBOL_GREATER = '>',				// 62
	TOKEN_SYMBOL_INTERROGATION = '?',		// 63
	TOKEN_SYMBOL_AT = '@',					// 64
	TOKEN_SYMBOL_OPEN_BRACE = '{',			// 91
	TOKEN_SYMBOL_CLOSE_BRACE = '}',			// 93
	TOKEN_SYMBOL_CARAT = '^',				// 94
	TOKEN_SYMBOL_BACK_TICK = '`',			// 96
	TOKEN_SYMBOL_OPEN_BRACKET = '[',		// 123
	TOKEN_SYMBOL_PIPE = '|',				// 124
	TOKEN_SYMBOL_CLOSE_BRACKET = ']',		// 125
	TOKEN_SYMBOL_TILDE = '~',				// 126
	

	// Literal tokens
	TOKEN_INT_LITERAL = 127,
	TOKEN_FLOAT_LITERAL = 128,
	TOKEN_CHAR_LITERAL = 129,
	TOKEN_STRING_LITERAL = 130,
	TOKEN_BOOL_LITERAL = 131,
	TOKEN_IDENTIFIER = 132,
	TOKEN_HEX_LITERAL = 133,

	TOKEN_KEYWORD_DIV,
	TOKEN_KEYWORD_BUTTON,
	TOKEN_KEYWORD_STYLE,
	TOKEN_KEYWORD_CLASS,
	TOKEN_KEYWORD_BODY,
};

const u32 TOKEN_FLAG_RESERVED_WORD = FLAG(0);
const u32 TOKEN_FLAG_NUMERIC_LITERAL = FLAG(1);
const u32 TOKEN_FLAG_LITERAL = FLAG(2);

struct Token
{
	Token_Type type;

	s32 line;
	s32 column;

	u32 flags = 0;

	string filename;
	string value;
};

struct Lexer
{
	string filename;
	s64 file_size = 0;
	u8* filedata = 0;

	s64 token_count = 0;
	s64 line_count = 1;

	s32 start(s8* filename);
	void rewind();
	Token* peek_token();
	Token* peek_token(s32 advance);
	Token_Type peek_token_type();
	Token_Type peek_token_type(s32 advance);
	Token* eat_token();
	Token* eat_token(s32 advance);
	Token* last_token();

	char* get_token_string(Token_Type);

	void lex_file();
	bool read_token(u8** at);

	s64 current_token = 0;
	s64 current_line = 0;
	s64 current_col = 0;
	Token* token_array = 0;
};