#include "application.h"
#include "input.h"
#include "quaternion.h"
#include "ResourceLoad/Texture.h"
#include "WindowApi/Window.h"
#include "chat.h"

extern Window_State win_state;
Chat* g_chat = 0;
Chat chat;
linked::Window* chat_window = 0;

#include "camera.cpp"
#include "load_model.cpp"

#define NUM_CHARS 12
#define NUM_SKILLS 4
#define NUM_ALLIES 3
#define NUM_ENEMIES 3
#define TURN_DURATION 60.0

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

enum Orb_ID {
	ORB_NONE = -1,
	ORB_HARD = 0,
	ORB_SOFT = 1,
	ORB_VR   = 2,
	ORB_BIOS = 3,
	ORB_NULL = 4,
	ORB_NUMBER,
};

int skill_costs[NUM_SKILLS * NUM_CHARS][ORB_NUMBER] = {
	{ 0, 1, 0, 0, 0 },
	{ 0, 0, 1, 0, 0 },
	{ 0, 0, 0, 2, 0 },
	{ 0, 0, 0, 0, 1 },
	  			    
	{ 1, 0, 0, 0, 1 },
	{ 0, 0, 1, 0, 1 },
	{ 0, 0, 0, 1, 0 },
	{ 0, 0, 0, 0, 1 },
	  			    
	{ 1, 0, 0, 0, 0 },
	{ 0, 1, 0, 0, 0 },
	{ 0, 0, 0, 0, 2 },
	{ 0, 0, 0, 0, 1 },
	  			    
	{ 0, 0, 1, 0, 0 },
	{ 0, 0, 1, 0, 1 },
	{ 0, 0, 1, 0, 2 },
	{ 0, 0, 0, 0, 1 },

	{ 0, 0, 0, 1, 0 },
	{ 0, 0, 1, 0, 1 },
	{ 0, 0, 1, 1, 0 },
	{ 0, 0, 0, 0, 1 },

	{ 0, 1, 0, 0, 1 },
	{ 0, 1, 0, 0, 1 },
	{ 0, 1, 0, 0, 1 },
	{ 0, 0, 0, 0, 1 },

	{ 0, 0, 0, 0, 2 },
	{ 0, 0, 0, 0, 1 },
	{ 0, 0, 0, 0, 3 },
	{ 0, 0, 0, 0, 1 },

	{ 0, 0, 0, 1, 0 },
	{ 1, 0, 0, 0, 1 },
	{ 0, 0, 0, 0, 1 },
	{ 0, 0, 0, 0, 1 },

	{ 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 1 },
	{ 2, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 1 },

	{ 1, 0, 0, 0, 1 },
	{ 0, 0, 1, 0, 1 },
	{ 0, 0, 0, 0, 2 },
	{ 0, 0, 0, 0, 1 },

	{ 0, 1, 0, 0, 1 },
	{ 0, 1, 0, 1, 0 },
	{ 0, 0, 0, 1, 0 },
	{ 0, 0, 0, 0, 1 },

	{ 0, 0, 0, 1, 1 },
	{ 0, 0, 0, 1, 0 },
	{ 0, 0, 0, 1, 1 },
	{ 0, 0, 0, 0, 1 },
};

int skill_cooldowns[NUM_SKILLS * NUM_CHARS] = {
	0, 0, 5, 4, // zero
	0, 1, 4, 4, // one
	0, 1, 3, 4, // serial
	1, 3, 1, 4, // ray
	0, 0, 5, 4, // astar
	0, 1, 2, 4,	// deadlock
	0, 3, 1, 4, // norma
	0, 4, 0, 4, // hazard
	5, 1, 0, 4, // qwerty
	0, 1, 3, 4, // bigo
	0, 1, 0, 4, // new
	1, 3, 4, 4  // clockboy
};

enum Skill_Type {
	SKILL_TYPE_NONE,
	SKILL_TYPE_PHYSICAL,
	SKILL_TYPE_VIRTUAL,
	SKILL_TYPE_MENTAL,
};
enum Skill_Mode {
	SKILL_MODE_NONE,
	SKILL_MODE_MELEE,
	SKILL_MODE_RANGED,
};
enum Skill_Category {
	SKILL_CATEGORY_NORMAL,
	SKILL_CATEGORY_COUNTER,
	SKILL_CATEGORY_STATUS,
};
enum Skill_Condition {
	SKILL_CONDITION_NONE,
	SKILL_CONDITION_NORMAL,
	SKILL_CONDITION_BURN,
	SKILL_CONDITION_FREEZE,
	SKILL_CONDITION_POISON,
	SKILL_CONDITION_PARALYZE,
	SKILL_CONDITION_SLEEP,
};
enum Skill_Damage {
	SKILL_DMG_NONE,
	SKILL_DMG_NORMAL,
	SKILL_DMG_PIERCING,
	SKILL_DMG_CRUSHING,
};
enum Skill_Defense {
	SKILL_DEF_NONE,
	SKILL_DEF_REDUCTION,
	SKILL_DEF_ABSORPTION,
	SKILL_DEF_RELECTION,
	SKILL_DEF_INVULNERABILITY,
};
enum Skill_Duration {
	SKILL_DURATION_NONE,
	SKILL_DURATION_STATIC,
	SKILL_DURATION_CONTROL,
	SKILL_DURATION_CONSTANT,
};
enum Skill_Unique {
	SKILL_NOT_UNIQUE,
	SKILL_UNIQUE,
};

struct Skill_Group {
	Skill_Type       type;
	Skill_Mode       mode;
	Skill_Category   category;
	Skill_Condition  condition;
	Skill_Damage     damage;
	Skill_Defense    defense;
	Skill_Duration   duration;
	Skill_Unique     unique;
};

Skill_Group skill_groups[NUM_SKILLS * NUM_CHARS] = {
	// { SKILL_TYPE_NONE, SKILL_MODE_NONE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	// { SKILL_TYPE_NONE, SKILL_MODE_NONE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	// { SKILL_TYPE_NONE, SKILL_MODE_NONE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	// { SKILL_TYPE_NONE, SKILL_MODE_NONE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_INVULNERABILITY, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },

	// Zero
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_COUNTER, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_MENTAL, SKILL_MODE_RANGED, SKILL_CATEGORY_COUNTER, SKILL_CONDITION_PARALYZE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_CONTROL, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_VIRTUAL, SKILL_MODE_RANGED, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_UNIQUE },
	{ SKILL_TYPE_VIRTUAL, SKILL_MODE_RANGED, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_INVULNERABILITY, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },

	// One
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NORMAL, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_MENTAL, SKILL_MODE_RANGED, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_PIERCING, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_STATUS, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_REDUCTION, SKILL_DURATION_STATIC, SKILL_UNIQUE },
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_INVULNERABILITY, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },

	// Serial Keyller
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NORMAL, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_MENTAL, SKILL_MODE_RANGED, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_PIERCING, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_RANGED, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_PARALYZE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_INVULNERABILITY, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },

	// Ray Tracey
	{ SKILL_TYPE_VIRTUAL, SKILL_MODE_RANGED, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_MENTAL, SKILL_MODE_RANGED, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_RELECTION, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_VIRTUAL, SKILL_MODE_RANGED, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_INVULNERABILITY, SKILL_DURATION_STATIC, SKILL_UNIQUE },

	// A-Star
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_CONSTANT, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_MENTAL, SKILL_MODE_RANGED, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_CRUSHING, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_UNIQUE },
	{ SKILL_TYPE_MENTAL, SKILL_MODE_RANGED, SKILL_CATEGORY_STATUS, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_ABSORPTION, SKILL_DURATION_STATIC, SKILL_UNIQUE },
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_INVULNERABILITY, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },

	// Deadlock
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_SLEEP, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_CONSTANT, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_VIRTUAL, SKILL_MODE_RANGED, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_CONSTANT, SKILL_UNIQUE },
	{ SKILL_TYPE_VIRTUAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_SLEEP, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_INVULNERABILITY, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },

	// Norma
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NORMAL, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_POISON, SKILL_DMG_NONE, SKILL_DEF_ABSORPTION, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_RANGED, SKILL_CATEGORY_STATUS, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_INVULNERABILITY, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },

	// Hazard
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_ABSORPTION, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_RANGED, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_REDUCTION, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_VIRTUAL, SKILL_MODE_MELEE, SKILL_CATEGORY_STATUS, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_INVULNERABILITY, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },

	// Qwerty
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_REDUCTION, SKILL_DURATION_CONSTANT, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_CONTROL, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_UNIQUE },
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_INVULNERABILITY, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },

	// Big O
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_BURN, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_VIRTUAL, SKILL_MODE_MELEE, SKILL_CATEGORY_STATUS, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_UNIQUE },
	{ SKILL_TYPE_VIRTUAL, SKILL_MODE_MELEE, SKILL_CATEGORY_STATUS, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_INVULNERABILITY, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },

	// New
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_REDUCTION, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_VIRTUAL, SKILL_MODE_RANGED, SKILL_CATEGORY_COUNTER, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_VIRTUAL, SKILL_MODE_RANGED, SKILL_CATEGORY_STATUS, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_VIRTUAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_INVULNERABILITY, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },

	// Clockboy
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_RANGED, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_PARALYZE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_STATIC, SKILL_UNIQUE },
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_STATUS, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_NONE, SKILL_DURATION_CONTROL, SKILL_NOT_UNIQUE },
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_STATUS, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_REDUCTION, SKILL_DURATION_STATIC, SKILL_UNIQUE },
	{ SKILL_TYPE_PHYSICAL, SKILL_MODE_MELEE, SKILL_CATEGORY_NORMAL, SKILL_CONDITION_NONE, SKILL_DMG_NONE, SKILL_DEF_INVULNERABILITY, SKILL_DURATION_STATIC, SKILL_NOT_UNIQUE },
};

Texture* char_textures[NUM_CHARS] = {};
Texture* chars_texture_big[NUM_CHARS] = {};
Texture* skill_textures[NUM_SKILLS * NUM_CHARS] = {};
Texture* orb_textures[ORB_NUMBER] = {};
Texture* orb_alive_ally = 0;
Texture* orb_dead_ally = 0;
Texture* orb_alive_enemy = 0;
Texture* orb_dead_enemy = 0;

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

enum Skill_ID {
	SKILL_NONE = -1,
	SKILL_FALSE_RUSH = 0,
	SKILL_CONTRADICTION,
	SKILL_REQUIEM_ZERO,
	SKILL_VOID_BARRIER,

	SKILL_TRUTH_SLASH,
	SKILL_TAUTOLOGY,
	SKILL_AXIOM_ONE,
	SKILL_TRUE_ENDURANCE,

	SKILL_BRUTE_FORCE,
	SKILL_BUFFER_OVERFLOW,
	SKILL_DDOS_ATTACK,
	SKILL_ENCRYPTION,

	SKILL_PARTICLE_RENDERING,
	SKILL_DIFFUSE_REFLECTION,
	SKILL_DYNAMIC_FRUSTUM_ATTACK,
	SKILL_RASTERIZATION,

	SKILL_Q_PUNCH,
	SKILL_PERCEPTRON,
	SKILL_NEURAL_NETWORK,
	SKILL_HILL_CLIMBING,

	SKILL_PREEMPTION,
	SKILL_MUTEX,
	SKILL_THREAD_SCHEDULING,
	SKILL_FORK,

	SKILL_PUMPING_UP,
	SKILL_AUTOMATA_SUMMON,
	SKILL_TURING_MACHINE,
	SKILL_NON_DETERMINISM,

	SKILL_TMR,
	SKILL_REDUNDANCY,
	SKILL_ROLLBACK,
	SKILL_ROLLFORWARD,

	SKILL_ALT,
	SKILL_CRTL,
	SKILL_DELETE,
	SKILL_ESC,

	SKILL_BEST_BOUND_FIRST,
	SKILL_DUAL_SIMPLEX,
	SKILL_GRAPH_COLORING,
	SKILL_KNAPSACK_HIDEOUT,

	SKILL_SPRINT_BURST,
	SKILL_INHERITANCE,
	SKILL_OVERRIDE,
	SKILL_POLIMORPHISM,

	SKILL_CLOCK_PULSE,
	SKILL_PIPELINE,
	SKILL_OVERCLOCK,
	SKILL_BRANCH_DAMAGE
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

struct Char_Selection_State {
	int num_selected;
	int selections[NUM_ALLIES];
	linked::WindowDiv* play_button_div;
	Character_ID last_hovered;
};

Char_Selection_State char_sel_state = {};

struct Combat_State {
	// Gameplay
	bool player_turn;
	int orbs_amount[ORB_NUMBER];
	int total_orbs;

	// Visual
	Skill_ID last_hovered;

	linked::WindowDiv* skill_info_image;
	linked::WindowDiv* skill_info_image_border;
	linked::WindowDiv* skill_info_title;
	linked::WindowDiv* skill_info_desc;
	linked::WindowDiv* skill_costs[ORB_NUMBER];

	linked::Label* orb_labels[ORB_NUMBER];
	linked::Label* all_orbs_label;

	linked::Button* end_turn_button;
};

Combat_State combat_state = {};

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
	linked::Window* char_info_skill_cost;
	linked::WindowDiv* skill_group_div;

	// combat
	linked::Window* allies[NUM_ALLIES];
	linked::Window* allies_info[NUM_ALLIES];
	linked::Window* enemies[NUM_ENEMIES];
	linked::Window* enemies_info[NUM_ENEMIES];
	linked::Window* allies_skills[NUM_ALLIES * NUM_SKILLS];
	linked::Window* combat_bottom_info;
	linked::Window* timer_window;
	linked::Window* player_name_window;

	linked::WindowDiv* allies_indicator[NUM_ALLIES];
	linked::WindowDiv* enemies_indicator[NUM_ENEMIES];
};

Game_Windows gw = {};

void change_game_mode(Game_Mode mode);

// Gameplay functions
static void layout_enemy_die(u32 enemy_index);
static void layout_ally_die(u32 ally_index);
static void layout_set_skill_group_from_skill(int skill_index, linked::Label* label);

#include "rendering.cpp"

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
			for (int i = 0; i < NUM_ALLIES; ++i) {
				if (char_sel_state.selections[i] == id)
					char_sel_state.selections[i] = char_sel_state.selections[char_sel_state.num_selected];
			}
		}
	} else if(char_sel_state.num_selected < 3) {
		// select one more character
		char_sel_state.selections[char_sel_state.num_selected] = id;
		char_sel_state.num_selected += 1;
	} else {
	
#if REPLACE_FIRST
		gui_toggle_char_selection(char_sel_state.selections[0], divs);
		char_sel_state.selections[0] = char_sel_state.selections[1];
		char_sel_state.selections[1] = char_sel_state.selections[2];
		char_sel_state.selections[2] = id;
#else
		// replace the last selected
		gui_toggle_char_selection(char_sel_state.selections[NUM_ALLIES - 1], divs);
		char_sel_state.selections[NUM_ALLIES - 1] = id;
#endif
	}

	gui_toggle_char_selection(id, divs);
}

void end_turn();
static double turn_time = TURN_DURATION;
void end_turn_button_callback(void* arg) {
	if (!combat_state.player_turn) return;
	turn_time = TURN_DURATION;
	end_turn();
}

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
	gw.char_info_skill_cost->setActive(false);

	// combat
	for (int i = 0; i < NUM_ALLIES; ++i) {
		gw.allies[i]->setActive(false);
		gw.allies_info[i]->setActive(false);
		for (int k = 0; k < NUM_SKILLS; ++k) {
			gw.allies_skills[i * NUM_SKILLS + k]->setActive(false);
		}
	}
	for (int i = 0; i < NUM_ENEMIES; ++i) {
		gw.enemies[i]->setActive(false);
		gw.enemies_info[i]->setActive(false);
	}
	gw.combat_bottom_info->setActive(false);
	gw.timer_window->setActive(false);
	gw.player_name_window->setActive(false);
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
	
	chars_texture_big[0] = new Texture("res/char_big/zero.png");
	chars_texture_big[1] = new Texture("res/char_big/one.png");
	chars_texture_big[2] = new Texture("res/char_big/serial-keiller.png");
	chars_texture_big[3] = new Texture("res/char_big/ray-tracey.png");
	chars_texture_big[4] = new Texture("res/char_big/astar.png");
	chars_texture_big[5] = new Texture("res/char_big/deadlock.png");
	chars_texture_big[6] = new Texture("res/char_big/norma.png");
	chars_texture_big[7] = new Texture("res/char_big/hazard.png");
	chars_texture_big[8] = new Texture("res/char_big/qwerty.png");
	chars_texture_big[9] = new Texture("res/char_big/big-o.png");
	chars_texture_big[10] = new Texture("res/char_big/new.png");
	chars_texture_big[11] = new Texture("res/char_big/clockboy.png");

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

	linked::Window* char_info_window = new linked::Window(6 * char_window_width + 100.0f - 20.0f, 630, hm::vec2(530, 30), hm::vec4(12.0f / 255.0f, 16.0f / 255.0f, 40.0f / 255.0f, 0.9f), 0, 0, linked::W_BORDER|linked::W_UNFOCUSABLE);
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

	// gotta be first
	linked::WindowDiv* skill_title_div = new linked::WindowDiv(*char_info_window, 300, 48, 0, 0, hm::vec2(25.0f, 140.0f + 35.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_info_window->divs.push_back(skill_title_div);
	linked::Label* skill_title_label = new linked::Label(*skill_title_div, (u8*)"", 0, hm::vec2(0, 0), hm::vec4(1,1,1,1), 38.0f, 0, 0);
	skill_title_div->getLabels().push_back(skill_title_label);

	// gotta be second
	linked::WindowDiv* skill_desc_div = new linked::WindowDiv(*char_info_window, 300, 48, 0, 0, hm::vec2(25.0f, 140.0f + 85.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_info_window->divs.push_back(skill_desc_div);
	linked::Label* skill_desc_label = new linked::Label(*skill_desc_div, (u8*)"", 0, hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), 34.0f, 0, 0);
	skill_desc_div->getLabels().push_back(skill_desc_label);

	linked::Window* skill_cost = new linked::Window(300, 48, hm::vec2(530 + 320, 20 + char_window_width + 20 * 2), hm::vec4(0, 0, 0, 0), 0, 0, linked::W_UNFOCUSABLE);
	gw.char_info_skill_cost = skill_cost;

	float orb_size = 32.0f;
	linked::WindowDiv* skill_cost_div_1 = new linked::WindowDiv(*skill_cost, orb_size, orb_size, 0, 0, hm::vec2(0, 0), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	skill_cost->divs.push_back(skill_cost_div_1);
	linked::WindowDiv* skill_cost_div_2 = new linked::WindowDiv(*skill_cost, orb_size, orb_size, 0, 0, hm::vec2(0 + (orb_size + 5), 0), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	skill_cost->divs.push_back(skill_cost_div_2);
	linked::WindowDiv* skill_cost_div_3 = new linked::WindowDiv(*skill_cost, orb_size, orb_size, 0, 0, hm::vec2(0 + (orb_size + 5) * 2, 0), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	skill_cost->divs.push_back(skill_cost_div_3);
	linked::WindowDiv* skill_cost_div_4 = new linked::WindowDiv(*skill_cost, orb_size, orb_size, 0, 0, hm::vec2(0 + (orb_size + 5) * 3, 0), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	skill_cost->divs.push_back(skill_cost_div_4);
	linked::WindowDiv* skill_cost_div_5 = new linked::WindowDiv(*skill_cost, orb_size, orb_size, 0, 0, hm::vec2(0 + (orb_size + 5) * 4, 0), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	skill_cost->divs.push_back(skill_cost_div_5);

	linked::WindowDiv* skill_group_div = new linked::WindowDiv(*char_info_window, 640, 48, 0, 0, hm::vec2(25.0f, 140.0f + 85.0f + 48 * 2), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	gw.skill_group_div = skill_group_div;
	char_info_window->divs.push_back(skill_group_div);
	linked::Label* skill_group_label = new linked::Label(*skill_group_div, (u8*)/*"VIRTUAL, RANGED, INV, STATIC"*/"", 0, hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), 26.0f, 0, 0);
	skill_group_div->getLabels().push_back(skill_group_label);
}

void init_combat_mode()
{
	char_textures[0]  = new Texture("res/char_square/zero_sq.png");
	char_textures[1]  = new Texture("res/char_square/one_sq.png");
	char_textures[2]  = new Texture("res/char_square/serial_sq.png");
	char_textures[3]  = new Texture("res/char_square/ray_sq.png");
	char_textures[4]  = new Texture("res/char_square/astar_sq.png");
	char_textures[5]  = new Texture("res/char_square/deadlock_sq.png");
	char_textures[6]  = new Texture("res/char_square/norma_sq.png");
	char_textures[7]  = new Texture("res/char_square/hazard_sq.png");
	char_textures[8]  = new Texture("res/char_square/qwerty_sq.png");
	char_textures[9]  = new Texture("res/char_square/bigo_sq.png");
	char_textures[10] = new Texture("res/char_square/new_sq.png");
	char_textures[11] = new Texture("res/char_square/clockboy_sq.png");

	for (int i = 0; i < NUM_CHARS * 4; i += 4) {
		skill_textures[i + 0] = new Texture("res/skills/unk_skill1.png");
		skill_textures[i + 1] = new Texture("res/skills/unk_skill2.png");
		skill_textures[i + 2] = new Texture("res/skills/unk_skill3.png");
		skill_textures[i + 3] = new Texture("res/skills/unk_skill4.png");
	}
	skill_textures[CHAR_ZERO * NUM_SKILLS + 0] = new Texture("res/skills/zero/false_rush.png");
	skill_textures[CHAR_ZERO * NUM_SKILLS + 1] = new Texture("res/skills/zero/contradiction.png");
	skill_textures[CHAR_ZERO * NUM_SKILLS + 2] = new Texture("res/skills/zero/requiem_zero.png");
	skill_textures[CHAR_ZERO * NUM_SKILLS + 3] = new Texture("res/skills/zero/void_barrier.png");

	skill_textures[CHAR_QWERTY * NUM_SKILLS + 0] = new Texture("res/skills/qwerty/alt.png");
	skill_textures[CHAR_QWERTY * NUM_SKILLS + 1] = new Texture("res/skills/qwerty/ctrl.png");
	skill_textures[CHAR_QWERTY * NUM_SKILLS + 2] = new Texture("res/skills/qwerty/del.png");
	skill_textures[CHAR_QWERTY * NUM_SKILLS + 3] = new Texture("res/skills/qwerty/esc.png");

	skill_textures[CHAR_A_STAR * NUM_SKILLS + 0] = new Texture("res/skills/astar/q_punch.png");
	skill_textures[CHAR_A_STAR * NUM_SKILLS + 1] = new Texture("res/skills/astar/perceptron.png");
	skill_textures[CHAR_A_STAR * NUM_SKILLS + 2] = new Texture("res/skills/astar/neural_network.png");
	skill_textures[CHAR_A_STAR * NUM_SKILLS + 3] = new Texture("res/skills/astar/hill_climbing.png");


	orb_textures[0] = new Texture("res/orbs/hard_orb.png");
	orb_textures[1] = new Texture("res/orbs/soft_orb.png");
	orb_textures[2] = new Texture("res/orbs/vr_orb.png");
	orb_textures[3] = new Texture("res/orbs/bios_orb.png");
	orb_textures[4] = new Texture("res/orbs/null_orb.png");

	hm::vec4 ally_hp_bar_full_color(0, 1, 1, 1);
	hm::vec4 ally_hp_bar_empty_color(0, 0.3f, 0.3f, 1.0f);
	hm::vec4 enem_hp_bar_full_color(1, 0.71f, 0.29f, 1.0f);
	hm::vec4 enem_hp_bar_empty_color(0.6f, 0.29f, 0.02f, 1.0f);
	hm::vec4 greener_cyan(0, 1, 0.7f, 1);

	{
		// Players Names
		linked::Window* player_name = new linked::Window(win_state.win_width, 80, hm::vec2(0, 0), hm::vec4(0, 0, 0, 0.6f), 0, 0, linked::W_UNFOCUSABLE);
		linked::WindowDiv* player_name_div = new linked::WindowDiv(*player_name, 300, 32, 0, 0, hm::vec2(0, 0), hm::vec4(1, 0, 0, 0), linked::DIV_CENTER_Y | linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		linked::WindowDiv* enemy_name_div = new linked::WindowDiv(*player_name, 300, 32, 0, 0, hm::vec2(0, 0), hm::vec4(1, 0, 0, 0), linked::DIV_CENTER_Y | linked::DIV_ANCHOR_RIGHT | linked::DIV_ANCHOR_TOP);
		player_name->divs.push_back(player_name_div);
		player_name->divs.push_back(enemy_name_div);
		gw.player_name_window = player_name;

		linked::Label* player_label = new linked::Label(*player_name_div, (u8*)"Player Name", sizeof "Player Name", hm::vec2(50.0f, 0), hm::vec4(1, 1, 1, 1), 50.0f, 0, 0);
		player_name_div->getLabels().push_back(player_label);

		linked::Label* enemy_label = new linked::Label(*enemy_name_div, (u8*)"Enemy Name", sizeof "Enemy Name", hm::vec2(50.0f, 0), hm::vec4(1, 1, 1, 1), 50.0f, 0, 0);
		enemy_name_div->getLabels().push_back(enemy_label);

		orb_alive_ally = new Texture("res/orbs/alive_orb.png");
		orb_dead_ally = new Texture("res/orbs/dead_orb.png");;
		orb_alive_enemy = new Texture("res/orbs/enemy_alive_orb.png");;
		orb_dead_enemy = new Texture("res/orbs/enemy_dead_orb.png");;
		float poff_x = 0.0f;
		for (int i = 0; i < NUM_ALLIES; ++i) {
			linked::WindowDiv* player_indicator = new linked::WindowDiv(*player_name, 32, 32, 0, 0, hm::vec2(250 + poff_x, 0), hm::vec4(1, 0, 0, 1), linked::DIV_CENTER_Y | linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
			player_indicator->setBackgroundTexture(orb_alive_ally);
			player_name->divs.push_back(player_indicator);
			poff_x += 32.0f + 5.0f;
			gw.allies_indicator[i] = player_indicator;
		}
		//gw.allies_indicator[0]->setBackgroundTexture(orb_dead_ally);
		poff_x = 0.0f;
		for (int i = 0; i < NUM_ENEMIES; ++i) {
			linked::WindowDiv* enemy_indicator = new linked::WindowDiv(*player_name, 32, 32, 0, 0, hm::vec2(1200.0f + poff_x, 0), hm::vec4(1, 0, 0, 1), linked::DIV_CENTER_Y | linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
			enemy_indicator->setBackgroundTexture(orb_alive_enemy);
			player_name->divs.push_back(enemy_indicator);
			poff_x += 32.0f + 5.0f;
			gw.enemies_indicator[i] = enemy_indicator;
		}

		// end turn button
		linked::WindowDiv* end_turn = new linked::WindowDiv(*player_name, 200, 45, 0, 0, hm::vec2(0, 20.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP |linked::DIV_CENTER_X);
		linked::Label* end_turn_label = new linked::Label(*end_turn, (u8*)"END TURN", sizeof "END TURN", hm::vec2(55.0f, 12.0f), hm::vec4(1, 1, 1, 1), 40.0f, 0, 0);
		linked::Button* end_turn_button = new linked::Button(*end_turn, end_turn_label, hm::vec2(0, 0), 200, 45, greener_cyan - hm::vec4(0.2f, 0.2f, 0.2f, 0.0f), 0);
		combat_state.end_turn_button = end_turn_button;
		end_turn_button->setClickedCallback(end_turn_button_callback);
		player_name->divs.push_back(end_turn);
		end_turn->getButtons().push_back(end_turn_button);
		end_turn_button->setHoveredBGColor(greener_cyan - hm::vec4(0.4f, 0.35f, 0.4f, 0.0f));
		end_turn_button->setHeldBGColor(hm::vec4(0.4f, 0.65f, 0.45f, 1.0f));
	}
	{
		// Timer window
		linked::Window* timer_window = new linked::Window(win_state.win_width, 5, hm::vec2(0, 80), greener_cyan, 0, 0, linked::W_UNFOCUSABLE);
		gw.timer_window = timer_window;
	}


	hm::vec2 start_pos(60, 120);
	float size_img = 160.0f;
	float size_info = 350.0f;
	float x_spacing = 10.0f;
	float y_spacing = 20.0f;
	float hp_bar_height = 25.0f;
	float size_enemy_bars = 80.0f;

	float skill_img_size = 75.0f;

	for (int i = 0; i < NUM_ALLIES; ++i) {
		gw.allies[i] = new linked::Window(size_img, size_img, start_pos, char_window_color, 0, 0, linked::W_UNFOCUSABLE | linked::W_BORDER);
		gw.allies[i]->setBorderSizeX(1.0f);
		gw.allies[i]->setBorderSizeY(1.0f);
		gw.allies[i]->setBorderColor(hm::vec4(0, 1, 1, 1));
		linked::WindowDiv* allies_div = new linked::WindowDiv(*gw.allies[i], size_img, size_img, 0, 0, hm::vec2(0, 0), char_window_color, linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		gw.allies[i]->divs.push_back(allies_div);

		start_pos.x += size_img + x_spacing;
		gw.allies_info[i] = new linked::Window(size_info, size_img, start_pos, char_window_color, 0, 0, linked::W_UNFOCUSABLE | linked::W_BORDER);
		gw.allies_info[i]->setBorderSizeX(1.0f);
		gw.allies_info[i]->setBorderSizeY(1.0f);
		gw.allies_info[i]->setBorderColor(hm::vec4(0, 1, 1, 1));

		float x_off = 0.0f;
		for (int k = 0; k < NUM_SKILLS; ++k) {
			gw.allies_skills[i * NUM_SKILLS + k] = new linked::Window(skill_img_size, skill_img_size, start_pos + hm::vec2(x_spacing + x_off, hp_bar_height + y_spacing * 2 + 5.0f), hm::vec4(1, 1, 1, 1), 0, 0, linked::W_BORDER | linked::W_UNFOCUSABLE);
			linked::WindowDiv* skill_div = new linked::WindowDiv(*gw.allies_skills[i * NUM_SKILLS + k], skill_img_size, skill_img_size, 0, 0, hm::vec2(0, 0), hm::vec4(0, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
			gw.allies_skills[i * NUM_SKILLS + k]->divs.push_back(skill_div);
			gw.allies_skills[i * NUM_SKILLS + k]->setBorderSizeX(1.0f);
			gw.allies_skills[i * NUM_SKILLS + k]->setBorderSizeY(1.0f);
			gw.allies_skills[i * NUM_SKILLS + k]->setBorderColor(hm::vec4(ally_hp_bar_full_color));
			linked::Button* skill_button = new linked::Button(*skill_div, 0, hm::vec2(0, 0), skill_img_size, skill_img_size, hm::vec4(0, 1, 1, 1), k);
			skill_button->setHoveredBGColor(hm::vec4(0, 1, 1, 0.8f));
			skill_div->getButtons().push_back(skill_button);
			x_off += skill_img_size + x_spacing;
		}

		linked::WindowDiv* hpempty = new linked::WindowDiv(*gw.allies_info[i], size_info - 2 * x_spacing + 2, hp_bar_height + 2.0f, 0, 0, hm::vec2(x_spacing - 1, x_spacing - 1), ally_hp_bar_empty_color, linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		gw.allies_info[i]->divs.push_back(hpempty);

		linked::WindowDiv* hpfilled = new linked::WindowDiv(*gw.allies_info[i], size_info - 2 * x_spacing, hp_bar_height, 0, 0, hm::vec2(x_spacing, x_spacing), ally_hp_bar_full_color, linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		gw.allies_info[i]->divs.push_back(hpfilled);

		linked::WindowDiv* dummy = new linked::WindowDiv(*gw.allies_info[i], size_info - 2 * x_spacing + 2, hp_bar_height + 2.0f, 0, 0, hm::vec2(x_spacing, y_spacing + hp_bar_height), hm::vec4(0, 0, 1, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		gw.allies_info[i]->divs.push_back(dummy);
		linked::Label* hplabel = new linked::Label(*dummy, (u8*)"100/100", sizeof("100/100"), hm::vec2(size_info - sizeof("100/100") * 10.0f, 0.0f), hm::vec4(0, 1, 1, 1), 28.0f, 0, 0);
		dummy->getLabels().push_back(hplabel);

		start_pos.x = 60;
		start_pos.y += size_img + y_spacing;
	}

	start_pos.x = 1400.0f;
	start_pos.y = 120.0f;

	for (int i = 0; i < NUM_ENEMIES; ++i) {
		gw.enemies[i] = new linked::Window(size_img, size_img, start_pos, char_window_color, 0, 0, linked::W_UNFOCUSABLE | linked::W_BORDER);
		gw.enemies[i]->setBorderSizeX(1.0f);
		gw.enemies[i]->setBorderSizeY(1.0f);
		gw.enemies[i]->setBorderColor(enem_hp_bar_full_color);

		linked::WindowDiv* enemy_image_div = new linked::WindowDiv(*gw.enemies[i], size_img, size_img, 0, 0, hm::vec2(0,0), hm::vec4(0,0,0,1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		enemy_image_div->setBackgroundTexture(char_textures[i]);
		gw.enemies[i]->divs.push_back(enemy_image_div);

		start_pos.x -= (x_spacing + size_info);
		gw.enemies_info[i] = new linked::Window(size_info, size_enemy_bars, start_pos, char_window_color, 0, 0, linked::W_UNFOCUSABLE | linked::W_BORDER);
		gw.enemies_info[i]->setBorderSizeX(1.0f);
		gw.enemies_info[i]->setBorderSizeY(1.0f);
		gw.enemies_info[i]->setBorderColor(enem_hp_bar_full_color);
		
		linked::WindowDiv* hpempty = new linked::WindowDiv(*gw.enemies_info[i], size_info - 2 * x_spacing + 2, hp_bar_height + 2.0f, 0, 0, hm::vec2(x_spacing - 1, x_spacing - 1), enem_hp_bar_empty_color, linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		gw.enemies_info[i]->divs.push_back(hpempty);
		
		linked::WindowDiv* hpfilled = new linked::WindowDiv(*gw.enemies_info[i], size_info - 2 * x_spacing, hp_bar_height, 0, 0, hm::vec2(x_spacing, x_spacing), enem_hp_bar_full_color, linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		gw.enemies_info[i]->divs.push_back(hpfilled);
		
		linked::WindowDiv* dummy = new linked::WindowDiv(*gw.enemies_info[i], size_info - 2 * x_spacing + 2, hp_bar_height + 2.0f, 0, 0, hm::vec2(x_spacing, y_spacing + hp_bar_height), hm::vec4(0, 0, 1, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		gw.enemies_info[i]->divs.push_back(dummy);
		linked::Label* hplabel = new linked::Label(*dummy, (u8*)"100/100", sizeof("100/100"), hm::vec2(size_info - sizeof("100/100") * 10.0f, 0.0f), enem_hp_bar_full_color, 28.0f, 0, 0);
		dummy->getLabels().push_back(hplabel);

		start_pos.x = 1400.0f;
		start_pos.y += size_img + y_spacing;
	}

	{
		float orbs_size = 64.0f;

		hm::vec4 combat_info_bar_color(15.0f / 255.0f, 17.0f / 255.0f, 42.0f / 255.0f, 0.8f);
		linked::Window* combat_bottom_info = new linked::Window(win_state.win_width, 200, hm::vec2(0, 660), combat_info_bar_color, 0, 0, linked::W_BORDER | linked::W_UNFOCUSABLE);
		linked::WindowDiv* orbs_div = new linked::WindowDiv(*combat_bottom_info, 500, 100, 0, 0, hm::vec2(45.0f, 0), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP | linked::DIV_CENTER_Y);
		combat_bottom_info->divs.push_back(orbs_div);

		linked::Button* multiple_orb_button = new linked::Button(*orbs_div, 0, hm::vec2(0, 0), orbs_size, orbs_size, hm::vec4(0, 0, 0, 1), 0);
		multiple_orb_button->setNormalBGTexture(new Texture("res/orbs/all_orbs.png"));
		orbs_div->getButtons().push_back(multiple_orb_button);

		linked::Button* hard_orb_button = new linked::Button(*orbs_div, 0, hm::vec2((orbs_size + 10) + 18, 0), orbs_size, orbs_size, hm::vec4(0, 0, 0, 1), 0);
		hard_orb_button->setNormalBGTexture(orb_textures[0]);
		orbs_div->getButtons().push_back(hard_orb_button);

		linked::Button* soft_orb_button = new linked::Button(*orbs_div, 0, hm::vec2((orbs_size + 10) * 2 + 18, 0), orbs_size, orbs_size, hm::vec4(0, 0, 0, 1), 0);
		soft_orb_button->setNormalBGTexture(orb_textures[1]);
		orbs_div->getButtons().push_back(soft_orb_button);

		linked::Button* vr_orb_button = new linked::Button(*orbs_div, 0, hm::vec2((orbs_size + 10) * 4 + 18, 0), orbs_size, orbs_size, hm::vec4(0, 0, 0, 1), 0);
		vr_orb_button->setNormalBGTexture(orb_textures[2]);
		orbs_div->getButtons().push_back(vr_orb_button);

		linked::Button* bios_orb_button = new linked::Button(*orbs_div, 0, hm::vec2((orbs_size + 10) * 3 + 18, 0), orbs_size, orbs_size, hm::vec4(0, 0, 0, 1), 0);
		bios_orb_button->setNormalBGTexture(orb_textures[3]);
		orbs_div->getButtons().push_back(bios_orb_button);

		linked::Label* multiple_orb_label = new linked::Label(*orbs_div, (u8*)"0", sizeof("0"), hm::vec2(20, 74), hm::vec4(1, 1, 1, 1), orbs_size, 0, 0);
		orbs_div->getLabels().push_back(multiple_orb_label);
		combat_state.all_orbs_label = multiple_orb_label;

		linked::Label* hard_orb_label = new linked::Label(*orbs_div, (u8*)"0", sizeof("0"), hm::vec2(20 * 2 + (orbs_size + 10), 74), hm::vec4(1, 1, 1, 1), orbs_size, 0, 0);
		orbs_div->getLabels().push_back(hard_orb_label);
		combat_state.orb_labels[ORB_HARD] = hard_orb_label;
		linked::Label* soft_orb_label = new linked::Label(*orbs_div, (u8*)"0", sizeof("0"), hm::vec2(20 * 2 + (orbs_size + 10) * 2, 74), hm::vec4(1, 1, 1, 1), orbs_size, 0, 0);
		orbs_div->getLabels().push_back(soft_orb_label);
		combat_state.orb_labels[ORB_SOFT] = soft_orb_label;
		linked::Label* bios_orb_label = new linked::Label(*orbs_div, (u8*)"0", sizeof("0"), hm::vec2(20 * 2 + (orbs_size + 10) * 3, 74), hm::vec4(1, 1, 1, 1), orbs_size, 0, 0);
		orbs_div->getLabels().push_back(bios_orb_label);
		combat_state.orb_labels[ORB_BIOS] = bios_orb_label;
		linked::Label* vr_orb_label = new linked::Label(*orbs_div, (u8*)"0", sizeof("0"), hm::vec2(20 * 2 + (orbs_size + 10) * 4, 74), hm::vec4(1, 1, 1, 1), orbs_size, 0, 0);
		orbs_div->getLabels().push_back(vr_orb_label);
		combat_state.orb_labels[ORB_VR] = vr_orb_label;

		combat_bottom_info->setBorderSizeY(4.0f);
		combat_bottom_info->setBorderColor(greener_cyan);
		gw.combat_bottom_info = combat_bottom_info;

		float skill_desc_height = 40.0f;
		linked::WindowDiv* skill_image_div_border = new linked::WindowDiv(*combat_bottom_info, 126, 126, 0, 0, hm::vec2(640.0f - 3, skill_desc_height - 3), hm::vec4(0, 1, 1, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_image_div_border);
		linked::WindowDiv* skill_image_div = new linked::WindowDiv(*combat_bottom_info, 120, 120, 0, 0, hm::vec2(640.0f, skill_desc_height), hm::vec4(0, 1, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_image_div);
		skill_image_div_border->m_render = false;
		skill_image_div->m_render = false;

		linked::WindowDiv* skill_title_div = new linked::WindowDiv(*combat_bottom_info, 300, 48, 0, 0, hm::vec2(780.0f, skill_desc_height), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_title_div);
		linked::Label* skill_title_label = new linked::Label(*skill_title_div, (u8*)"", 0, hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), 38.0f, 0, 0);
		skill_title_div->getLabels().push_back(skill_title_label);
		skill_title_div->m_render = false;

		linked::WindowDiv* skill_desc_div = new linked::WindowDiv(*combat_bottom_info, 300, 48, 0, 0, hm::vec2(780.0f, skill_desc_height + 28 + 12), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_desc_div);
		linked::Label* skill_desc_label = new linked::Label(*skill_desc_div, (u8*)"", 0, hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), 30.0f, 0, 0);
		skill_desc_div->getLabels().push_back(skill_desc_label);
		skill_desc_div->m_render = false;

		float orb_size = 32.0f;
		linked::WindowDiv* skill_cost_div_1 = new linked::WindowDiv(*combat_bottom_info, orb_size, orb_size, 0, 0, hm::vec2(1100.0f, skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_cost_div_1);
		linked::WindowDiv* skill_cost_div_2 = new linked::WindowDiv(*combat_bottom_info, orb_size, orb_size, 0, 0, hm::vec2(1100.0f + (orb_size + 5), skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_cost_div_2);
		linked::WindowDiv* skill_cost_div_3 = new linked::WindowDiv(*combat_bottom_info, orb_size, orb_size, 0, 0, hm::vec2(1100.0f + (orb_size + 5) * 2, skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_cost_div_3);
		linked::WindowDiv* skill_cost_div_4 = new linked::WindowDiv(*combat_bottom_info, orb_size, orb_size, 0, 0, hm::vec2(1100.0f + (orb_size + 5) * 3, skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_cost_div_4);
		linked::WindowDiv* skill_cost_div_5 = new linked::WindowDiv(*combat_bottom_info, orb_size, orb_size, 0, 0, hm::vec2(1100.0f + (orb_size + 5) * 4, skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_cost_div_5);

		skill_cost_div_1->m_render = false;
		skill_cost_div_2->m_render = false;
		skill_cost_div_3->m_render = false;
		skill_cost_div_4->m_render = false;
		skill_cost_div_5->m_render = false;

		combat_state.skill_costs[0] = skill_cost_div_1;
		combat_state.skill_costs[1] = skill_cost_div_2;
		combat_state.skill_costs[2] = skill_cost_div_3;
		combat_state.skill_costs[3] = skill_cost_div_4;
		combat_state.skill_costs[4] = skill_cost_div_5;

		combat_state.skill_info_image = skill_image_div;
		combat_state.skill_info_image_border = skill_image_div_border;
		combat_state.skill_info_title = skill_title_div;
		combat_state.skill_info_desc = skill_desc_div;
	}
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

	combat_state.player_turn = true;	//@todo randomize
	init_char_selection_mode();
	init_char_information_mode();
	init_combat_mode();

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

void end_turn() {

	static char all_orbs_buffer[16] = { 0 };
	static char hard_orb_buffer[16] = { 0 };
	static char soft_orb_buffer[16] = { 0 };
	static char vr_orb_buffer[16] = { 0 };
	static char bios_orb_buffer[16] = { 0 };

	if (!combat_state.player_turn) {
		int orb_generated = rand() % (ORB_NUMBER - 1);
		printf("generated orb %d\n", orb_generated);
		combat_state.total_orbs += 1;

		int count = s32_to_str_base10(combat_state.total_orbs, all_orbs_buffer);
		combat_state.all_orbs_label->setText((u8*)all_orbs_buffer, count + 1);
		combat_state.orbs_amount[orb_generated] += 1;
		switch (orb_generated) {
		case ORB_HARD: {
			memset(hard_orb_buffer, 0, sizeof(hard_orb_buffer));
			int count = s32_to_str_base10(combat_state.orbs_amount[ORB_HARD], hard_orb_buffer);
			combat_state.orb_labels[ORB_HARD]->setText((u8*)hard_orb_buffer, count + 1);
		}break;
		case ORB_SOFT: {
			memset(soft_orb_buffer, 0, sizeof(soft_orb_buffer));
			int count = s32_to_str_base10(combat_state.orbs_amount[ORB_SOFT], soft_orb_buffer);
			combat_state.orb_labels[ORB_SOFT]->setText((u8*)soft_orb_buffer, count + 1);
		}break;
		case ORB_VR: {
			memset(vr_orb_buffer, 0, sizeof(vr_orb_buffer));
			int count = s32_to_str_base10(combat_state.orbs_amount[ORB_VR], vr_orb_buffer);
			combat_state.orb_labels[ORB_VR]->setText((u8*)vr_orb_buffer, count + 1);
		}break;
		case ORB_BIOS: {
			memset(bios_orb_buffer, 0, sizeof(bios_orb_buffer));
			int count = s32_to_str_base10(combat_state.orbs_amount[ORB_BIOS], bios_orb_buffer);
			combat_state.orb_labels[ORB_BIOS]->setText((u8*)bios_orb_buffer, count + 1);
		}break;
		}
	}
	
	combat_state.player_turn = !combat_state.player_turn;
	printf("SWITCH TURN\n");
	hm::vec4 greener_cyan(0, 1, 0.7f, 1);
	if (combat_state.player_turn) {
		combat_state.end_turn_button->getLabel()->setText((u8*)"END TURN", sizeof("END TURN"));
		combat_state.end_turn_button->setNormalBGColor(greener_cyan - hm::vec4(0.2f, 0.2f, 0.2f, 0.0f));
	} else {
		combat_state.end_turn_button->getLabel()->setText((u8*)"ENEMY TURN", sizeof("ENEMY TURN"));
		combat_state.end_turn_button->setNormalBGColor(hm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	}
}

void update_game_mode(double frametime)
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
					//gw.left_char_window->divs[0]->setBackgroundTexture(char_tex);
					gw.left_char_window->divs[0]->setBackgroundTexture(chars_texture_big[i]);
					linked::Label* name_label = gw.left_char_window->divs[1]->getLabels()[0];
					name_label->setText((u8*)char_names[i], char_names_length[i]);
					name_label->setPosition(hm::vec2(380.0f - (char_names_length[i] - 1) * 18.0f, 15.0f));
					char_sel_state.last_hovered = (Character_ID)i;
				}
			}
			for (int i = 0; i < NUM_ALLIES; ++i) {
				if (i < char_sel_state.num_selected) {
					gw.char_selected_window->divs[i]->m_render = true;
					gw.char_selected_window->divs[i]->setBackgroundTexture(char_textures[char_sel_state.selections[i]]);
				} else
					gw.char_selected_window->divs[i]->m_render = false;
			}

		}break;
		case MODE_CHAR_INFO: {
			linked::Label* skill_label = gw.char_info_window->divs[NUM_SKILLS]->getLabels()[0];
			linked::Label* skill_desc_label = gw.char_info_window->divs[NUM_SKILLS + 1]->getLabels()[0];
			for (int i = 0; i < NUM_SKILLS; ++i) {
				int index = i + char_sel_state.last_hovered * NUM_SKILLS;
				// first 4(NUM_SKILLS) are buttons
				gw.char_info_window->divs[i]->getButtons()[0]->setNormalBGTexture(skill_textures[index]);
				if (gw.char_info_window->divs[i]->isButtonHovered()) {
					gw.char_info_skill_cost->setActive(true);
					skill_label->setText((u8*)skill_names[index], skill_names_length[index]);
					skill_desc_label->setText((u8*)skill_desc[index], skill_desc_length[index]);

					// set skill group desc
					layout_set_skill_group_from_skill(index, gw.skill_group_div->getLabels()[0]);

					for (int n = 0, orb_index = 0; n < ORB_NUMBER; ++n) {
						gw.char_info_skill_cost->divs[n]->m_render = false;
					}
					for (int n = 0, orb_index = 0; n < ORB_NUMBER; ++n) {
						int cost = skill_costs[index][n];
						while (cost > 0) {
							//gw.char_info_skill_cost
							gw.char_info_skill_cost->divs[orb_index]->m_render = true;
							gw.char_info_skill_cost->divs[orb_index++]->setBackgroundTexture(orb_textures[n]);
							cost--;
						}
					}
					break;
				}
			}

		}break;
		case MODE_COMBAT: {
			//frametime = 0.016 aprox
			turn_time -= frametime;
			if (turn_time <= 0.0) {
				turn_time = TURN_DURATION;
				// this is when the turn is flipped
				end_turn();
			}
			double new_w = (int)((double)win_state.win_width * turn_time / TURN_DURATION);
			gw.timer_window->setWidth(new_w);

			bool is_hovering_skill = false;
			for (int i = 0; i < NUM_ALLIES; ++i) {
				for (int k = 0; k < NUM_SKILLS; ++k) {
					if (gw.allies_skills[i * NUM_SKILLS + k]->divs[0]->getButtons()[0]->isHovered()) {
						//printf("skill %d of ally %d hovered\n", k, i);
						int skill_index = char_sel_state.selections[i] * NUM_SKILLS + k;
						combat_state.skill_info_image->setBackgroundTexture(skill_textures[skill_index]);
						combat_state.skill_info_title->getLabels()[0]->setText((u8*)skill_names[skill_index], skill_names_length[skill_index]);
						combat_state.skill_info_desc->getLabels()[0]->setText((u8*)skill_desc[skill_index], skill_desc_length[skill_index]);
						combat_state.last_hovered = (Skill_ID)(skill_index);
						for (int n = 0, orb_index = 0; n < ORB_NUMBER; ++n) {
							combat_state.skill_costs[n]->m_render = false;
						}
						for (int n = 0, orb_index = 0; n < ORB_NUMBER; ++n) {
							int cost = skill_costs[skill_index][n];
							while (cost > 0) {
								combat_state.skill_costs[orb_index]->m_render = true;
								combat_state.skill_costs[orb_index++]->setBackgroundTexture(orb_textures[n]);
								cost--;
							}
						}
						is_hovering_skill = true;
						break;
					}
				}
			}
			if (!is_hovering_skill) {
				for (int i = 0; i < ORB_NUMBER; ++i)
					combat_state.skill_costs[i]->m_render = false;
			}
			combat_state.skill_info_image_border->m_render = is_hovering_skill;
			combat_state.skill_info_image->m_render = is_hovering_skill;
			combat_state.skill_info_title->m_render = is_hovering_skill;
			combat_state.skill_info_desc->m_render = is_hovering_skill;
		}break;
	}
}

void change_game_mode(Game_Mode mode)
{
	if (ggs.mode == mode) return;
	switch (ggs.mode) {
		// cleanup the mode you are in
		case MODE_CHAR_SELECT: {
			gw.char_selected_window->setActive(false);
			gw.left_char_window->setActive(false);
			gw.char_selection_window->setActive(false);
		}break;
		case MODE_CHAR_INFO: {
			gw.left_char_window->setActive(false);
			gw.char_info_window->setActive(false);
			gw.char_info_window_bot->setActive(false);
			gw.char_info_skill_cost->setActive(false);
			linked::Label* skill_label = gw.char_info_window->divs[NUM_SKILLS]->getLabels()[0];
			linked::Label* skill_desc_label = gw.char_info_window->divs[NUM_SKILLS + 1]->getLabels()[0];
			skill_label->setText(0, 0);
			skill_desc_label->setText(0, 0);
		}break;
		case MODE_COMBAT: {
			for (int i = 0; i < NUM_ALLIES; ++i) {
				gw.allies[i]->setActive(false);
				gw.allies_info[i]->setActive(false);
				for (int k = 0; k < NUM_SKILLS; ++k) {
					gw.allies_skills[i * NUM_SKILLS + k]->setActive(false);
				}
			}
			for (int i = 0; i < NUM_ENEMIES; ++i) {
				gw.enemies[i]->setActive(false);
				gw.enemies_info[i]->setActive(false);
			}
			gw.combat_bottom_info->setActive(false);
			gw.timer_window->setActive(false);
			gw.player_name_window->setActive(false);
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
		case MODE_COMBAT: {
			for (int i = 0; i < NUM_ALLIES; ++i) {
				int index = char_sel_state.selections[i];
				Texture* t = char_textures[char_sel_state.selections[i]];
				gw.allies[i]->divs[0]->setBackgroundTexture(t);

				gw.allies[i]->setActive(true);
				gw.allies_info[i]->setActive(true);
				for (int k = 0; k < NUM_SKILLS; ++k) {
					gw.allies_skills[i * NUM_SKILLS + k]->setActive(true);
					linked::Button* b = gw.allies_skills[i * NUM_SKILLS + k]->divs[0]->getButtons()[0];
					Texture* skill = skill_textures[index * NUM_SKILLS + k];
					gw.allies_skills[i * NUM_SKILLS + k]->divs[0]->getButtons()[0]->setNormalBGTexture(skill);
					gw.allies_skills[i * NUM_SKILLS + k]->divs[0]->getButtons()[0]->setHoveredBGTexture(skill);
				}
			}
			for (int i = 0; i < NUM_ENEMIES; ++i) {
				gw.enemies[i]->setActive(true);
				gw.enemies_info[i]->setActive(true);
			}
			gw.combat_bottom_info->setActive(true);
			gw.timer_window->setActive(true);
			gw.player_name_window->setActive(true);
		}break;
	}
}

void input();

void update_and_render(double frametime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (chat.m_active)
		chat.update();

	update_game_mode(frametime);

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

	if (keyboard_state.key_event['A']) {
		keyboard_state.key_event['A'] = false;
		layout_enemy_die(0);
	}
	if (keyboard_state.key_event['B']) {
		keyboard_state.key_event['B'] = false;
		layout_ally_die(2);
	}

	if (!chat_window->isFocused())
		chat_window->setFocus();
}

static void layout_enemy_die(u32 enemy_index) {
	assert(enemy_index <= NUM_ENEMIES);
	gw.enemies_indicator[enemy_index]->setBackgroundTexture(orb_dead_enemy);
}

static void layout_ally_die(u32 ally_index) {
	assert(ally_index <= NUM_ALLIES);
	gw.allies_indicator[ally_index]->setBackgroundTexture(orb_dead_ally);
}

static void put_space(int* length, char* buffer) {
	buffer[(*length)++] = ',';
	buffer[(*length)++] = ' ';
}

#define PUT_STR(STR, BUFFER, LEN) strcpy(BUFFER + LEN, STR); LEN += sizeof(STR) - 1

static void layout_set_skill_group_from_skill(int skill_index, linked::Label* label) {
	static char buffer[512] = {0};
	int length = 0;
	bool insert_space = false;

	switch (skill_groups[skill_index].type) {
	case SKILL_TYPE_NONE: break;
	case SKILL_TYPE_PHYSICAL:	PUT_STR("PHYSICAL", buffer, length); insert_space = true; break;
	case SKILL_TYPE_MENTAL:		PUT_STR("MENTAL", buffer, length);   insert_space = true; break;
	case SKILL_TYPE_VIRTUAL:	PUT_STR("VIRTUAL", buffer, length);  insert_space = true; break;
	default: break;
	}

	switch (skill_groups[skill_index].mode) {
	case SKILL_MODE_NONE: break;
	case SKILL_MODE_MELEE:  if (insert_space) put_space(&length, buffer); PUT_STR("MELEE", buffer, length);  insert_space = true;  break;
	case SKILL_MODE_RANGED: if (insert_space) put_space(&length, buffer); PUT_STR("RANGED", buffer, length); insert_space = true;  break;
	default: break;
	}

	switch (skill_groups[skill_index].category) {
	case SKILL_CATEGORY_NORMAL: break;
	case SKILL_CATEGORY_COUNTER: if (insert_space) put_space(&length, buffer); PUT_STR("COUNTER", buffer, length); insert_space = true; break;
	case SKILL_CATEGORY_STATUS:  if (insert_space) put_space(&length, buffer); PUT_STR("STATUS", buffer, length);  insert_space = true; break;
	default: break;
	}

	switch (skill_groups[skill_index].condition) {
	case SKILL_CONDITION_NONE:
	case SKILL_CONDITION_NORMAL: break;
	case SKILL_CONDITION_BURN: if (insert_space) put_space(&length, buffer); PUT_STR("BURN", buffer, length); insert_space = true; break;
	case SKILL_CONDITION_FREEZE: if (insert_space) put_space(&length, buffer); PUT_STR("FREEZE", buffer, length); insert_space = true; break;
	case SKILL_CONDITION_POISON: if (insert_space) put_space(&length, buffer); PUT_STR("POISON", buffer, length); insert_space = true; break;
	case SKILL_CONDITION_PARALYZE: if (insert_space) put_space(&length, buffer); PUT_STR("PARALYZE", buffer, length); insert_space = true; break;
	case SKILL_CONDITION_SLEEP: if (insert_space) put_space(&length, buffer); PUT_STR("SLEEP", buffer, length); insert_space = true; break;
	default: break;
	}

	switch (skill_groups[skill_index].damage) {
	case SKILL_DMG_NONE: break;
	case SKILL_DMG_NORMAL: if (insert_space) put_space(&length, buffer); PUT_STR("DMG NORMAL", buffer, length); insert_space = true; break;
	case SKILL_DMG_PIERCING: if (insert_space) put_space(&length, buffer); PUT_STR("PIERCING", buffer, length); insert_space = true; break;
	case SKILL_DMG_CRUSHING: if (insert_space) put_space(&length, buffer); PUT_STR("CRUSHING", buffer, length); insert_space = true; break;
	default: break;
	}

	switch (skill_groups[skill_index].defense) {
	case SKILL_DEF_NONE: break;
	case SKILL_DEF_REDUCTION: if (insert_space) put_space(&length, buffer); PUT_STR("REDUCTION", buffer, length); insert_space = true; break;
	case SKILL_DEF_ABSORPTION: if (insert_space) put_space(&length, buffer); PUT_STR("ABSORPTION", buffer, length); insert_space = true; break;
	case SKILL_DEF_RELECTION: if (insert_space) put_space(&length, buffer); PUT_STR("REFLECTION", buffer, length); insert_space = true; break;
	case SKILL_DEF_INVULNERABILITY: if (insert_space) put_space(&length, buffer); PUT_STR("INVUL", buffer, length); insert_space = true; break;
	default: break;
	}

	switch (skill_groups[skill_index].duration) {
	case SKILL_DURATION_NONE: break;
	case SKILL_DURATION_STATIC: if (insert_space) put_space(&length, buffer); PUT_STR("STATIC", buffer, length); insert_space = true; break;
	case SKILL_DURATION_CONTROL: if (insert_space) put_space(&length, buffer); PUT_STR("CONTROL", buffer, length); insert_space = true; break;
	case SKILL_DURATION_CONSTANT: if (insert_space) put_space(&length, buffer); PUT_STR("CONSTANT", buffer, length); insert_space = true; break;
	}

	switch (skill_groups[skill_index].unique) {
	case SKILL_NOT_UNIQUE: break;
	case SKILL_UNIQUE: if (insert_space) put_space(&length, buffer); PUT_STR("UNIQUE", buffer, length); break;
	default: break;
	}

	label->setText((u8*)buffer, length);
}