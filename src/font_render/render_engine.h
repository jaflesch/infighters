#pragma once
#include "../common.h"
#include "hmath.h"
#include "font_rendering.h"
#include <stb_image.h>

namespace engine {

	struct Texture;
	void texture_create_rgba(Texture* texture, s32 width, s32 height, u8* data);

	struct Texture {
		GLuint id;
		s32 width, height, channels;
		bool valid;
		u8* data;

		Texture() { valid = false; }
		Texture(const char* filename) {
			data = stbi_load(filename, &width, &height, &channels, 4);
			if (data == 0) {
				valid = false;
			}
			else {
				texture_create_rgba(this, width, height, data);
				valid = true;
				stbi_image_free(data);
			}
		}
	};

	struct Vertex3D {
		hm::vec3 position;
		hm::vec2 texture_coord;
		hm::vec4 color;
	};

	struct Quad_2D {
		hm::vec3 position;
		GLuint vao, vbo, ebo;
	};

	struct Border_2D {
		hm::vec3 position;
		GLuint vao, vbo, ebo;

		hm::vec4 size;

		hm::vec4 color_left;
		hm::vec4 color_right;
		hm::vec4 color_top;
		hm::vec4 color_bottom;

		Border_2D() {
			vao = -1, vbo = -1, ebo = -1;
			size = { 0.0f, 0.0f, 0.0f, 0.0f };

			color_left = { 0.0f, 0.0f, 0.0f, 1.0f };
			color_right = { 0.0f, 0.0f, 0.0f, 1.0f };
			color_top = { 0.0f, 0.0f, 0.0f, 1.0f };
			color_bottom = { 0.0f, 0.0f, 0.0f, 1.0f };

			position = { 0.0f, 0.0f, 0.0f };
		}
	};

	// Quad2D
	void quad_create(Quad_2D* quad, hm::vec3 pos, r32 width, r32 height);
	void quad_delete(Quad_2D* quad);
	void quad_prepare_render();
	void quad_render(Quad_2D* quad, Texture* texture);
	void quad_change_data(Quad_2D* quad, r32 width, r32 height, hm::vec4 color);
	void quad_change_position(Quad_2D* quad, hm::vec2 pos);

	// Border2D
	void border_create(Border_2D* border, hm::vec3 pos, hm::vec4 dimensions, r32 width, r32 height);
	void border_prepare_render();
	void border_render(Border_2D* border);
	void border_change_data(Border_2D* border, hm::vec3 pos, r32 width, r32 height);
	void border_delete(Border_2D* border);

	void render_engine_init();
	void update_window_size();
	GLuint shader_load(const char* vert_shader, const char* frag_shader, GLint vert_length, GLint frag_length);

	// render
	void enable_blend();
	void disable_blend();
}