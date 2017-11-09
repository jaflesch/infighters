#include "application.h"
#include "input.h"
#include "quaternion.h"
#include "ResourceLoad/Texture.h"
#include "debug_table.h"
#include "WindowApi/Window.h"
#include "chat.h"

extern Window_State win_state;
extern DebugTable debug_table;
Chat* g_chat = 0;
Chat chat;
linked::Window* chat_window = 0;

#include "camera.cpp"
#include "load_model.cpp"

#define NUM_CHARS 12
#define NUM_SKILLS 4

char* char_names[NUM_CHARS] = {
	"Zer0",
	"On1",
	"Serial K3YLL3R",
	"R4y Tr4c3y",
	"A-St4r",
	"D3ADL0CK",
	"N0rma",
	"Haz4rd",
	"Qwerty",
	"=Big-O=",
	"New()",
	"Cl0ckb0y"
};

int char_names_length[NUM_CHARS] = {
	sizeof "Zer0",
	sizeof "On1",
	sizeof "Serial K3YLL3R",
	sizeof "R4y Tr4c3y",
	sizeof "A-St4r",
	sizeof "D3ADL0CK",
	sizeof "N0rma",
	sizeof "Haz4rd",
	sizeof "Qwerty",
	sizeof "=Big-O=",
	sizeof "New()",
	sizeof "Cl0ckb0y"
};


char* skill_names[NUM_CHARS * NUM_SKILLS] = {
	"FALSE RUSH",
	"CONTRADICTION",
	"REQUIEM ZERO",
	"VOID BARRIER",

	"TRUTH SLASH",
	"TAUTOLOGY",
	"AXIOM ONE",
	"TRUE ENDURANCE",

	"BRUTE FORCE",
	"BUFFER OVERFLOW",
	"DDOS ATTACK",
	"ENCRYPTION",

	"PARTICLE RENDERING",
	"DIFFUSE REFLECTION",
	"DYNAMIC FRUSTUM ATTACK",
	"RASTERIZATION",

	"Q-PUNCH",
	"PERCEPTRON",
	"NEURAL NETWORK",
	"HILL CLIMBING",

	"PREEMPTION",
	"MUTEX",
	"THREAD SCHEDULING",
	"FORK()",

	"PUMPING UP",
	"AUTOMATA SUMMON",
	"TURING MACHINE",
	"NON-DETERMINISM",

	"TMR",
	"REDUNDANCY",
	"ROLLBACK",
	"ROLLFORWARD",

	"ALT",
	"CRTL",
	"DELETE",
	"ESC",

	"BEST BOUND FIRST",
	"DUAL SIMPLEX",
	"GRAPH COLORING",
	"KNAPSACK HIDEOUT",

	"SPRINT BURST",
	"INHERITANCE",
	"OVERRIDE",
	"POLIMORPHISM",

	"CLOCK PULSE",
	"PIPELINE",
	"OVERCLOCK",
	"BRANCH DAMAGE"
};

int skill_names_length[sizeof(skill_names) / sizeof(char*)] = {
	sizeof "FALSE RUSH",
	sizeof "CONTRADICTION",
	sizeof "REQUIEM ZERO",
	sizeof "VOID BARRIER",

	sizeof "TRUTH SLASH",
	sizeof "TAUTOLOGY",
	sizeof "AXIOM ONE",
	sizeof "TRUE ENDURANCE",

	sizeof "BRUTE FORCE",
	sizeof "BUFFER OVERFLOW",
	sizeof "DDOS ATTACK",
	sizeof "ENCRYPTION",

	sizeof "PARTICLE RENDERING",
	sizeof "DIFFUSE REFLECTION",
	sizeof "DYNAMIC FRUSTUM ATTACK",
	sizeof "RASTERIZATION",

	sizeof "Q-PUNCH",
	sizeof "PERCEPTRON",
	sizeof "NEURAL NETWORK",
	sizeof "HILL CLIMBING",

	sizeof "PREEMPTION",
	sizeof "MUTEX",
	sizeof "THREAD SCHEDULING",
	sizeof "FORK()",

	sizeof "PUMPING UP",
	sizeof "AUTOMATA SUMMON",
	sizeof "TURING MACHINE",
	sizeof "NON-DETERMINISM",

	sizeof "TMR",
	sizeof "REDUNDANCY",
	sizeof "ROLLBACK",
	sizeof "ROLLFORWARD",

	sizeof "ALT",
	sizeof "CRTL",
	sizeof "DELETE",
	sizeof "ESC",

	sizeof "BEST BOUND FIRST",
	sizeof "DUAL SIMPLEX",
	sizeof "GRAPH COLORING",
	sizeof "KNAPSACK HIDEOUT",

	sizeof "SPRINT BURST",
	sizeof "INHERITANCE",
	sizeof "OVERRIDE",
	sizeof "POLIMORPHISM",

	sizeof "CLOCK PULSE",
	sizeof "PIPELINE",
	sizeof "OVERCLOCK",
	sizeof "BRANCH DAMAGE"
};

char* skill_desc[NUM_SKILLS * NUM_CHARS] = {
	// Zer0
	"Ataca um oponente e realiza 20 de dano.",
	"Marca um adversário e, se no próximo turno ele realizar um ataque em\nZer0, seu golpe é negado, sofre 20 de dano e recebe status Paralyze\npor 1 turno.",
	"Cria uma atmosfera negativa no campo de batalha. False Rush e\nContradiction acertam todos os adversários durante 3 turnos.",
	"Utilizando uma barreira criada após um vácuo, Zer0 fica invulnerável\npor 1 turno.",

	// On1
	"Ataca um oponente e realiza 30 de dano.",
	"Ataca um inimigo e realiza 15 de dano piercing. Se, no próximo turno,\no adversário utilizar qualquer habilidade, sofrerá 15 de dano crushing.",
	"On1 canaliza sua positividade e aumenta suas habilidades. Por 3\nturnos, On1 tem redução de dano de 15% e Truth Slash e Tautology\ngastam uma energia a menos.",
	"Concentrando toda sua energia positiva, On1 obtém uma defesa\nabsoluta e fica invulnerável por 1 turno.",

	// Serial
	"Ataca um oponente e realiza 20 de dano. Este golpe ignora INVUL.\nPorém, o adversário recebe 10 de dano se estiver nesse status.",
	"Infringe 15 de dano piercing a um oponente e adiciona status STUN\npor 1 turno.",
	"Todos os adversários recebem status PARALYZE por 3 turnos.",
	"Envolto por uma densa camada encriptada, Serial K3yll3R fica\ninvulnerável por 1 turno.",

	// Ray
	"Ataca um oponente, realiza 15 de dano e R4y Tr4c3y fica\ninvulnerável a golpes PHYSICAL por 1 turno.",
	"Por 1 turno, todos os aliados recebem REFLEX DEFENSE.",
	"Aplica um golpe em todos os inimigos e realiza 35 de dano normal.",
	"Usando suas técnicas de computação gráfica, R4y Tr4c3y fica\ninvulnerável por 1 turno.",

	// AStar
	"Ataca um oponente e realiza 20 de dano. Para cada vez que um\noponente recebe novamente este golpe, o dano é aumentado em 5\npontos. O efeito é acumulativo.",
	"Produz 25 de dano crushing a um oponente.",
	"Durante 4 turnos, todos os adversários atingidos por Neural Network\npodem receber o ataque Perceptron. A-St4r recebe 30 pontos de ABSORPTION.",
	"Para se proteger, A-St4r corre até as colinas e fica invulnerável por 1 turno.",

	// Deadlock
	"Ataca um oponente e infringe 25 de dano. Caso ele tenha alguma\nhabilidade com duração CONTROL, elimina a habilidade e este sofre\nstatus SLEEP por 2 turnos.",
	"Atinge todos os adversários e, no próximo turno, apenas um pode\nutilizar alguma habilidade.",
	"Acerta um inimigo aleatório e coloca o status SLEEP por 3 turnos.",
	"D3ADL0CK cria um clone seu para receber golpe no seu lugar,\nengana o adversário e fica invulnerável por 1 turno.",

	// Norma
	"Ataca um oponente, realiza 25 de dano piercing e há 50% de chance\nde causar status BURN por 3 turnos.",
	"Invoca um aliado, garantindo 15 pontos de defesa ABSORPTION.\nO adversário que atacar N0rma recebe o status POISON por 2 turnos.",
	"Acerta todos os adversários com 30 de dano. Só pode ser utilizado\nenquanto a habilidade Automata Summon estiver ativa.",
	"Aproveitando o universo não-determinístico, N0rma se esconde num\nestado protegido e e fica invulnerável por 1 turno.",

	// Hazard
	"Ataca todos os oponentes e realiza 15 de dano. Haz4rd recebe 5\npontos de defesa ABSORPTION por cada ataque bem sucedido.",
	"Cria uma barreira redundante para os aliados. Cada personagem\nrecebe 15 pontos de redução de defesa por 3 turnos.",
	"Cura qualquer status negativo de um aliado.",
	"Concentrando toda sua energia positiva, On1 obtém uma defesa\nabsoluta e fica invulnerável por 1 turno.",

	// Qwerty
	"Qwerty altera sua forma física e ganha redução de dano de 40 pontos\npor 4 turnos.",
	"Controla um inimigo por dois turnos. DELETE pode ser usado no\nadversário controlado.",
	"Mata instantaneamente qualquer adversário que recebe este golpe,\ndesde que esteja sob efeito de CTRL.",
	"Concentrando toda sua energia positiva, On1 obtém uma defesa\nabsoluta e fica invulnerável por 1 turno.",

	// Big O
	"Ataca o oponente com menor HP e realiza 20 de dano. Por 2 turnos,\no alvo fica com status BURN. Se o alvo já possui status BURN, este\nsofre 10 de dano crushing adicional.",
	"Se utilizado em um adversário, gera status BURN por 3 turnos. Se,\nem um aliado, recupera 25 de energia e atribui status FROZEN por 2 turnos.",
	"No próximo turno, recebe 4 orbs de energia: uma de cada categoria.",
	"Concentrando toda sua energia positiva, On1 obtém uma defesa\nabsoluta e fica invulnerável por 1 turno.",

	// New
	"Ataca um oponente e realiza 25 de dano. New() recebe redução de\ndano de 10 pontos no próximo turno.",
	"Seleciona um inimigo e se, no próximo turno, ele utilizar um ataque\nque não seja UNIQUE nem exija uma técnica de preparação, sua\nhabilidade será copiada e poderá ser usada por New() também.",
	"Cura todos os status negativos de um aliado.",
	"Alterando sua estrutura física de forma dinâmica, New() fica\ninvulnerável por 1 turno.",

	// Clockboy
	"Desfera 20 de dano a um adversário. Atribui status PARALYZE por 2 turnos.",
	"Durante 2 turnos, nenhum golpe de Cl0ckB0y sofre COUNTER.",
	"Cl0ckb0y fica invulnerável por 2 turnos, perde 50% de sua energia e\nseus ataques causam 500% a mais de dano.",
	"Assumindo que a batalha levou a uma condição de dano, Cl0ckB0y\nfaz um pulo condicional que o torna invulnerável por 1 turno."
};

int skill_desc_length[sizeof(skill_desc) / sizeof(char*)] = {
	// Zer0
	sizeof "Ataca um oponente e realiza 20 de dano.",
	sizeof "Marca um adversário e, se no próximo turno ele realizar um ataque em\nZer0, seu golpe é negado, sofre 20 de dano e recebe status Paralyze\npor 1 turno.",
	sizeof "Cria uma atmosfera negativa no campo de batalha. False Rush e\nContradiction acertam todos os adversários durante 3 turnos.",
	sizeof "Utilizando uma barreira criada após um vácuo, Zer0 fica invulnerável\npor 1 turno.",

	// On1
	sizeof "Ataca um oponente e realiza 30 de dano.",
	sizeof "Ataca um inimigo e realiza 15 de dano piercing. Se, no próximo turno,\no adversário utilizar qualquer habilidade, sofrerá 15 de dano crushing.",
	sizeof "On1 canaliza sua positividade e aumenta suas habilidades. Por 3\nturnos, On1 tem redução de dano de 15% e Truth Slash e Tautology\ngastam uma energia a menos.",
	sizeof "Concentrando toda sua energia positiva, On1 obtém uma defesa\nabsoluta e fica invulnerável por 1 turno.",

	// Serial
	sizeof "Ataca um oponente e realiza 20 de dano. Este golpe ignora INVUL.\nPorém, o adversário recebe 10 de dano se estiver nesse status.",
	sizeof "Infringe 15 de dano piercing a um oponente e adiciona status STUN\npor 1 turno.",
	sizeof "Todos os adversários recebem status PARALYZE por 3 turnos.",
	sizeof "Envolto por uma densa camada encriptada, Serial K3yll3R fica\ninvulnerável por 1 turno.",

	// Ray
	sizeof "Ataca um oponente, realiza 15 de dano e R4y Tr4c3y fica\ninvulnerável a golpes PHYSICAL por 1 turno.",
	sizeof "Por 1 turno, todos os aliados recebem REFLEX DEFENSE.",
	sizeof "Aplica um golpe em todos os inimigos e realiza 35 de dano normal.",
	sizeof "Usando suas técnicas de computação gráfica, R4y Tr4c3y fica\ninvulnerável por 1 turno.",

	// AStar
	sizeof "Ataca um oponente e realiza 20 de dano. Para cada vez que um\noponente recebe novamente este golpe, o dano é aumentado em 5\npontos. O efeito é acumulativo.",
	sizeof "Produz 25 de dano crushing a um oponente.",
	sizeof "Durante 4 turnos, todos os adversários atingidos por Neural Network\npodem receber o ataque Perceptron. A-St4r recebe 30 pontos de ABSORPTION.",
	sizeof "Para se proteger, A-St4r corre até as colinas e fica invulnerável por 1 turno.",

	// Deadlock
	sizeof "Ataca um oponente e infringe 25 de dano. Caso ele tenha alguma\nhabilidade com duração CONTROL, elimina a habilidade e este sofre\nstatus SLEEP por 2 turnos.",
	sizeof "Atinge todos os adversários e, no próximo turno, apenas um pode\nutilizar alguma habilidade.",
	sizeof "Acerta um inimigo aleatório e coloca o status SLEEP por 3 turnos.",
	sizeof "D3ADL0CK cria um clone seu para receber golpe no seu lugar,\nengana o adversário e fica invulnerável por 1 turno.",

	// Norma
	sizeof "Ataca um oponente, realiza 25 de dano piercing e há 50% de chance\nde causar status BURN por 3 turnos.",
	sizeof "Invoca um aliado, garantindo 15 pontos de defesa ABSORPTION.\nO adversário que atacar N0rma recebe o status POISON por 2 turnos.",
	sizeof "Acerta todos os adversários com 30 de dano. Só pode ser utilizado\nenquanto a habilidade Automata Summon estiver ativa.",
	sizeof "Aproveitando o universo não-determinístico, N0rma se esconde num\nestado protegido e e fica invulnerável por 1 turno.",

	// Hazard
	sizeof "Ataca todos os oponentes e realiza 15 de dano. Haz4rd recebe 5\npontos de defesa ABSORPTION por cada ataque bem sucedido.",
	sizeof "Cria uma barreira redundante para os aliados. Cada personagem\nrecebe 15 pontos de redução de defesa por 3 turnos.",
	sizeof "Cura qualquer status negativo de um aliado.",
	sizeof "Concentrando toda sua energia positiva, On1 obtém uma defesa\nabsoluta e fica invulnerável por 1 turno.",

	// Qwerty
	sizeof "Qwerty altera sua forma física e ganha redução de dano de 40 pontos\npor 4 turnos.",
	sizeof "Controla um inimigo por dois turnos. DELETE pode ser usado no\nadversário controlado.",
	sizeof "Mata instantaneamente qualquer adversário que recebe este golpe,\ndesde que esteja sob efeito de CTRL.",
	sizeof "Concentrando toda sua energia positiva, On1 obtém uma defesa\nabsoluta e fica invulnerável por 1 turno.",

	// Big O
	sizeof "Ataca o oponente com menor HP e realiza 20 de dano. Por 2 turnos,\no alvo fica com status BURN. Se o alvo já possui status BURN, este\nsofre 10 de dano crushing adicional.",
	sizeof "Se utilizado em um adversário, gera status BURN por 3 turnos. Se,\nem um aliado, recupera 25 de energia e atribui status FROZEN por 2 turnos.",
	sizeof "No próximo turno, recebe 4 orbs de energia: uma de cada categoria.",
	sizeof "Concentrando toda sua energia positiva, On1 obtém uma defesa\nabsoluta e fica invulnerável por 1 turno.",

	// New
	sizeof "Ataca um oponente e realiza 25 de dano. New() recebe redução de\ndano de 10 pontos no próximo turno.",
	sizeof "Seleciona um inimigo e se, no próximo turno, ele utilizar um ataque\nque não seja UNIQUE nem exija uma técnica de preparação, sua\nhabilidade será copiada e poderá ser usada por New() também.",
	sizeof "Cura todos os status negativos de um aliado.",
	sizeof "Alterando sua estrutura física de forma dinâmica, New() fica\ninvulnerável por 1 turno.",

	// Clockboy
	sizeof "Desfera 20 de dano a um adversário. Atribui status PARALYZE por 2 turnos.",
	sizeof "Durante 2 turnos, nenhum golpe de Cl0ckB0y sofre COUNTER.",
	sizeof "Cl0ckb0y fica invulnerável por 2 turnos, perde 50% de sua energia e\nseus ataques causam 500% a mais de dano.",
	sizeof "Assumindo que a batalha levou a uma condição de dano, Cl0ckB0y\nfaz um pulo condicional que o torna invulnerável por 1 turno."
};

enum Character_ID {
	CHAR_NONE = -1,
	CHAR_ZERO = 0,
	CHAR_ONE,
	CHAR_SERIAL_KEYLLER,
	CHAR_RAY_TRACEY,
	CHAR_A_STAR,
	CHAR_DEADLOCK,
	CHAR_NORMA,
	CHAR_HAZARD,
	CHAR_QWERTY,
	CHAR_BIG_O,
	CHAR_NEW,
	CHAR_CLOCKBOY,
};

enum Game_Mode {
	MODE_INTRO,
	MODE_CHAR_SELECT,
	MODE_CHAR_INFO,
	MODE_COMBAT,
};

struct GameState {
	Camera camera;
	GLuint shader;

	IndexedModel3D* models;
	int num_models;

	Game_Mode mode;
	Game_Mode last_mode;
};

static GameState ggs = {};

#define PLAYING_CHARACTERS 3
struct Char_Selection_State {
	int num_selected;
	int selections[PLAYING_CHARACTERS];
	linked::WindowDiv* play_button_div;
	Character_ID last_hovered;
};

Char_Selection_State char_sel_state = {};

struct Char_Info_State {

};

struct Game_Windows {
	// background window
	linked::Window* bgwindow;

	// character selection
	linked::Window* left_char_window;
	linked::Window* char_selected_window;
	linked::Window* char_selection_window;

	// character info
	linked::Window* char_info_window;
	linked::Window* char_info_window_bot;
};

Game_Windows gw = {};

void load_model(char* filename, IndexedModel3D* model) {
	load_objfile(filename, model);
	hm::mat4::identity(model->model_matrix);
	model->position = hm::vec3(0.0f, 0.0f, 0.0f);
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

hm::vec4 char_window_held_color(0.5f, 1.0f, 1.0f, 0.65f);
hm::vec4 char_window_hover_color(0.35f, 0.6f, 0.6f, 0.65f);
hm::vec4 char_window_color(15.0f / 255.0f, 17.0f / 255.0f, 42.0f / 255.0f, 1.0f);
hm::vec4 char_selected_bg_color(0.4f, 0.7f, 0.7f, 1.0f);

void gui_toggle_char_selection(int id, std::vector<linked::WindowDiv*>* divs)
{
	// update the selection screen
	bool notselected = (*divs)[id * 3 + 1]->m_render = !(*divs)[id * 3 + 1]->m_render;
	(*divs)[id * 3 + 2]->m_render = !(*divs)[id * 3 + 2]->m_render;
	hm::vec4 selected_color;
	hm::vec4 selected_bg_color;
	if (notselected) {
		selected_bg_color = char_selected_bg_color;
		selected_color = char_window_hover_color;
	} else {
		selected_bg_color = char_window_color;
		selected_color = hm::vec4(0, 0, 0, 1);
	}

	(*divs)[id * 3]->getButtons()[0]->setNormalBGColor(selected_color);
	(*divs)[id * 3]->setBackgroundColor(selected_bg_color);
}

void select_character_button(void* arg) {
	auto divs = (std::vector<linked::WindowDiv*>*)((linked::Button_Info*)arg)->data;
	int id = ((linked::Button_Info*)arg)->id;
	bool selected = (*divs)[id * 3 + 1]->m_render;
	// update game state
	if (selected) {
		char_sel_state.num_selected -= 1;
		if (char_sel_state.num_selected > 1) {
			for (int i = 0; i < PLAYING_CHARACTERS; ++i) {
				if (char_sel_state.selections[i] == id)
					char_sel_state.selections[i] = char_sel_state.selections[char_sel_state.num_selected];
			}
		}
	} else if(char_sel_state.num_selected < 3) {
		// select one more character
		char_sel_state.selections[char_sel_state.num_selected] = id;
		char_sel_state.num_selected += 1;
	} else {
		// replace the last selected
		gui_toggle_char_selection(char_sel_state.selections[PLAYING_CHARACTERS - 1], divs);
		char_sel_state.selections[PLAYING_CHARACTERS - 1] = id;
	}

	gui_toggle_char_selection(id, divs);
}

void change_game_mode(Game_Mode mode);

void combat_start_mode(void* arg)
{
	change_game_mode(MODE_COMBAT);
}

void hide_all_windows() {
	//gw.bgwindow->setActive(false);

	// character selection
	gw.left_char_window->setActive(false);
	gw.char_selected_window->setActive(false);
	gw.char_selection_window->setActive(false);

	// character info
	gw.char_info_window->setActive(false);
	gw.char_info_window_bot->setActive(false);
}

void init_char_selection_mode()
{
	// left character window
	linked::Window* left_char_window = new linked::Window(400, 840, hm::vec2(100, 30), hm::vec4(12.0f / 255.0f, 16.0f / 255.0f, 40.0f / 255.0f, 1.0f), 0, 0, linked::W_BORDER);
	left_char_window->setBorderSizeX(10.0f);
	left_char_window->setBorderSizeY(0.0f);
	left_char_window->setBorderColor(hm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
	gw.left_char_window = left_char_window;
	linked::WindowDiv* left_char_div = new linked::WindowDiv(*left_char_window, 400, 840, 0, 0, hm::vec2(0, 0), hm::vec4(12.0f / 255.0f, 16.0f / 255.0f, 40.0f / 255.0f, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_RIGHT);
	left_char_window->divs.push_back(left_char_div);
	char_sel_state.last_hovered = CHAR_NONE;
	linked::WindowDiv* left_char_name_div = new linked::WindowDiv(*left_char_window, 400, 100, 0, 0, hm::vec2(0, 220), hm::vec4(12.0f / 255.0f, 16.0f / 255.0f, 40.0f / 255.0f, 0.97f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_RIGHT);
	
	linked::Label* left_char_name_label = new linked::Label(*left_char_name_div, (u8*)"", sizeof "", hm::vec2(320.0f, 15.0f), hm::vec4(1, 1, 1, 1), 60.0f, 0, 0);
	left_char_name_div->getLabels().push_back(left_char_name_label);
	left_char_window->divs.push_back(left_char_name_div);

	linked::WindowDiv* left_char_div_bar = new linked::WindowDiv(*left_char_window, 380, 3, 0, 0, hm::vec2(10, 220), hm::vec4(1, 0.2f, 1, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_RIGHT);
	left_char_window->divs.push_back(left_char_div_bar);

	float char_window_width = 140.0f;
	float char_window_height = 300.0f;

	// Character selected lower window (bottom)
	linked::Window* char_selected_window = new linked::Window(6 * char_window_width + 100.0f, 200, hm::vec2(520, 670), hm::vec4(12.0f / 255.0f, 16.0f / 255.0f, 40.0f / 255.0f, 0.55f), 0, 0, 0);
	gw.char_selected_window = char_selected_window;

	linked::WindowDiv* s_div1 = new linked::WindowDiv(*char_selected_window, char_window_width, char_window_width, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 0.0f + char_window_width * 0.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* s_div2 = new linked::WindowDiv(*char_selected_window, char_window_width, char_window_width, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 1.0f + char_window_width * 1.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* s_div3 = new linked::WindowDiv(*char_selected_window, char_window_width, char_window_width, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 2.0f + char_window_width * 2.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_selected_window->divs.push_back(s_div1);
	char_selected_window->divs.push_back(s_div2);
	char_selected_window->divs.push_back(s_div3);

	linked::WindowDiv* info_div = new linked::WindowDiv(*char_selected_window, 24, 24, 0, 0, hm::vec2(600.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	Texture* space_texture = new Texture("res/textures/spacebar.png");
	info_div->setBackgroundTexture(space_texture);
	char_selected_window->divs.push_back(info_div);

	linked::WindowDiv* info_label_div = new linked::WindowDiv(*char_selected_window, 256, 24, 0, 0, hm::vec2(624.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_selected_window->divs.push_back(info_label_div);
	linked::Label* info_label = new linked::Label(*info_label_div, (u8*)"Information", sizeof("Information"), hm::vec2(0.0f, 0.0f), hm::vec4(1, 1, 1, 1), 28.0f, 5.0f, 0);
	info_label_div->getLabels().push_back(info_label);

	linked::WindowDiv* confirm_div = new linked::WindowDiv(*char_selected_window, 24, 24, 0, 0, hm::vec2(740.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	Texture* enter_texture = new Texture("res/textures/enter.png");
	confirm_div->setBackgroundTexture(enter_texture);
	char_selected_window->divs.push_back(confirm_div);

	linked::WindowDiv* confirm_label_div = new linked::WindowDiv(*char_selected_window, 256, 24, 0, 0, hm::vec2(764.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_selected_window->divs.push_back(confirm_label_div);
	linked::Label* confirm_label = new linked::Label(*confirm_label_div, (u8*)"Confirm", sizeof("Confirm"), hm::vec2(0.0f, 0.0f), hm::vec4(1, 1, 1, 1), 28.0f, 5.0f, 0);
	confirm_label_div->getLabels().push_back(confirm_label);

	linked::WindowDiv* back_div = new linked::WindowDiv(*char_selected_window, 24, 24, 0, 0, hm::vec2(840.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	Texture* esc_texture = new Texture("res/textures/esc.png");
	back_div->setBackgroundTexture(esc_texture);
	char_selected_window->divs.push_back(back_div);

	linked::WindowDiv* back_label_div = new linked::WindowDiv(*char_selected_window, 256, 24, 0, 0, hm::vec2(864.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_selected_window->divs.push_back(back_label_div);
	linked::Label* back_label = new linked::Label(*back_label_div, (u8*)"Back", sizeof("Back"), hm::vec2(0.0f, 0.0f), hm::vec4(1, 1, 1, 1), 28.0f, 5.0f, 0);
	back_label_div->getLabels().push_back(back_label);

	linked::WindowDiv* play_div = new linked::WindowDiv(*char_selected_window, 148, 48, 0, 0, hm::vec2(740.0f, 50.0f), hm::vec4(0.34f, 0.9f, 0.72f, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_selected_window->divs.push_back(play_div);
	linked::Label* play_label = new linked::Label(*play_div, (u8*)"FIGHT", sizeof("FIGHT"), hm::vec2(42.0f, 14.0f), hm::vec4(1, 1, 1, 1), 40.0f, 0, 0);
	linked::Button* play_button = new linked::Button(*play_div, play_label, hm::vec2(0, 0), 148, 48, hm::vec4(0.34f, 0.73f, 0.62f, 1), 0);
	play_button->setHoveredBGColor(hm::vec4(0.24f, 0.63f, 0.52f, 1));
	play_button->setHeldBGColor(char_window_held_color);
	play_button->setClickedCallback(combat_start_mode);
	play_div->getButtons().push_back(play_button);
	play_div->m_render = false;
	char_sel_state.play_button_div = play_div;

	// Character selection window (top)
	linked::Window* char_selection_window = new linked::Window(6 * char_window_width + 100.0f, 630, hm::vec2(520, 30), hm::vec4(12.0f / 255.0f, 16.0f / 255.0f, 40.0f / 255.0f, 0.55f), 0, 0, 0);
	gw.char_selection_window = char_selection_window;

	Texture* chars_texture[NUM_CHARS] = {};
	chars_texture[0] = new Texture("res/textures/zero.png");
	chars_texture[1] = new Texture("res/textures/one.png");
	chars_texture[2] = new Texture("res/textures/serial.png");
	chars_texture[3] = new Texture("res/textures/ray.png");
	chars_texture[4] = new Texture("res/textures/astar.png");
	chars_texture[5] = new Texture("res/textures/deadlock.png");
	chars_texture[6] = new Texture("res/textures/norma.png");
	chars_texture[7] = new Texture("res/textures/hazard.png");
	chars_texture[8] = new Texture("res/textures/qwerty.png");
	chars_texture[9] = new Texture("res/textures/bigo.png");
	chars_texture[10] = new Texture("res/textures/new.png");
	chars_texture[11] = new Texture("res/textures/clockboy.png");

	linked::WindowDiv* char_divs[NUM_CHARS] = {};
	float char_div_offset_x = 0.0f;
	float char_div_offset_y = 0.0f;
	for (int i = 0; i < NUM_CHARS; ++i) {
		char_divs[i] = new linked::WindowDiv(*char_selection_window, char_window_width, char_window_height, 0.0f, 0.0f,
			hm::vec2(25.0f + 10.0f * char_div_offset_x + char_window_width * char_div_offset_x, 10.0f * (char_div_offset_y + 1.0f) + char_div_offset_y * char_window_height),
			char_window_color, linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		char_selection_window->divs.push_back(char_divs[i]);

		linked::WindowDiv* select_div_l = new linked::WindowDiv(*char_selection_window, 5.0f, char_window_height, 0.0f, 0.0f,
			hm::vec2(25.0f + 10.0f * char_div_offset_x + char_window_width * char_div_offset_x, 10.0f * (char_div_offset_y + 1.0f) + char_div_offset_y * char_window_height),
			hm::vec4(0, 1, 1, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		char_selection_window->divs.push_back(select_div_l);
		select_div_l->m_render = false;

		linked::WindowDiv* select_div_r = new linked::WindowDiv(*char_selection_window, 5.0f, char_window_height, 0.0f, 0.0f,
			hm::vec2(25.0f + 10.0f * char_div_offset_x + char_window_width * char_div_offset_x + char_window_width - 5.0f, 10.0f * (char_div_offset_y + 1.0f) + char_div_offset_y * char_window_height),
			hm::vec4(0, 1, 1, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		char_selection_window->divs.push_back(select_div_r);
		select_div_r->m_render = false;

		char_div_offset_x += 1.0f;
		if (i + 1 == NUM_CHARS / 2) {
			char_div_offset_x = 0.0f;
			char_div_offset_y += 1.0f;
		}
		linked::Button* button = new linked::Button(*char_divs[i], 0, hm::vec2(0, 0), char_window_width, char_window_height, char_window_color, i);
		button->setHoveredBGColor(char_window_hover_color);
		button->setHeldBGColor(char_window_held_color);
		Texture* char_texture = chars_texture[i];
		button->setNormalBGTexture(char_texture);
		button->setHoveredBGTexture(char_texture);
		button->setHeldBGTexture(char_texture);
		char_divs[i]->getButtons().push_back(button);
		button->button_info.data = &char_selection_window->divs;
		button->setClickedCallback(select_character_button);
	}
}

void init_char_information_mode()
{
	float char_window_width = 140.0f;
	float char_window_height = 140.0f;

	linked::Window* char_info_window = new linked::Window(6 * char_window_width + 100.0f - 20.0f, 630, hm::vec2(530, 30), hm::vec4(12.0f / 255.0f, 16.0f / 255.0f, 40.0f / 255.0f, 0.9f), 0, 0, linked::W_BORDER);
	char_info_window->setBorderSizeX(10.0f);
	char_info_window->setBorderSizeY(0.0f);
	hm::vec4 border_color = hm::vec4(1.0f, 0.71f, 0.29f, 1.0f);
	char_info_window->setBorderColor(border_color);
	gw.char_info_window = char_info_window;

	linked::Window* char_info_window_bot = new linked::Window(6 * char_window_width + 100.0f, 200, hm::vec2(520, 670), hm::vec4(12.0f / 255.0f, 16.0f / 255.0f, 40.0f / 255.0f, 0.55f), 0, 0, 0);
	gw.char_info_window_bot = char_info_window_bot;

	linked::WindowDiv* back_div = new linked::WindowDiv(*char_info_window_bot, 24, 24, 0, 0, hm::vec2(840.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	Texture* esc_texture = new Texture("res/textures/esc.png");
	back_div->setBackgroundTexture(esc_texture);
	char_info_window_bot->divs.push_back(back_div);

	linked::WindowDiv* back_label_div = new linked::WindowDiv(*char_info_window_bot, 256, 24, 0, 0, hm::vec2(864.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_info_window_bot->divs.push_back(back_label_div);
	linked::Label* back_label = new linked::Label(*back_label_div, (u8*)"Back", sizeof("Back"), hm::vec2(0.0f, 0.0f), hm::vec4(1, 1, 1, 1), 28.0f, 5.0f, 0);
	back_label_div->getLabels().push_back(back_label);
	

	linked::WindowDiv* skills_divs[NUM_SKILLS] = {};
	float char_div_offset_x = 0.0f;
	float char_div_offset_y = 0.0f;
	for (int i = 0; i < NUM_SKILLS; ++i) {
		skills_divs[i] = new linked::WindowDiv(*char_info_window, char_window_width, char_window_height, 0.0f, 0.0f,
			hm::vec2(25.0f + 10.0f * char_div_offset_x + char_window_width * char_div_offset_x, 10.0f * (char_div_offset_y + 1.0f) + char_div_offset_y * char_window_height),
			char_window_color, linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		char_info_window->divs.push_back(skills_divs[i]);

		linked::Button* button = new linked::Button(*skills_divs[i], 0, hm::vec2(0, 0), char_window_width, char_window_height, char_window_color, i);
		button->setHoveredBGColor(char_window_hover_color);
		button->setHeldBGColor(char_window_held_color);
		skills_divs[i]->getButtons().push_back(button);
		button->button_info.data = &char_info_window->divs;
		char_div_offset_x += 1.0f;
	}

	linked::WindowDiv* skill_title_div = new linked::WindowDiv(*char_info_window, 300, 48, 0, 0, hm::vec2(25.0f, 140.0f + 35.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_info_window->divs.push_back(skill_title_div);
	linked::Label* skill_title_label = new linked::Label(*skill_title_div, (u8*)"", 0, hm::vec2(0, 0), hm::vec4(1,1,1,1), 38.0f, 0, 0);
	skill_title_div->getLabels().push_back(skill_title_label);

	linked::WindowDiv* skill_desc_div = new linked::WindowDiv(*char_info_window, 300, 48, 0, 0, hm::vec2(25.0f, 140.0f + 65.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_info_window->divs.push_back(skill_desc_div);
	linked::Label* skill_desc_label = new linked::Label(*skill_desc_div, (u8*)"", 0, hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), 34.0f, 0, 0);
	skill_desc_div->getLabels().push_back(skill_desc_label);
}

void init_combar_mode()
{

}

void init_application()
{
	using namespace linked;

#if 0
	// init camera and 3d shader
	init_camera(&ggs.camera, (float)win_state.win_width / win_state.win_height, 45.0f, 0.2f, 1000.0f);
	ggs.camera.set_cam_position(hm::vec3(5.0f, 30.0f, 60.0f));
	ggs.shader = load_shader(vert_shader, frag_shader, sizeof(vert_shader) - 1, sizeof(frag_shader) - 1);

	ggs.models = (IndexedModel3D*)malloc(sizeof(IndexedModel3D) * 256);

	load_model("res/cube.obj", &ggs.models[0]);
	init_object(&ggs.models[0]);
	ggs.models[0].position = hm::vec3(0.0f, 45.0f, 0.0f);
	ggs.models[0].scale = 0.3f;
	ggs.models[0].simulating = true;
	ggs.models[0].static_object = false;
	ggs.models[0].texture = 0;
	ggs.models[0].last_pos = ggs.models[0].position;

	ggs.num_models = 1;
#endif

	ggs.mode = MODE_INTRO;
	ggs.last_mode = MODE_INTRO;

	// background @temporary
	linked::Window* bgwindow = new linked::Window(win_state.win_width, win_state.win_height, hm::vec2(0, 0), hm::vec4(0, 0, 0, 0.5f), 0, 0, W_UNFOCUSABLE);
	linked::WindowDiv* bgdiv = new linked::WindowDiv(*bgwindow, win_state.win_width, win_state.win_height, 0, 0, hm::vec2(0, 0), hm::vec4(0, 0, 0, 1), DIV_ANCHOR_LEFT | DIV_ANCHOR_TOP);
	bgwindow->divs.push_back(bgdiv);
	Texture* bgtexture = new Texture("res/textures/bg2.jpg");
	bgdiv->setBackgroundTexture(bgtexture);
	gw.bgwindow = bgwindow;

	init_char_selection_mode();
	init_char_information_mode();

	// init console chat
	chat_window = chat.init_chat();
	chat_window->setActive(false);
	chat.m_active = false;
	g_chat = &chat;

	hide_all_windows();
	change_game_mode(MODE_CHAR_SELECT);

	// opengl
	glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
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

void update_game_mode()
{
	switch (ggs.mode) {
		case MODE_CHAR_SELECT: {
			if (char_sel_state.num_selected == 3)
				char_sel_state.play_button_div->m_render = true;
			else
				char_sel_state.play_button_div->m_render = false;

			for (int i = 0; i < NUM_CHARS; ++i) {
				if (gw.char_selection_window->divs[i * 3]->isButtonHovered()) {
					Texture* char_tex = (Texture*)gw.char_selection_window->divs[i * 3]->getButtons()[0]->getNormalBGTexture();
					gw.left_char_window->divs[0]->setBackgroundTexture(char_tex);
					linked::Label* name_label = gw.left_char_window->divs[1]->getLabels()[0];
					name_label->setText((u8*)char_names[i], char_names_length[i]);
					name_label->setPosition(hm::vec2(380.0f - (char_names_length[i] - 1) * 18.0f, 15.0f));
					char_sel_state.last_hovered = (Character_ID)i;
				}
			}
		}break;
		case MODE_CHAR_INFO: {
			linked::Label* skill_label = gw.char_info_window->divs[NUM_SKILLS]->getLabels()[0];
			linked::Label* skill_desc_label = gw.char_info_window->divs[NUM_SKILLS + 1]->getLabels()[0];
			for (int i = 0; i < NUM_SKILLS; ++i) {
				if (gw.char_info_window->divs[i]->isButtonHovered()) {
					int index = i + char_sel_state.last_hovered * NUM_SKILLS;
					skill_label->setText((u8*)skill_names[index], skill_names_length[index]);
					skill_desc_label->setText((u8*)skill_desc[index], skill_desc_length[index]);
				}
			}
		}break;
	}
}

void change_game_mode(Game_Mode mode)
{
	if (ggs.mode == mode) return;
	switch (ggs.mode) {
		case MODE_CHAR_SELECT: {
			gw.char_selected_window->setActive(false);
			gw.left_char_window->setActive(false);
			gw.char_selection_window->setActive(false);
		}break;
		case MODE_CHAR_INFO: {
			gw.left_char_window->setActive(false);
			gw.char_info_window->setActive(false);
			gw.char_info_window_bot->setActive(false);
			linked::Label* skill_label = gw.char_info_window->divs[NUM_SKILLS]->getLabels()[0];
			linked::Label* skill_desc_label = gw.char_info_window->divs[NUM_SKILLS + 1]->getLabels()[0];
			skill_label->setText(0, 0);
			skill_desc_label->setText(0, 0);
		}break;
	}

	ggs.last_mode = ggs.mode;
	ggs.mode = mode;

	switch (mode) {
		case MODE_CHAR_SELECT: {
			gw.char_selected_window->setActive(true);
			gw.left_char_window->setActive(true);
			gw.char_selection_window->setActive(true);
		}break;
		case MODE_CHAR_INFO: {
			gw.left_char_window->setActive(true);
			gw.char_info_window->setActive(true);
			gw.char_info_window_bot->setActive(true);
		}break;
	}
}

void input();

void update_and_render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (chat.m_active)
		chat.update();

	update_game_mode();

	input();
#if 0
	glUseProgram(ggs.shader);
	input_camera(&ggs.camera);

	for (int i = 0; i < ggs.num_models; ++i) {
		update_model(ggs.models + i);
		render_object(ggs.models + i);
	}
	//glUseProgram(0);
#endif
}

void input()
{
	win_state.move_camera = !chat_window->isAttached();
	win_state.do_input = !chat.m_enabled;

	if (keyboard_state.key_event[VK_F1]) {
		keyboard_state.key_event[VK_F1] = false;
		chat_window->setActive(!chat_window->getActive());
		g_chat->m_active = !g_chat->m_active;
	}
	if (keyboard_state.key_event[VK_ESCAPE]) {
		keyboard_state.key_event[VK_ESCAPE] = false;
		change_game_mode(ggs.last_mode);
	}

	if (keyboard_state.key_event[VK_SPACE]) {
		keyboard_state.key_event[VK_SPACE] = false;
		
		if (ggs.mode == MODE_CHAR_SELECT && char_sel_state.last_hovered != CHAR_NONE) {
			printf("last hovered %d\n", char_sel_state.last_hovered);
			change_game_mode(MODE_CHAR_INFO);
		}
	}

	if (!chat_window->isFocused())
		chat_window->setFocus();
}