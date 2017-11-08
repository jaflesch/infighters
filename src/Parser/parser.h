#pragma once
#include "..\common.h"
#include "lexer.h"
#include "..\debug_table.h"

struct Memory_Arena
{
#define MAX_ARENA 1024
	s64 block_size;
	int current_using;
	int size[MAX_ARENA];
	int allocated[MAX_ARENA];
	void* memory[MAX_ARENA];
	void* free_ptr[MAX_ARENA];

	Memory_Arena(s64 block_size);
	void* allocate(int s);
	void release();
};

enum BinaryOperation
{
	BINARY_OP_UNKNOWN = 0,

	BINARY_OP_PLUS = 1,
	BINARY_OP_MINUS = 2,
	BINARY_OP_MULT = 3,
	BINARY_OP_DIV = 4,
	BINARY_OP_AND = 5,
	BINARY_OP_OR = 6,
	BINARY_OP_XOR = 7,
	BINARY_OP_MOD = 8,
	BINARY_OP_LOGIC_AND = 9,
	BINARY_OP_LOGIC_OR = 10,
	BINARY_OP_BITSHIFT_LEFT = 11,
	BINARY_OP_BITSHIFT_RIGHT = 12,

	BINARY_OP_LESS_THAN = 13,
	BINARY_OP_GREATER_THAN = 14,
	BINARY_OP_LESS_EQUAL = 15,
	BINARY_OP_GREATER_EQUAL = 16,
	BINARY_OP_EQUAL_EQUAL = 17,
	BINARY_OP_NOT_EQUAL = 18,

	BINARY_OP_DOT = 19,
};

enum UnaryOperation
{
	UNARY_OP_UNKNOWN = 0,

	UNARY_OP_MINUS,
	UNARY_OP_PLUS,
	UNARY_OP_DEREFERENCE,
	UNARY_OP_ADDRESS_OF,
	UNARY_OP_VECTOR_ACCESS,
	UNARY_OP_NOT_LOGICAL,
	UNARY_OP_NOT_BITWISE,
	UNARY_OP_CAST,
};

enum AssignmentOperation
{
	ASSIGNMENT_OPERATION_UNKNOWN,

	ASSIGNMENT_OPERATION_EQUAL,
	ASSIGNMENT_OPERATION_PLUS_EQUAL,
	ASSIGNMENT_OPERATION_MINUS_EQUAL,
	ASSIGNMENT_OPERATION_TIMES_EQUAL,
	ASSIGNMENT_OPERATION_DIVIDE_EQUAL,
	ASSIGNMENT_OPERATION_AND_EQUAL,
	ASSIGNMENT_OPERATION_OR_EQUAL,
	ASSIGNMENT_OPERATION_XOR_EQUAL,
	ASSIGNMENT_OPERATION_SHL_EQUAL,
	ASSIGNMENT_OPERATION_SHR_EQUAL,
	ASSIGNMENT_OPERATION_MOD_EQUAL,
};

enum Precedence
{
	PRECEDENCE_0 = 0,	//	 || &&
	PRECEDENCE_1 = 1,	//	 == >= <= != > <
	PRECEDENCE_2 = 2,	//	 ^ | & >> <<
	PRECEDENCE_3 = 3,	//	 + -
	PRECEDENCE_4 = 4,	//	 * / %
	PRECEDENCE_5 = 5,	//	 &(addressof) ~ ++POST --POST
	PRECEDENCE_6 = 6,	//	 *(dereference)	cast ! ++PRE --PRE
	PRECEDENCE_7 = 7,	//	 .
	PRECEDENCE_MAX = 8,
};

enum NodeType {
	AST_ASSIGNMENT,
	AST_BINARY_EXPRESSION,
	AST_UNARY_EXPRESSION,
	AST_LITERAL_EXPRESSION,
	AST_VARIABLE,
};

struct Ast;

struct Ast_Binop {
	Precedence precedence;
	u32 flags;
	BinaryOperation bop;
	Ast* left;
	Ast* right;
};

const u32 UNARY_EXP_FLAG_PREFIXED = FLAG(0);
const u32 UNARY_EXP_FLAG_POSTFIXED = FLAG(1);
struct Ast_Unary_Exp {
	Precedence precedence;
	u32 flags;
	UnaryOperation op;
	Ast* operand;
};

struct Ast_Assignment {
	Ast* left;
	Ast* right;
	AssignmentOperation op;
};

struct Ast_Literal {
	Token* lit_tok;
	Type type;
};

struct Ast_Variable {
	Token* name;
};

struct Ast {
	NodeType type;
	union {
		Ast_Assignment assign;
		Ast_Unary_Exp unop;
		Ast_Binop binop;
		Ast_Literal lit;
		Ast_Variable var;
	};
};

struct Parser {
	Parser(Lexer* lexer);
	~Parser();

	Memory_Arena arena;

	Lexer* lexer;
	Ast* parse_expression(Precedence caller_prec = PRECEDENCE_0, bool quit_on_precedence = false);
	Ast* parse_literal();
	Ast* parse_assignment(Ast* left);

	Ast* parse();
};