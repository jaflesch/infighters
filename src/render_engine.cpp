#include "render_engine.h"
#include <GL/glew.h>
#include "os.h"
#include "input.h"

extern Window_Info window_info;
extern Keyboard_State keyboard_state;
extern Mouse_State mouse_state;

GLuint quad2d_shader;
GLuint quad2d_projection_location;
GLuint quad2d_position_location;
GLuint quad2d_color_location;
GLuint quad2d_use_texture;
GLuint quad2d_view_matrix_location;

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

using namespace hm;

#if 1
struct Camera {
	mat4 view_matrix;
	mat4 projection_matrix;

	vec3 cam_position;
	vec3 cam_orientation;
	vec3 cam_up;

	bool locked;

	float current_speed;
	float current_turn_speed;

	float fov;
	float far_plane;
	float near_plane;

	void set_cam_position(vec3 newpos) {
		cam_position = newpos;
		view_matrix = mat4::look_at(newpos, cam_orientation, cam_up);
	}
	void set_cam_orientation(vec3 newori) {
		cam_orientation = newori;
		view_matrix = mat4::look_at(cam_position, cam_position + newori, cam_up);
	}

	void move_forward_and_back(float amt) {
		set_cam_position(cam_position + (cam_orientation * amt));
		set_cam_orientation(cam_orientation);
	}
	void move_sideways(float amt) {
		vec3 left = vec3::normalize(vec3::cross(cam_up, cam_orientation));
		set_cam_position(cam_position + (left * amt));
		set_cam_orientation(cam_orientation);
	}

	void rotate_x(float amt) {
		mat4 rot = mat4::rotate(cam_up, amt);
		vec3 normalized_ori = vec3::normalize(cam_orientation);
		normalized_ori = rot * normalized_ori;
		set_cam_orientation(normalized_ori);
	}

	void rotate_y(float amt) {
		mat4 rot = mat4::rotate(vec3::cross(cam_orientation, cam_up), amt);
		vec3 normalized_ori = vec3::normalize(cam_orientation);
		normalized_ori = rot * normalized_ori;
		set_cam_orientation(normalized_ori);
	}
};

void init_camera(Camera* camera, float aspect, float fov, float znear, float zfar) {
	vec3 campos(0, 0, 0);
	vec3 camori(0, 0, -1);
	vec3 up(0, 1, 0);

	camera->view_matrix = mat4::look_at(campos, camori, up);
	camera->projection_matrix = mat4::perspective(fov, aspect, znear, zfar);
	camera->fov = fov;
	camera->far_plane = zfar;
	camera->near_plane = znear;

	camera->cam_position = campos;
	camera->set_cam_orientation(camori);
	camera->cam_up = up;
	camera->locked = false;
}

extern Keyboard_State keyboard_state;
extern Mouse_State mouse_state;

void mouse_move_event(Camera* camera, int valuex, int valuey)
{
#define TURNSPEED -0.1f

	float x = (2.0f * valuex) / window_info.width - 1;
	float y = (2.0f * valuey) / window_info.height - 1;

	float amtx = valuex * TURNSPEED;
	float amty = valuey * TURNSPEED;

	camera->rotate_x(amtx);
	camera->rotate_y(amty);
}

void input_camera(Camera* camera)
{
	float CAMSPEED = 20.0f;
	float CAMTURNSPEED = 30.0f;

	if (keyboard_state.key[VK_SHIFT]) {
		CAMSPEED = 10.0f;
		CAMTURNSPEED = 6.0f;
	}

	if (keyboard_state.key['W']) {
		camera->current_speed = CAMSPEED;
	} else if (keyboard_state.key['S']) {
		camera->current_speed = -CAMSPEED;
	} else {
		camera->current_speed = 0;
	}

	if (keyboard_state.key['A']) {
		camera->current_turn_speed = CAMTURNSPEED;
	} else if (keyboard_state.key['D']) {
		camera->current_turn_speed = -CAMTURNSPEED;
	} else {
		camera->current_turn_speed = 0;
	}

	if (keyboard_state.key['R']) {
		float amt = 0.001f;
		if (keyboard_state.key[VK_SHIFT]) {
			amt = -amt;
		}
		camera->rotate_y(amt);
	}

	if (mouse_state.is_captured) {
		mouse_move_event(camera, mouse_state.x - mouse_state.x_left, mouse_state.y - mouse_state.y_left);
		mouse_state.x_left = mouse_state.x;
		mouse_state.y_left = mouse_state.y;
	}

	float aspect = (float)window_info.width / window_info.height;
	camera->projection_matrix = mat4::perspective(camera->fov, aspect, camera->near_plane, camera->far_plane);

	camera->move_forward_and_back(camera->current_speed * (1.0f / 60.0f));
	camera->move_sideways(camera->current_turn_speed * (1.0f / 60.0f));
}
#endif

Camera camera;

namespace engine {
	const char quad_vshader[] = R"(
	#version 330 core
	layout(location = 0) in vec3 vertex;
	layout(location = 1) in vec2 tcoords;
	layout(location = 2) in vec4 vert_color;

	out vec2 texcoords;
	out vec4 o_color;

	uniform mat4 projection = mat4(1.0);
	uniform mat4 position = mat4(1.0);
	uniform mat4 view_matrix = mat4(1.0);

	void main(){
		gl_Position = projection * view_matrix * position * vec4(vertex, 1.0);
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
		color = vec4(0.8, 0.5, 0.9, 1.0);
		//if(use_texture)
		//	color = texture(text, texcoords);
		//else
		//	color = o_color;
	}
	)";

	void render_engine_init() {
		quad2d_shader = engine::shader_load(quad_vshader, quad_fshader, sizeof(quad_vshader) - 1, sizeof(quad_fshader) - 1);
		quad2d_projection_location = glGetUniformLocation(quad2d_shader, "projection");
		quad2d_color_location = glGetUniformLocation(quad2d_shader, "uni_color");
		quad2d_use_texture = glGetUniformLocation(quad2d_shader, "use_texture");
		quad2d_position_location = glGetUniformLocation(quad2d_shader, "position");
		quad2d_view_matrix_location = glGetUniformLocation(quad2d_shader, "view_matrix");

		init_camera(&camera, (float)window_info.width / window_info.height, 45.0f, 0.2f, 1000.0f);
		camera.set_cam_position(vec3(0.0f, 45.0f, 2000.0f));
		camera.rotate_y(5.0f);
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

	void grid_prepare_render() {
		glUseProgram(quad2d_shader);

		//hm::mat4 projection = hm::mat4::ortho(0, window_info.width, 0, window_info.height);
		//glUniformMatrix4fv(quad2d_projection_location, 1, GL_TRUE, projection.data);
		glUniform1i(quad2d_use_texture, 0);
	}

	void grid_create(Grid_3D* grid, u32 width, u32 height, r32 size_x, r32 size_y) {
		glGenVertexArrays(1, &grid->vao);
		glBindVertexArray(grid->vao);
		glGenBuffers(1, &grid->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, grid->vbo);
		glGenBuffers(1, &grid->ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grid->ebo);

		grid->position = { 10.0f, 10.0f, 1.0f };

		Vertex3D* vertices = (Vertex3D*)calloc(4, width * height * sizeof(Vertex3D));
		u32* indices = (u32*)calloc(6, width * height * sizeof(u32));

		r32 stepx = 0.0f;
		r32 stepy = 0.0f;
		int i = 0;
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				vertices[i + 0] = { { stepx,			1.0f, stepy },{ 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f, 0.0f } };
				vertices[i + 1] = { { stepx + size_x,	1.0f, stepy },{ 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f, 0.0f } };
				vertices[i + 2] = { { stepx,			1.0f, stepy + size_y },{ 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f, 0.0f } };
				vertices[i + 3] = { { stepx + size_x,	1.0f, stepy + size_y },{ 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f, 0.0f } };
				stepx += size_x;
				i += 4;
			}
			stepy += size_y;
			stepx = 0.0f;
		}

		for (int i = 0, j = 0; i < 6 * width * height; i += 6, j += 4) {
			indices[i    ] = j;
			indices[i + 1] = j + 1;
			indices[i + 2] = j + 2;
			indices[i + 3] = j + 2;
			indices[i + 4] = j + 1;
			indices[i + 5] = j + 3;
		}

		grid->count = width * height;

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * width * height * sizeof(u32), indices, GL_DYNAMIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, 4 * width * height * sizeof(Vertex3D), vertices, GL_DYNAMIC_DRAW);

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

	void grid_render(Grid_3D* grid) {
		input_camera(&camera);

		glUniformMatrix4fv(quad2d_projection_location, 1, GL_FALSE, (GLfloat*)camera.projection_matrix.data);
		glUniformMatrix4fv(quad2d_view_matrix_location, 1, GL_FALSE, (GLfloat*)camera.view_matrix.data);

		hm::mat4 position = hm::mat4::translate(grid->position);
		glUniformMatrix4fv(quad2d_position_location, 1, GL_TRUE, position.data);

		glBindVertexArray(grid->vao);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//glDrawElements(GL_TRIANGLES, grid->count * 6, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_LINES, grid->count * 6, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
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

		hm::mat4 projection = hm::mat4::ortho(0, window_info.width, 0, window_info.height);
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
			{ pos,									{ 0.0f, 1.0f }, hm::vec4(0.0f, 0.0f, 0.0f, 0.0f) },
			{ pos + hm::vec3(width, 0.0f, 0.0f),	{ 1.0f, 1.0f }, hm::vec4(0.0f, 0.0f, 0.0f, 0.0f) },
			{ pos + hm::vec3(0.0f, height, 0.0f),	{ 0.0f, 0.0f }, hm::vec4(0.0f, 0.0f, 0.0f, 0.0f) },
			{ pos + hm::vec3(width, height, 0.0f),	{ 1.0f, 0.0f }, hm::vec4(0.0f, 0.0f, 0.0f, 0.0f) }
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

		hm::mat4 projection = hm::mat4::ortho(0, window_info.width, 0, window_info.height);
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
			{ quad->position,									{ 0.0f, 1.0f }, color},
			{ quad->position + hm::vec3(width, 0.0f, 0.0f),		{ 1.0f, 1.0f }, color },
			{ quad->position + hm::vec3(0.0f, height, 0.0f),	{ 0.0f, 0.0f }, color },
			{ quad->position + hm::vec3(width, height, 0.0f),	{ 1.0f, 0.0f }, color }
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
