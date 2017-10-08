#pragma once
#include "lexer.h"
#include "gui.h"

struct Parser {
	Lexer* lexer;

	Parser(Lexer* lexer);
	s32 parse_top_level();
	s32 parse_body();
	s32 parse_div(size_t parent);
	s32 parse_style(gui::Div_Style* style);

	s32 parse_margin(gui::Div_Style* style);
	s32 parse_padding(gui::Div_Style* style);
	s32 parse_border(gui::Div_Style* style);
	s32 parse_width(gui::Div_Style* style);
	s32 parse_height(gui::Div_Style* style);
	s32 parse_text(gui::Div_Style* style);
	s32 parse_background(gui::Div_Style* style);
	s32 parse_font(gui::Div_Style* style);

	s32 require_and_eat(Token_Type type);
	s32 require_and_eat(s8 type);
	s32 report_syntax_error(Token* loc_token, char* msg, ...);
};