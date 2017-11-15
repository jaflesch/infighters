#pragma once
#include "../common.h"

#include "../util.h"

#include <hmath.h>
#include <GL/glew.h>
#undef internal
#include <ft2build.h>
#include FT_FREETYPE_H

typedef int Font_ID;

enum Font_Type {
	FONT_OSWALD_REGULAR_16 = 0,
	FONT_OSWALD_REGULAR_14 = 1,
	FONT_OSWALD_REGULAR_12 = 2,
	FONT_OSWALD_REGULAR_24 = 3,
	FONT_OSWALD_REGULAR_32 = 4,
	FONT_OSWALD_REGULAR_38 = 5,

	FONT_OSWALD_LIGHT_16 = 6,
	FONT_OSWALD_LIGHT_14 = 7,
	FONT_OSWALD_LIGHT_12 = 8,
	FONT_OSWALD_LIGHT_24 = 9,
	FONT_OSWALD_LIGHT_32 = 10,
	FONT_OSWALD_LIGHT_38 = 11,

	FONT_NUMBER,
};

struct Character {
	hm::vec2 botl, botr, topl, topr;
	s32 size[2];
	s32 bearing[2];
	GLuint advance;
	bool renderable;
};

const u32 FONT_ASCII_ONLY = FLAG(0);
const u32 FONT_UTF8       = FLAG(1);

#define MAX_UNICODE 65536

struct Font_Table_Entry {
	Font_ID id;
	int size;
	string name;
	bool used;
};

struct Font_Table {
	int num_entries;
	int max_entries;
	Font_Table_Entry* entries;
	
	Font_Table(int max_entries);
	Font_ID entry_exist(string name, int size);
	void insert(Font_ID id, string name, int size);
};

struct Font_Info {
	string name;

	s64 font_size;
	s64 atlas_size;
	s64 max_height;
	s64 max_width;

	s64 text_buffer_length;
	s64 text_buffer_max_length;
	s64 text_buffer_offset;

	u32 flags = 0;

	GLuint shader = -1;
	GLuint atlas_full_id = -1;
	GLuint atlas_asci_id = -1;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	u8* atlas_data = 0;

	bool error_loading = false;
	bool loaded = false;
	bool finish_load = false;
	bool kerning = false;
	bool unicode = true;

	FT_Face face;
	Character characters[MAX_UNICODE];
};

const char vshader[] = R"(
	#version 330 core
	layout(location = 0) in vec3 vertex;
	layout(location = 1) in vec2 tcoords;
	layout(location = 2) in vec4 v_color;

	out vec2 texcoords;
	out vec4 out_color;

	uniform mat4 projection = mat4(1.0);

	void main(){
		gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
		texcoords = tcoords;
		out_color = v_color;
	}
)";

const char fshader[] = R"(
	#version 330 core
	in vec2 texcoords;
	in vec4 out_color;
	out vec4 color;

	uniform sampler2D text;

	void main(){
		vec4 sampled = texture(text, texcoords);
		color = vec4(out_color.rgb, sampled.r * out_color.a);
		//color = sampled;
	}
)";

u32 get_unicode(u8* text, u32* advance);

int font_load(Font_Info* font, const s8* filepath, u32 pixel_point, u32 load_limit, bool is_unicode = true);
void font_finish_load(Font_Info* font);

void text_buffer_init(Font_Info* font, s64 size);
void text_buffer_realloc(Font_Info* font, s64 new_size);
void text_buffer_change(float* dest, float* data, s64 size);
void text_buffer_draw(Font_Info* font, s32 size, hm::vec2 window_size);


extern "C" u32 load_font_thread(void *arg);

void font_rendering_init();
void font_rendering_release();
void font_rendering_flush();

// return value:
// positive number is the font id
// -1 the font list is full
// -2 could not find the font
Font_ID load_font(const char* name, u32 type_size, bool is_unicode = true);

Font_ID load_font_not_repeat(string name, u32 type_size);

// return value:
// always the font id, even if errored
// font.error_loading field is true if and error occurred while loading
Font_ID load_font_async(const char* name, u32 type_size);

int render_text(Font_ID font_id, string text, hm::vec2& position, hm::vec4 color);
int render_text(Font_ID font_id, u8* text, u32 length, hm::vec2& position, hm::vec4 color);
int render_text_get_info(Font_ID font_id, string text, hm::vec2& position);
GLuint get_font_shader();
GLuint get_font_texture(Font_ID font_id);
r32 get_font_max_height(Font_ID font_id);
r32 get_font_max_width(Font_ID font_id);
r32 get_font_size(Font_ID font_id);