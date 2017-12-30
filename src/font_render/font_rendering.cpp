#include "font_rendering.h"
#include "render_engine.h"
#include "os.h"

extern Window_Info window_info;

struct Font_List {
	Font_Info list[64];
	bool used[64];
	GLuint shader = -1;
};

Font_List font_list;
Font_Table font_table(512);

GLuint get_font_shader() {
	return font_list.shader;
}

GLuint get_font_texture(Font_ID font_id) {
	if (font_id >= 0 && font_list.used[font_id] && font_list.list[font_id].loaded) {
		return font_list.list[font_id].atlas_full_id;
	}
}

r32 get_font_max_height(Font_ID font_id) {
	if (font_id >= 0 && font_list.used[font_id] && font_list.list[font_id].loaded) {
		return (r32)font_list.list[font_id].max_height;
	}
}

r32 get_font_max_width(Font_ID font_id) {
	if (font_id >= 0 && font_list.used[font_id] && font_list.list[font_id].loaded) {
		return (r32)font_list.list[font_id].max_width;
	}
}

r32 get_font_size(Font_ID font_id) {
	if (font_id >= 0 && font_list.used[font_id] && font_list.list[font_id].loaded) {
		return (r32)font_list.list[font_id].font_size;
	}
}

bool get_font_is_unicode(Font_ID font_id) {
	if (font_id >= 0 && font_list.used[font_id] && font_list.list[font_id].loaded) {
		return (r32)font_list.list[font_id].unicode;
	}
}

int font_load(Font_Info* font, const s8* filepath, u32 pixel_point, u32 load_limit, bool is_unicode)
{
	font->unicode = is_unicode;
	assert(load_limit <= MAX_UNICODE);

	FT_Library library;
	
	int error = FT_Init_FreeType(&library);
	if (error) {
		print("Error initializing freetype\n");
		return -1;
	}
	error = FT_New_Face(library, filepath, 0, &font->face);
	if (error == FT_Err_Unknown_File_Format) {
		print("Error loading font file format %s\n", filepath);
		return -1;
	}
	else if (error) {
		print("Error loading font %s\n", filepath);
		return -1;
	}

	if (FT_HAS_VERTICAL(font->face)) {
		print("Font %s has vertical metrics\n", filepath);
		//assert(0);	// @TODO
	}
	if (FT_HAS_KERNING(font->face)) {
		font->kerning = false;
		print("Font %s has kerning metrics\n", filepath);
		// @TODO
	}

	error = FT_Set_Pixel_Sizes(font->face, 0, pixel_point);

	
	s32 x_adv = 0, y_adv = 0;
	s32 previous_max_height = 0;

	s32 max_height = (font->face->size->metrics.ascender - font->face->size->metrics.descender) >> 6;
	s32 max_width = font->face->size->metrics.max_advance >> 6;
	s32 num_glyphs = font->face->num_glyphs;
	s32 num_glyphs_loaded = 0;
	s32 size = next_2_pow((s32)sqrtf(max_width * num_glyphs)) * 4;
	r32 atlasf_size = (r32)size;
	font->atlas_size = size;

	font->font_size = pixel_point;
	font->max_height = max_height;
	font->max_width = max_width;

	// allocate memory for the texture atlas of the font
	font->atlas_data = (u8*)calloc(1, size * size * 4);

	for (u32 i = 0; i < load_limit; ++i) {
		u32 index = FT_Get_Char_Index(font->face, i);
		error = FT_Load_Glyph(font->face, index, FT_LOAD_RENDER);

		if (index == 0) {
			continue;
		}

		s32 width = font->face->glyph->bitmap.width;
		s32 height = font->face->glyph->bitmap.rows;

		hm::vec2 topl, topr, botl, botr;

		if (width && height) {
			// if got to the end of the first row, reset x_advance and sum y
			if (x_adv + width >= size) {
				y_adv += previous_max_height;
				x_adv = 0;
			}
			// copy from the FT bitmap to atlas in the correct position
			for (u32 h = 0; h < height; ++h) {
				u8* b = font->face->glyph->bitmap.buffer;
				memory_copy(font->atlas_data + (size * (h + y_adv)) + x_adv, b + width * h, width);
			}
			font->characters[i].botl = { x_adv / atlasf_size,			  y_adv / atlasf_size };
			font->characters[i].botr = { (x_adv + width) / atlasf_size, y_adv / atlasf_size };
			font->characters[i].topl = { x_adv / atlasf_size,			  (y_adv + height) / atlasf_size };
			font->characters[i].topr = { (x_adv + width) / atlasf_size, (y_adv + height) / atlasf_size };

			// keep the packing by getting the max height of the previous row of packing
			if (height > previous_max_height) previous_max_height = height;
			x_adv += width;
		}

		// this glyph exists
		font->characters[i].renderable = true;
		font->characters[i].advance = font->face->glyph->advance.x;
		font->characters[i].size[0] = font->face->glyph->bitmap.width;
		font->characters[i].size[1] = font->face->glyph->bitmap.rows;
		font->characters[i].bearing[0] = font->face->glyph->bitmap_left;
		font->characters[i].bearing[1] = font->face->glyph->bitmap_top;

		num_glyphs_loaded += 1;
	}

	FT_Done_Face(font->face);
	FT_Done_FreeType(library);

	font->finish_load = true;
	return 0;
}

void text_buffer_init(Font_Info* font, s64 size) {
	glGenVertexArrays(1, &font->vao);
	glBindVertexArray(font->vao);
	glGenBuffers(1, &font->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, font->vbo);
	glGenBuffers(1, &font->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, font->ebo);

	const int num_vertices = 4;
	const int num_attribs = 9;

	u16* indices = (u16*)memory_alloc(size * 6 * sizeof(u16));
	float* data = (float*)memory_alloc(size * num_attribs * num_vertices * sizeof(float));

	for (u16 i = 0, j = 0, k = 0; i < size; ++i, j += 4, k += 6) {
		indices[k] = j;
		indices[k + 1] = j + 1;
		indices[k + 2] = j + 3;
		indices[k + 3] = j + 3;
		indices[k + 4] = j + 1;
		indices[k + 5] = j + 2;
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * 6 * sizeof(u16), indices, GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, size * num_attribs * num_vertices * sizeof(float), data, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * num_attribs, (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * num_attribs, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * num_attribs, (void*)(sizeof(float) * (3 + 2)));

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	font->text_buffer_max_length = size;
	font->text_buffer_length = 0;

	memory_free(indices);
	memory_free(data);
}

void text_buffer_realloc(Font_Info* font, s64 new_size) {
	glBindVertexArray(font->vao);
	glDeleteBuffers(1, &font->vbo);
	glDeleteBuffers(1, &font->ebo);

	text_buffer_init(font, new_size);
}

void text_buffer_change(float* dest, float* data, s64 size)
{
	const int num_vertices = 4;
	const int num_attribs = 9;
	memcpy(dest, data, size * sizeof(float) * num_attribs * num_vertices);
}

void text_buffer_draw(Font_Info* font, s32 size, hm::vec2 window_size)
{
	if (!font->loaded) {
		font_finish_load(font);
		return;
	}

	glBindVertexArray(font->vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	hm::mat4 projection = hm::mat4::ortho(0, window_size.x, 0, window_size.y);
	GLuint loc = glGetUniformLocation(font->shader, "projection");
	glUniformMatrix4fv(loc, 1, GL_TRUE, projection.data);

	glActiveTexture(GL_TEXTURE0);
	if (font->flags & FONT_ASCII_ONLY) {
		glBindTexture(GL_TEXTURE_2D, font->atlas_asci_id);
	} else {
		glBindTexture(GL_TEXTURE_2D, font->atlas_full_id);
	}

	glDrawElements(GL_TRIANGLES, 6 * size, GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

}

u32 get_unicode(u8* text, u32* advance) {
	u32 result = 0;
	if (text[0] & 128) {
		// 1xxx xxxx
		if (text[0] > 0xF0) {
			// 4 bytes
			*advance = 4;
			result = ((text[0] & 0x07) << 26) | ((text[1] & 0x3F) << 12) | ((text[2] & 0x3F) << 6) | (text[3] | 0x3F);
		}
		else if (text[0] > 0xE0) {
			// 3 bytes
			*advance = 3;
			result = ((text[0] & 0x0F) << 12) | ((text[1] & 0x3F) << 6) | (text[2] & 0x3F);
		}
		else if (text[0] > 0xC0) {
			// 2 bytes
			*advance = 2;
			result = ((text[0] & 0x1F) << 6) | (text[1] & 0x3F);
		}
		else {
			// continuation byte
			*advance = 1;
			result = text[0];
		}
	} else {
		*advance = 1;
		result = (u32)text[0];
	}
	if (result >= 2000) {
		int x = 0;
	}
	return result;
}

void font_finish_load(Font_Info* font)
{
	if (font->finish_load) {
		font->finish_load = false;
		u32 size = font->atlas_size;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &font->atlas_full_id);
		glBindTexture(GL_TEXTURE_2D, font->atlas_full_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, size, size, 0, GL_RED, GL_UNSIGNED_BYTE, font->atlas_data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		memory_free(font->atlas_data);
		font->loaded = true;
	}
}

void text_draw(Font_Info* font, s64 offset_, u8* text, s32 length, hm::vec2& position, hm::vec4 color, bool unicode = true)
{
	Character* characters = font->characters;
	glBindBuffer(GL_ARRAY_BUFFER, font->vbo);
	void* buffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	position.x = roundf(position.x);
	position.y = roundf(position.y);
	r32 start_x = position.x;

	u32 offset = 0, num_chars = 0;
	u32 previous_index = 0;
	for (u32 i = 0, c = 0; text[c] != 0; ++i) {
		u32 advance = 0;
		u32 index = 0;
		u32 original = 0;
		if (unicode) {
			index = get_unicode(&text[c], &advance);
		} else {
			index = text[c];
			advance = 1;
		}
		original = index;
		c += advance;

		if (!characters[index].renderable && original != '\n') {
			//index = ' ';
			continue;
		}

		if (font->kerning) {
			FT_Vector delta;
			FT_Get_Kerning(font->face, previous_index, index, FT_KERNING_DEFAULT, &delta);
			position.x += delta.x >> 6;
		}
		previous_index = index;

		if (original == '\n') {
			position.x = start_x;
			position.y -= (r32)font->max_height;
		}

		GLfloat xpos = (GLfloat)position.x + characters[index].bearing[0];
		GLfloat ypos = (GLfloat)position.y - (characters[index].size[1] - characters[index].bearing[1]);

		GLfloat w = characters[index].size[0];
		GLfloat h = characters[index].size[1];

		float data[] = {
			xpos + w, ypos      , 0.0f, characters[index].topr.x, characters[index].topr.y, color.r, color.g, color.b, color.a,
			xpos, ypos          , 0.0f, characters[index].topl.x, characters[index].topl.y, color.r, color.g, color.b, color.a,
			xpos, ypos + h      , 0.0f, characters[index].botl.x, characters[index].botl.y, color.r, color.g, color.b, color.a,
			xpos + w, ypos + h  , 0.0f, characters[index].botr.x, characters[index].botr.y, color.r, color.g, color.b, color.a
		};

		text_buffer_change((float*)(((u8*)buffer + (sizeof(data) * offset_)) + offset), data, 1);

		offset += sizeof(data);
		num_chars += 1;
		position.x += characters[index].advance >> 6;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

extern "C" u32 load_font_thread(void *arg)
{
	Font_Info* font = (Font_Info*)arg;
	int err = font_load(font, (s8*)font->name.data, font->font_size, 1024);
	if (err == -1) font->error_loading = true;
	return 0;
}

void font_rendering_init() {
	font_list.shader = engine::shader_load(vshader, fshader, sizeof(vshader) - 1, sizeof(fshader) - 1);
}

void font_rendering_release() {
	glDeleteProgram(font_list.shader);
}

Font_ID load_font_not_repeat(string name, u32 type_size) {
	Font_ID id = font_table.entry_exist(name, type_size);
	if (id == -1) {
		id = load_font((const char*)name.data, type_size);
		font_table.insert(id, name, type_size);
	}
	return id;
}

Font_ID load_font(const char* name, u32 type_size, bool is_unicode) {
	s32 index = 0;
	while (font_list.used[index]) {
		index++;
		if (index == 64) return -1;
	}
	int err = font_load(&font_list.list[index], name, type_size, 1024, is_unicode);
	if (err == -1) return -2;
	font_finish_load(&font_list.list[index]);
	font_list.used[index] = true;
	font_list.list[index].shader = font_list.shader;

	text_buffer_init(&font_list.list[index], 1024);
	return index;
}

Font_ID load_font_async(const char* name, u32 type_size) {
	s32 index = 0;
	while (font_list.used[index]) {
		index++;
		if (index == 64) return -1;
	}
	font_list.used[index] = true;
	u32 tid;
// @temp
#if defined(_WIN32) || defined(_WIN64)
	create_thread(load_font_thread, &font_list.list[index], &tid);
#elif defined(__linux__)
	create_thread((void*(*)(void*))load_font_thread, &font_list.list[index], &tid);
#endif

	text_buffer_init(&font_list.list[index], 1024);
	return index;
}

int render_text(Font_ID font_id, string text, hm::vec2& position, hm::vec4 color) {
	Font_Info* font = 0;
	if (font_id >= 0) font = &font_list.list[font_id];

	if (font->loaded) {
		if (font->text_buffer_offset + text.length >= font->text_buffer_max_length) {
			text_buffer_realloc(font, font->text_buffer_offset + text.length + 512);
		}
		text_draw(font, font->text_buffer_offset, text.data, text.length, position, color, font->unicode);
		font->text_buffer_offset += text.length;
		font->text_buffer_length += text.length;
	} else {
		return 1;
	}
	return 0;
}

int render_text(Font_ID font_id, u8* text, u32 length, hm::vec2& position, hm::vec4 color) {
	Font_Info* font = 0;
	if (font_id >= 0) font = &font_list.list[font_id];

	if (font->loaded) {
		if (font->text_buffer_offset + length >= font->text_buffer_max_length) {
			text_buffer_realloc(font, font->text_buffer_offset + length + 512);
		}
		text_draw(font, font->text_buffer_offset, text, length, position, color, font->unicode);
		font->text_buffer_offset += length;
		font->text_buffer_length += length;
	}
	else {
		return 1;
	}
	return 0;
}

int render_text_get_info(Font_ID font_id, string text_in, hm::vec2& position_out) {
	bool unicode = get_font_is_unicode(font_id);
	r32 max_height = position_out.y;
	u8* text = text_in.data;

	position_out.x = roundf(position_out.x);
	position_out.y = roundf(position_out.y);
	r32 start_x = position_out.x;

	Font_Info* font = 0;
	if (font_id >= 0) font = &font_list.list[font_id];
	if (font->loaded) {
		Character* characters = font->characters;

		u32 num_chars = 0;
		u32 previous_index = 0;
		for (u32 i = 0, c = 0; text[c] != 0; ++i) {
			u32 advance = 0;
			u32 index = 0;
			u32 original = 0;
			if (unicode) {
				index = get_unicode(&text[c], &advance);
			} else {
				index = text[c];
				advance = 1;
			}
			original = index;
			c += advance;

			if (!characters[index].renderable && original != '\n') {
				//index = ' ';
				continue;
			}

			if (font->kerning) {
				FT_Vector delta;
				FT_Get_Kerning(font->face, previous_index, index, FT_KERNING_DEFAULT, &delta);
				position_out.x += delta.x >> 6;
			}
			previous_index = index;

			if (original == '\n') {
				position_out.x = start_x;
				position_out.y -= font->max_height;
			}

			GLfloat xpos = (GLfloat)position_out.x + characters[index].bearing[0];
			GLfloat ypos = (GLfloat)position_out.y - (characters[index].size[1] - characters[index].bearing[1]);

			GLfloat w = characters[index].size[0];
			GLfloat h = characters[index].size[1];

			if (h > max_height) {
				max_height = h;
			}

			num_chars += 1;
			position_out.x += characters[index].advance >> 6;
		}
	} else {
		return 1;
	}
	position_out.y = max_height;
	return 0;
}


void font_rendering_flush() {
	glUseProgram(font_list.shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < sizeof(font_list.list) / sizeof(Font_Info); ++i) {
		if (font_list.used[i] && font_list.list[i].loaded) {
			text_buffer_draw(&font_list.list[i], font_list.list[i].text_buffer_length, hm::vec2(window_info.width, window_info.height));
			font_list.list[i].text_buffer_length = 0;
			font_list.list[i].text_buffer_offset = 0;
		}
	}
	glDisable(GL_BLEND);
}

u32 djb2_hash(u8 *str, int size)
{
	u32 hash = 5381;
	int c;
	int i = 0;
	while (i < size) {
		c = *str++;
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
		++i;
	}
	return hash;
}

Font_Table::Font_Table(int max_entries) : max_entries(max_entries){
	entries = (Font_Table_Entry*)memory_alloc(max_entries * sizeof(Font_Table_Entry));
}

Font_ID Font_Table::entry_exist(string name, int size) {
	u32 hash = (djb2_hash(name.data, name.length) + size) % max_entries;
	if (entries[hash].used) {
		if (equal(&name, &entries[hash].name)) {
			return entries[hash].id;
		} else {
			while (entries[hash].used) {
				hash += 1;
				if (hash >= max_entries) hash = 0;
				if (equal(&name, &entries[hash].name)) {
					return entries[hash].id;
				}
			}
		}
	}
	return -1;
}

void Font_Table::insert(Font_ID id, string name, int size) {
	u32 hash = (djb2_hash(name.data, name.length) + size) % max_entries;
	while (entries[hash].used) {
		hash += 1;
		if (hash >= max_entries) hash = 0;
	}
	entries[hash].used = true;
	entries[hash].id = id;
	entries[hash].name = name;
	entries[hash].size = size;
}
