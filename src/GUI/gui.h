#pragma once
#include <hmath.h>
#include "GL/glew.h"

#include "../common.h"
#include "../font_rendering.h"
#include "../render_engine.h"

struct Texture;

namespace gui {
	
	enum Text_Alignment {
		TEXT_ALIGN_LEFT,
		TEXT_ALIGN_RIGHT,
		TEXT_ALIGN_CENTER,
	};

	extern Font_ID default_font_16;

	const u64 DIV_POSITION_ABSOLUTE_ON = FLAG(0);
	const u64 DIV_POSITION_ABSOLUTE_PERCENT = FLAG(1);
	const u64 DIV_POSITION_RELATIVE_PERCENT = FLAG(2);
	const u64 DIV_WIDTH_PERCENT  = FLAG(3);
	const u64 DIV_HEIGHT_PERCENT = FLAG(4);
	const u64 DIV_BACKGROUND_TEXTURE = FLAG(5);
	const u64 DIV_BORDER_ON = FLAG(6);
	const u64 DIV_MARGIN_AUTO = FLAG(7);

	struct Div_Style {
		s32 width;
		s32 height;
		r32 f_width;
		r32 f_height;

		s32 z_index;
		Text_Alignment text_align;

		hm::vec2 position_absolute;
		hm::vec2 position_relative;

		hm::vec4 margin_size;	// left_right top bottom
		hm::vec4 padding_size;	// left right top bottom

		hm::vec4 color_bg;

		engine::Border_2D* border;

		hm::vec4 text_color;

		Texture* texture_bg;

		Font_ID font_family;
		s64 font_size;

		u64 flags;

		Div_Style() 
			: width(0), height(0), f_width(0.0f), f_height(0.0f), z_index(0), text_align(TEXT_ALIGN_LEFT),
			position_absolute({0.0f, 0.0f}), position_relative({0.0f, 0.0f}), margin_size({0.0f, 0.0f, 0.0f, 0.0f}),
			padding_size({0.0f, 0.0f, 0.0f, 0.0f}), color_bg({0.0f, 0.0f, 0.0f, 0.0f}), border(0),
			text_color({0.0f, 0.0f, 0.0f, 1.0f}), texture_bg(0), font_family(default_font_16), font_size(16), flags(0)
		{
		}
	};

	struct Div {
		Div_Style* style;
		size_t* children;
		Div* parent;
		string text;
		engine::Quad_2D div_quad;
	};

	void gui_init();
	void gui_release();

	size_t div_create(s8* str, size_t parent, Div_Style* style);
	size_t div_create(string str, size_t parent, Div_Style* style);
	size_t div_create(size_t parent, Div_Style* style);
	void div_push_child(size_t parent, size_t child);
	Div_Style* div_get_style(size_t div);
	void div_set_text(size_t index, string s);
	void div_release(Div* div);
	void div_render(size_t index, hm::vec2& base);
	void div_render_all();

	hm::vec2 style_get_base_position(Div* div);
}