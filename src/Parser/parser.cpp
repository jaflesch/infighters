#include "parser.h"
#include <malloc.h>

#define ALLOC_AST(A) (Ast*)A->allocate(sizeof(Ast))

Memory_Arena::Memory_Arena(s64 block_size)
{
	current_using = 0;
	memset(size, 0, sizeof(int) * MAX_ARENA);
	size[0] = block_size;
	memory[0] = _aligned_malloc(size[0], 16);
	allocated[0] = 0;
	free_ptr[0] = memory[0];
}

void* Memory_Arena::allocate(int s)
{
	void* result = free_ptr[current_using];
	if (((char*)memory[current_using] + size[current_using]) < ((char*)free_ptr[current_using] + s))
	{
		current_using++;
		if (current_using == MAX_ARENA)
			return 0;	// error, full arena
		size[current_using] = block_size;
		memory[current_using] = _aligned_malloc(block_size, 16);
		allocated[current_using] = 0;
		free_ptr[current_using] = memory[current_using];
	}
	free_ptr[current_using] = (char*)free_ptr[current_using] + s;
	allocated[current_using] += s;
	return result;
}

void Memory_Arena::release()
{

	for (int i = 0; i < MAX_ARENA; ++i)
	{
		if (size[i] != 0) {
			size[i] = 0;
			allocated[i] = 0;
			_aligned_free(memory[i]);
			memory[i] = 0;
			free_ptr[i] = 0;
		}
		else
		{
			break;
		}
	}
	current_using = 0;
}

Ast* create_literal(Memory_Arena* arena, u32 flags, Token* lit_tok, Type type)
{
	Ast* lit = ALLOC_AST(arena);

	lit->type = AST_LITERAL_EXPRESSION;
	lit->lit.lit_tok = lit_tok;
	lit->lit.type = type;

	return lit;
}

Ast* create_variable(Memory_Arena* arena, Token* token)
{
	Ast* var = ALLOC_AST(arena);

	var->type = AST_VARIABLE;
	var->var.name = token;

	return var;
}

Ast* create_unary_expression(Memory_Arena* arena, Ast* operand, UnaryOperation op, u32 flags, Precedence precedence)
{
	Ast* unexp = ALLOC_AST(arena);

	unexp->type = AST_UNARY_EXPRESSION;
	unexp->unop.flags = flags;
	unexp->unop.op = op;
	unexp->unop.operand = operand;
	unexp->unop.precedence = precedence;

	return unexp;
}

BinaryOperation get_binary_op(Token* token);

Ast* create_binary_operation(Memory_Arena* arena, Ast* left_op, Ast *right_op, Token* op, Precedence precedence)
{
	Ast* binexp = ALLOC_AST(arena);
	binexp->type = AST_BINARY_EXPRESSION;

	binexp->binop.flags = 0;
	binexp->binop.bop = get_binary_op(op);
	binexp->binop.precedence = precedence;
	binexp->binop.left = left_op;
	binexp->binop.right = right_op;

	return binexp;
}

Ast* create_assignment(Memory_Arena* arena, Ast* left, Ast* right, AssignmentOperation op)
{
	Ast* assign = ALLOC_AST(arena);

	assign->type = AST_ASSIGNMENT;

	assign->assign.left = left;
	assign->assign.right = right;
	assign->assign.op = op;

	return assign;
}

UnaryOperation get_unary_op(Token* token)
{
	Token_Type t = token->type;
	switch (t) {
	case '-':			return UNARY_OP_MINUS;
	case '+':			return UNARY_OP_PLUS;
	case '*':			return UNARY_OP_DEREFERENCE;
	case '&':			return UNARY_OP_ADDRESS_OF;
	case '[':			return UNARY_OP_VECTOR_ACCESS;
	case '~':			return UNARY_OP_NOT_BITWISE;
	case TOKEN_CAST:	return UNARY_OP_CAST;
	}
}

BinaryOperation get_binary_op(Token* token)
{
	Token_Type t = token->type;
	switch (t) {
	case TOKEN_LOGIC_AND:			return BINARY_OP_LOGIC_AND;
	case TOKEN_LOGIC_OR:			return BINARY_OP_LOGIC_OR;
	case TOKEN_EQUAL_COMPARISON:	return BINARY_OP_EQUAL_EQUAL;
	case TOKEN_LESS_EQUAL:			return BINARY_OP_LESS_EQUAL;
	case TOKEN_GREATER_EQUAL:		return BINARY_OP_GREATER_EQUAL;
	case TOKEN_NOT_EQUAL:			return BINARY_OP_NOT_EQUAL;
	case '>':						return BINARY_OP_GREATER_THAN;
	case '<':						return BINARY_OP_LESS_THAN;
	case '^':						return BINARY_OP_XOR;
	case '|':						return BINARY_OP_OR;
	case '&':						return BINARY_OP_AND;
	case TOKEN_BITSHIFT_LEFT:		return BINARY_OP_BITSHIFT_LEFT;
	case TOKEN_BITSHIFT_RIGHT:		return BINARY_OP_BITSHIFT_RIGHT;
	case '+':						return BINARY_OP_PLUS;
	case '-':						return BINARY_OP_MINUS;
	case '*':						return BINARY_OP_MULT;
	case '/':						return BINARY_OP_DIV;
	case '%':						return BINARY_OP_MOD;
	case '.':						return BINARY_OP_DOT;
	}
}

Precedence get_precedence_level(Token_Type type, bool postfixed, bool unary)
{
	if (unary) {
		if (postfixed) {
			if (type == TOKEN_PLUS_PLUS)
				return PRECEDENCE_5;
			if (type == TOKEN_MINUS_MINUS)
				return PRECEDENCE_5;
		}
		else {
			if (type == '*') return PRECEDENCE_6;	// dereference
			if (type == '&') return PRECEDENCE_6;	// address of
			if (type == '!') return PRECEDENCE_6;	// not
			if (type == '~') return PRECEDENCE_5;	// xor
			if (type == '-') return PRECEDENCE_6;	// minus
			if (type == TOKEN_PLUS_PLUS)
				return PRECEDENCE_6;
			if (type == TOKEN_MINUS_MINUS)
				return PRECEDENCE_6;
		}
	}

	switch (type)
	{
	case TOKEN_LOGIC_AND:
	case TOKEN_LOGIC_OR:
		return PRECEDENCE_0;
	case TOKEN_EQUAL_COMPARISON:
	case TOKEN_LESS_EQUAL:
	case TOKEN_GREATER_EQUAL:
	case TOKEN_NOT_EQUAL:
	case '>':
	case '<':
		return PRECEDENCE_1;
	case '^':
	case '|':
	case '&':
	case TOKEN_BITSHIFT_LEFT:
	case TOKEN_BITSHIFT_RIGHT:
		return PRECEDENCE_2;
	case '+':
	case '-':
		return PRECEDENCE_3;
	case '*':
	case '/':
	case '%':
		return PRECEDENCE_4;
	case '.':
		return PRECEDENCE_7;
	default:
		return PRECEDENCE_0;
	}
}

Precedence get_precedence_level(UnaryOperation unop, bool prefixed)
{
	/*
	UNARY_OP_MINUS,
	UNARY_OP_PLUS,
	UNARY_OP_DEREFERENCE,
	UNARY_OP_ADDRESS_OF,
	UNARY_OP_VECTOR_ACCESS,
	UNARY_OP_NOT_LOGICAL,
	UNARY_OP_NOT_BITWISE,
	UNARY_OP_CAST,
	*/
	if (prefixed)
	{
		if (unop == UNARY_OP_DEREFERENCE)	return PRECEDENCE_6;	// dereference
		if (unop == UNARY_OP_ADDRESS_OF)	return PRECEDENCE_6;	// address of
		if (unop == UNARY_OP_NOT_BITWISE)	return PRECEDENCE_6;	// not
		if (unop == UNARY_OP_MINUS)			return PRECEDENCE_6;	// minus
		if (unop == UNARY_OP_NOT_LOGICAL)	return PRECEDENCE_6;	// not logic
		if (unop == UNARY_OP_CAST)			return PRECEDENCE_6;	// cast
	}
	else {
		assert(0);
	}
}

Precedence get_precedence_level(BinaryOperation bo)
{
	Precedence p = PRECEDENCE_0;
	switch (bo) {
	case BINARY_OP_PLUS:			 p = PRECEDENCE_3; break;
	case BINARY_OP_MINUS:			 p = PRECEDENCE_3; break;
	case BINARY_OP_MULT:			 p = PRECEDENCE_4; break;
	case BINARY_OP_DIV:				 p = PRECEDENCE_4; break;
	case BINARY_OP_AND:				 p = PRECEDENCE_2; break;
	case BINARY_OP_OR:				 p = PRECEDENCE_2; break;
	case BINARY_OP_XOR:				 p = PRECEDENCE_2; break;
	case BINARY_OP_MOD:				 p = PRECEDENCE_4; break;
	case BINARY_OP_LOGIC_AND:		 p = PRECEDENCE_0; break;
	case BINARY_OP_LOGIC_OR:		 p = PRECEDENCE_0; break;
	case BINARY_OP_BITSHIFT_LEFT:	 p = PRECEDENCE_2; break;
	case BINARY_OP_BITSHIFT_RIGHT:	 p = PRECEDENCE_2; break;
	case BINARY_OP_LESS_THAN:		 p = PRECEDENCE_1; break;
	case BINARY_OP_GREATER_THAN:	 p = PRECEDENCE_1; break;
	case BINARY_OP_LESS_EQUAL:		 p = PRECEDENCE_1; break;
	case BINARY_OP_GREATER_EQUAL:	 p = PRECEDENCE_1; break;
	case BINARY_OP_EQUAL_EQUAL:		 p = PRECEDENCE_1; break;
	case BINARY_OP_NOT_EQUAL:		 p = PRECEDENCE_1; break;
	case BINARY_OP_DOT:				 p = PRECEDENCE_7; break;
	}
	return p;
}

Parser::Parser(Lexer* lexer)
	: arena(65536), lexer(lexer)
{
}

Parser::~Parser()
{
	arena.release();
}

Ast* Parser::parse_literal()
{
	Token* lit_token = lexer->eat_token();
	Ast* literal_ast = 0;
	u32 flags = 0;

	Type t = TYPE_UNKNOWN;
	switch (lit_token->type) {
	case TOKEN_INT_LITERAL: t = TYPE_S32;	break;
	case TOKEN_FLOAT_LITERAL: t = TYPE_R32;	break;
	case TOKEN_CHAR_LITERAL: t = TYPE_S8;	break;
	case TOKEN_BOOL_LITERAL: t = TYPE_S32;	break;
	}

	literal_ast = create_literal(&arena, flags, lit_token, t);
	return literal_ast;
}

Ast* Parser::parse_expression(Precedence caller_prec, bool quit_on_precedence)
{
	Ast* left_op = 0;
	Token* first = lexer->peek_token();
	if (first->type == TOKEN_END_OF_STREAM) return 0;

	if (first->flags & TOKEN_FLAG_LITERAL) {
		left_op = parse_literal();
	}
	else if (first->type == TOKEN_IDENTIFIER) {
		first = lexer->eat_token();
		left_op = create_variable(&arena, first);
	}
	else if (first->type == (Token_Type)'(') {
		lexer->eat_token();
		left_op = parse_expression(PRECEDENCE_0);
		if (lexer->eat_token()->type != ')') {
			return 0;
		}
	}
	else if (first->flags & TOKEN_FLAG_UNARY_OPERATOR) {
		UnaryOperation uop = get_unary_op(lexer->eat_token());
		Precedence unop_precedence = get_precedence_level(uop, true);
		Ast* operand = parse_expression(unop_precedence, true);
		left_op = create_unary_expression(&arena, operand, uop, UNARY_EXP_FLAG_PREFIXED, unop_precedence);
	}

	Token* optor = lexer->eat_token();
	if (optor->flags & TOKEN_FLAG_BINARY_OPERATOR) {
		Precedence bop_precedence = get_precedence_level(get_binary_op(optor));
		if (bop_precedence < caller_prec && quit_on_precedence) {
			lexer->rewind();
			return left_op;
		}
		Ast* right_op = parse_expression(bop_precedence, quit_on_precedence);
		Ast* binop = create_binary_operation(&arena, left_op, right_op, optor, bop_precedence);
		if (right_op->type == AST_BINARY_EXPRESSION) {
			if (get_precedence_level(optor->type, false, false) >= get_precedence_level(right_op->binop.bop)) {
				binop->binop.right = right_op->binop.left;
				right_op->binop.left = binop;
				return right_op;
			}
		}
		return binop;
	}
	else {
		lexer->rewind();
		return left_op;
	}
}

Ast* Parser::parse_assignment(Ast* left)
{
	Token* assign_op = lexer->eat_token();
	AssignmentOperation assop;
	switch (assign_op->type) {
	case '=':					assop = ASSIGNMENT_OPERATION_EQUAL; break;
	case TOKEN_PLUS_EQUAL:		assop = ASSIGNMENT_OPERATION_PLUS_EQUAL; break;
	case TOKEN_MINUS_EQUAL:		assop = ASSIGNMENT_OPERATION_MINUS_EQUAL; break;
	case TOKEN_TIMES_EQUAL:		assop = ASSIGNMENT_OPERATION_TIMES_EQUAL; break;
	case TOKEN_DIV_EQUAL:		assop = ASSIGNMENT_OPERATION_DIVIDE_EQUAL; break;
	case TOKEN_MOD_EQUAL:		assop = ASSIGNMENT_OPERATION_MOD_EQUAL; break;
	case TOKEN_AND_EQUAL:		assop = ASSIGNMENT_OPERATION_AND_EQUAL; break;
	case TOKEN_OR_EQUAL:		assop = ASSIGNMENT_OPERATION_OR_EQUAL; break;
	case TOKEN_XOR_EQUAL:		assop = ASSIGNMENT_OPERATION_XOR_EQUAL; break;
	case TOKEN_SHL_EQUAL:		assop = ASSIGNMENT_OPERATION_SHL_EQUAL; break;
	case TOKEN_SHR_EQUAL:		assop = ASSIGNMENT_OPERATION_SHR_EQUAL; break;
	}

	Ast* right = parse_expression();

	return create_assignment(&arena, left, right, assop);
}

Ast* Parser::parse()
{
	Ast* left = parse_expression();
	if (left == 0) return 0;
	Ast* result = 0;
	if (lexer->peek_token()->flags & TOKEN_FLAG_ASSIGNMENT_OPERATOR) {
		result = parse_assignment(left);
	} else {
		result = left;
	}
	return result;
}