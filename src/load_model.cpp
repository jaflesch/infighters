//#include "util.h"

struct Token_Model {
	u8* value;
	int size;
	union {
		int int_v;
		float float_v;
	};
};

static bool str_equal(const char* str1, int str1_size, const char* str2, int str2_size)
{
	if (str1_size == str2_size) {
		for (int i = 0; i < str1_size; ++i) {
			if (str1[i] != str2[i])
				return false;
		}
		return true;
	}
	return false;
}

float parse_float(u8** at) {
	bool floating_point = false;
	int i = 0;
	int length = 0;
	do {
		++i;
		length++;
		if ((*at)[i] == '.' && !floating_point) {
			floating_point = true;
			++i;
			length++;
		}
	} while ((*at)[i] && (is_number((*at)[i])));
	char buffer[128] = { 0 };
	memcpy(buffer, (void*)*at, length);
	float result = atof(buffer);
	*at += length;
	return result;
}

int parse_int(u8** at)
{
	int length = 0;
	while (is_number((*at)[length])) {
		length++;
	}
	char buffer[128] = { 0 };
	memcpy(buffer, (void*)*at, length);
	*at += length;
	return atoi(buffer);
}

void goto_next_line(u8** at) {
	while (**at != '\n') (*at)++;
}

void eat_whitespace(u8** at)
{
	while (is_white_space(**at)) {
		(*at)++;
	}
}

void parse_normal(u8** at, Vertex3D* vertex_data, int index)
{
	float x, y, z;

	eat_whitespace(at);
	x = parse_float(at);
	eat_whitespace(at);
	y = parse_float(at);
	eat_whitespace(at);
	z = parse_float(at);
	goto_next_line;

	(vertex_data + index)->normal[0] = x;
	(vertex_data + index)->normal[1] = y;
	(vertex_data + index)->normal[2] = z;
}

void parse_vertex(u8** at, Vertex3D* vertex_data, int index)
{
	float x, y, z;

	eat_whitespace(at);
	x = parse_float(at);
	eat_whitespace(at);
	y = parse_float(at);
	eat_whitespace(at);
	z = parse_float(at);
	goto_next_line;

	(vertex_data + index)->pos[0] = x;
	(vertex_data + index)->pos[1] = y;
	(vertex_data + index)->pos[2] = z;
}

void parse_texture_coord(u8** at, Vertex3D* vertex_data, int index)
{
	float x, y;

	eat_whitespace(at);
	x = parse_float(at);
	eat_whitespace(at);
	y = parse_float(at);
	goto_next_line(at);

	(vertex_data + index)->tex[0] = x;
	(vertex_data + index)->tex[1] = y;
}

struct IndexData {
	int vertex;
	int texcoord;
	int normal;
};

void parse_triangle(u8** at, IndexData* index_data, int *index) {
	int x, y, z;
	int vindex = 0, tindex = 0, nindex = 0;
	eat_whitespace(at);

	for (int i = 0; i < 3; ++i)
	{
		vindex = parse_int(at); (*at)++;
		tindex = parse_int(at); (*at)++;
		nindex = parse_int(at); (*at)++;

		(index_data + *index)->vertex = vindex - 1;
		(index_data + *index)->texcoord = tindex - 1;
		(index_data + *index)->normal = nindex - 1;

		eat_whitespace(at);
		(*index)++;
	}

}

void to_index_model(IndexedModel3D* model, Vertex3D* verts, IndexData* indices, int num_pos, int num_texcoord, int num_normal, int num_indices);

void load_objfile(char* filename, IndexedModel3D* model) 
{
	s64 size = 0;
	u8* data = read_entire_file((u8*)filename, &size);
	u8* start_data = data;
	if (!data) {
		char buffer[512] = { 0 };
		sprintf(buffer, "could not read file %s\n", filename);
		error_fatal("File IO error.\n", buffer);
	}

	Vertex3D* vertex_data = (Vertex3D*)malloc(size * sizeof(Vertex3D));
	IndexData* index_data = (IndexData*)malloc(size * sizeof(IndexData));

	int position_index = 0;
	int texcoord_index = 0;
	int normal_index = 0;
	int faces_index = 0;

	while (data != 0)
	{
		//getline(objFile, line);
		if (data[0] == 'v' && data[1] == ' ') {
			data++;
			parse_vertex(&data, vertex_data, position_index);
			position_index++;
		} else if (data[0] == 'v' && data[1] == 't') {
			data += 2;
			parse_texture_coord(&data, vertex_data, texcoord_index);
			texcoord_index++;
		} else if (data[0] == 'v' && data[1] == 'n') {
			data += 2;
			parse_normal(&data, vertex_data, normal_index);
			normal_index++;
		} else if (data[0] == 'f' && data[1] == ' ') {
			data++;
			parse_triangle(&data, index_data, &faces_index);
		} else {
			while (*data != '\n') {
				if (*data == 0) break;
				data++;
			}
			if (*data == 0) break;
			data++;
		}
	}
#if 0
	model->bshape.num_vertices = position_index;
	model->bshape.vertices = (vec3*)malloc(model->bshape.num_vertices * sizeof(vec3));
	model->bshape_temp.num_vertices = position_index;
	model->bshape_temp.vertices = (vec3*)malloc(model->bshape.num_vertices * sizeof(vec3));

	for (int i = 0; i < model->bshape.num_vertices; ++i) {
		model->bshape.vertices[i] = vec3(vertex_data[i].pos[0], vertex_data[i].pos[1], vertex_data[i].pos[2]);
	}
	memcpy(model->bshape_temp.vertices, model->bshape.vertices, model->bshape.num_vertices * sizeof(vec3));
#endif
	to_index_model(model, vertex_data, index_data, position_index, texcoord_index, normal_index, faces_index);
}


void to_index_model(IndexedModel3D* model, Vertex3D* verts, IndexData* indices, int num_pos, int num_texcoord, int num_normal, int num_indices)
{
	int max = num_indices;

	Vertex3D* new_vertices = (Vertex3D*)malloc(max * sizeof(Vertex3D));
	u16* new_indices = (u16*)malloc(num_indices * sizeof(u16));

	for (int i = 0; i < num_indices; ++i) {
		int pos_index = indices[i].vertex;
		int normal_index = indices[i].normal;
		int tcoord_index = indices[i].texcoord;

		new_vertices[i].pos[0] = verts[pos_index].pos[0];
		new_vertices[i].pos[1] = verts[pos_index].pos[1]; 
		new_vertices[i].pos[2] = verts[pos_index].pos[2];

		new_vertices[i].tex[0] = verts[tcoord_index].tex[0];
		new_vertices[i].tex[1] = verts[tcoord_index].tex[1];

		new_vertices[i].normal[0] = verts[normal_index].normal[0];
		new_vertices[i].normal[1] = verts[normal_index].normal[1];
		new_vertices[i].normal[2] = verts[normal_index].normal[2];

		new_indices[i] = i;
	}

	model->indices = new_indices;
	model->vertices = new_vertices;
	model->num_indices = num_indices;
	model->num_vertices = max;

	free(verts);
	free(indices);
}
