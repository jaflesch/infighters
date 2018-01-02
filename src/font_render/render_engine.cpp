#include "render_engine.h"
#include <GL/glew.h>
#include "os.h"

extern Window_Info window_info;

GLuint quad2d_shader;
GLuint quad2d_projection_location;
GLuint quad2d_position_location;
GLuint quad2d_color_location;
GLuint quad2d_use_texture;

namespace engine {

	void texture_create_rgba(Texture* texture, s32 width, s32 height, u8* data) {
		glGenTextures(1, &texture->id);
		glBindTexture(GL_TEXTURE_2D, texture->id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void texture_delete(Texture* texture) {
		glDeleteTextures(1, &texture->id);
	}

	const char quad_vshader[] = R"(
	#version 330 core
	layout(location = 0) in vec3 vertex;
	layout(location = 1) in vec2 tcoords;
	layout(location = 2) in vec4 vert_color;

	out vec2 texcoords;
	out vec4 o_color;

	uniform mat4 projection = mat4(1.0);
	uniform mat4 position;

	void main(){
		gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
		texcoords = tcoords;
		o_color = vert_color;
	}
	)";

	const char quad_fshader[] = R"(
	#version 330 core
	in vec2 texcoords;
	out vec4 color;
	in vec4 o_color;

	uniform sampler2D text;
	uniform vec4 uni_color;
	uniform bool use_texture;

	void main(){
		if(use_texture)
			color = texture(text, texcoords);
		else
			color = o_color;
	}
	)";

	void render_engine_init() {
		quad2d_shader = engine::shader_load(quad_vshader, quad_fshader, sizeof(quad_vshader) - 1, sizeof(quad_fshader) - 1);
		quad2d_projection_location = glGetUniformLocation(quad2d_shader, "projection");
		quad2d_color_location = glGetUniformLocation(quad2d_shader, "uni_color");
		quad2d_use_texture = glGetUniformLocation(quad2d_shader, "use_texture");
	}

	GLuint shader_load(const char* vert_shader, const char* frag_shader, GLint vert_length, GLint frag_length) {

		GLuint vs_id = glCreateShader(GL_VERTEX_SHADER);
		GLuint fs_id = glCreateShader(GL_FRAGMENT_SHADER);

		GLint compile_status;

		const GLchar* p_v[1] = { vert_shader };
		glShaderSource(vs_id, 1, p_v, &vert_length);

		const GLchar* p_f[1] = { frag_shader };
		glShaderSource(fs_id, 1, p_f, &frag_length);

		glCompileShader(vs_id);
		glGetShaderiv(vs_id, GL_COMPILE_STATUS, &compile_status);
		if (!compile_status) {
			char error_buffer[512] = { 0 };
			glGetShaderInfoLog(vs_id, sizeof(error_buffer), NULL, error_buffer);
			print("%s", error_buffer);
			return -1;
		}

		glCompileShader(fs_id);
		glGetShaderiv(fs_id, GL_COMPILE_STATUS, &compile_status);
		if (!compile_status) {
			char error_buffer[512] = { 0 };
			glGetShaderInfoLog(fs_id, sizeof(error_buffer) - 1, NULL, error_buffer);
			print("%s", error_buffer);
			return -1;
		}

		GLuint shader_id = glCreateProgram();
		glAttachShader(shader_id, vs_id);
		glAttachShader(shader_id, fs_id);
		glDeleteShader(vs_id);
		glDeleteShader(fs_id);
		glLinkProgram(shader_id);

		glGetProgramiv(shader_id, GL_LINK_STATUS, &compile_status);
		if (compile_status == 0) {
			GLchar error_buffer[512] = { 0 };
			glGetProgramInfoLog(shader_id, sizeof(error_buffer) - 1, NULL, error_buffer);
			print("%s", error_buffer);
			return -1;
		}

		glValidateProgram(shader_id);
		return shader_id;
	}

	void update_window_size() {
		glViewport(0, 0, window_info.width, window_info.height);
	}

	void enable_blend() {
		glEnable(GL_BLEND);
	}

	void disable_blend() {
		glDisable(GL_BLEND);
	}

	void border_create(Border_2D* border, hm::vec3 pos, hm::vec4 dimensions, r32 width, r32 height) {
		glGenVertexArrays(1, &border->vao);
		glBindVertexArray(border->vao);
		glGenBuffers(1, &border->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, border->vbo);
		glGenBuffers(1, &border->ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, border->ebo);

		border->position = pos;

		u16 indices[6 * 4] = {  0,   1,  2,  2,  1,  3,
								4,   5,  6,  6,  5,  7,
								8,   9, 10, 10,  9, 11,
								12, 13, 14, 14, 13, 15 };

		r32 l = dimensions.x;
		r32 r = dimensions.y;
		r32 t = dimensions.z;
		r32 b = dimensions.w;

		Vertex3D vertices[4][4] = { {
				// left
				{ pos + hm::vec3(-l, -b, 0.0f),{ 0.0f, 0.0f }, border->color_left },
				{ pos + hm::vec3(0.0f, 0.0f, 0.0f),{ 1.0f, 0.0f }, border->color_left },
				{ pos + hm::vec3(-l, height + t, 0.0f),{ 0.0f, 1.0f }, border->color_left },
				{ pos + hm::vec3(0.0f, height, 0.0f),{ 1.0f, 1.0f }, border->color_left }
			},{
				// right
				{ pos + hm::vec3(width, 0.0f, 0.0f),{ 0.0f, 0.0f }, border->color_right },
				{ pos + hm::vec3(width + r, -b, 0.0f),{ 1.0f, 0.0f }, border->color_right },
				{ pos + hm::vec3(width, height, 0.0f),{ 0.0f, 1.0f }, border->color_right },
				{ pos + hm::vec3(width + r, height + t, 0.0f),{ 1.0f, 1.0f }, border->color_right }
			},{
				// top
				{ pos + hm::vec3(0.0f, height, 0.0f),{ 0.0f, 0.0f }, border->color_top },
				{ pos + hm::vec3(width, height, 0.0f),{ 1.0f, 0.0f }, border->color_top },
				{ pos + hm::vec3(-l, height + t, 0.0f),{ 0.0f, 1.0f }, border->color_top },
				{ pos + hm::vec3(width + r, height + t, 0.0f),{ 1.0f, 1.0f }, border->color_top }
			},{
				// bottom
				{ pos + hm::vec3(-l, -b, 0.0f),{ 0.0f, 0.0f }, border->color_bottom },
				{ pos + hm::vec3(width + r, -b, 0.0f),{ 1.0f, 0.0f }, border->color_bottom },
				{ pos + hm::vec3(0.0f, 0.0f, 0.0f),{ 0.0f, 1.0f }, border->color_bottom },
				{ pos + hm::vec3(width, 0.0f, 0.0f),{ 1.0f, 1.0f }, border->color_bottom }
			}
		};

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)&((Vertex3D*)0)->texture_coord);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)&((Vertex3D*)0)->color);

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

	}

	void border_change_data(Border_2D* border, hm::vec3 pos, r32 width, r32 height) {
		glBindBuffer(GL_ARRAY_BUFFER, border->vbo);
		r32 l = border->size.x;
		r32 r = border->size.y;
		r32 t = border->size.z;
		r32 b = border->size.w;

		Vertex3D vertices[4][4] = { {
				// left
				{ pos + hm::vec3(-l, -b, 0.0f),					{ 0.0f, 0.0f }, border->color_left },
				{ pos + hm::vec3(0.0f, 0.0f, 0.0f),				{ 1.0f, 0.0f }, border->color_left },
				{ pos + hm::vec3(-l, height + t, 0.0f),			{ 0.0f, 1.0f }, border->color_left },
				{ pos + hm::vec3(0.0f, height, 0.0f),			{ 1.0f, 1.0f }, border->color_left }
			},{
				// right
				{ pos + hm::vec3(width, 0.0f, 0.0f),			{ 0.0f, 0.0f }, border->color_right },
				{ pos + hm::vec3(width + r, -b, 0.0f),			{ 1.0f, 0.0f }, border->color_right },
				{ pos + hm::vec3(width, height, 0.0f),			{ 0.0f, 1.0f }, border->color_right },
				{ pos + hm::vec3(width + r, height + t, 0.0f),	{ 1.0f, 1.0f }, border->color_right }
			},{
				// top
				{ pos + hm::vec3(0.0f, height, 0.0f),			{ 0.0f, 0.0f }, border->color_top },
				{ pos + hm::vec3(width, height, 0.0f),			{ 1.0f, 0.0f }, border->color_top },
				{ pos + hm::vec3(-l, height + t, 0.0f),			{ 0.0f, 1.0f }, border->color_top },
				{ pos + hm::vec3(width + r, height + t, 0.0f),	{ 1.0f, 1.0f }, border->color_top }
			},{
				// bottom
				{ pos + hm::vec3(-l, -b, 0.0f),					{ 0.0f, 0.0f }, border->color_bottom },
				{ pos + hm::vec3(width + r, -b, 0.0f),			{ 1.0f, 0.0f }, border->color_bottom },
				{ pos + hm::vec3(0.0f, 0.0f, 0.0f),				{ 0.0f, 1.0f }, border->color_bottom },
				{ pos + hm::vec3(width, 0.0f, 0.0f),			{ 1.0f, 1.0f }, border->color_bottom }
			}
		};

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	}

	void border_prepare_render() {
		glUseProgram(quad2d_shader);

		hm::mat4 projection = hm::mat4::ortho(0, (float)window_info.width, 0, (float)window_info.height);
		glUniformMatrix4fv(quad2d_projection_location, 1, GL_TRUE, projection.data);
		glUniform1i(quad2d_use_texture, 0);
	}

	void border_render(Border_2D* border) {
		glBindVertexArray(border->vao);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glDrawElements(GL_TRIANGLES, 6 * 4, GL_UNSIGNED_SHORT, 0);

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	}

	void border_delete(Border_2D* border) {
		glDeleteBuffers(1, &border->ebo);
		glDeleteBuffers(1, &border->vbo);
		glDeleteVertexArrays(1, &border->vao);
	}

	void quad_create(Quad_2D* quad, hm::vec3 pos, r32 width, r32 height) {
		glGenVertexArrays(1, &quad->vao);
		glBindVertexArray(quad->vao);
		glGenBuffers(1, &quad->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, quad->vbo);
		glGenBuffers(1, &quad->ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad->ebo);

		quad->position = pos;

		u16 indices[6] = { 0,   1,  2,  2,  1,  3 };

		Vertex3D vertices[4] = {
			{ pos,									{ 1.0f, 1.0f }, hm::vec4(0.0f, 0.0f, 0.0f, 0.0f) },
			{ pos + hm::vec3(width, 0.0f, 0.0f),	{ 0.0f, 1.0f }, hm::vec4(0.0f, 0.0f, 0.0f, 0.0f) },
			{ pos + hm::vec3(0.0f, height, 0.0f),	{ 1.0f, 0.0f }, hm::vec4(0.0f, 0.0f, 0.0f, 0.0f) },
			{ pos + hm::vec3(width, height, 0.0f),	{ 0.0f, 0.0f }, hm::vec4(0.0f, 0.0f, 0.0f, 0.0f) }
		};

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)&((Vertex3D*)0)->texture_coord);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)&((Vertex3D*)0)->color);

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	}

	void quad_delete(Quad_2D* quad) {
		glDeleteVertexArrays(1, &quad->vao);
		glDeleteBuffers(1, &quad->vbo);
		glDeleteBuffers(2, &quad->ebo);
	}

	void quad_prepare_render() {
		glUseProgram(quad2d_shader);

		hm::mat4 projection = hm::mat4::ortho(0, (float)window_info.width, 0, (float)window_info.height);
		glUniformMatrix4fv(quad2d_projection_location, 1, GL_TRUE, projection.data);
		glUniform1i(quad2d_use_texture, 0);
	}

	void quad_change_position(Quad_2D* quad, hm::vec2 pos) {
		quad->position = hm::vec3(pos.x, pos.y, 0.0f);
		hm::mat4 pos_matrix = hm::mat4::translate(quad->position);
		glUniform4fv(quad2d_position_location, 1, pos_matrix.data);
	}

	void quad_change_data(Quad_2D* quad, r32 width, r32 height, hm::vec4 color) {
		glBindBuffer(GL_ARRAY_BUFFER, quad->vbo);
		Vertex3D vertices[4] = {
			{ quad->position,									{ 1.0f, 1.0f }, color},
			{ quad->position + hm::vec3(width, 0.0f, 0.0f),		{ 0.0f, 1.0f }, color },
			{ quad->position + hm::vec3(0.0f, height, 0.0f),	{ 1.0f, 0.0f }, color },
			{ quad->position + hm::vec3(width, height, 0.0f),	{ 0.0f, 0.0f }, color }
		};
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	}

	void quad_render(Quad_2D* quad, Texture* texture) {
		if (texture) {
			glBindTexture(GL_TEXTURE_2D, texture->id);
			glUniform1i(quad2d_use_texture, 1);
		}
		glBindVertexArray(quad->vao);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	}
}
