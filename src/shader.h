#pragma once
#include "util.h"
#include <GL/glew.h>

static const char vert_shader[] =
"#version 330\n"
"layout(location=0) in vec3 pos_coord;\n"
"layout(location=1) in vec3 normal_coord;\n"
"layout(location=2) in vec2 tex_coord;\n"
"out vec2 uv;\n"
"out vec3 normals;\n"
"out vec3 tolight;\n"
"uniform mat4 persp_matrix = mat4(1.0);\n"
"uniform mat4 view_matrix = mat4(1.0);\n"
"uniform mat4 model_matrix = mat4(1.0);\n"

"void main(){\n"
"uv = tex_coord;\n"
"vec3 world_pos = (model_matrix * vec4(pos_coord, 1.0)).xyz;\n"
"normals = vec3(model_matrix * vec4(normal_coord, 0.0));\n"
"tolight = vec3(30.0, 30.0, 30.0) - world_pos;\n"

"gl_Position = persp_matrix * view_matrix * model_matrix * vec4(pos_coord, 1.0);\n"
"}\n";

static  const char frag_shader[] =
"#version 330\n"
"out vec4 color;\n"
"in vec2 uv;\n"
"in vec3 normals;\n"
"in vec3 tolight;\n"
"uniform sampler2D tex_sampler;\n"
"uniform vec4 vertex_color;\n"
"uniform bool is_colliding;\n"
"void main(){\n"
"color = texture(tex_sampler, uv);\n"
"float factor = dot(normalize(normals), normalize(tolight));\n"
"vec3 diffuse = max(factor * vec3(1.0, 1.0, 1.0), 0.2);\n"

"//color = vec4(normalize(normals), 1.0);\n"
"color = color * vec4(diffuse, 1.0);\n"
	"if(is_colliding){\n"
	"color = color * vertex_color;\n"
	"}\n"
"}\n";

static GLuint load_shader(const char* vert_shader, const char* frag_shader, GLint vert_length, GLint frag_length)
{
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
		error_warning("Error: vertex shader compilation:\n");
		error_warning(error_buffer);
		return -1;
	}

	glCompileShader(fs_id);
	glGetShaderiv(fs_id, GL_COMPILE_STATUS, &compile_status);
	if (!compile_status) {
		char error_buffer[512] = { 0 };
		glGetShaderInfoLog(fs_id, sizeof(error_buffer) - 1, NULL, error_buffer);
		error_warning("Error: fragment shader compilation:\n");
		error_warning(error_buffer);
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
		error_warning("Error: shader link:\n");
		error_warning(error_buffer);
		return -1;
	}

	glValidateProgram(shader_id);
	return shader_id;
}