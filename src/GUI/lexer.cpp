#include "lexer.h"
#include <ho_system.h>
#include <stdio.h>

#define MAX(X, Y) ((X > Y) ? (X) : (Y))

s32 Lexer::start(s8* filename, Memory_Arena* arena)
{
	this->arena = arena;
	this->filename = make_string(filename);
	file_size = ho_getfilesize(filename);
	HANDLE filehandle = ho_openfile(filename, OPEN_EXISTING);
	if (filehandle == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Error: could not open file %s.\nexiting...\n", filename);
		return -1;
	}
	this->file_size = file_size;
	void* file_memory = ho_bigalloc_rw(file_size + 1);
	*((char*)file_memory + file_size) = 0;
	filedata = (u8*)ho_readentirefile(filehandle, file_size, file_memory);
	ho_closefile(filehandle);

	lex_file();
	current_token = 0;
	return 0;
}

Lexer::~Lexer() {
	free_string(&filename);
	memory_free(token_array);
}

static int remove_comments(u8** start, s64* col_ptr);
static int remove_white_space(u8** start, s64* col_ptr);

void Lexer::lex_file()
{
	token_array = (Token*)memory_alloc(sizeof(Token) * (MAX(file_size, 64) + 4));
	u8* at = filedata;

	current_col = 0;
	bool lexing = true;
	do {
		line_count += remove_white_space(&at, &current_col);
		lexing = read_token(&at);
		current_token++;
	} while (lexing);
	ho_bigfree(filedata, file_size);
}

// return lines that advanced
static int remove_comments(u8** start, s64* col_ptr)
{
	s64 column_ptr = *col_ptr;
	u8* at = *start;
	int lines = 0;
	if (at[0] && at[0] == '/' && at[1] == '/') {
		at += 2;
		while (*at && !(*at == '\n' || *at == '\r')) {
			at++;
		}
		column_ptr = 0;
	}
	else if (at[0] && at[0] == '/' && at[1] == '*') {
		at += 2;
		column_ptr += 2;
		int keep_going = 1;
		while (*at && keep_going) {
			if (*at && at[0] == '/' && at[1] == '*')
				keep_going++;
			if (at[0] == '*' && at[1] == '/')
				keep_going--;
			if (*at == '\n') {
				lines++;
				column_ptr = -1;
			}
			at++;
			column_ptr++;
		}
		at++;
		column_ptr++;
	}
	*start = at;
	*col_ptr = column_ptr;
	return lines;
}

static int remove_white_space(u8** start, s64* col_ptr)
{
	s64 column_ptr = *col_ptr;
	u8* at = *start;
	int line_count = 0;
	int advance = 0;
	if (!at || !*at) return 0;
	line_count += remove_comments(&at, &column_ptr);
	while (is_white_space(*at)) {
		if (*at == '\n') {
			line_count++;
			column_ptr = -1;
		}
		at++;
		column_ptr++;
		line_count += remove_comments(&at, &column_ptr);
	}
	*start = at;
	*col_ptr = column_ptr;
	return line_count;
}


#define CHECK_STR_EQUAL(S) str_equal(text, length, S, sizeof(S) - 1)
// TODO: make this into a hash table
static Token_Type resolve_id_vs_keyword(char* text, int length, u32* flags)
{
	// primitive types
	Token_Type prim = TOKEN_UNKNOWN;
	
	// statements/commands
	if (CHECK_STR_EQUAL("div"))		return TOKEN_KEYWORD_DIV;
	if (CHECK_STR_EQUAL("button"))	return TOKEN_KEYWORD_BUTTON;
	if (CHECK_STR_EQUAL("style"))	return TOKEN_KEYWORD_STYLE;
	if (CHECK_STR_EQUAL("class"))	return TOKEN_KEYWORD_CLASS;
	if (CHECK_STR_EQUAL("body"))	return TOKEN_KEYWORD_BODY;

	if (CHECK_STR_EQUAL("true") || CHECK_STR_EQUAL("false")) {
		*flags |= TOKEN_FLAG_LITERAL;
		return TOKEN_BOOL_LITERAL;
	}

	return TOKEN_IDENTIFIER;
}

static bool is_number(char c)
{
	if (c >= '0' && c <= '9')
		return true;
	return false;
}

static bool is_letter(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return true;
	return false;
}

bool Lexer::read_token(u8** begin)
{
	Token& token = this->token_array[current_token];

	u8* at = *begin;

	int skip = 0;
	int length = 1;
	Token_Type type = TOKEN_UNKNOWN;
	u32 flags = 0;

	char ch_1 = 0;
	if (at) {
		ch_1 = at[0];
	}
	char ch_2 = 0;
	if (ch_1) {
		ch_2 = at[1];
	}

	token.filename = filename;

	switch (ch_1) {

	case '\0': type = TOKEN_END_OF_STREAM;			break;
	case '{': type = TOKEN_SYMBOL_OPEN_BRACE;		break;
	case '}': type = TOKEN_SYMBOL_CLOSE_BRACE;		break;
	case '[': type = TOKEN_SYMBOL_OPEN_BRACKET;		break;
	case ']': type = TOKEN_SYMBOL_CLOSE_BRACKET;	break;
	case '(': type = TOKEN_SYMBOL_OPEN_PAREN;		break;
	case ')': type = TOKEN_SYMBOL_CLOSE_PAREN;		break;
	case ',': type = TOKEN_SYMBOL_COMMA;			break;
	case ';': type = TOKEN_SYMBOL_SEMICOLON;		break;
	case '@': type = TOKEN_SYMBOL_AT;				break;
	case '$': type = TOKEN_SYMBOL_DOLLAR;			break;
	case '~': type = TOKEN_SYMBOL_TILDE;			break;
	case '?': type = TOKEN_SYMBOL_INTERROGATION;	break;
	case '%': type = TOKEN_SYMBOL_MOD;				break;
	case '*': type = TOKEN_SYMBOL_TIMES;			break;
	case '^': type = TOKEN_SYMBOL_CARAT;			break;
	case '.': type = TOKEN_SYMBOL_DOT;				break;
	case '/': type = TOKEN_SYMBOL_DIV;				break;
	case '+': type = TOKEN_SYMBOL_PLUS;				break;
	case '-': type = TOKEN_SYMBOL_MINUS;			break;
	case '=': type = TOKEN_SYMBOL_EQUAL;			break;
	case '<': type = TOKEN_SYMBOL_LESS;				break;
	case '>': type = TOKEN_SYMBOL_GREATER;			break;
	case '!': type = TOKEN_SYMBOL_NOT;				break;
	case '|': type = TOKEN_SYMBOL_PIPE;				break;
	case '&': type = TOKEN_SYMBOL_AND;				break;
	case ':': type = TOKEN_SYMBOL_COLON;			break;

	case '"': type = TOKEN_SYMBOL_QUOTE;			break;
	case '#': type = TOKEN_SYMBOL_POUND; {
		if (is_number(ch_2) ||
			(ch_2 >= 'A' && ch_2 <= 'F') ||
			(ch_2 >= 'a' && ch_2 <= 'f')) 
		{
			type = TOKEN_HEX_LITERAL;
			int l = 0;
			while (is_number(at[l + 1]) || (at[l + 1] >= 'A' && at[l + 1] <= 'F') || (at[l + 1] >= 'a' && at[l + 1] <= 'f')) {
				l++;
			}
			length += l;
			flags |= TOKEN_FLAG_NUMERIC_LITERAL | TOKEN_FLAG_LITERAL;
		}
	}break;

	case '\'': {
		flags |= TOKEN_FLAG_LITERAL;
		type = TOKEN_CHAR_LITERAL;
		int i = 0;
		at++;
		length = 0;
		for (; at[i] != '\''; ++i) {
			if (at[i] == 0) {
				type = TOKEN_UNKNOWN;
				break;
			}
			length++;
		}
		if (i == 0) type = TOKEN_UNKNOWN;
		skip++;
	}break;

	default: {
		if (is_letter(ch_1) || ch_1 == '_') {
			length = 0;
			for (int i = 0; is_letter(at[i]) || is_number(at[i]) || at[i] == '_'; ++i)
				length++;
			type = resolve_id_vs_keyword((s8*)at, length, &flags);
			if (type != TOKEN_IDENTIFIER) flags |= TOKEN_FLAG_RESERVED_WORD;
		}
		else if (is_number(ch_1)) {
			bool floating_point = false;
			int i = 0;
			length = 0;
			do {
				++i;
				length++;
				if (at[i] == '.' && !floating_point) {
					floating_point = true;
					++i;
					length++;
				}
			} while (at[i] && (is_number(at[i])));

			flags |= TOKEN_FLAG_NUMERIC_LITERAL | TOKEN_FLAG_LITERAL;
			type = (floating_point) ? TOKEN_FLOAT_LITERAL : TOKEN_INT_LITERAL;
		}
		else {
			type = TOKEN_UNKNOWN;
		}
	}break;
	}

	token.value = make_string(arena, (s8*)at, length);
	token.type = type;
	token.line = line_count;
	token.column = current_col;
	token.flags = flags;

	at += length + skip;
	current_col += skip + length;
	token_count++;

	*begin = at;

	if (type == TOKEN_END_OF_STREAM) return false;
	return true;
}

void Lexer::rewind()
{
	if (current_token == 0) return;
	current_token--;
}
Token* Lexer::last_token()
{
	if (current_token > 0)
		return &token_array[current_token - 1];
	return &token_array[0];
}
Token* Lexer::peek_token()
{
	return &token_array[current_token];
}
Token* Lexer::peek_token(s32 advance)
{
	if (advance + current_token >= token_count)
		return &token_array[token_count - 1];
	if (advance + current_token < 0)
		return 0;
	return &token_array[current_token + advance];
}
Token_Type Lexer::peek_token_type()
{
	return token_array[current_token].type;
}
Token_Type Lexer::peek_token_type(s32 advance)
{
	if (advance + current_token >= token_count)
		return token_array[token_count - 1].type;
	return token_array[current_token + advance].type;
}

Token* Lexer::eat_token(s32 advance) {
	if (current_token + advance >= token_count)
		return &token_array[token_count];
	current_token += advance;
	return &token_array[current_token - 1];
}

Token* Lexer::eat_token()
{
	if (current_token >= token_count)
		return &token_array[token_count];
	current_token++;
	return &token_array[current_token - 1];
}

static char* make_null_term_string(char* s)
{
	s32 len = strlen(s);
	char* ptr = (char*)malloc(len + 1);
	memcpy(ptr, s, len);
	ptr[len] = 0;
	return ptr;
}

char* Lexer::get_token_string(Token_Type t)
{
	switch (t) {
	case TOKEN_SYMBOL_NOT:				return  make_null_term_string("!"); break;
	case TOKEN_SYMBOL_QUOTE:			return make_null_term_string("\""); break;
	case TOKEN_SYMBOL_POUND:			return  make_null_term_string("#"); break;
	case TOKEN_SYMBOL_DOLLAR:			return  make_null_term_string("$"); break;
	case TOKEN_SYMBOL_MOD:				return  make_null_term_string("%"); break;
	case TOKEN_SYMBOL_AND:				return  make_null_term_string("&"); break;
	case TOKEN_SYMBOL_OPEN_PAREN:		return  make_null_term_string("("); break;
	case TOKEN_SYMBOL_CLOSE_PAREN:		return  make_null_term_string(")"); break;
	case TOKEN_SYMBOL_TIMES:			return  make_null_term_string("*"); break;
	case TOKEN_SYMBOL_PLUS:				return  make_null_term_string("+"); break;
	case TOKEN_SYMBOL_COMMA:			return  make_null_term_string(","); break;
	case TOKEN_SYMBOL_MINUS:			return  make_null_term_string("-"); break;
	case TOKEN_SYMBOL_DOT:				return  make_null_term_string("."); break;
	case TOKEN_SYMBOL_DIV:				return  make_null_term_string("/"); break;
	case TOKEN_SYMBOL_COLON:			return  make_null_term_string(":"); break;
	case TOKEN_SYMBOL_SEMICOLON:		return  make_null_term_string(";"); break;
	case TOKEN_SYMBOL_LESS:				return  make_null_term_string("<"); break;
	case TOKEN_SYMBOL_EQUAL:			return  make_null_term_string("="); break;
	case TOKEN_SYMBOL_GREATER:			return  make_null_term_string(">"); break;
	case TOKEN_SYMBOL_INTERROGATION:	return  make_null_term_string("?"); break;
	case TOKEN_SYMBOL_AT:				return  make_null_term_string("@"); break;
	case TOKEN_SYMBOL_OPEN_BRACKET:		return  make_null_term_string("["); break;
	case TOKEN_SYMBOL_CLOSE_BRACKET:	return  make_null_term_string("]"); break;
	case TOKEN_SYMBOL_CARAT:			return  make_null_term_string("^"); break;
	case TOKEN_SYMBOL_BACK_TICK:		return  make_null_term_string("`"); break;
	case TOKEN_SYMBOL_OPEN_BRACE:		return  make_null_term_string("{"); break;
	case TOKEN_SYMBOL_PIPE:				return  make_null_term_string("|"); break;
	case TOKEN_SYMBOL_CLOSE_BRACE:		return  make_null_term_string("}"); break;
	case TOKEN_SYMBOL_TILDE:			return  make_null_term_string("~"); break;
	}
}
