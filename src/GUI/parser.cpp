#include "parser.h"
#include <stdio.h>
#include <stdarg.h>
#include "../util.h"
#include "../render_engine.h"
#include <dynamic_array.h>
#include <new>
#include <ho_system.h>

Parser::Parser(Lexer* lexer) {
	this->lexer = lexer;
}

#define REQUIRE_AND_EAT_OPEN_TAG if(require_and_eat('<') == -1) return -1
#define REQUIRE_AND_EAT_CLOSE_TAG if(require_and_eat('>') == -1) return -1
#define REQUIRE_AND_EAT_TOKEN(X) if(require_and_eat(X) == -1) return -1
#define CHECK_STR_EQUAL(S, Y) str_equal((const char*)S.data, S.length, Y, sizeof(Y) - 1)

hm::vec4 hex_to_vec4(string s) {
	hm::vec4 result = { 0.0f, 0.0f, 0.0f, 1.0f };
	if (s.length == 4 || s.length == 5) {
		// #000
		u8 r_ = hex_from_ascii(s.data[1]);
		u8 g_ = hex_from_ascii(s.data[2]);
		u8 b_ = hex_from_ascii(s.data[3]);
		u8 r = r_ | (r_ << 4);
		u8 g = g_ | (g_ << 4);
		u8 b = b_ | (b_ << 4);
		result.x = (r32)r / 255.0f;
		result.y = (r32)g / 255.0f;
		result.z = (r32)b / 255.0f;
		if (s.length == 5) {
			u8 a_ = hex_from_ascii(s.data[4]);
			u8 a = a_ | (a_ << 4);
			result.w = (r32)a / 255.0f;
		}
	} else if (s.length == 7 || s.length == 9) {
		// #0055FF
		u8 r_0 = hex_from_ascii(s.data[1]);
		u8 r_1 = hex_from_ascii(s.data[2]);
		u8 g_0 = hex_from_ascii(s.data[3]);
		u8 g_1 = hex_from_ascii(s.data[4]);
		u8 b_0 = hex_from_ascii(s.data[5]);
		u8 b_1 = hex_from_ascii(s.data[6]);
		u8 r = r_1 | (r_0 << 4);
		u8 g = g_1 | (g_0 << 4);
		u8 b = b_1 | (b_0 << 4);
		result.x = (r32)r / 255.0f;
		result.y = (r32)g / 255.0f;
		result.z = (r32)b / 255.0f;

		if (s.length == 9) {
			u8 a_0 = hex_from_ascii(s.data[7]);
			u8 a_1 = hex_from_ascii(s.data[8]);
			u8 a = a_1 | (a_0 << 4);
			result.w = (r32)a / 255.0f;
		}
	}
	return result;
}

s32 Parser::parse_margin(gui::Div_Style* style) {
	Token* next = lexer->eat_token();
	bool negate_next = false;
	if (next->type == ':') {
		r32 margin_value = 0.0f;
		next = lexer->eat_token();
		if (next->type == '-') {
			negate_next = true;
			next = lexer->eat_token();
		}
		if (next->type == TOKEN_INT_LITERAL) {
			margin_value = str_to_r32(next->value);
			if (negate_next) {
				margin_value = -margin_value;
				negate_next = false;
			}
			style->margin_size.x = margin_value;
			next = lexer->eat_token();
			if (!CHECK_STR_EQUAL(next->value, "px")) {
				report_syntax_error(next, "invalid value, missing 'px' specifier.\n");
				return -1;
			}
		} else if (CHECK_STR_EQUAL(next->value, "auto")) {
			style->flags |= gui::DIV_MARGIN_AUTO;
			REQUIRE_AND_EAT_TOKEN(';');
			return 0;
		} else {
			report_syntax_error(next, "expected pixel value specification on margin declaration.\n");
			return -1;
		}
		next = lexer->eat_token();
		if (next->type == '-') {
			negate_next = true;
			next = lexer->eat_token();
		}
		if (next->type == TOKEN_INT_LITERAL) {
			r32 margin_value_right = str_to_r32(next->value);
			if (negate_next) {
				margin_value_right = -margin_value_right;
				negate_next = false;
			}
			style->margin_size.y = margin_value_right;
			next = lexer->eat_token();
			if(!CHECK_STR_EQUAL(next->value, "px")) {
				report_syntax_error(next, "invalid value, missing 'px' specifier.\n");
				return -1;
			}
			next = lexer->eat_token();
			if (next->type == '-') {
				negate_next = true;
				next = lexer->eat_token();
			}
			if (next->type != TOKEN_INT_LITERAL) {
				report_syntax_error(next, "margin declaration must have 1 or 4 pixel specifiers [left right top bottom].\n");
				return -1;
			}
			r32 margin_value_top = str_to_r32(next->value);
			if (negate_next) {
				margin_value_top = -margin_value_top;
				negate_next = false;
			}
			style->margin_size.z = margin_value_top;
			next = lexer->eat_token();
			if (!CHECK_STR_EQUAL(next->value, "px")) {
				report_syntax_error(next, "invalid value, missing 'px' specifier.\n");
				return -1;
			}
			next = lexer->eat_token();
			if (next->type == '-') {
				negate_next = true;
				next = lexer->eat_token();
			}
			if (next->type != TOKEN_INT_LITERAL) {
				report_syntax_error(next, "margin declaration must have 1 or 4 pixel specifiers [left right top bottom].\n");
				return -1;
			}
			r32 margin_value_bottom = str_to_r32(next->value);
			if (negate_next) {
				margin_value_bottom = -margin_value_bottom;
				negate_next = false;
			}
			style->margin_size.w = margin_value_bottom;
			next = lexer->eat_token();
			if (!CHECK_STR_EQUAL(next->value, "px")) {
				report_syntax_error(next, "invalid value, missing 'px' specifier.\n");
				return -1;
			}
			REQUIRE_AND_EAT_TOKEN(';');
		} else if (next->type == ';') {
			style->margin_size = { margin_value, margin_value, margin_value, margin_value };
			return 0;
		} else if (next->type == TOKEN_END_OF_STREAM) {
			report_syntax_error(next, "unexpected end of stream in margin declaration.\n");
			return -1;
		} else {
			report_syntax_error(next, "expected ';' but got '%s'.\n", next->value.data);
			return -1;
		}
	}
	else if (next->type == '-') {
		next = lexer->eat_token();
		int error = 0;
		REQUIRE_AND_EAT_TOKEN(':');
		Token* value = lexer->eat_token();
		if (value->type == '-') {
			negate_next = true;
			value = lexer->eat_token();
		}
		if (CHECK_STR_EQUAL(next->value, "left")) {
			if (value->type == TOKEN_INT_LITERAL) {
				style->margin_size.x = str_to_r32(value->value);
				if (negate_next) style->margin_size.x = -style->margin_size.x;
			} else
				error = -1;
		} else if (CHECK_STR_EQUAL(next->value, "right")) {
			if (value->type == TOKEN_INT_LITERAL) {
				style->margin_size.y = str_to_r32(value->value);
				if (negate_next) style->margin_size.y = -style->margin_size.y;
			} else
				error = -1;
		} else if (CHECK_STR_EQUAL(next->value, "top")) {
			if (value->type == TOKEN_INT_LITERAL) {
				style->margin_size.z = str_to_r32(value->value);
				if (negate_next) style->margin_size.z = -style->margin_size.z;
			} else
				error = -1;
		} else if (CHECK_STR_EQUAL(next->value, "bottom")) {
			if (value->type == TOKEN_INT_LITERAL) {
				style->margin_size.w = str_to_r32(value->value);
				if (negate_next) style->margin_size.w = -style->margin_size.w;
			} else
				error = -1;
		} else if (next->type == TOKEN_END_OF_STREAM) {
			report_syntax_error(next, "unexpected end of stream in margin declaration.\n");
			return -1;
		} else {
			report_syntax_error(next, "invalid margin selector '%s', valid values are [left right top bottom].\n", next->value.data);
			return -1;
		}
		if (error == -1) {
			report_syntax_error(value, "expected integer value as first argument of margin specification.\n");
			return -1;
		}
		next = lexer->eat_token();
		if (!CHECK_STR_EQUAL(next->value, "px")) {
			report_syntax_error(next, "invalid value, missing 'px' specifier.\n");
			return -1;
		}
		REQUIRE_AND_EAT_TOKEN(';');
		return 0;
	}
	else {
		report_syntax_error(next, "invalid margin property, expected '-' followed by valid property or ':' and specification.\n");
		return -1;
	}
	return 0;
}

s32 Parser::parse_padding(gui::Div_Style* style) {
	Token* next = lexer->eat_token();
	if (next->type == ':') {
		r32 padding_value = 0.0f;
		next = lexer->eat_token();
		if (next->type == TOKEN_INT_LITERAL) {
			padding_value = str_to_r32(next->value);
			style->padding_size.x = padding_value;
			next = lexer->eat_token();
			if (!CHECK_STR_EQUAL(next->value, "px")) {
				report_syntax_error(next, "invalid value, missing 'px' specifier.\n");
				return -1;
			}
		} else {
			report_syntax_error(next, "expected pixel value specification in padding declaration.\n");
			return -1;
		}
		next = lexer->eat_token();
		if (next->type == TOKEN_INT_LITERAL) {
			r32 padding_value_right = str_to_r32(next->value);
			style->padding_size.y = padding_value_right;
			next = lexer->eat_token();
			if (!CHECK_STR_EQUAL(next->value, "px")) {
				report_syntax_error(next, "invalid value, missing 'px' specifier.\n");
				return -1;
			}
			next = lexer->eat_token();
			if (next->type != TOKEN_INT_LITERAL) {
				report_syntax_error(next, "padding declaration must have 1 or 4 pixel specifiers [left right top bottom].\n");
				return -1;
			}
			r32 padding_value_top = str_to_r32(next->value);
			style->padding_size.z = padding_value_top;
			next = lexer->eat_token();
			if (!CHECK_STR_EQUAL(next->value, "px")) {
				report_syntax_error(next, "invalid value, missing 'px' specifier.\n");
				return -1;
			}
			next = lexer->eat_token();
			if (next->type != TOKEN_INT_LITERAL) {
				report_syntax_error(next, "padding declaration must have 1 or 4 pixel specifiers [left right top bottom].\n");
				return -1;
			}
			r32 padding_value_bottom = str_to_r32(next->value);
			style->padding_size.w = padding_value_bottom;
			next = lexer->eat_token();
			if (!CHECK_STR_EQUAL(next->value, "px")) {
				report_syntax_error(next, "invalid value, missing 'px' specifier.\n");
				return -1;
			}
			REQUIRE_AND_EAT_TOKEN(';');
		} else if (next->type == ';') {
			style->padding_size = { padding_value, padding_value, padding_value, padding_value };
			return 0;
		} else if (next->type == TOKEN_END_OF_STREAM) {
			report_syntax_error(next, "unexpected end of stream in padding declaration.\n");
			return -1;
		} else {
			report_syntax_error(next, "expected ';' but got '%s'.\n", next->value.data);
			return -1;
		}
	} else if (next->type == '-') {
		next = lexer->eat_token();
		int error = 0;
		REQUIRE_AND_EAT_TOKEN(':');
		Token* value = lexer->eat_token();
		if (CHECK_STR_EQUAL(next->value, "left")) {
			if (value->type == TOKEN_INT_LITERAL)
				style->padding_size.x = str_to_r32(value->value);
			else
				error = -1;
		} else if (CHECK_STR_EQUAL(next->value, "right")) {
			if (value->type == TOKEN_INT_LITERAL)
				style->padding_size.y = str_to_r32(value->value);
			else
				error = -1;
		} else if (CHECK_STR_EQUAL(next->value, "top")) {
			if (value->type == TOKEN_INT_LITERAL)
				style->padding_size.z = str_to_r32(value->value);
			else
				error = -1;
		} else if (CHECK_STR_EQUAL(next->value, "bottom")) {
			if (value->type == TOKEN_INT_LITERAL)
				style->padding_size.w = str_to_r32(value->value);
			else
				error = -1;
		} else if (next->type == TOKEN_END_OF_STREAM) {
			report_syntax_error(next, "unexpected end of stream in padding declaration.\n");
			return -1;
		} else {
			report_syntax_error(next, "invalid padding selector '%s', valid values are [left right top bottom].\n", next->value.data);
			return -1;
		}
		if (error == -1) {
			report_syntax_error(value, "expected integer value as first argument of padding specification.\n");
			return -1;
		}
		next = lexer->eat_token();
		if (!CHECK_STR_EQUAL(next->value, "px")) {
			report_syntax_error(next, "invalid value, missing 'px' specifier.\n");
			return -1;
		}
		REQUIRE_AND_EAT_TOKEN(';');
		return 0;
	} else {
		report_syntax_error(next, "invalid padding property, expected '-' followed by valid property or ':' and specification.\n");
		return -1;
	}
	return 0;
}

s32 Parser::parse_border(gui::Div_Style* style) {
	if (!style->border) {
		void* border_mem = arena_alloc(lexer->arena, sizeof(engine::Border_2D));
		style->border = new (border_mem) engine::Border_2D();
		engine::border_create(style->border, hm::vec3(0, 0, 0), hm::vec4(0, 0, 0, 0), 0, 0);
	}
	engine::Border_2D* border = style->border;
	// 1 = left, 2 = right, 3 = top, 4 = bottom
	u32 specific_selector = 0;
	Token* next = lexer->eat_token();
	if (next->type == ':') {
		next = lexer->eat_token();
		if (next->type == TOKEN_INT_LITERAL) {
			r32 border_val = str_to_r32(next->value);
			border->size.x = border_val;
			border->size.y = border_val;
			border->size.z = border_val;
			border->size.w = border_val;
		} else {
			report_syntax_error(next, "expected pixel value specification in border declaration.\n");
			return -1;
		}
	} else if (next->type == '-') {
		next = lexer->eat_token();
		int error = 0;
		REQUIRE_AND_EAT_TOKEN(':');
		Token* value = lexer->eat_token();
		if (CHECK_STR_EQUAL(next->value, "left")) {
			specific_selector = 1;
			if (value->type == TOKEN_INT_LITERAL)
				border->size.x = str_to_r32(value->value);
			else
				error = -1;
		} else if (CHECK_STR_EQUAL(next->value, "right")) {
			specific_selector = 2;
			if (value->type == TOKEN_INT_LITERAL)
				border->size.y = str_to_r32(value->value);
			else
				error = -1;
		} else if (CHECK_STR_EQUAL(next->value, "top")) {
			specific_selector = 3;
			if (value->type == TOKEN_INT_LITERAL)
				border->size.z = str_to_r32(value->value);
			else
				error = -1;
		} else if (CHECK_STR_EQUAL(next->value, "bottom")) {
			specific_selector = 4;
			if (value->type == TOKEN_INT_LITERAL)
				border->size.w = str_to_r32(value->value);
			else
				error = -1;
		} else if (next->type == TOKEN_END_OF_STREAM) {
			report_syntax_error(next, "unexpected end of stream in border declaration.\n");
			return -1;
		} else {
			report_syntax_error(next, "invalid border selector '%s', valid values are [left right top bottom].\n", next->value.data);
			return -1;
		}
		if (error == -1) {
			report_syntax_error(value, "expected integer value as first argument of border specification.\n");
			return -1;
		}
	} else {
		report_syntax_error(next, "invalid border property, expected '-' followed by valid property or ':' and specification.\n");
		return -1;
	}
	next = lexer->eat_token();
	if (!CHECK_STR_EQUAL(next->value, "px")) {
		report_syntax_error(next, "invalid value, missing 'px' specifier.\n");
		return -1;
	}
	next = lexer->eat_token();
	if (CHECK_STR_EQUAL(next->value, "solid")) {		
		next = lexer->eat_token();
		hm::vec4 color_parsed;
		if (next->type == TOKEN_HEX_LITERAL) {
			color_parsed = hex_to_vec4(next->value);
		} else if (CHECK_STR_EQUAL(next->value, "red")) {
			color_parsed = { 1.0f, 0.0f, 0.0f, 1.0f };
		} else if (CHECK_STR_EQUAL(next->value, "green")) {
			color_parsed = { 0.0f, 1.0f, 0.0f, 1.0f };
		} else if (CHECK_STR_EQUAL(next->value, "blue")) {
			color_parsed = { 0.0f, 0.0f, 1.0f, 1.0f };
		} else if (CHECK_STR_EQUAL(next->value, "gray")) {
			color_parsed = { 0.5f, 0.5f, 0.5f, 1.0f };
		} else if (CHECK_STR_EQUAL(next->value, "black")) {
			color_parsed = { 0.0f, 0.0f, 0.0f, 1.0f };
		} else if (CHECK_STR_EQUAL(next->value, "white")) {
			color_parsed = { 1.0f, 1.0f, 1.0f, 1.0f };
		} else if (CHECK_STR_EQUAL(next->value, "yellow")) {
			color_parsed = { 1.0f, 1.0f, 0.0f, 1.0f };
		} else if (CHECK_STR_EQUAL(next->value, "magenta")) {
			color_parsed = { 1.0f, 0.0f, 1.0f, 1.0f };
		} else if (CHECK_STR_EQUAL(next->value, "cyan")) {
			color_parsed = { 0.0f, 1.0f, 1.0f, 1.0f };
		} else {
			if (next->type == TOKEN_END_OF_STREAM) {
				report_syntax_error(next, "unexpected end of stream in color specifier of border.\n");
			} else {
				report_syntax_error(next, "invalid color '%s' in border specifier.\n", next->value.data);
			}
			return -1;
		}
		if (specific_selector == 0) {
			border->color_left = color_parsed;
			border->color_right = color_parsed;
			border->color_top = color_parsed;
			border->color_bottom = color_parsed;
		} else if (specific_selector == 1) {
			border->color_left = color_parsed;
		} else if (specific_selector == 2) {
			border->color_right = color_parsed;
		} else if (specific_selector == 3) {
			border->color_top = color_parsed;
		} else if (specific_selector == 4) {
			border->color_bottom = color_parsed;
		}
	} else {
		report_syntax_error(next, "invalid specifier '%s', only valid is 'solid'.\n", next->value.data);
		return -1;
	}
	REQUIRE_AND_EAT_TOKEN(';');
	return 0;
}

s32 Parser::parse_width(gui::Div_Style* style) {
	REQUIRE_AND_EAT_TOKEN(':');
	Token* next = lexer->eat_token();
	if (next->type == TOKEN_INT_LITERAL) {
		style->f_width = str_to_r32(next->value);
		style->width = (s32)style->f_width;
	}
	next = lexer->eat_token();
	if (next->type == '%') {
		style->flags |= gui::DIV_WIDTH_PERCENT;
		style->f_width /= 100.0f;
	} else if (CHECK_STR_EQUAL(next->value, "px")) {
	} else {
		report_syntax_error(next, "invalid value, missing 'px' or '%' specifier for width declaration.\n");
		return -1;
	}
	REQUIRE_AND_EAT_TOKEN(';');
	return 0;
}

s32 Parser::parse_height(gui::Div_Style* style) {
	REQUIRE_AND_EAT_TOKEN(':');
	Token* next = lexer->eat_token();
	if (next->type == TOKEN_INT_LITERAL) {
		style->f_height = str_to_r32(next->value);
		style->height = (s32)style->f_height;
	}
	next = lexer->eat_token();
	if (next->type == '%') {
		style->flags |= gui::DIV_WIDTH_PERCENT;
		style->f_height /= 100.0f;
	} else if (CHECK_STR_EQUAL(next->value, "px")) {
	} else {
		report_syntax_error(next, "invalid value, missing 'px' or '%' specifier for width declaration.\n");
		return -1;
	}
	REQUIRE_AND_EAT_TOKEN(';');
	return 0;
}

s32 Parser::parse_text(gui::Div_Style* style) {
	REQUIRE_AND_EAT_TOKEN('-');
	Token* next = lexer->eat_token();
	if (!CHECK_STR_EQUAL(next->value, "align")) {
		if (next->type == TOKEN_END_OF_STREAM)
			report_syntax_error(next, "unexpected end of stream in text-align declaration.\n");
		else
			report_syntax_error(next, "invalid accesser '%s' in text.\n", next->value.data);
	}
	REQUIRE_AND_EAT_TOKEN(':');
	Token* align = lexer->eat_token();
	if (CHECK_STR_EQUAL(align->value, "center")) {
		style->text_align = gui::TEXT_ALIGN_CENTER;
	} else if (CHECK_STR_EQUAL(align->value, "left")) {
		style->text_align = gui::TEXT_ALIGN_LEFT;
	} else if (CHECK_STR_EQUAL(align->value, "right")) {
		style->text_align = gui::TEXT_ALIGN_RIGHT;
	} else {
		if (align->type == TOKEN_END_OF_STREAM)
			report_syntax_error(align, "unexpected end of stream in text-align declaration.\n");
		else
			report_syntax_error(align, "invalid parameter '%s' in text-align.\n", align->value.data);
		return -1;
	}
	REQUIRE_AND_EAT_TOKEN(';');
	return 0;
}

s32 Parser::parse_background(gui::Div_Style* style) {
	REQUIRE_AND_EAT_TOKEN('-');
	Token* next = lexer->eat_token();
	REQUIRE_AND_EAT_TOKEN(':');

	if (CHECK_STR_EQUAL(next->value, "color")) {
		Token* color = lexer->eat_token();
		if (color->type == TOKEN_HEX_LITERAL) {
			style->color_bg = hex_to_vec4(color->value);
		} else {
			if (next->type == TOKEN_END_OF_STREAM)
				report_syntax_error(color, "unexpected end of stream in background declaration.\n");
			else
				report_syntax_error(color, "invalid color declaration, must be '#' followed by hexadecimal_literal.\n");
			return -1;
		}
	} else if (CHECK_STR_EQUAL(next->value, "image")) {
		string filepath = make_string("./res/images/");
		Token* next = lexer->eat_token();
		
		string filename = concat(&filepath, &next->value, false);
		style->texture_bg = new (arena_alloc(lexer->arena, sizeof(Texture))) Texture((const char*)filename.data);
		free_string(&filepath);
		free_string(&filename);

		if (!style->texture_bg->valid) {
			report_syntax_error(next, "file '%s' not found or is not a valid image.\n", next->value.data);
			return -1;
		}
		style->flags |= gui::DIV_BACKGROUND_TEXTURE;
	} else {
		if(next->type == TOKEN_END_OF_STREAM)
			report_syntax_error(next, "unexpected end of stream in background declaration.\n");
		else
			report_syntax_error(next, "invalid selector '%s' in background declaration.\n", next->value.data);
		return -1;
	}
	REQUIRE_AND_EAT_TOKEN(';');
	return 0;
}

s32 Parser::parse_font(gui::Div_Style* style) {
	REQUIRE_AND_EAT_TOKEN(':');
	string path = make_string("./res/fonts/");
	// font:'LiberationMono-Regular.ttf' 16px;
	Token* name = lexer->eat_token();
	string filename = concat(&path, &name->value);
	free_string(&path);

	if (!ho_file_exist((const char*)filename.data)) {
#if defined(_WIN64) || defined(_WIN32)
		path = make_string("C:\\Windows\\Fonts\\");
#elif defined(__linux__)
#error NOT YET SUPPORTED
		//path = make_string()
#endif
		free_string(&filename);
		filename = concat(&path, &name->value);

		if (!ho_file_exist((const char*)filename.data)) {
			report_syntax_error(name, "font file '%s' does not exist.\n", name->value.data);
			return -1;
		}
	}

	Token* next = lexer->eat_token();
	if (next->type == TOKEN_INT_LITERAL) {
		u32 type_size = (u32)str_to_r32(next->value);
		next = lexer->eat_token();
		if (!CHECK_STR_EQUAL(next->value, "px")) {
			report_syntax_error(next, "invalid value, missing 'px' specifier.\n");
			return -1;
		}
		Font_ID id = load_font_not_repeat(filename, type_size);
		style->font_family = id;
		style->font_size = type_size;
		free_string(&filename);
	} else {
		report_syntax_error(next, "expected integer, size of font.\n");
		return -1;
	}
	REQUIRE_AND_EAT_TOKEN(';');
	return 0;
}

s32 Parser::parse_style(gui::Div_Style* style) {
	REQUIRE_AND_EAT_TOKEN('=');
	REQUIRE_AND_EAT_TOKEN('"');
	int err = 0;
	do {
		Token* param = lexer->eat_token();
		if (CHECK_STR_EQUAL(param->value, "margin")) {
			err = parse_margin(style);
		} else if (CHECK_STR_EQUAL(param->value, "width")) {
			err = parse_width(style);
		} else if (CHECK_STR_EQUAL(param->value, "height")) {
			err = parse_height(style);
		} else if (CHECK_STR_EQUAL(param->value, "border")) {
			err = parse_border(style);
		} else if (CHECK_STR_EQUAL(param->value, "text")) {
			err = parse_text(style);
		} else if (CHECK_STR_EQUAL(param->value, "background")) {
			err = parse_background(style);
		} else if (CHECK_STR_EQUAL(param->value, "padding")) {
			err = parse_padding(style);
		} else if (CHECK_STR_EQUAL(param->value, "font")) {
			err = parse_font(style);
		} else if (param->type == '"') {
			break;
		} else {
			report_syntax_error(param, "invalid parameter '%s'.\n", param->value.data);
			return -1;
		}
		if (err) return err;
	} while (true);
	
	if (err) return err;
	return 0;
}

s32 Parser::parse_div(size_t parent) {
	gui::Div_Style* style = new (arena_alloc(lexer->arena, sizeof(gui::Div_Style))) gui::Div_Style();

	// <div
	// already parsed
	Token* tok = lexer->eat_token();
	if (tok->type == TOKEN_KEYWORD_CLASS) {
		assert(0); // @implement
	} else if (tok->type == TOKEN_KEYWORD_STYLE) {
		int err = parse_style(style);
		if (err) return err;
		REQUIRE_AND_EAT_CLOSE_TAG;
	} else if(tok->type == '>'){
		// use default style
	}
	size_t div = gui::div_create(parent, style);
	gui::div_push_child(parent, div);

	Token* str = lexer->eat_token();
	string div_string;
	while (str->type != '<') {
		if (div_string.length != 0) {
			div_string = concat(&div_string, " ");
			div_string = concat(&div_string, &str->value, false);
		} else {
			div_string = make_string(lexer->arena, (s8*)str->value.data, str->value.length);
		}
		str = lexer->eat_token();
		if (str->type == '<' && lexer->peek_token_type() == TOKEN_KEYWORD_DIV) {
			lexer->eat_token();	// eat div
			int err = parse_div(div);
			if (err) return err;
			str = lexer->eat_token();
		}
	}
	REQUIRE_AND_EAT_TOKEN('/');
	REQUIRE_AND_EAT_TOKEN(TOKEN_KEYWORD_DIV);
	REQUIRE_AND_EAT_CLOSE_TAG;
	gui::div_set_text(div, div_string);
	return 0;
}

s32 Parser::parse_body() {
	do {
		REQUIRE_AND_EAT_OPEN_TAG;
		Token* tag = lexer->eat_token();
		if (tag->type == TOKEN_KEYWORD_DIV) {
			int err = parse_div(-1);
			if(err) return err;
		} else if (tag->type == TOKEN_KEYWORD_BUTTON) {
			assert(0); // @implement
		} else if (tag->type == TOKEN_KEYWORD_STYLE) {
			assert(0); // @implement
		} else {
			break;
		}
	} while (true);
	return 0;
}

s32 Parser::parse_top_level() {
	REQUIRE_AND_EAT_OPEN_TAG;
	Token* tag = lexer->eat_token();
	if (tag->type != TOKEN_KEYWORD_BODY) {
		return report_syntax_error(tag, "expected body tag at beggining of file '%s'\n", lexer->filename.data);
	}
	REQUIRE_AND_EAT_CLOSE_TAG;
	REQUIRE_AND_EAT_OPEN_TAG;
	tag = lexer->eat_token();
	if (tag->type == '/') {
		REQUIRE_AND_EAT_TOKEN(TOKEN_KEYWORD_BODY);
		REQUIRE_AND_EAT_CLOSE_TAG;
	} else {
		lexer->rewind();
		lexer->rewind();
		return parse_body();
	}
	return 0;
}

s32 Parser::require_and_eat(Token_Type type) {
	Token* t = lexer->eat_token();
	if (t->type != type) {
		if(t->type == TOKEN_END_OF_STREAM)
			return report_syntax_error(t, "unexpected end of stream.\n");
		return report_syntax_error(t, "expected '%s' but got '%s'\n", lexer->get_token_string(type), t->value.data);
	}
	return 0;
}

s32 Parser::require_and_eat(s8 type) {
	Token* t = lexer->eat_token();
	if (t->type != type) {
		if (t->type == TOKEN_END_OF_STREAM)
			return report_syntax_error(t, "unexpected end of stream.\n");
		return report_syntax_error(t, "expected '%c' but got '%s'\n", type, t->value.data);
	}
	return 0;
}

s32 Parser::report_syntax_error(Token* loc_token, char* msg, ...) {
	va_list args;
	va_start(args, msg);
	fprintf(stdout, "%s (%d:%d): Syntax Error: ", loc_token->filename.data, loc_token->line, loc_token->column);
	vfprintf(stdout, msg, args);
	va_end(args);
	return -1;
}