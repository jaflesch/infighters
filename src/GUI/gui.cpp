#include "gui.h"
#include "ho_system.h"
#include "../render_engine.h"
#include "../os.h"
#include "lexer.h"
#include "parser.h"

#if defined(_WIN64)
#define DEFAULT_FONT "C:\\Windows\\fonts\\arial.ttf"
#elif defined(__linux__)
#define DEFAULT_FONT "./res/fonts/LiberationMono-Regular.ttf"
#endif

extern Window_Info window_info;

namespace gui {

	Div* divs = 0;
	int num_divs = 0;
	Font_ID default_font_16 = -1;

	void gui_init() {
		divs = array_create(Div, 16);
		default_font_16 = load_font(DEFAULT_FONT, 16);

		Lexer lexer;
		lexer.start("./res/h.html");
		Parser parser(&lexer);
		parser.parse_top_level();
	}

	void gui_release() {
		array_release(divs);
	}

	void div_set_text(size_t index, string s) {
		divs[index].text = s;
	}

	void div_push_child(size_t parent, size_t child) {
		if(((s64)parent) < 0) return; 
		if (divs[parent].children == 0) {
			divs[parent].children = array_create(size_t, 1);
		}
		array_push(divs[parent].children, &child);
	}

	size_t div_create(string str, size_t parent, Div_Style* style) {
		size_t index = array_emplace(divs);
		engine::quad_create(&divs[index].div_quad, hm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
		engine::quad_change_data(&divs[index].div_quad, 0.0f, 0.0f, hm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		if (!style) {
			divs[index].style = new Div_Style();
		} else {
			divs[index].style = style;
		}
		divs[index].text = str;
		divs[index].children = 0;
		assert(parent != index);
		if (parent == -1)
			divs[index].parent = 0;
		else
			divs[index].parent = &divs[parent];
		num_divs++;
		return index;
	}

	size_t div_create(s8* str, size_t parent, Div_Style* style) {
		size_t index = array_emplace(divs);
		engine::quad_create(&divs[index].div_quad, hm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
		engine::quad_change_data(&divs[index].div_quad, 0.0f, 0.0f, hm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		if (!style) {
			divs[index].style = new Div_Style();
		} else {
			divs[index].style = style;
		}
		divs[index].text = make_string(str);
		divs[index].children = 0;
		assert(parent != index);
		if (parent == -1)
			divs[index].parent = 0;
		else
			divs[index].parent = &divs[parent];
		num_divs++;
		return index;
	}

	size_t div_create(size_t parent, Div_Style* style) {
		size_t index = array_emplace(divs);
		engine::quad_create(&divs[index].div_quad, hm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
		engine::quad_change_data(&divs[index].div_quad, 0.0f, 0.0f, hm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		if (!style) {
			divs[index].style = new Div_Style();
		}else {
			divs[index].style = style;
		}
		divs[index].text.length = 0;
		divs[index].text.data = 0;
		divs[index].children = 0;
		assert(parent != index);
		if (parent == -1)
			divs[index].parent = 0;
		else
			divs[index].parent = &divs[parent];
		num_divs++;
		return index;
	}

	void div_release() {

	}

	void div_render_all() {
		hm::vec2 base = {0.0f, 0.0f};
		for (int i = 0; i < num_divs; ++i) {
			if(divs[i].parent == 0)
				div_render(i, base);
		}
		font_rendering_flush();
	}

	void div_render(size_t index, hm::vec2& base) {
		using namespace hm;
		Div* div = &divs[index];

		r32 original_width = div->style->f_width, original_height = div->style->f_height;

		vec2 pos_info(0.0f, 0.0f);
		render_text_get_info(div->style->font_family, div->text, pos_info);

		// adapt div dimensions to text if needed
		r32 div_width = div->style->f_width, div_height = div->style->f_height;
		if (div->style->flags & DIV_WIDTH_PERCENT) {
			div_width = window_info.width * div->style->f_width;
			original_width = div_width;
		}
		if (div->style->flags & DIV_HEIGHT_PERCENT) {
			div_height = window_info.height * div->style->f_height;
			original_height = div_height;
		}
		div_width  = MAX(div_width, pos_info.x);
		div_height = MAX(div_height, pos_info.y);
		div_width  += div->style->padding_size.x + div->style->padding_size.y;
		div_height += div->style->padding_size.z + div->style->padding_size.w;

		vec2 render_pos = style_get_base_position(div) + base;
		render_pos = vec2(render_pos.x, window_info.height - render_pos.y - div_height);

		if (div->style->flags & DIV_MARGIN_AUTO) {
			render_pos.x += (window_info.width / 2.0f) - (div_width / 2.0f);
		}
		render_pos.x += div->style->margin_size.x;		// margin
		render_pos.y -= div->style->margin_size.z;		// margin

		if (div->style->border) {
			render_pos.x += div->style->border->size.x;	// border
			render_pos.y -= div->style->border->size.z;	// border
		}

		engine::enable_blend();
		if (div->style->border) {
			// render border in the correct position
			engine::border_prepare_render();
			engine::border_change_data(div->style->border, vec3(render_pos.x, render_pos.y, 0.0f), div_width, div_height);
			engine::border_render(div->style->border);
		}

		// render quad in the correct position
		engine::quad_change_data(&div->div_quad, div_width, div_height, div->style->color_bg);
		engine::quad_prepare_render();
		engine::quad_change_position(&div->div_quad, render_pos);
		engine::quad_render(&div->div_quad, div->style->texture_bg);

		engine::disable_blend();

		{
			// render text in the correct position
			hm::vec2 text_pos = style_get_base_position(div) + base;
			text_pos = vec2(text_pos.x, window_info.height - text_pos.y - pos_info.y);

			if (div->style->flags & DIV_MARGIN_AUTO) {
				text_pos.x += (window_info.width / 2.0f) - (div_width / 2.0f);
			}
			text_pos.x += div->style->margin_size.x;	// margin
			text_pos.y -= div->style->margin_size.z;	// margin

			text_pos.x += div->style->padding_size.x;	// padding
			text_pos.y -= div->style->padding_size.z;	// padding
			if (div->style->border) {
				text_pos.x += div->style->border->size.x;	// border
				text_pos.y -= div->style->border->size.z;	// border
			}

			if (div->style->text_align == TEXT_ALIGN_RIGHT) {
				text_pos.x += original_width - pos_info.x;
			} else if (div->style->text_align == TEXT_ALIGN_CENTER) {
				text_pos.x += (original_width / 2.0f) - (pos_info.x / 2.0f);
			}

			text_pos.x = floorf(text_pos.x);
			text_pos.y = floorf(text_pos.y);
			render_text(div->style->font_family, div->text, text_pos, div->style->text_color);
			
			if(div->children != 0){
				hm::vec2 b = {0.0f, window_info.height - text_pos.y};
				div_render(div->children[0], b);
			}
		}
		base.y = window_info.height - render_pos.y;
		
	}

	Div_Style* get_div_style(size_t div) {
		return divs[div].style;
	}

	hm::vec2 style_get_base_position(Div* div) {
		if (div->style->flags & DIV_POSITION_ABSOLUTE_ON) {
			return div->style->position_absolute;
		} else {
			if (div->parent) {
				Div_Style* style = div->parent->style;
				hm::vec2 base_pos = style_get_base_position(div->parent);
				if (style->flags & DIV_BORDER_ON) {
					base_pos.x += style->border->size.x;
					base_pos.y += style->border->size.z;
				}
				base_pos.x += style->margin_size.x;	// margin
				base_pos.y += style->margin_size.z;	// margin

				base_pos.x += style->padding_size.x; // padding
				base_pos.y += style->padding_size.z; // padding

				return base_pos;
			} else {
				return div->style->position_relative;
			}
		}
	}
}