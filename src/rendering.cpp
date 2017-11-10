void load_model(char* filename, IndexedModel3D* model) {
	load_objfile(filename, model);
	hm::mat4::identity(model->model_matrix);
	model->position = hm::vec3(0.0f, 0.0f, 0.0f);
	model->rotation = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
	model->is_colliding = false;
}

void update_model(IndexedModel3D* im)
{
	hm::vec3 last_pos = im->position;
	mat4 rot = RotFromQuat(im->rotation);

	hm::vec3 pos = im->position;
	mat4 rotation_matrix = rot;
	mat4 scale_matrix = mat4::scale(im->scale);
	mat4 final_matrix = mat4::translate(pos) * rotation_matrix * scale_matrix;

	im->model_matrix = final_matrix;
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

void render_line(hm::vec3 start, hm::vec3 end)
{
	mat4 ident;
	mat4::identity(ident);
	hm::vec4 magenta(1, 0, 1, 1);
	glUniformMatrix4fv(glGetUniformLocation(ggs.shader, "model_matrix"), 1, GL_TRUE, (float*)ident.data);
	glUniform4fv(glGetUniformLocation(ggs.shader, "vertex_color"), 1, (float*)&magenta);
	glLineWidth(4.0f);
	glBegin(GL_LINES);
	glVertex3f(start.x, start.y, start.z);
	glVertex3f(end.x, end.y, end.z);
	glEnd();
	glLineWidth(1.0f);
}

void render_vector(hm::vec3 vec, hm::vec3 position)
{
	mat4 ident;
	mat4::identity(ident);
	hm::vec4 black(0, 0, 0, 1);
	glUniformMatrix4fv(glGetUniformLocation(ggs.shader, "model_matrix"), 1, GL_TRUE, (float*)ident.data);
	glUniform4fv(glGetUniformLocation(ggs.shader, "vertex_color"), 1, (float*)&black);
	glBegin(GL_LINES);
	glVertex3f(position.x, position.y, position.z);
	glVertex3f(vec.x + position.x, vec.y + position.y, vec.z + position.z);
	glEnd();
}

void render_face(hm::vec3 p1, hm::vec3 p2, hm::vec3 p3, hm::vec3 c) {
	mat4 ident;
	mat4::identity(ident);
	hm::vec4 color(c.r, c.g, c.b, 1);
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

	hm::vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
	hm::vec4 green(0.0f, 1.0f, 0.0f, 1.0f);
	if (model->is_colliding) {
		glUniform4fv(glGetUniformLocation(ggs.shader, "vertex_color"), 1, (float*)&red);
	}
	else {
		glUniform4fv(glGetUniformLocation(ggs.shader, "vertex_color"), 1, (float*)&green);
	}
	u32 render_form = GL_TRIANGLES;
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

	hm::vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
	hm::vec4 green(0.0f, 1.0f, 0.0f, 1.0f);
	if (model->is_colliding) {
		glUniform4fv(glGetUniformLocation(ggs.shader, "vertex_color"), 1, (float*)&red);
	}
	else {
		glUniform4fv(glGetUniformLocation(ggs.shader, "vertex_color"), 1, (float*)&green);
	}
	u32 render_form = GL_TRIANGLES;
	glDrawElements(render_form, model->num_indices, GL_UNSIGNED_SHORT, 0);
}

void create_object(char* filename)
{
	int index = ggs.num_models;
	if (index == 256) return;
	memset(&ggs.models[index], 0, sizeof(IndexedModel3D));
	load_model(filename, &ggs.models[index]);
	init_object(&ggs.models[index]);
	ggs.models[index].position = hm::vec3(0.0f, 45.0f, 0.0f);
	ggs.models[index].scale = 0.3f;
	ggs.models[index].simulating = true;
	ggs.models[index].static_object = false;
	ggs.models[index].texture = 0;

	ggs.num_models++;
}