#include "application.h"
#include "input.h"
#include "chat.h"
#include "font_render/os.h"
#include "font_render/font_rendering.h"
#include "client.h"

#define ARROW_UP 1
#define ARROW_DOWN 0

extern Window_Info window_info;
extern Keyboard_State keyboard_state;
extern Mouse_State mouse_state;
Chat* g_chat = 0;
Chat chat;
linked::Window* chat_window = 0;

Font_ID fonts[32] = {};
int x = 0;

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

	"BEST BOUND FIST",
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

	sizeof "BEST BOUND FIST",
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
	"Marca um adversário e, se no próximo turno ele realizar um ataque em Zer0, seu golpe é negado, \nsofre 20 de dano e recebe status Paralyze por 1 turno.",
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
	"Concentrando toda sua energia positiva, On1 obtém uma defesa\nabsoluta e fica invulnerável por 1 turno.",	// @TODO

	// Qwerty
	"Qwerty altera sua forma física e ganha redução de dano de 40 pontos\npor 4 turnos.",
	"Controla um inimigo por dois turnos. DELETE pode ser usado no\nadversário controlado.",
	"Mata instantaneamente qualquer adversário que recebe este golpe,\ndesde que esteja sob efeito de CTRL.",
	"Concentrando toda sua energia positiva, On1 obtém uma defesa\nabsoluta e fica invulnerável por 1 turno.",	// @TODO

	// Big O
	"Ataca o oponente com menor HP e realiza 20 de dano. Por 2 turnos,\no alvo fica com status BURN. Se o alvo já possui status BURN, este\nsofre 10 de dano crushing adicional.",
	"Se utilizado em um adversário, gera status BURN por 3 turnos. Se,\nem um aliado, recupera 25 de energia e atribui status FROZEN por 2 turnos.",
	"No próximo turno, recebe 4 orbs de energia: uma de cada categoria.",
	"Concentrando toda sua energia positiva, On1 obtém uma defesa\nabsoluta e fica invulnerável por 1 turno.",	// @TODO

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
	sizeof "Marca um adversário e, se no próximo turno ele realizar um ataque em Zer0, seu golpe é negado, \nsofre 20 de dano e recebe status Paralyze por 1 turno.",
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
	sizeof "Concentrando toda sua energia positiva, On1 obtém uma defesa\nabsoluta e fica invulnerável por 1 turno.",	//@TODO

	// Qwerty
	sizeof "Qwerty altera sua forma física e ganha redução de dano de 40 pontos\npor 4 turnos.",
	sizeof "Controla um inimigo por dois turnos. DELETE pode ser usado no\nadversário controlado.",
	sizeof "Mata instantaneamente qualquer adversário que recebe este golpe,\ndesde que esteja sob efeito de CTRL.",
	sizeof "Concentrando toda sua energia positiva, On1 obtém uma defesa\nabsoluta e fica invulnerável por 1 turno.",	//@TODO

	// Big O
	sizeof "Ataca o oponente com menor HP e realiza 20 de dano. Por 2 turnos,\no alvo fica com status BURN. Se o alvo já possui status BURN, este\nsofre 10 de dano crushing adicional.",
	sizeof "Se utilizado em um adversário, gera status BURN por 3 turnos. Se,\nem um aliado, recupera 25 de energia e atribui status FROZEN por 2 turnos.",
	sizeof "No próximo turno, recebe 4 orbs de energia: uma de cada categoria.",
	sizeof "Concentrando toda sua energia positiva, On1 obtém uma defesa\nabsoluta e fica invulnerável por 1 turno.",	//@TODO

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

char* char_descriptions[NUM_CHARS] = {
	"O vilão que nunca tem suas condições atendidas!",
	"O super herói mais otimista possível!",
	"Possui um papagaio chamado Cracken.",
	"Fotogênica, ama câmeras e sempre anda pelas sombras.",
	"Em sua busca sistemática para achar o caminho do estrelato.",
	"Graduado em direito, mestre em processos.",
	"Uma máquina na arte da luta: simples e eficaz.",
	"Codinome catástrofe.",
	"I/OLO",
	"Temperamental, complexo de se relacionar.",
	"Elegante, cheia de classe.",
	"Nunca se atrasou para o ENEM."
};

int char_descriptions_length[NUM_CHARS] = {
	sizeof "O vilão que nunca tem suas condições atendidas!",
	sizeof "O super herói mais otimista possível!",
	sizeof "Possui um papagaio chamado Cracken.",
	sizeof "Fotogênica, ama câmeras e sempre anda pelas sombras.",
	sizeof "Em sua busca sistemática para achar o caminho do estrelato.",
	sizeof "Graduado em direito, mestre em processos.",
	sizeof "Uma máquina na arte da luta: simples e eficaz.",
	sizeof "Codinome catástrofe.",
	sizeof "I/OLO",
	sizeof "Temperamental, complexo de se relacionar.",
	sizeof "Elegante, cheia de classe.",
	sizeof "Nunca se atrasou para o ENEM."
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

static GameState ggs = {};
static Char_Selection_State char_sel_state = {};
extern Combat_State combat_state = {};
static Game_Windows gw = {};

#include "game_skills.cpp"

static double turn_time = TURN_DURATION;

hm::vec4 char_window_held_color(0.5f, 1.0f, 1.0f, 0.65f);
hm::vec4 char_window_hover_color(0.35f, 0.6f, 0.6f, 0.65f);
hm::vec4 char_window_color(15.0f / 255.0f, 17.0f / 255.0f, 42.0f / 255.0f, 1.0f);
hm::vec4 char_selected_bg_color(0.4f, 0.7f, 0.7f, 1.0f);
hm::vec4 greener_cyan(0, 1, 0.7f, 1);
hm::vec4 color_red(1, 0, 0, 1);
hm::vec4 darker_gray(0.6f, 0.6f, 0.6f, 0.0f);
hm::vec4 cyan(0.0f, 1.0f, 1.0f, 1.0f);
hm::vec4 ally_hp_bar_full_color(0, 1, 1, 1);
hm::vec4 ally_hp_bar_empty_color(0, 0.3f, 0.3f, 1.0f);
hm::vec4 enem_hp_bar_full_color(1, 0.3529f, 0, 1.0f);
hm::vec4 enem_hp_bar_empty_color(0.61f, 0.21f, 0.0f, 1.0f);

//hm::vec4 enem_hp_bar_full_color(1, 0, 0.4f, 1.0f);			//#ff0066
//hm::vec4 enem_hp_bar_empty_color(0.6f, 0.0f, 0.2392f, 1.0f);	// #99003d

static SOCKET * connection;
static client_info * player;

// Button Callbacks
static void button_select_character(void* arg) {
	AudioController::navigationAudio.play();

	auto divs = (std::vector<linked::WindowDiv*>*)((linked::Button_Info*)arg)->data;
	int id = ((linked::Button_Info*)arg)->id;
	bool selected = (*divs)[id]->getButtons()[0]->getIsToggled();

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
	
#if 0
		layout_toggle_char_selection(char_sel_state.selections[0], divs);
		char_sel_state.selections[0] = char_sel_state.selections[1];
		char_sel_state.selections[1] = char_sel_state.selections[2];
		char_sel_state.selections[2] = id;
#else
		// replace the last selected
		layout_toggle_char_selection(char_sel_state.selections[NUM_ALLIES - 1], divs, selected);
		(*divs)[char_sel_state.selections[NUM_ALLIES - 1]]->getButtons()[0]->toggle();
		char_sel_state.selections[NUM_ALLIES - 1] = id;
#endif
	}
	(*divs)[id]->getButtons()[0]->toggle();
	layout_toggle_char_selection(id, divs, !selected);
}

static void temporary_modify_orbs(Skill_ID skill_used, s32 add);
static void reset_targets_animation();

static void button_end_turn(void* arg) {

	if (combat_state.player.targeting) {
		Skill_ID skill_used = combat_state.player.targeting_info.skill_used;
		temporary_modify_orbs(skill_used, 1);
		reset_targets_animation();
	}

	if (!combat_state.player_turn) {
		AudioController::cancelAudio.play();
		return;
	}
	AudioController::confirmAudio.play();
	turn_time = TURN_DURATION;
	end_turn();
}

static void button_combat_start_mode(void* arg)
{
	AudioController::confirmAudio.play();
	change_game_mode(MODE_COMBAT);
}

static void button_callback_exchange_orb_cancel(void* arg) {
	printf("Exchange Cancel!\n");
	gw.exchange_orb_ui->window->setActive(false);
	combat_state.exchange_orbs_state.active = false;
	for (int i = 0; i < ORB_NUMBER - 1; ++i) {
		layout_set_exchange_modal_quantity(false, (Orb_ID)i, 0);
		combat_state.exchange_orbs_state.orb_changes[i] = 0;
	}
	for (int i = 0; i < 3; ++i) {
		layout_set_exchange_modal_upper_orbs(ORB_NONE, (Orb_ID)i);
		combat_state.exchange_orbs_state.exchanging[i] = ORB_NONE;
	}
	combat_state.exchange_orbs_state.exchanging_count = 0;
}
static void button_callback_exchange_orb_confirm(void* arg) {
	printf("Exchange Confirm!\n");

	s32 sum = 0;
	for (s32 i = 0; i < ORB_NUMBER - 1; ++i) {
		sum += combat_state.exchange_orbs_state.orb_new_amount[i];
	}

	if ((combat_state.exchange_orbs_state.exchanging_count < 3 && combat_state.exchange_orbs_state.exchanging_count > 0) || sum > 0) {
			return;
	}

	sum = 0;

	for (s32 i = 0; i < ORB_NUMBER - 1; ++i) {
		combat_state.orbs_amount[i] += combat_state.exchange_orbs_state.orb_changes[i];
		sum += combat_state.exchange_orbs_state.orb_changes[i];
		layout_change_orb_amount((Orb_ID)i, combat_state.orbs_amount[i]);
	}
	combat_state.total_orbs += sum;
	layout_change_orb_amount(ORB_ALL, combat_state.total_orbs);

	button_callback_exchange_orb_cancel(0);
}
static void button_callback_exchange_orb_arrow_left(void* arg) {
	printf("Exchange Arrow Left\n");
	int id = ((linked::Button_Info*)arg)->id;

	// it was really exchanging
	assert(combat_state.exchange_orbs_state.exchanging_count >= 3);

	// Unset upper orbs
	for (s32 i = 0; i < 3; ++i) {
		layout_set_exchange_modal_upper_orbs(ORB_NONE, i);
		combat_state.exchange_orbs_state.exchanging[i] = ORB_NONE;
	}
	combat_state.exchange_orbs_state.exchanging_count = 0;

	// Remove from right
	for (s32 i = 0; i < ORB_NUMBER - 1; ++i) {
		if(combat_state.exchange_orbs_state.orb_new_amount[i] > 0)
			combat_state.exchange_orbs_state.orb_changes[i] -= combat_state.exchange_orbs_state.orb_new_amount[i];
		combat_state.exchange_orbs_state.orb_new_amount[i] = 0;
		layout_set_exchange_modal_quantity(false, (Orb_ID)i, 0);
	}

	// Put on the left
	combat_state.exchange_orbs_state.orb_changes[id] += 1;
	combat_state.exchange_orbs_state.orb_left_amount[id] += 1;
	layout_set_exchange_modal_quantity(true, (Orb_ID)id, combat_state.exchange_orbs_state.orb_left_amount[id]);

	// Reset arrow buttons
	for (int i = 0; i < ORB_NUMBER - 1; ++i) {
		if(combat_state.exchange_orbs_state.orb_left_amount[i] > 0)
			gw.exchange_orb_ui->arrows_right[i]->setActive(true);
		gw.exchange_orb_ui->arrows_left[i]->setActive(false, false);
	}
}
static void button_callback_exchange_orb_arrow_right(void* arg) {
	printf("Exchange Arrow Right\n");
	Orb_ID id = (Orb_ID)((linked::Button_Info*)arg)->id;

	// the button was really active?
	assert(combat_state.exchange_orbs_state.orb_left_amount[id] > 0);

	// if it was ...

	// Set the upper orbs
	for (s32 i = 0; i < 3; ++i) {
		if (combat_state.exchange_orbs_state.exchanging[i] == ORB_NONE) {
			layout_set_exchange_modal_upper_orbs(id, i);
			combat_state.exchange_orbs_state.exchanging[i] = id;
			break;
		}
	}
	// Remove from left
	combat_state.exchange_orbs_state.orb_left_amount[id] -= 1;
	combat_state.exchange_orbs_state.orb_new_amount[id] += 1;
	layout_set_exchange_modal_quantity(true, id, combat_state.exchange_orbs_state.orb_left_amount[id]);
	layout_set_exchange_modal_quantity(false, id, combat_state.exchange_orbs_state.orb_new_amount[id]);

	// Exchanging count gets bigger, if it ever gets to 3 deactivate
	// right arrows and activate all left arrows except the ones present
	// in the exchanging
	combat_state.exchange_orbs_state.exchanging_count += 1;
	if (combat_state.exchange_orbs_state.exchanging_count >= 3) {

		for (s32 i = 0; i < ORB_NUMBER - 1; ++i) {
			gw.exchange_orb_ui->arrows_right[i]->setActive(false, false);
			bool different_orb = true;
			for (s32 j = 0; j < 3; ++j) {
				if (combat_state.exchange_orbs_state.exchanging[j] == (Orb_ID)i) {
					different_orb = false;
					break;
				}
			}
			if(different_orb)
				gw.exchange_orb_ui->arrows_left[i]->setActive(true);
		}
	}

	if (combat_state.exchange_orbs_state.orb_left_amount[id] == 0)
		gw.exchange_orb_ui->arrows_right[id]->setActive(false, false);
}
static void button_callback_exchange_orb(void* arg) {
	gw.exchange_orb_ui->window->setActive(true);
	combat_state.exchange_orbs_state.active = true;

	for (int i = 0; i < ORB_NUMBER - 1; ++i) {
		combat_state.exchange_orbs_state.orb_changes[i] = 0;
		combat_state.exchange_orbs_state.orb_new_amount[i] = 0;
		combat_state.exchange_orbs_state.orb_left_amount[i] = combat_state.orbs_amount[i];
		layout_set_exchange_modal_quantity(true, (Orb_ID)i, combat_state.orbs_amount[i]);
		gw.exchange_orb_ui->arrows_left[i]->setActive(false, false);
		if(combat_state.orbs_amount[i] > 0)
			gw.exchange_orb_ui->arrows_right[i]->setActive(true);
	}
	for (int i = 0; i < 3; ++i) {
		layout_set_exchange_modal_upper_orbs(ORB_NONE, (Orb_ID)i);
		combat_state.exchange_orbs_state.exchanging[i] = ORB_NONE;
	}
	combat_state.exchange_orbs_state.exchanging_count = 0;
}

static void button_callback_sacrifice_orb_arrow_left(void* arg) {
	printf("Sacrifice Arrow Left\n");
	Orb_ID id = (Orb_ID)((linked::Button_Info*)arg)->id;

	s32 sum = 0;
	for (s32 i = 0; i < ORB_NUMBER - 1; ++i) {
		sum += combat_state.sacrifice_orbs_state.orb_right_amount[i];
	}

	if (combat_state.sacrifice_orbs_state.orb_right_amount[id] > 0) {
		// Update left and right
		combat_state.sacrifice_orbs_state.orb_right_amount[id] -= 1;
		layout_set_sacrifice_modal_quantity(false, id, combat_state.sacrifice_orbs_state.orb_right_amount[id]);
		combat_state.sacrifice_orbs_state.orb_left_amount[id] += 1;
		layout_set_sacrifice_modal_quantity(true, id, combat_state.sacrifice_orbs_state.orb_left_amount[id]);
		gw.sacrifice_orb_ui->orb_number_label->getText()[0] = combat_state.total_null_orbs_in_temp_use - sum + 1 + 0x30;
		// Activate right arrows
		for (s32 i = 0; i < ORB_NUMBER; ++i) {
			if(combat_state.sacrifice_orbs_state.orb_left_amount[id] > 0)
				gw.sacrifice_orb_ui->arrows_right[i]->setActive(true);
		}

		if (combat_state.sacrifice_orbs_state.orb_right_amount[id] == 0) {
			gw.sacrifice_orb_ui->arrows_left[id]->setActive(false, false);
		}
	}
}
static void button_callback_sacrifice_orb_arrow_right(void* arg) {
	printf("Sacrifice Arrow Right\n");
	Orb_ID id = (Orb_ID)((linked::Button_Info*)arg)->id;
	// the button was really active?
	assert(combat_state.sacrifice_orbs_state.orb_left_amount[id] > 0);

	s32 sum = 0;
	for (s32 i = 0; i < ORB_NUMBER - 1; ++i) {
		sum += combat_state.sacrifice_orbs_state.orb_right_amount[i];
	}

	assert(sum < combat_state.total_null_orbs_in_temp_use);
	if (sum == combat_state.total_null_orbs_in_temp_use - 1) {
		for (s32 i = 0; i < ORB_NUMBER - 1; ++i) {
			gw.sacrifice_orb_ui->arrows_right[i]->setActive(false, false);
		}
	}

	// Remove from left
	combat_state.sacrifice_orbs_state.orb_left_amount[id] -= 1;
	combat_state.sacrifice_orbs_state.orb_right_amount[id] += 1;
	layout_set_sacrifice_modal_quantity(true, id, combat_state.sacrifice_orbs_state.orb_left_amount[id]);
	layout_set_sacrifice_modal_quantity(false, id, combat_state.sacrifice_orbs_state.orb_right_amount[id]);

	if (combat_state.sacrifice_orbs_state.orb_left_amount[id] == 0) {
		gw.sacrifice_orb_ui->arrows_right[id]->setActive(false, false);
	}

	// Update left arrow to be enabled
	gw.sacrifice_orb_ui->arrows_left[id]->setActive(true);

	// Update Label number
	gw.sacrifice_orb_ui->orb_number_label->getText()[0] = combat_state.total_null_orbs_in_temp_use - sum - 1 + 0x30;
}
static void button_callback_sacrifice_orb_cancel(void* arg) {
	gw.sacrifice_orb_ui->window->setActive(false);
	// Put the skills used in place
	for (s32 i = 0; i < NUM_ALLIES; ++i) {
		gw.sacrifice_orb_ui->skills_miniatures->getButtons()[i]->setAllBGTexture(gw.sacrifice_orb_ui->empty_skill_texture);
	}
}
static void button_callback_sacrifice_orb_endturn(void* arg) {
	
}
static void sacrifice_orbs_start() {
	gw.sacrifice_orb_ui->window->setActive(true);
	for (s32 i = 0; i < ORB_NUMBER - 1; ++i) {
		gw.sacrifice_orb_ui->arrows_left[i]->setActive(false, false);
		layout_set_sacrifice_modal_quantity(true, (Orb_ID)i, combat_state.orbs_amount[i]);
		layout_set_sacrifice_modal_quantity(false, (Orb_ID)i, 0);
		combat_state.sacrifice_orbs_state.orb_left_amount[i] = combat_state.orbs_amount[i];
		combat_state.sacrifice_orbs_state.orb_right_amount[i] = 0;
		if(combat_state.orbs_amount[i] > 0)
			gw.sacrifice_orb_ui->arrows_right[i]->setActive(true);
	}
	assert(combat_state.total_null_orbs_in_temp_use < 10);	// 10 not supported yet

	gw.sacrifice_orb_ui->orb_number_label->getText()[0] = combat_state.total_null_orbs_in_temp_use + 0x30;

	// Put the skills used in place
	for (s32 i = 0; i < NUM_ALLIES; ++i) {
		Skill_ID used = combat_state.player.skill_in_use[i];
		if(used != SKILL_NONE)
			gw.sacrifice_orb_ui->skills_miniatures->getButtons()[i]->setAllBGTexture(skill_textures[used]);
	}
	combat_state.sacrifice_orbs_state.active = true;
}


#define ARCSIN_1 1.57079633f

struct Enemy_Target_Animation {
	bool is_animating;
	r32 opacity_animation;

	Enemy_Target_Animation() {
		is_animating = false;
		opacity_animation = ARCSIN_1;
	}
};
static Enemy_Target_Animation enemy_target_animation[NUM_ENEMIES];
struct Ally_Target_Animation {
	bool is_animating;
	r32 opacity_animation;
	Ally_Target_Animation() {
		is_animating = false;
		opacity_animation = ARCSIN_1;
	}
};
static Enemy_Target_Animation allies_target_animation[NUM_ALLIES];

static void reset_targets_animation() {
	for (int i = 0; i < NUM_ENEMIES; ++i) {
		if (combat_state.enemy.hp[i] <= 0)
			continue;
		enemy_target_animation[i].opacity_animation = ARCSIN_1;
		enemy_target_animation[i].is_animating = false;
		layout_set_enemy_image_opacity(i, 1.0f, hm::vec4(0, 1, 1, 1));
	}
	for (int i = 0; i < NUM_ALLIES; ++i) {
		if (combat_state.player.hp[i] <= 0)
			continue;
		allies_target_animation[i].opacity_animation = ARCSIN_1;
		allies_target_animation[i].is_animating = false;
		layout_set_ally_image_opacity(i, 1.0f, hm::vec4(0, 1, 1, 1));
	}
}

static void reset_targets() {
	combat_state.player.targeting = false;
	for (int i = 0; i < NUM_ENEMIES; ++i) {
		for (int k = 0; k < MAX(NUM_ALLIES, NUM_ENEMIES); ++k) {
			gw.enemy_target[i][k]->setActive(false);
		}
	}
	for (int i = 0; i < NUM_ALLIES; ++i) {
		for (int k = 0; k < NUM_ALLIES; ++k) {
			gw.ally_target[i][k]->setActive(false);
		}
	}
}

static void reset_selections() {
	for (int i = 0; i < NUM_ALLIES; ++i) {
		for (int j = 0; j < NUM_SKILLS; ++j) {
			linked::Button* b = gw.allies_skills[i * NUM_SKILLS + j]->divs[0]->getButtons()[0];
			b->setActive(true);
			b->setOpacity(1.0f);
			if (b->getIsToggled())
				b->toggle();
		}
	}
}

static void combat_state_reset_target(s32 char_id, Skill_ID skill_used) {
	combat_state.player.targeting = false;
	
	for (int k = 0; k < NUM_ALLIES; ++k) {
		if (combat_state.player.targets[char_id].ally_target_index[k] != -1) {
			combat_state.player.targets[char_id].ally_target_image[k]->setActive(false);
			combat_state.player.targets[char_id].ally_target_index[k] = -1;
		}
		combat_state.player.targets[char_id].skill_used = SKILL_NONE;
		combat_state.player.targets[char_id].attacking_character = CHAR_NONE;
	}
	for (int k = 0; k < NUM_ENEMIES; ++k) {
		if (combat_state.player.targets[char_id].enemy_target_index[k] != -1) {
			combat_state.player.targets[char_id].enemy_target_image[k]->setActive(false);
			combat_state.player.targets[char_id].ally_target_index[k] = -1;
		}
	}
}

static void combat_state_reset_all_targets() {
	for (int i = 0; i < NUM_ALLIES; ++i) {
		for (int k = 0; k < NUM_ENEMIES; ++k) {
			combat_state.player.targets[i].enemy_target_index[k] = -1;
		}
		for (int k = 0; k < NUM_ALLIES; ++k) {
			combat_state.player.targets[i].ally_target_index[k] = -1;
		}
		combat_state.player.targets[i].attacking_character = CHAR_NONE;
		combat_state.player.targets[i].skill_used = SKILL_NONE;
	}
}

static bool has_enough_orbs(Skill_ID skill_used) {
	s32 total_orbs = combat_state.total_orbs;
	for (int i = 0; i < ORB_NUMBER - 1; ++i) {
		total_orbs -= skill_costs[skill_used][i];
		if (skill_costs[skill_used][i] > combat_state.orbs_amount[i]) {
			return false;
		}
	}
	s32 null_orb_cost = skill_costs[skill_used][ORB_NULL];
	if (null_orb_cost > 0) {
		if (total_orbs - combat_state.total_null_orbs_in_temp_use < null_orb_cost) {
			printf("Not enough orbs to sacrifice\n");
			return false;
		}
	}
	return true;
}

static void temporary_modify_orbs(Skill_ID skill_used, s32 add) {

	for (int i = 0; i < ORB_NUMBER - 1; ++i) {
		s32 cost = skill_costs[skill_used][i];
		combat_state.orbs_amount[i] += add * cost;
		combat_state.total_orbs += add * cost;
		layout_change_orb_amount((Orb_ID)i, combat_state.orbs_amount[i]);

		layout_change_orb_amount(ORB_ALL, combat_state.total_orbs);
	}
}

static void target_enemy(void* arg);
static void target_ally(void* arg);

static void button_skill(void* arg) {
	if (!combat_state.player_turn) {
		AudioController::cancelAudio.play();
		return;
	}

	linked::Button_Info* eba = (linked::Button_Info*)arg;
	int char_id = (int)eba->data;
	int skill_id = (int)eba->id;
	Skill_ID skill_used = (Skill_ID)(NUM_SKILLS * combat_state.player.char_id[char_id] + skill_id);

	if (skill_used == SKILL_INHERITANCE) {
		if(skill_state_ally.inheritance_copy != SKILL_NONE)
			skill_used = skill_state_ally.inheritance_copy; // @temp test
	}

	if (combat_state.player.hp <= 0)
		return;

	bool is_toggled = eba->this_button->getIsToggled();

	if (!is_toggled && eba->this_button->getActive() && !combat_state.player.targeting) {
		if (!has_enough_orbs(skill_used)) {
			return;
		} else {
			temporary_modify_orbs(skill_used, -1);
			combat_state.total_null_orbs_in_temp_use += skill_costs[skill_used][ORB_NULL];
			combat_state.player.skill_in_use[char_id] = skill_used;
		}
	}

	if (is_toggled) {
		// untoggle?
		printf("untoggle\n");
		temporary_modify_orbs(skill_used, 1);
		combat_state.total_null_orbs_in_temp_use -= skill_costs[skill_used][ORB_NULL];
		combat_state.player.skill_in_use[char_id] = SKILL_NONE;
		combat_state_reset_target(char_id, skill_used);
		reset_targets_animation();
	} else {
		if (combat_state.player.targeting) {
			AudioController::cancelAudio.play();
			return;
		}
		printf("toggle\n");
	}

	// activate button
	if (!eba->this_button->getActive()) {
		for (int i = 0; i < NUM_SKILLS; ++i) {
			linked::Button* b = gw.allies_skills[char_id * NUM_SKILLS + i]->divs[0]->getButtons()[0];
			b->setActive(true);
			b->setOpacity(1.0f);
			if (b->getIsToggled())
				b->toggle();
		}
	}
	for (int i = 0; i < NUM_SKILLS; ++i) {
		if (i == skill_id)
			continue;
		linked::Button* b = gw.allies_skills[char_id * NUM_SKILLS + i]->divs[0]->getButtons()[0];
		if (is_toggled) {
			b->setActive(true);
			b->setOpacity(1.0f);
		} else {
			b->setActive(false);
			b->setOpacity(0.2f);
		}
	}
	eba->this_button->toggle();
	AudioController::confirmAudio.play();

	Skill_Target target = skill_need_targeting(skill_used, &combat_state);
	if (target.number > 0 && !is_toggled) {

		combat_state.player.targeting = true;
		combat_state.player.targeting_info.skill_used = skill_used;
		combat_state.player.targeting_info.attacking_character = (Character_ID)char_id;

		printf("Need %d", target.number);

		// Special case, select instantly
		if (skill_used == SKILL_BEST_BOUND_FIST) {
			// find the enemy with least hp and target it
			// linked::Button_Info info;
			// info.id = enemy_index;
			// info.this_button = gw.enemies[enemy_index]->divs[0]->getButtons()[0];
			// target_enemy(&info);
		}

		if (target.enemy) {
			for (int i = 0; i < NUM_ENEMIES; ++i) {
				if (!is_enemy_targetable_by_skill(skill_used, i, &combat_state)) continue;
				enemy_target_animation[i].is_animating = true;
			}
			printf(" enemy");
		} 
		if (target.ally) {
			for (int i = 0; i < NUM_ALLIES; ++i) {
				if (!is_ally_targetable_by_skill(skill_used, i, &combat_state)) continue;
				allies_target_animation[i].is_animating = true;
			}
			printf(" ally");
		}
		printf(" target.\n");
	} else if (target.number == 0 && target.all && !is_toggled) {
		if (target.enemy) {
			for (int i = 0; i < NUM_ENEMIES; ++i) {
				if (!is_enemy_targetable_by_skill(skill_used, i, &combat_state))
					continue;
				combat_state.player.targeting = true;
				combat_state.player.targeting_info.attacking_character = (Character_ID)char_id;
				combat_state.player.targeting_info.skill_used = skill_used;
				linked::Button_Info info;
				info.id = i;
				info.this_button = gw.enemies[i]->divs[0]->getButtons()[0];
				target_enemy(&info);
			}
		} 
		if (target.ally) {
			for (int i = 0; i < NUM_ALLIES; ++i) {
				if (!is_ally_targetable_by_skill(skill_used, i, &combat_state))
					continue;
				combat_state.player.targeting = true;
				combat_state.player.targeting_info.attacking_character = (Character_ID)char_id;
				combat_state.player.targeting_info.skill_used = skill_used;
				linked::Button_Info info;
				info.id = i;
				info.this_button = gw.allies[i]->divs[0]->getButtons()[0];
				target_ally(&info);
			}
		}
	} else if (target.number == 0 && target.self && !is_toggled) {
		combat_state.player.targeting = true;
		combat_state.player.targeting_info.attacking_character = (Character_ID)char_id;
		combat_state.player.targeting_info.skill_used = skill_used;
		linked::Button_Info info;
		info.id = char_id;
		info.this_button = gw.allies[char_id]->divs[0]->getButtons()[0];
		target_ally(&info);
	}
}

static void target_enemy(void* arg) {
	linked::Button_Info* eba = (linked::Button_Info*)arg;

	if(!is_enemy_targetable_by_skill(combat_state.player.targeting_info.skill_used, eba->id, &combat_state))
		return;

	if (!combat_state.player.targeting)
		return;

	for (int i = 0; i < NUM_ALLIES; ++i) {
		if (gw.enemy_target[eba->id][i]->getActive() == true)
			continue;
		gw.enemy_target[eba->id][i]->setActive(true);
		gw.enemy_target[eba->id][i]->divs[0]->setBackgroundTexture(skill_textures[combat_state.player.targeting_info.skill_used]);
		s32 attacker_index = combat_state.player.targeting_info.attacking_character;
		combat_state.player.targets[attacker_index].enemy_target_image[eba->id] = gw.enemy_target[eba->id][i];
		combat_state.player.targets[attacker_index].enemy_target_index[eba->id] = attacker_index;

		combat_state.player.targets[attacker_index].skill_used = combat_state.player.targeting_info.skill_used;
		combat_state.player.targets[attacker_index].attacking_character = combat_state.player.targeting_info.attacking_character;
		break;
	}

	combat_state.player.targeting = false;

	reset_targets_animation();
}

static void target_ally(void* arg) {
	linked::Button_Info* eba = (linked::Button_Info*)arg;

	if (!is_ally_targetable_by_skill(combat_state.player.targeting_info.skill_used, eba->id, &combat_state))
		return;

	if (!combat_state.player.targeting)
		return;

	for (int i = 0; i < NUM_ALLIES; ++i) {
		if (gw.ally_target[eba->id][i]->getActive() == true)
			continue;
		gw.ally_target[eba->id][i]->setActive(true);
		gw.ally_target[eba->id][i]->divs[0]->setBackgroundTexture(skill_textures[combat_state.player.targeting_info.skill_used]);
		s32 attacker_index = combat_state.player.targeting_info.attacking_character;
		combat_state.player.targets[attacker_index].ally_target_image[eba->id] = gw.ally_target[eba->id][i];
		combat_state.player.targets[attacker_index].ally_target_index[eba->id] = attacker_index;

		combat_state.player.targets[attacker_index].skill_used = combat_state.player.targeting_info.skill_used;
		combat_state.player.targets[attacker_index].attacking_character = combat_state.player.targeting_info.attacking_character;
		break;
	}

	combat_state.player.targeting = false;

	reset_targets_animation();
}

static void button_play_game(void* arg) {
	change_game_mode(MODE_CHAR_SELECT);
}

#include "ui.cpp"

void init_combat_state() {
	#if MULTIPLAYER
		player = client_searching();
		connection = connect(player);
		exchange_char_selection(connection, player, &char_sel_state);
		
		combat_state.player_turn = player->first;
		turn_time = TURN_DURATION;
	#else 
		combat_state.player_turn = true;
	#endif

	layout_update_endturn_button();
		
	for (int i = 0; i < NUM_ENEMIES; ++i) {
		// @todo enemy selection?
		int index = char_sel_state.enemy_selections[i];
		combat_state.enemy.char_id[i] = (Character_ID)index;
		combat_state.enemy.max_hp[i] = 100;
		combat_state.enemy.hp[i] = 100;
	}
	for (int i = 0; i < NUM_ALLIES; ++i) {
		int index = char_sel_state.selections[i];
		combat_state.player.skill_in_use[i] = SKILL_NONE;
		combat_state.player.char_id[i] = (Character_ID)index;
		combat_state.player.max_hp[i] = 100;
		combat_state.player.hp[i] = 100;
		combat_state.player.targeting = false;
		combat_state.player.targets[i].skill_used = SKILL_NONE;
		for(int k = 0; k < NUM_ALLIES; ++k)
			combat_state.player.targets[i].ally_target_index[k] = -1;
		for (int k = 0; k < NUM_ALLIES; ++k)
			combat_state.player.targets[i].enemy_target_index[k] = -1;
	}
	combat_state.player.targeting_info.skill_used = SKILL_NONE;
	for (int k = 0; k < NUM_ENEMIES; ++k)
		combat_state.player.targeting_info.enemy_target_index[k] = -1;
	for (int k = 0; k < NUM_ALLIES; ++k) {
		combat_state.player.targeting_info.ally_target_index[k] = -1;
		combat_state.player.targets[k].skill_used = SKILL_NONE;
		combat_state.player.targets[k].attacking_character = CHAR_NONE;
	}
}

void init_application()
{
	using namespace linked;

	// Initialize game mode
	ggs.mode = MODE_NONE;
	ggs.last_mode = MODE_NONE;

	AudioController::introAudio.setVolume(10);
	AudioController::charselectAudio.setVolume(5);
	AudioController::confirmAudio.setVolume(20);
	AudioController::cancelAudio.setVolume(20);
	AudioController::navigationAudio.setVolume(30);
	AudioController::combat1Audio.setVolume(10);

	// background @temporary
	linked::Window* bgwindow = new linked::Window(window_info.width, window_info.height, hm::vec2(0, 0), hm::vec4(0, 0, 0, 0.5f), 0, 0, W_UNFOCUSABLE);
	linked::WindowDiv* bgdiv = new linked::WindowDiv(*bgwindow, window_info.width, window_info.height, 0, 0, hm::vec2(0, 0), hm::vec4(0, 0, 0, 1), DIV_ANCHOR_LEFT | DIV_ANCHOR_TOP);
	bgwindow->divs.push_back(bgdiv);
	gw.bg_logo = new Texture("../../../res/textures/bg.png");
	gw.bg_normal = new Texture("../../../res/textures/bg2.jpg");
	bgdiv->setBackgroundTexture(gw.bg_logo);
	gw.bgwindow = bgwindow;

	char_sel_state.enemy_selections[0] = CHAR_BIG_O;
	char_sel_state.enemy_selections[1] = CHAR_ONE;
	char_sel_state.enemy_selections[2] = CHAR_ZERO;

	init_intro_mode();
	init_char_selection_mode();
	init_char_information_mode();
	init_combat_mode();

#if 1
	combat_state.orbs_amount[ORB_HARD] = 2;
	combat_state.orbs_amount[ORB_SOFT] = 3;
	combat_state.orbs_amount[ORB_VR] = 4;
	combat_state.orbs_amount[ORB_BIOS] = 5;
	combat_state.total_orbs = 5 + 2 + 3 + 4;
	layout_change_orb_amount(ORB_HARD, 2);
	layout_change_orb_amount(ORB_SOFT, 3);
	layout_change_orb_amount(ORB_VR, 4);
	layout_change_orb_amount(ORB_BIOS, 5);
	layout_change_orb_amount(ORB_ALL, 5 + 2 + 3 + 4);
#else
	combat_state.orbs_amount[ORB_HARD] = 1;
	combat_state.orbs_amount[ORB_SOFT] = 0;
	combat_state.orbs_amount[ORB_VR] = 0;
	combat_state.orbs_amount[ORB_BIOS] = 0;
	combat_state.total_orbs = 1;
	layout_change_orb_amount(ORB_HARD, 1);
	layout_change_orb_amount(ORB_SOFT, 0);
	layout_change_orb_amount(ORB_VR, 0);
	layout_change_orb_amount(ORB_BIOS, 0);
	layout_change_orb_amount(ORB_ALL, 1);
#endif

	// init console chat
	chat_window = chat.init_chat();
	chat_window->setActive(false);
	chat.m_active = false;
	g_chat = &chat;

	hide_all_windows();
	change_game_mode(MODE_INTRO);

	// opengl
	glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	AudioController::introAudio.play();
}

static void apply_skills_and_send() {
	for (int i = 0; i < NUM_ALLIES; ++i) {
		#if MULTIPLAYER
			if (combat_state.player_turn) {
					send_struct(connection, combat_state.player.targets[i]);
			}
		#endif

		if (combat_state.player.targets[i].skill_used == SKILL_NONE)
			continue;
		
		s32 target = -1;
		bool target_enemy = true;
		for (int k = 0; k < MAX(NUM_ALLIES, NUM_ENEMIES); ++k) {
			s32 t = combat_state.player.targets[i].enemy_target_index[k];
			if (t != -1)
				target = k;
		}
		if (target == -1) {
			for (int k = 0; k < NUM_ALLIES; ++k) {
				s32 t = combat_state.player.targets[i].ally_target_index[k];
				if (t != -1) {
					target = k;
					target_enemy = false;
				}
			}
		}

		#if MULTIPLAYER
			if (combat_state.player_turn)
				execute_skill(combat_state.player.targets[i].skill_used, target, combat_state.player.targets[i].attacking_character, &combat_state, false, true);
			else
				execute_skill(combat_state.player.targets[i].skill_used, target, combat_state.player.targets[i].attacking_character, &combat_state, true, false);
		#else
			execute_skill(combat_state.player.targets[i].skill_used, target, combat_state.player.targets[i].attacking_character, &combat_state, false, target_enemy);
		#endif
	}
	combat_state_reset_all_targets();
}

static s32 get_num_players_alive() {
	s32 count = 0;
	for (int i = 0; i < NUM_ALLIES; ++i) {
		if (combat_state.player.hp[i] > 0)
			count++;
	}
	return count;
}

static void add_orb(Orb_ID orb_type, s32 count) {
	combat_state.orbs_amount[orb_type] += 1;

	combat_state.total_orbs += 1;
	layout_change_orb_amount(ORB_ALL, combat_state.total_orbs);
	layout_change_orb_amount(orb_type, combat_state.orbs_amount[orb_type]);
}

// Gameplay functions
void end_turn() {
	if (combat_state.total_null_orbs_in_temp_use > 0) {
		sacrifice_orbs_start();
		return;
	}

	// apply skills
	apply_skills_and_send();
	printf("\nEND TURN\n");
	// reset targets after
	turn_time = TURN_DURATION;

	reset_selections();
	reset_targets();
	reset_targets_animation();

	// generate orbs if is player turn
	combat_state.player_turn = !combat_state.player_turn;
	if (combat_state.player_turn) {
		update_skill_state_end_enemy_turn(&combat_state);
		update_status_end_enemy_turn(&combat_state);
		s32 num_alive = get_num_players_alive();
		printf("Generated Orbs: ");
		for (int i = 0; i < num_alive; ++i) {
			int orb_generated = rand() % (ORB_NUMBER - 1);
			add_orb((Orb_ID)orb_generated, 1);
			printf("%d ", orb_generated);
		}
		printf("\n");
	} else {
		update_skill_state_end_turn(&combat_state);
		update_status_end_turn(&combat_state);
	}
	
	printf("SWITCH TURN\n");
	layout_update_endturn_button();
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
				if (gw.char_selection_window->divs[i]->isButtonHovered()) {
					Texture* char_tex = (Texture*)gw.char_selection_window->divs[i]->getButtons()[0]->getNormalBGTexture();
					gw.left_char_window->divs[0]->setBackgroundTexture(chars_texture_big[i]);
					linked::Label* name_label = gw.left_char_window->divs[1]->getLabels()[0];
					name_label->setText((u8*)char_names[i], char_names_length[i]);
					name_label->setPosition(hm::vec2(gw.left_char_window->divs[0]->getWidth() - name_label->getTextWidth() - 10.0f, 5.0f));
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
				gw.char_info_window->divs[i]->getButtons()[0]->setAllBGTexture(skill_textures[index]);
				if (gw.char_info_window->divs[i]->isButtonHovered()) {
					gw.skill_group_div->m_render = true;
					gw.char_info_skill_cost->setActive(true);
					skill_label->setText((u8*)skill_names[index], skill_names_length[index]);
					skill_desc_label->setText((u8*)skill_desc[index], skill_desc_length[index]);

					// set skill group desc
					layout_set_skill_group_from_skill(index, gw.skill_group_div->getLabels()[0]);
					layout_set_cooldown_from_skill(index, gw.skill_group_div->getLabels()[1]);

					for (int n = 0, orb_index = 0; n < ORB_NUMBER; ++n) {
						gw.char_info_skill_cost->divs[n]->m_render = false;
					}
					int n_ = 0;
					for (int n = 0, orb_index = 0; n < ORB_NUMBER; ++n) {
						int cost = skill_costs[index][n];
						while (cost > 0) {
							//gw.char_info_skill_cost
							gw.char_info_skill_cost->divs[orb_index]->m_render = true;
							gw.char_info_skill_cost->divs[orb_index++]->setBackgroundTexture(orb_textures[n]);
							cost--;
							n_++;
						}
					}
					hm::vec2 pos_ = gw.char_info_skill_cost->getPosition();
					pos_.x = 630.0f - n_ * (32.0f + 5.0f);
					gw.char_info_skill_cost->setPosition(pos_);
					break;
				}
			}
			for (int i = 0; i < NUM_ALLIES; ++i) {
				if (i < char_sel_state.num_selected) {
					gw.char_info_window_bot->divs[i]->m_render = true;
					gw.char_info_window_bot->divs[i]->setBackgroundTexture(char_textures[char_sel_state.selections[i]]);
				} else
					gw.char_info_window_bot->divs[i]->m_render = false;
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
			layout_set_timer_percentage((r32)(turn_time / TURN_DURATION));

			// enemy targeting
			for (int i = 0; i < NUM_ENEMIES; ++i) {
				const r32 animation_speed = 0.05f;
				if (enemy_target_animation[i].is_animating) {
					r32 value = (sinf(enemy_target_animation[i].opacity_animation) + 1.0f) / 4.0f + 0.5f;

					layout_set_enemy_image_opacity(i, value, cyan);
					enemy_target_animation[i].opacity_animation += animation_speed;
				}
			}
			// ally targeting
			for (int i = 0; i < NUM_ALLIES; ++i) {
				const r32 animation_speed = 0.05f;
				if (allies_target_animation[i].is_animating) {
					r32 value = (sinf(allies_target_animation[i].opacity_animation) + 1.0f) / 4.0f + 0.5f;

					layout_set_ally_image_opacity(i, value, cyan);
					allies_target_animation[i].opacity_animation += animation_speed;
				}
			}


			bool is_hovering_skill = false;
			bool is_hovering_char = false;
			for (int i = 0; i < NUM_ALLIES; ++i) {
				if (gw.allies[i]->divs[0]->getButtons()[0]->isHovered()) {
					//printf("Ally %d hovered!\n", i);
					int char_index = char_sel_state.selections[i];
					combat_state.skill_info_image->setBackgroundTexture(char_textures[char_index]);
					combat_state.skill_info_image->setBorderColor(hm::vec4(greener_cyan));
					combat_state.skill_info_title->getLabels()[0]->setText((u8*)char_names[char_index], char_names_length[char_index]);
					combat_state.skill_info_desc->getLabels()[0]->setText((u8*)char_descriptions[char_index], char_descriptions_length[char_index]);
					is_hovering_char = true;
					break;
				} else {
					for (int k = 0; k < NUM_SKILLS; ++k) {
						if (gw.allies_skills[i * NUM_SKILLS + k]->divs[0]->getButtons()[0]->isHovered()) {
							//printf("skill %d of ally %d hovered\n", k, i);
							int skill_index = char_sel_state.selections[i] * NUM_SKILLS + k;
							combat_state.skill_info_image->setBackgroundTexture(skill_textures[skill_index]);
							combat_state.skill_info_image->setBorderColor(greener_cyan);
							combat_state.skill_info_title->getLabels()[0]->setText((u8*)skill_names[skill_index], skill_names_length[skill_index]);
							combat_state.skill_info_desc->getLabels()[0]->setText((u8*)skill_desc[skill_index], skill_desc_length[skill_index]);
							layout_set_skill_group_from_skill(skill_index, combat_state.skill_info_group->getLabels()[0]);
							layout_set_cooldown_from_skill(skill_index, combat_state.skill_info_group->getLabels()[1]);
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
			}

			for (int i = 0; i < NUM_ENEMIES; ++i) {
				if (gw.enemies[i]->divs[0]->getButtons()[0]->isHovered()) {
					//printf("Enemy %d hovered!\n", i);
					int char_index = char_sel_state.enemy_selections[i];
					combat_state.skill_info_image->setBackgroundTexture(char_textures[char_index]);
					combat_state.skill_info_image->setBorderColor(hm::vec4(enem_hp_bar_full_color));
					combat_state.skill_info_title->getLabels()[0]->setText((u8*)char_names[char_index], char_names_length[char_index]);
					combat_state.skill_info_desc->getLabels()[0]->setText((u8*)char_descriptions[char_index], char_descriptions_length[char_index]);
					is_hovering_char = true;
					break;
				}
			}
			if (!is_hovering_skill) {
				for (int i = 0; i < ORB_NUMBER; ++i) {
					combat_state.skill_costs[i]->m_render = false;
				}
			}
			combat_state.skill_info_image->m_render = is_hovering_skill | is_hovering_char;
			combat_state.skill_info_title->m_render = is_hovering_skill | is_hovering_char;
			combat_state.skill_info_desc->m_render = is_hovering_skill | is_hovering_char;
			combat_state.skill_info_group->m_render = is_hovering_skill;

#if MULTIPLAYER
			if (combat_state.player_turn == 0 && receive_struct(connection, combat_state.player.targets))
					end_turn();			
#endif
		}break;
	}
}

void change_game_mode(Game_Mode mode)
{
	if (ggs.mode == mode) return;
	
	hide_all_windows();

	ggs.last_mode = ggs.mode;
	ggs.mode = mode;

	switch (mode) {
		case MODE_INTRO: {
			AudioController::pauseAllMusic();
			AudioController::introAudio.rewind();
			AudioController::introAudio.play();
			gw.intro_logo->setActive(true);
			gw.bgwindow->divs[0]->setBackgroundTexture(gw.bg_logo);
		}break;
		case MODE_CHAR_SELECT: {
			if (ggs.last_mode != MODE_CHAR_INFO) {
				AudioController::pauseAllMusic();
				AudioController::charselectAudio.play();
			}
			gw.char_selected_window->setActive(true);
			gw.left_char_window->setActive(true);
			gw.char_selection_window->setActive(true);
			gw.bgwindow->divs[0]->setBackgroundTexture(gw.bg_normal);
		}break;
		case MODE_CHAR_INFO: {
			if (ggs.last_mode != MODE_CHAR_SELECT) {
				AudioController::pauseAllMusic();
				AudioController::charselectAudio.play();
			}
			gw.left_char_window->setActive(true);
			gw.char_info_window->setActive(true);
			gw.char_info_window_bot->setActive(true);
			gw.bgwindow->divs[0]->setBackgroundTexture(gw.bg_normal);
		}break;
		case MODE_COMBAT: {
			AudioController::pauseAllMusic();
			AudioController::combat1Audio.rewind();
			AudioController::combat1Audio.play();
			gw.bgwindow->divs[0]->setBackgroundTexture(gw.bg_normal);
			init_combat_state();
			for (int i = 0; i < NUM_ALLIES; ++i) {
				int index = char_sel_state.selections[i];
				Texture* t = char_textures[index];
				gw.allies[i]->divs[0]->setBackgroundTexture(t);

				gw.allies[i]->setActive(true);
				gw.allies_info[i]->setActive(true);
				for (int k = 0; k < NUM_SKILLS; ++k) {
					gw.allies_skills[i * NUM_SKILLS + k]->setActive(true);
					linked::Button* b = gw.allies_skills[i * NUM_SKILLS + k]->divs[0]->getButtons()[0];
					Texture* skill = skill_textures[index * NUM_SKILLS + k];
					gw.allies_skills[i * NUM_SKILLS + k]->divs[0]->getButtons()[0]->setAllBGTexture(skill);
				}
			}
			for (int i = 0; i < NUM_ENEMIES; ++i) {
				int index = char_sel_state.enemy_selections[i];
				Texture* t = char_textures[index];

				gw.enemies[i]->divs[0]->setBackgroundTexture(t);
				gw.enemies[i]->setActive(true);
				gw.enemies_info[i]->setActive(true);
			}
			gw.combat_bottom_info->setActive(true);
			gw.timer_window->setActive(true);
			gw.player_name_window->setActive(true);
		}break;
	}
}

void update_and_render(double frametime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (chat.m_active)
		chat.update();

	update_game_mode(frametime);

	input();
}

s32 execute_skill(Skill_ID id, int target_index, int source_index, Combat_State* combat_state, bool from_enemy = false, bool on_enemy = true);

void input()
{
	if (keyboard_state.key_event[KEY_F1]) {
		keyboard_state.key_event[KEY_F1] = false;
		chat_window->setActive(!chat_window->getActive());
		g_chat->m_active = !g_chat->m_active;
	}
	if (keyboard_state.key_event[KEY_ESCAPE]) {
		keyboard_state.key_event[KEY_ESCAPE] = false;
		change_game_mode(ggs.last_mode);
	}

	if (keyboard_state.key_event[KEY_SPACE]) {
		keyboard_state.key_event[KEY_SPACE] = false;
		
		if (ggs.mode == MODE_CHAR_SELECT && char_sel_state.last_hovered != CHAR_NONE) {
			printf("last hovered %d\n", char_sel_state.last_hovered);
			change_game_mode(MODE_CHAR_INFO);
		}
	}
	if (keyboard_state.key_event[VK_F2]) {
		keyboard_state.key_event[VK_F2] = false;
		end_turn();
	}
#if 0
	if (keyboard_state.key_event['P']) {
		keyboard_state.key_event['P'] = false;
		if (keyboard_state.key[VK_SHIFT]) {
			remove_status_from_ally(0, SKILL_CONDITION_PARALYZE, &combat_state);
		} else {
			//apply_status_to_enemy(0, SKILL_CONDITION_PARALYZE, 1, &combat_state);
			apply_status_to_ally(0, SKILL_CONDITION_PARALYZE, 1, &combat_state);
		}
	}
	if (keyboard_state.key_event['B']) {
		keyboard_state.key_event['B'] = false;
		if (keyboard_state.key[VK_SHIFT]) {
			remove_status_from_enemy(0, SKILL_CONDITION_BURN, &combat_state);
		} else {
			apply_status_to_enemy(0, SKILL_CONDITION_BURN, 1, &combat_state);
		}
	}
	if (keyboard_state.key_event['F']) {
		keyboard_state.key_event['F'] = false;
		if (keyboard_state.key[VK_SHIFT]) {
			remove_status_from_enemy(0, SKILL_CONDITION_FREEZE, &combat_state);
		} else {
			apply_status_to_enemy(0, SKILL_CONDITION_FREEZE, 1, &combat_state);
		}
	}
	if (keyboard_state.key_event['I']) {
		keyboard_state.key_event['I'] = false;
		if (keyboard_state.key[VK_SHIFT]) {
			remove_status_from_enemy(0, SKILL_CONDITION_POISON, &combat_state);
		} else {
			apply_status_to_enemy(0, SKILL_CONDITION_POISON, 1, &combat_state);
		}
	}
	if (keyboard_state.key_event['S']) {
		keyboard_state.key_event['S'] = false;
		if (keyboard_state.key[VK_SHIFT]) {
			remove_status_from_enemy(0, SKILL_CONDITION_SLEEP, &combat_state);
		} else {
			apply_status_to_enemy(0, SKILL_CONDITION_SLEEP, 1, &combat_state);
		}
	}
	if (keyboard_state.key_event['T']) {
		keyboard_state.key_event['T'] = false;
		if (keyboard_state.key[VK_SHIFT]) {
			remove_status_from_enemy(0, SKILL_CONDITION_STUN, &combat_state);
		} else {
			apply_status_to_enemy(0, SKILL_CONDITION_STUN, 1, &combat_state);
		}
	}
#endif
}

// Layout functions
static void layout_toggle_char_selection(int id, std::vector<linked::WindowDiv*>* divs, bool selected)
{
	// update the selection screen
	hm::vec4 selected_color;
	hm::vec4 selected_bg_color;
	if (selected) {
		selected_bg_color = char_selected_bg_color;
		selected_color = char_window_hover_color;
		(*divs)[id]->setBorderColor(cyan);
	} else {
		selected_bg_color = char_window_color;
		selected_color = hm::vec4(0, 0, 0, 1);
		(*divs)[id]->setBorderColor(hm::vec4(0,0,0,0));
	}

	(*divs)[id]->getButtons()[0]->setNormalBGColor(selected_color);
	(*divs)[id]->setBackgroundColor(selected_bg_color);
}

static void layout_enemy_die(u32 enemy_index) {
	assert(enemy_index <= NUM_ENEMIES);
	gw.enemies_indicator[enemy_index]->setBackgroundTexture(orb_dead_enemy);
	layout_set_enemy_image_opacity(enemy_index, 0.5f, hm::vec4(0,0,0,1));
}

static void layout_ally_die(u32 ally_index) {
	assert(ally_index <= NUM_ALLIES);
	gw.allies_indicator[ally_index]->setBackgroundTexture(orb_dead_ally);
	layout_set_ally_image_opacity(ally_index, 0.5f, hm::vec4(0, 0, 0, 1));
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

	label->setText((u8*)buffer, length + 1);
}

static void layout_set_cooldown_from_skill(int skill_index, linked::Label* label) {
	static char buffer[] = "COOLDOWN -\0\0\0\0\0";
	int length = sizeof("COOLDOWN -");

	int count =	s32_to_str_base10(skill_cooldowns[skill_index], buffer + length - 2);
	length += count - 1;

	if (skill_cooldowns[skill_index] == 0)
		buffer[9] = '-';

	label->setText((u8*)buffer, length);
}

static void layout_change_orb_amount(Orb_ID id, int amt) {
	static char all_orbs_buffer[16] = { 0 };
	static char hard_orb_buffer[16] = { 0 };
	static char soft_orb_buffer[16] = { 0 };
	static char vr_orb_buffer[16] = { 0 };
	static char bios_orb_buffer[16] = { 0 };

	char* selected = 0;
	linked::Label* label = 0;

	switch (id) {
	case ORB_HARD: selected = (char*)&hard_orb_buffer; label = combat_state.orb_labels[ORB_HARD]; break;
	case ORB_SOFT: selected = (char*)&soft_orb_buffer; label = combat_state.orb_labels[ORB_SOFT]; break;
	case ORB_VR:   selected = (char*)&vr_orb_buffer;   label = combat_state.orb_labels[ORB_VR]; break;
	case ORB_BIOS: selected = (char*)&bios_orb_buffer; label = combat_state.orb_labels[ORB_BIOS]; break;
	case ORB_ALL:  selected = (char*)&all_orbs_buffer; label = combat_state.all_orbs_label; break;
	}

	memset(selected, 0, sizeof(all_orbs_buffer));
	int count = s32_to_str_base10(amt, selected);

	label->setText((u8*)selected, count + 1);
}

static void layout_set_ally_hp(int ally_index, int max_hp, int hp_to_set)
{
	// hp_empty
	// hp_full
	// hp_label

	static char hp_buffer[NUM_ALLIES][32] = {};
	memset(hp_buffer[ally_index], 0, sizeof(hp_buffer[NUM_ALLIES]));
	int count = s32_to_str_base10(hp_to_set, hp_buffer[ally_index]);
	hp_buffer[ally_index][count++] = '/';
	count += s32_to_str_base10(max_hp, hp_buffer[ally_index] + count);

	int max_width = gw.allies_info[ally_index]->divs[0]->getWidth() - 2;
	int new_width = (int)roundf((r32)(hp_to_set * max_width) / (r32)max_hp);
	gw.allies_info[ally_index]->divs[1]->setWidth(new_width);

	linked::Label* hp_label = gw.allies_info[ally_index]->divs[2]->getLabels()[0];
	hp_label->setText((u8*)hp_buffer[ally_index], count + 1);
}

static void layout_set_enemy_hp(int enemy_index, int max_hp, int hp_to_set)
{
	// hp_empty
	// hp_full
	// hp_label

	static char hp_buffer[NUM_ENEMIES][32] = {};
	memset(hp_buffer[enemy_index], 0, sizeof(hp_buffer[NUM_ENEMIES]));
	int count = s32_to_str_base10(hp_to_set, hp_buffer[enemy_index]);
	hp_buffer[enemy_index][count++] = '/';
	count += s32_to_str_base10(max_hp, hp_buffer[enemy_index] + count);

	int max_width = gw.enemies_info[enemy_index]->divs[0]->getWidth() - 2;
	int new_width = (int)roundf((r32)(hp_to_set * max_width) / (r32)max_hp);
	gw.enemies_info[enemy_index]->divs[1]->setWidth(new_width);

	linked::Label* hp_label = gw.enemies_info[enemy_index]->divs[2]->getLabels()[0];
	hp_label->setText((u8*)hp_buffer[enemy_index], count + 1);
}

static void layout_set_timer_percentage(r32 percentage)
{
	int new_w = (int)((double)window_info.width * percentage);
	gw.timer_window->setWidth(new_w);
}

static void layout_set_enemy_image_opacity(s32 index, r32 percentage, hm::vec4 color) {
	gw.enemies[index]->divs[0]->setOpacity(percentage);
	color.a = percentage;
	gw.enemies[index]->divs[0]->setBackgroundColor(color);
}
static void layout_set_ally_image_opacity(s32 index, r32 percentage, hm::vec4 color) {
	gw.allies[index]->divs[0]->setOpacity(percentage);
	color.a = percentage;
	gw.allies[index]->divs[0]->setBackgroundColor(color);
}

static void layout_update_endturn_button() {
	hm::vec4 greener_cyan(0, 1, 0.7f, 1);
	linked::Label* end_turn_label = combat_state.end_turn_button->getLabel();
	if (combat_state.player_turn) {
		end_turn_label->setText((u8*)"END TURN", sizeof("END TURN"));
		combat_state.end_turn_button->setHoveredBGTexture(gw.end_turn_button_player_turn_hover);
		combat_state.end_turn_button->setHeldBGTexture(gw.end_turn_button_player_turn_hover);
		combat_state.end_turn_button->setNormalBGTexture(gw.end_turn_button_player_turn);
	} else {
		end_turn_label->setText((u8*)"ENEMY TURN", sizeof("ENEMY TURN"));
		combat_state.end_turn_button->setAllBGTexture(gw.end_turn_button_enemy_turn);
	}

	end_turn_label->setPosition(hm::vec2((combat_state.end_turn_button->getWidth() - end_turn_label->getTextWidth()) / 2.0f, 10.0f));
}

static void layout_apply_status_ally(s32 target_index, s32 stat_index, Texture* status_image) {
	gw.allies_info[target_index]->divs[2]->getButtons()[stat_index]->setAllBGColor(hm::vec4(1, 1, 1, 1));
	gw.allies_info[target_index]->divs[2]->getButtons()[stat_index]->setAllBGTexture(status_image);
	if (!status_image) {
		gw.allies_info[target_index]->divs[2]->getButtons()[stat_index]->setAllBGColor(hm::vec4(1, 1, 1, 0));
	}
}

static void layout_apply_status_enemy(s32 target_index, s32 stat_index, Texture* status_image) {
	gw.enemies_info[target_index]->divs[2]->getButtons()[stat_index]->setAllBGColor(hm::vec4(1, 1, 1, 1));
	gw.enemies_info[target_index]->divs[2]->getButtons()[stat_index]->setAllBGTexture(status_image);
	if (!status_image) {
		gw.enemies_info[target_index]->divs[2]->getButtons()[stat_index]->setAllBGColor(hm::vec4(1, 1, 1, 0));
	}
}
