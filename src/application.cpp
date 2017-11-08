#include "application.h"
#include "input.h"
#include "quaternion.h"
#include "ResourceLoad\Texture.h"
#include "debug_table.h"

extern Window_State win_state;
extern DebugTable debug_table;

#include "camera.cpp"
#include "load_model.cpp"

struct GameState {
	Camera camera;
	GLuint shader;

	IndexedModel3D* models;
	int num_models;

	bool start_simulation = false;
	bool move_objects = false;
	bool wireframe = false;

	hm::vec3 gravity;
};

static GameState ggs = {};

void load_model(char* filename, IndexedModel3D* model) {
	load_objfile(filename, model);
	mat4::identity(model->model_matrix);
	model->position = vec3(0.0f, 0.0f, 0.0f);
	model->rotation = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
	model->is_colliding = false;
}

void init_object(IndexedModel3D* m) {
	
	glGenVertexArrays(1, &m->vao);
	glBindVertexArray(m->vao);

	glGenBuffers(1, &m->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->num_indices * sizeof(u16), m->indices, GL_STATIC_DRAW);

	glGenBuffers(1, &m->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
	glBufferData(GL_ARRAY_BUFFER, m->num_vertices * sizeof(Vertex3D), m->vertices, GL_STATIC_DRAW);

	GLuint pos_attrib_loc = glGetAttribLocation(ggs.shader, "pos_coord");
	GLuint tex_coord_attrib_loc = glGetAttribLocation(ggs.shader, "tex_coord");
	GLuint normal_attrib_loc = glGetAttribLocation(ggs.shader, "normal_coord");
	glEnableVertexAttribArray(pos_attrib_loc);
	glEnableVertexAttribArray(normal_attrib_loc);
	glEnableVertexAttribArray(tex_coord_attrib_loc);

	glVertexAttribPointer(pos_attrib_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)0);
	glVertexAttribPointer(normal_attrib_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)&((Vertex3D*)0)->normal);
	glVertexAttribPointer(tex_coord_attrib_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)&((Vertex3D*)0)->tex);
}

void render_line(vec3 start, vec3 end) 
{
	mat4 ident;
	mat4::identity(ident);
	vec4 magenta(1, 0, 1, 1);
	glUniformMatrix4fv(glGetUniformLocation(ggs.shader, "model_matrix"), 1, GL_TRUE, (float*)ident.data);
	glUniform4fv(glGetUniformLocation(ggs.shader, "vertex_color"), 1, (float*)&magenta);
	glLineWidth(4.0f);
	glBegin(GL_LINES);
	glVertex3f(start.x, start.y, start.z);
	glVertex3f(end.x, end.y, end.z);
	glEnd();
	glLineWidth(1.0f);
}

void render_vector(vec3 vec, vec3 position)
{
	mat4 ident;
	mat4::identity(ident);
	vec4 black(0, 0, 0, 1);
	glUniformMatrix4fv(glGetUniformLocation(ggs.shader, "model_matrix"), 1, GL_TRUE, (float*)ident.data);
	glUniform4fv(glGetUniformLocation(ggs.shader, "vertex_color"), 1, (float*)&black);
	glBegin(GL_LINES);
	glVertex3f(position.x, position.y, position.z);
	glVertex3f(vec.x + position.x, vec.y + position.y, vec.z + position.z);
	glEnd();
}

void render_face(vec3 p1, vec3 p2, vec3 p3, vec3 c) {
	mat4 ident;
	mat4::identity(ident);
	vec4 color(c.r, c.g, c.b, 1);
	glUniformMatrix4fv(glGetUniformLocation(ggs.shader, "model_matrix"), 1, GL_TRUE, (float*)ident.data);
	glUniform4fv(glGetUniformLocation(ggs.shader, "vertex_color"), 1, (float*)&color);
	glDisable(GL_CULL_FACE);
	glBegin(GL_TRIANGLES);
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p3.x, p3.y, p3.z);
	glVertex3f(p2.x, p2.y, p2.z);
	glEnd();

	glLineWidth(4.0f);
	glBegin(GL_LINES);
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p3.x, p3.y, p3.z);
	glVertex3f(p2.x, p2.y, p2.z);
	glVertex3f(p1.x, p1.y, p1.z);
	glEnd();
	glLineWidth(1.0f);
	glEnable(GL_CULL_FACE);

	if (p1 == p2 && p2 == p3) {
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		glVertex3f(p1.x, p1.y, p1.z);
		glEnd();
	}
}

void render_object_default(hm::vec3 position, float scale);

void create_object(char* filename)
{
	int index = ggs.num_models;
	if (index == 256) return;
	memset(&ggs.models[index], 0, sizeof(IndexedModel3D));
	load_model(filename, &ggs.models[index]);
	init_object(&ggs.models[index]);
	ggs.models[index].position = vec3(0.0f, 45.0f, 0.0f);
	ggs.models[index].scale = 0.3f;
	ggs.models[index].simulating = true;
	ggs.models[index].static_object = false;
	ggs.models[index].texture = 0;

	ggs.num_models++;
}

int model_input_index = 0;
int move_up_model = 'R';
int move_down_model = 'F';
int turn_left_model = VK_LEFT;
int turn_right_model = VK_RIGHT;
int move_forward_model = VK_UP;
int move_backward_model = VK_DOWN;
int rotate_x_model = 'X';
int rotate_y_model = 'Y';
int rotate_z_model = 'Z';
int start_simulation_key = 'H';
int move_objects = 'M';
int toggle_simulate_key = 'L';

void init_debug_entries()
{
	debug_table.create_entry("model_input", &model_input_index, TYPE_S32);
	debug_table.create_entry("position_x", &ggs.models[model_input_index].position.x, TYPE_R32);
	debug_table.create_entry("position_y", &ggs.models[model_input_index].position.y, TYPE_R32);
	debug_table.create_entry("position_z", &ggs.models[model_input_index].position.z, TYPE_R32);
	debug_table.create_entry("scale", &ggs.models[model_input_index].scale, TYPE_R32);
	debug_table.create_entry("move_up", &move_up_model, TYPE_S32);
	debug_table.create_entry("move_down", &move_down_model, TYPE_S32);
	debug_table.create_entry("turn_left", &turn_left_model, TYPE_S32);
	debug_table.create_entry("turn_right", &turn_right_model, TYPE_S32);
	debug_table.create_entry("move_forward_model", &move_forward_model, TYPE_S32);
	debug_table.create_entry("move_backward_model", &move_backward_model, TYPE_S32);
	debug_table.create_entry("rotate_x", &rotate_x_model, TYPE_S32);
	debug_table.create_entry("rotate_y", &rotate_y_model, TYPE_S32);
	debug_table.create_entry("rotate_z", &rotate_z_model, TYPE_S32);
	debug_table.create_entry("start_simulation", &start_simulation_key, TYPE_S32);
	debug_table.create_entry("move_objects", &move_objects, TYPE_S32);
	debug_table.create_entry("gravity_x", &ggs.gravity.x, TYPE_R32);
	debug_table.create_entry("gravity_y", &ggs.gravity.y, TYPE_R32);
	debug_table.create_entry("gravity_z", &ggs.gravity.z, TYPE_R32);
	debug_table.create_entry("toggle_simulate", &toggle_simulate_key, TYPE_S32);
}

void init_application()
{
	Texture* texture = new Texture(std::string("res/textures/wood.png"));

	init_camera(&ggs.camera, (float)win_state.win_width / win_state.win_height, 45.0f, 0.2f, 1000.0f);
	ggs.camera.set_cam_position(vec3(5.0f, 30.0f, 60.0f));
	ggs.shader = load_shader(vert_shader, frag_shader, sizeof(vert_shader) - 1, sizeof(frag_shader) - 1);

	// Models
	ggs.models = (IndexedModel3D*)malloc(sizeof(IndexedModel3D) * 256);
	ggs.gravity = vec3(0, -10, 0);

	load_model("res/cube.obj", &ggs.models[0]);
	init_object(&ggs.models[0]);
	ggs.models[0].position = vec3(0.0f, 45.0f, 0.0f);
	ggs.models[0].scale = 0.3f;
	ggs.models[0].simulating = true;
	ggs.models[0].static_object = false;
	ggs.models[0].texture = texture;
	ggs.models[0].last_pos = ggs.models[0].position;

	load_model("res/cube.obj", &ggs.models[1]);
	init_object(&ggs.models[1]);
	ggs.models[1].scale = 1.0f;
	ggs.models[1].simulating = false;
	ggs.models[1].static_object = true;
	ggs.models[1].texture = texture;
	ggs.models[1].last_pos = ggs.models[1].position;

	load_model("res/esfera_10.obj", &ggs.models[2]);
	init_object(&ggs.models[2]);
	ggs.models[2].position = vec3(-15.0f, 45.0f, 0.0f);
	ggs.models[2].scale = 0.3f;
	ggs.models[2].simulating = true;
	ggs.models[2].static_object = false;
	ggs.models[2].texture = texture;
	ggs.models[2].last_pos = ggs.models[2].position;

	load_model("res/cube.obj", &ggs.models[3]);
	init_object(&ggs.models[3]);
	ggs.models[3].position = vec3(-15.0f, 75.0f, 0.0f);
	ggs.models[3].scale = 0.3f;
	ggs.models[3].simulating = true;
	ggs.models[3].static_object = false;
	ggs.models[3].texture = texture;
	ggs.models[3].last_pos = ggs.models[3].position;

	ggs.num_models = 4;

	init_debug_entries();

	// opengl
	glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
}

void render_object(IndexedModel3D* model) {
	if (model->texture) {
		glBindTexture(GL_TEXTURE_2D, model->texture->textureID);
	}
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(model->vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ebo);
	glBindBuffer(GL_ARRAY_BUFFER, model->vbo);

	glUniformMatrix4fv(glGetUniformLocation(ggs.shader, "persp_matrix"), 1, GL_FALSE, (GLfloat*)ggs.camera.projection_matrix.data);
	glUniformMatrix4fv(glGetUniformLocation(ggs.shader, "view_matrix"), 1, GL_FALSE, (GLfloat*)ggs.camera.view_matrix.data);
	mat4 ident;
	mat4::identity(ident);
	glUniformMatrix4fv(glGetUniformLocation(ggs.shader, "model_matrix"), 1, GL_TRUE, (GLfloat*)model->model_matrix.m);
	glUniform1i(glGetUniformLocation(ggs.shader, "is_colliding"), model->is_colliding);

	vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
	vec4 green(0.0f, 1.0f, 0.0f, 1.0f);
	if (model->is_colliding) {
		glUniform4fv(glGetUniformLocation(ggs.shader, "vertex_color"), 1, (float*)&red);
	}
	else {
		glUniform4fv(glGetUniformLocation(ggs.shader, "vertex_color"), 1, (float*)&green);
	}
	u32 render_form = GL_TRIANGLES;
	if (ggs.wireframe) render_form = GL_LINES;
	glDrawElements(render_form, model->num_indices, GL_UNSIGNED_SHORT, 0);
}

void render_object_default(hm::vec3 position, float scale) {
	IndexedModel3D* model = &ggs.models[2];
	if (model->texture) {
		glBindTexture(GL_TEXTURE_2D, model->texture->textureID);
	}
	glBindVertexArray(model->vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ebo);
	glBindBuffer(GL_ARRAY_BUFFER, model->vbo);

	glUniformMatrix4fv(glGetUniformLocation(ggs.shader, "persp_matrix"), 1, GL_FALSE, (GLfloat*)ggs.camera.projection_matrix.data);
	glUniformMatrix4fv(glGetUniformLocation(ggs.shader, "view_matrix"), 1, GL_FALSE, (GLfloat*)ggs.camera.view_matrix.data);
	mat4 pos = mat4::translate(position);
	mat4 scale_mat = mat4::scale(scale);
	mat4 model_mat = pos * scale_mat;

	glUniformMatrix4fv(glGetUniformLocation(ggs.shader, "model_matrix"), 1, GL_TRUE, (GLfloat*)model_mat.m);

	vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
	vec4 green(0.0f, 1.0f, 0.0f, 1.0f);
	if (model->is_colliding) {
		glUniform4fv(glGetUniformLocation(ggs.shader, "vertex_color"), 1, (float*)&red);
	}
	else {
		glUniform4fv(glGetUniformLocation(ggs.shader, "vertex_color"), 1, (float*)&green);
	}
	u32 render_form = GL_TRIANGLES;
	if (ggs.wireframe) render_form = GL_LINES;
	glDrawElements(render_form, model->num_indices, GL_UNSIGNED_SHORT, 0);
}

void input()
{
	if (!win_state.do_input) return;

	float velocity = 0.5f;
	float rot_velocity = 3.0f;

	if (keyboard_state.key[VK_SHIFT]) {
		velocity = 0.005f;
		rot_velocity = -3.0f;
	}
	if (keyboard_state.key[turn_left_model]) {
		Quaternion local2 = QuatFromAxisAngle(vec3(0, 1, 0), rot_velocity);
		ggs.models[model_input_index].rotation = local2 * ggs.models[model_input_index].rotation;
	}
	if (keyboard_state.key[turn_right_model]) {
		Quaternion local2 = QuatFromAxisAngle(vec3(0, -1, 0), rot_velocity);
		ggs.models[model_input_index].rotation = local2 * ggs.models[model_input_index].rotation;
	}
	if (keyboard_state.key[move_forward_model]) {
		vec3 res = RotFromQuat(ggs.models[model_input_index].rotation) * vec3(0, 0, -1);
		ggs.models[model_input_index].position = ggs.models[model_input_index].position + res;
	}
	if (keyboard_state.key[move_backward_model]) {
		vec3 res = RotFromQuat(ggs.models[model_input_index].rotation) * vec3(0, 0, -1);
		ggs.models[model_input_index].position = ggs.models[model_input_index].position - res;
	}

	if (keyboard_state.key[move_up_model]) {
		ggs.models[model_input_index].last_pos.y = ggs.models[model_input_index].position.y;
		ggs.models[model_input_index].position.y += velocity;
	}
	if (keyboard_state.key[move_down_model]) {
		ggs.models[model_input_index].last_pos.y = ggs.models[model_input_index].position.y;
		ggs.models[model_input_index].position.y -= velocity;
	}
	
	if (keyboard_state.key[rotate_x_model]) {
		Quaternion local2 = QuatFromAxisAngle(vec3(1, 0, 0), rot_velocity);
		ggs.models[model_input_index].rotation = local2 * ggs.models[model_input_index].rotation;
	}

	if (keyboard_state.key[rotate_y_model]) {
		Quaternion local2 = QuatFromAxisAngle(vec3(0, 1, 0), rot_velocity);
		ggs.models[model_input_index].rotation = local2 * ggs.models[model_input_index].rotation;
	}

	if (keyboard_state.key[rotate_z_model]) {
		Quaternion local2 = QuatFromAxisAngle(vec3(0, 0, 1), rot_velocity);
		ggs.models[model_input_index].rotation = local2 * ggs.models[model_input_index].rotation;
	}

	if (keyboard_state.key_event[start_simulation_key]) {
		ggs.start_simulation = !ggs.start_simulation;
		keyboard_state.key_event[start_simulation_key] = false;
	}
	if (keyboard_state.key_event[move_objects]) {
		ggs.move_objects = !ggs.move_objects;
		keyboard_state.key_event[move_objects] = false;
	}
	if (keyboard_state.key_event[toggle_simulate_key]) {
		ggs.models[model_input_index].simulating = !ggs.models[model_input_index].simulating;
		ggs.models[model_input_index].static_object = !ggs.models[model_input_index].static_object;
		keyboard_state.key_event[toggle_simulate_key] = false;
	}

	if (keyboard_state.key_event['I']) {
		ggs.wireframe = !ggs.wireframe;
		keyboard_state.key_event['I'] = false;
	}
}

void update(IndexedModel3D* im) {
	if (ggs.move_objects) return;
	vec3 acceleration = ggs.gravity;

	const float time_step = 1.0f / 250.0f;
	im->time += time_step;
	im->velocity = acceleration * im->time + im->velocity;
	im->position = im->velocity * im->time + im->position;
}

void update_model(IndexedModel3D* im) 
{
	bool colliding = false;

	vec3 last_pos = im->position;
	mat4 rot = RotFromQuat(im->rotation);

	vec3 pos = im->position;
	mat4 rotation_matrix = rot;
	mat4 scale_matrix = mat4::scale(im->scale);
	mat4 final_matrix = mat4::translate(pos) * rotation_matrix * scale_matrix;

	im->model_matrix = final_matrix;
}

void update_and_render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(ggs.shader);
	
	input_camera(&ggs.camera);
	input();

#if 0
	for (int i = 0; i < ggs.num_models; ++i) {
		update_model(ggs.models + i);
		render_object(ggs.models + i);
	}
#endif
	//glUseProgram(0);
}