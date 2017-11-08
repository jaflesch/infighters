#pragma once
#include "parser.h"
#include "..\debug_table.h"

struct Evaluated {
	void* data;
	Type t;
};

s64 calc_binop_s64(Ast* exp, Evaluated left, Evaluated right) {
	s64 l = *(s64*)left.data;
	s64 r = *(s64*)right.data;
	s64 result = 0;
	switch (exp->binop.bop) {
	case BINARY_OP_PLUS:			result = l + r; break;
	case BINARY_OP_MINUS:			result = l - r; break;
	case BINARY_OP_MULT:			result = l * r; break;
	case BINARY_OP_DIV:				if(r) result = l / r; break;
	case BINARY_OP_AND:				result = l & r; break;
	case BINARY_OP_OR:				result = l | r; break;
	case BINARY_OP_XOR:				result = l ^ r; break;
	case BINARY_OP_MOD:				result = l % r; break;
	case BINARY_OP_BITSHIFT_LEFT:	result = l << r; break;
	case BINARY_OP_BITSHIFT_RIGHT:	result = l >> r; break;
	}
	return result;
}
u64 calc_binop_u64(Ast* exp, Evaluated left, Evaluated right) {
	u64 l = *(u64*)left.data;
	u64 r = *(u64*)right.data;
	u64 result = 0;
	switch (exp->binop.bop) {
	case BINARY_OP_PLUS:			result = l + r; break;
	case BINARY_OP_MINUS:			result = l - r; break;
	case BINARY_OP_MULT:			result = l * r; break;
	case BINARY_OP_DIV:				if (r) result = l / r; break;
	case BINARY_OP_AND:				result = l & r; break;
	case BINARY_OP_OR:				result = l | r; break;
	case BINARY_OP_XOR:				result = l ^ r; break;
	case BINARY_OP_MOD:				result = l % r; break;
	case BINARY_OP_BITSHIFT_LEFT:	result = l << r; break;
	case BINARY_OP_BITSHIFT_RIGHT:	result = l >> r; break;
	}
	return result;
}
double calc_binop_r64(Ast* exp, Evaluated left, Evaluated right) {
	double l = *(double*)left.data;
	double r = *(double*)right.data;
	double result = 0;
	switch (exp->binop.bop) {
	case BINARY_OP_PLUS:			result = l + r; break;
	case BINARY_OP_MINUS:			result = l - r; break;
	case BINARY_OP_MULT:			result = l * r; break;
	case BINARY_OP_DIV:				if (r) result = l / r; break;
	}
	return result;
}
float calc_binop_r32(Ast* exp, Evaluated left, Evaluated right) {
	float l = *(float*)left.data;
	float r = *(float*)right.data;
	float result = 0;
	switch (exp->binop.bop) {
	case BINARY_OP_PLUS:			result = l + r; break;
	case BINARY_OP_MINUS:			result = l - r; break;
	case BINARY_OP_MULT:			result = l * r; break;
	case BINARY_OP_DIV:				if (r) result = l / r; break;
	}
	return result;
}

Evaluated eval_expr(Ast* exp)
{
	if (exp->type == AST_BINARY_EXPRESSION) {
		Evaluated left = eval_expr(exp->binop.left);
		Evaluated right = eval_expr(exp->binop.right);
		if (!left.data || !right.data) {
			Evaluated r;
			r.data = 0;
			return r;
		}

		Evaluated r;
		if (left.t == TYPE_S16 || left.t == TYPE_S32 || left.t == TYPE_S64 || left.t == TYPE_S8) {
			s64* res = (s64*)malloc(sizeof(s64));
			*res = calc_binop_s64(exp, left, right);
			r.data = res;
			r.t = left.t;
		}
		if (left.t == TYPE_U16 || left.t == TYPE_U32 || left.t == TYPE_U64 || left.t == TYPE_U8) {
			u64* res = (u64*)malloc(sizeof(u64));
			*res = calc_binop_u64(exp, left, right);
			r.data = res;
			r.t = left.t;
		}
		if (left.t == TYPE_R32) {
			float* res = (float*)malloc(sizeof(float));
			*res = calc_binop_r32(exp, left, right);
			r.data = res;
			r.t = left.t;
		}else if (left.t == TYPE_R64) {
			double* res = (double*)malloc(sizeof(double));
			*res = calc_binop_r64(exp, left, right);
			r.data = res;
			r.t = left.t;
		}
		if(left.data) free(left.data);
		if(right.data) free(right.data);
		return r;
	}
	else if (exp->type == AST_UNARY_EXPRESSION) {
		Evaluated r;
		r.data = 0;
		return r;
	}
	else if (exp->type == AST_LITERAL_EXPRESSION) {
		Type t = exp->lit.type;
		Evaluated r;
		char buffer[256] = { 0 };
		if (t == TYPE_S16) {
			s16* res = (s16*)malloc(sizeof(s16));
			sprintf(buffer, "%.*s", exp->lit.lit_tok->value.length, exp->lit.lit_tok->value.data);
			*res = (s16)atoi(buffer);
			r.data = res;
			r.t = t;
		} else if (t == TYPE_S32) {
			s32* res = (s32*)malloc(sizeof(s32));
			sprintf(buffer, "%.*s", exp->lit.lit_tok->value.length, exp->lit.lit_tok->value.data);
			*res = (s32)atoi(buffer);
			r.data = res;
			r.t = t;
		} else if (t == TYPE_S64) {
			s64* res = (s64*)malloc(sizeof(s64));
			sprintf(buffer, "%.*s", exp->lit.lit_tok->value.length, exp->lit.lit_tok->value.data);
			*res = (s64)atoi(buffer);
			r.data = res;
			r.t = t;
		} else if(t == TYPE_S8) {
			s8* res = (s8*)malloc(sizeof(s8));
			sprintf(buffer, "%.*s", exp->lit.lit_tok->value.length, exp->lit.lit_tok->value.data);
			*res = (s8)atoi(buffer);
			r.data = res;
			r.t = t;
		} else if (t == TYPE_U16) {
			u16* res = (u16*)malloc(sizeof(u16));
			sprintf(buffer, "%.*s", exp->lit.lit_tok->value.length, exp->lit.lit_tok->value.data);
			*res = (u16)atoi(buffer);
			r.data = res;
			r.t = t;
		}
		else if (t == TYPE_U32) {
			u32* res = (u32*)malloc(sizeof(u32));
			sprintf(buffer, "%.*s", exp->lit.lit_tok->value.length, exp->lit.lit_tok->value.data);
			*res = (u32)atoi(buffer);
			r.data = res;
			r.t = t;
		}
		else if (t == TYPE_U64) {
			u64* res = (u64*)malloc(sizeof(s64));
			sprintf(buffer, "%.*s", exp->lit.lit_tok->value.length, exp->lit.lit_tok->value.data);
			*res = (u64)atoi(buffer);
			r.data = res;
			r.t = t;
		}
		else if (t == TYPE_U8) {
			u8* res = (u8*)malloc(sizeof(u8));
			sprintf(buffer, "%.*s", exp->lit.lit_tok->value.length, exp->lit.lit_tok->value.data);
			*res = (u8)atoi(buffer);
			r.data = res;
			r.t = t;
		}
		else if (t == TYPE_R32) {
			float* res = (float*)malloc(sizeof(float));
			sprintf(buffer, "%.*s", exp->lit.lit_tok->value.length, exp->lit.lit_tok->value.data);
			*res = (float)atof(buffer);
			r.data = res;
			r.t = t;
		}
		else if (t == TYPE_R64) {
			double* res = (double*)malloc(sizeof(double));
			sprintf(buffer, "%.*s", exp->lit.lit_tok->value.length, exp->lit.lit_tok->value.data);
			*res = (double)atof(buffer);
			r.data = res;
			r.t = t;
		}
		return r;
	}
	else if (exp->type == AST_VARIABLE) {
		Evaluated r;
		r.data = 0;
		return r;
	}
}

extern DebugTable debug_table;

Evaluated interpret(Ast* exp)
{
	if (exp->type == AST_ASSIGNMENT) {
		Evaluated evald = eval_expr(exp->assign.right);
		if (exp->assign.left->type == AST_VARIABLE) {
			char buffer[256] = { 0 };
			strncpy(buffer, exp->assign.left->var.name->value.data, exp->assign.left->var.name->value.length);
			DebugTable_Entry entry = debug_table.find_in_entries(buffer);
			Evaluated right_evald = eval_expr(exp->assign.right);

			switch (right_evald.t) {
			case TYPE_S64:	*(s64*)entry.data = *(s64*)right_evald.data;		   break;
			case TYPE_S32:	*(s32*)entry.data = *(s32*)right_evald.data;		   break;
			case TYPE_S16:	*(s16*)entry.data = *(s16*)right_evald.data;		   break;
			case TYPE_S8:	*(s8*)entry.data = *(s8*)right_evald.data;			   break;
			case TYPE_U64:	*(u64*)entry.data = *(u64*)right_evald.data;		   break;
			case TYPE_U32:	*(u32*)entry.data = *(u32*)right_evald.data;		   break;
			case TYPE_U16:	*(u16*)entry.data = *(u16*)right_evald.data;		   break;
			case TYPE_U8:	*(u8*)entry.data = *(u8*)right_evald.data;			   break;
			case TYPE_R32:	*(float*)entry.data = *(float*)right_evald.data;	   break;
			case TYPE_R64:	*(double*)entry.data = *(double*)right_evald.data;	   break;
			}
		}
		return Evaluated();
	} else {
		Evaluated eval = eval_expr(exp);
		return eval;
	}
}