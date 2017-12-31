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

#include "game_skills.cpp"

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
hm::vec4 enem_hp_bar_full_color(1, 0.71f, 0.29f, 1.0f);
hm::vec4 enem_hp_bar_empty_color(0.6f, 0.29f, 0.02f, 1.0f);
hm::vec4 enemy_color(1, 0.71f, 0.29f, 1.0f);

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

static void button_exchange_orb(void* arg) {
	// Setup view and state
	layout_change_exchange_orb_amount(ORB_ALL, 0);
	combat_state.exchange_orbs_state.accumulated = 0;
	for (int i = 0; i < ORB_NUMBER; ++i) {
		layout_change_exchange_orb_amount((Orb_ID)i, combat_state.orbs_amount[i]);
		combat_state.exchange_orbs_state.orbs_start_amount[i] = combat_state.orbs_amount[i];
		combat_state.exchange_orbs_state.orbs_amount_added_subtracted[i] = 0;
	}

	// reset confirm button color
	linked::Button* confirm_button = gw.exchange_orbs->divs[0]->getButtons()[1];
	confirm_button->setActive(false, false);

	// Gray out all upper arrows
	for (int i = 0; i < ORB_NUMBER - 1; ++i) {
		combat_state.exchange_orbs_state.info_div->getButtons()[i + 5]->setActive(false, false);
		if (combat_state.exchange_orbs_state.orbs_start_amount[i] <= 0)
			combat_state.exchange_orbs_state.info_div->getButtons()[i + 9]->setActive(false, false);
		else
			combat_state.exchange_orbs_state.info_div->getButtons()[i + 9]->setActive(true);
	}

	combat_state.exchange_orbs_state.active = true;
	gw.exchange_orbs->setActive(true);
}

static void button_exchange_orbs_close(void* arg) {
	AudioController::cancelAudio.play();
	combat_state.exchange_orbs_state.active = false;
	gw.exchange_orbs->setActive(false);
}

static void button_exchange_orbs_confirm(void* arg) {
	assert(combat_state.exchange_orbs_state.active == true);
	if (!combat_state.exchange_orbs_state.can_confirm)
		return;
	if (combat_state.exchange_orbs_state.state_changed) {
		// reset state
		combat_state.exchange_orbs_state.state_changed = false;

		for (int i = 0; i < ORB_NUMBER; ++i) {
			if (i == ORB_NULL) continue;
			combat_state.orbs_amount[i] += combat_state.exchange_orbs_state.orbs_amount_added_subtracted[i];
			layout_change_orb_amount((Orb_ID)i, combat_state.orbs_amount[i]);
			memset(&combat_state.exchange_orbs_state.orbs_amount_added_subtracted[i], 0, sizeof(combat_state.exchange_orbs_state.orbs_amount_added_subtracted[i]));
		}
		combat_state.total_orbs -= combat_state.exchange_orbs_state.num_lost;
		combat_state.total_orbs += combat_state.exchange_orbs_state.num_gain;

		// @todo test this
		int sum_orbs = 0;
		for (int i = 0; i < ORB_NUMBER; ++i)
			sum_orbs += combat_state.orbs_amount[i];
		assert(combat_state.total_orbs == sum_orbs);

		layout_change_orb_amount(ORB_ALL, combat_state.total_orbs);

		combat_state.exchange_orbs_state.num_lost = 0;
		combat_state.exchange_orbs_state.num_gain = 0;

		// close
		combat_state.exchange_orbs_state.active = false;
		gw.exchange_orbs->setActive(false);
	}
}

static void button_exchange_orb_arrow(void* arg) {
	linked::Button_Info* eba = (linked::Button_Info*)arg;
	int curr_orbs = combat_state.exchange_orbs_state.orbs_start_amount[eba->id] + combat_state.exchange_orbs_state.orbs_amount_added_subtracted[eba->id];
	bool state_changed = false;
	if (eba->data == (void*)ARROW_UP) {
		// Arrow up
		if (combat_state.exchange_orbs_state.accumulated >= 3) {
			state_changed = true;
			combat_state.exchange_orbs_state.state_changed = true;
			combat_state.exchange_orbs_state.accumulated -= 3;
			combat_state.exchange_orbs_state.orbs_amount_added_subtracted[eba->id] += 1;
			combat_state.exchange_orbs_state.num_gain += 1;
		}
	} else {
		// Arrow down
		if (curr_orbs > 0) {
			state_changed = true;
			combat_state.exchange_orbs_state.state_changed = true;
			combat_state.exchange_orbs_state.accumulated += 1;
			combat_state.exchange_orbs_state.orbs_amount_added_subtracted[eba->id] -= 1;
			combat_state.exchange_orbs_state.num_lost += 1;
		}
	}
	
	if (state_changed) {
		layout_change_exchange_orb_amount((Orb_ID)eba->id, combat_state.exchange_orbs_state.orbs_start_amount[eba->id] + combat_state.exchange_orbs_state.orbs_amount_added_subtracted[eba->id]);
		layout_change_exchange_orb_amount(ORB_ALL, combat_state.exchange_orbs_state.accumulated);

		// update confirm button
		linked::Button* confirm_button = gw.exchange_orbs->divs[0]->getButtons()[1];
		if (combat_state.exchange_orbs_state.accumulated == 0) {
			assert(combat_state.exchange_orbs_state.num_lost % 3 == 0);
			confirm_button->setActive(true);
			combat_state.exchange_orbs_state.can_confirm = true;
		} else {
			confirm_button->setActive(false);
			combat_state.exchange_orbs_state.can_confirm = false;
		}
	}

	hm::vec4 arrow_hovered_color = hm::vec4(1, 1, 1, 0.1f);
	if (combat_state.exchange_orbs_state.accumulated >= 3) {
		for (int i = 0; i < ORB_NUMBER - 1; ++i)
			combat_state.exchange_orbs_state.info_div->getButtons()[i + 5]->setActive(true);
	} else {
		for (int i = 0; i < ORB_NUMBER - 1; ++i)
			combat_state.exchange_orbs_state.info_div->getButtons()[i + 5]->setActive(false);
	}
	for (int i = 0; i < ORB_NUMBER - 1; ++i) {
		int curr_amt = combat_state.exchange_orbs_state.orbs_start_amount[i] + combat_state.exchange_orbs_state.orbs_amount_added_subtracted[i];
		if (curr_amt <= 0)
			combat_state.exchange_orbs_state.info_div->getButtons()[i + 9]->setActive(false);
		else
			combat_state.exchange_orbs_state.info_div->getButtons()[i + 9]->setActive(true);
	}
}

#define ARCSIN_1 1.57079633

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
	for (int i = 0; i < ORB_NUMBER - 1; ++i) {
		if (skill_costs[skill_used][i] > combat_state.orbs_amount[i]) {
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

	if (combat_state.player.hp <= 0)
		return;

	bool is_toggled = eba->this_button->getIsToggled();

	if (!is_toggled && eba->this_button->getActive() && !combat_state.player.targeting) {
		if (!has_enough_orbs(skill_used)) {
			return;
		} else {
			temporary_modify_orbs(skill_used, -1);
		}
	}

	if (is_toggled) {
		// untoggle?
		printf("untoggle\n");
		temporary_modify_orbs(skill_used, 1);
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

		if (target.enemy) {
			for (int i = 0; i < NUM_ENEMIES; ++i) {
				if (combat_state.enemy.hp[i] <= 0) continue;
				enemy_target_animation[i].is_animating = true;
			}
			printf(" enemy");
		} 
		if (target.ally) {
			for (int i = 0; i < NUM_ALLIES; ++i) {
				if (combat_state.player.hp[i] <= 0) continue;
				allies_target_animation[i].is_animating = true;
			}
			printf(" ally");
		}
		printf(" target.\n");
	} else if (target.number == 0 && target.all && !is_toggled) {
		if (target.enemy) {
			for (int i = 0; i < NUM_ENEMIES; ++i) {
				if (combat_state.enemy.hp[i] <= 0)
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
				if (combat_state.player.hp[i] <= 0)
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

	if (combat_state.enemy.hp[eba->id] <= 0)
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

	if (combat_state.player.hp[eba->id] <= 0)
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

// Layout initialization functions
void hide_all_windows() {
	//gw.bgwindow->setActive(false);

	gw.intro_logo->setActive(false);

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
		for (int k = 0; k < NUM_ALLIES; ++k) {
			gw.ally_target[i][k]->setActive(false);
		}
	}
	for (int i = 0; i < NUM_ENEMIES; ++i) {
		gw.enemies[i]->setActive(false);
		gw.enemies_info[i]->setActive(false);
		for (int k = 0; k < MAX(NUM_ALLIES, NUM_ENEMIES); ++k) {
			gw.enemy_target[i][k]->setActive(false);
		}
	}
	gw.combat_bottom_info->setActive(false);
	gw.timer_window->setActive(false);
	gw.player_name_window->setActive(false);
	gw.exchange_orbs->setActive(false);
	gw.null_orb_modal->setActive(false);
}

void init_intro_mode() {
	Texture* logo = new Texture("../../../res/logo_clean.png");
	Texture* playbutton_normal = new Texture("../../../res/textures/playbutton_normal.png");
	Texture* playbutton_hover = new Texture("../../../res/textures/playbutton_hover.png");
	
	linked::Window* logo_window = new linked::Window(logo->width, logo->height, hm::vec2(window_info.width / 2 - logo->width / 2, window_info.height / 2 - logo->height / 2), hm::vec4(0, 0, 0, 0), 0, 0, linked::W_UNFOCUSABLE);
	linked::WindowDiv* logo_div = new linked::WindowDiv(*logo_window, logo->width, logo->height, 0, 0, hm::vec2(0, 0), hm::vec4(0, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	//logo_div->setBackgroundTexture(logo);
	logo_window->divs.push_back(logo_div);
	gw.intro_logo = logo_window;

	linked::Label* play_label = new linked::Label(*logo_div, (u8*)"PLAY", sizeof("PLAY"), hm::vec2(90.0f, 15.0f), hm::vec4(1,1,1,1), FONT_OSWALD_REGULAR_24, 0, 0);
	linked::Button* play_button = new linked::Button(*logo_div, play_label, hm::vec2(logo->width / 2 - 230 / 2, logo->height - 140.0f), 230, 60, hm::vec4(0.91, 0.23f, 0.88, 1), 0);
	play_button->setAllBGTexture(playbutton_hover);
	play_button->setNormalBGTexture(playbutton_normal);
	play_button->setHoveredTextColor(hm::vec4(1, 1, 1, 1));
	play_button->setNormalTextColor(hm::vec4(1.0f, 0.91f, 0.95f, 1.0f));
	play_button->setHoveredTextColor(hm::vec4(1.0f, 0.91f, 0.95f, 1.0f));
	play_button->setClickedCallback(button_play_game);
	logo_div->getButtons().push_back(play_button);

	linked::Button* up_button = new linked::Button(*logo_div, 0, hm::vec2(logo->width / 2 - 230 / 2 + playbutton_normal->width / 2.0f - 12.0f, logo->height - 180.0f), 24, 24, hm::vec4(1, 0, 0, 1), 0);
	linked::Button* down_button = new linked::Button(*logo_div, 0, hm::vec2(logo->width / 2 - 230 / 2 + playbutton_normal->width / 2.0f - 12.0f, logo->height - 60.0f), 24, 24, hm::vec4(1, 0, 0, 1), 0);
	Texture* hovered_arrow_up = new Texture("../../../res/orbs/arrow_up_blue.png");
	Texture* hovered_arrow_down = new Texture("../../../res/orbs/arrow_down_blue.png");
	up_button->setAllBGTexture(hovered_arrow_up);
	down_button->setAllBGTexture(hovered_arrow_down);
	up_button->setNormalBGTexture(new Texture("../../../res/orbs/arrow_pink_up.png"));
	down_button->setNormalBGTexture(new Texture("../../../res/orbs/arrow_pink_down.png"));
	logo_div->getButtons().push_back(up_button);
	logo_div->getButtons().push_back(down_button);
}

void init_char_selection_mode()
{
	// left character window
	linked::Window* left_char_window = new linked::Window(400, 840, hm::vec2(100, 30), hm::vec4(12.0f / 255.0f, 16.0f / 255.0f, 40.0f / 255.0f, 1.0f), 0, 0, linked::W_BORDER | linked::W_UNFOCUSABLE);
	left_char_window->setBorderSizeX(10.0f);
	left_char_window->setBorderSizeY(0.0f);
	left_char_window->setBorderColor(cyan);
	gw.left_char_window = left_char_window;
	linked::WindowDiv* left_char_div = new linked::WindowDiv(*left_char_window, 400, 840, 0, 0, hm::vec2(0, 0), hm::vec4(12.0f / 255.0f, 16.0f / 255.0f, 40.0f / 255.0f, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_RIGHT);
	left_char_window->divs.push_back(left_char_div);
	char_sel_state.last_hovered = CHAR_NONE;
	linked::WindowDiv* left_char_name_div = new linked::WindowDiv(*left_char_window, 400, 110, 0, 0, hm::vec2(0, 220), hm::vec4(12.0f / 255.0f, 16.0f / 255.0f, 40.0f / 255.0f, 0.65f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_LEFT);
	linked::Button* arrow_up_char_select = new linked::Button(*left_char_name_div, 0, hm::vec2(30, 20), 24, 24, hm::vec4(1, 0, 0, 1), 0);
	linked::Button* arrow_down_char_select = new linked::Button(*left_char_name_div, 0, hm::vec2(30, 65), 24, 24, hm::vec4(1, 0, 0, 1), 0);
	arrow_up_char_select->setAllBGTexture(new Texture("../../../res/orbs/arrow_pink_up.png"));
	arrow_down_char_select->setAllBGTexture(new Texture("../../../res/orbs/arrow_pink_down.png"));
	left_char_name_div->getButtons().push_back(arrow_up_char_select);
	left_char_name_div->getButtons().push_back(arrow_down_char_select);
	
	linked::Label* left_char_name_label = new linked::Label(*left_char_name_div, (u8*)"", sizeof "", hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_38, 0, 0);
	left_char_name_div->getLabels().push_back(left_char_name_label);
	left_char_window->divs.push_back(left_char_name_div);

	linked::WindowDiv* left_char_div_bar = new linked::WindowDiv(*left_char_window, 380, 3, 0, 0, hm::vec2(10, 220), hm::vec4(1, 0.2f, 1, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_RIGHT);
	left_char_window->divs.push_back(left_char_div_bar);

	float char_window_width = 140.0f;
	float char_window_height = 300.0f;

	// Character selected lower window (bottom)
	linked::Window* char_selected_window = new linked::Window(6 * char_window_width + 100.0f, 200, hm::vec2(520, 670), hm::vec4(12.0f / 255.0f, 16.0f / 255.0f, 40.0f / 255.0f, 0.55f), 0, 0, linked::W_UNFOCUSABLE);
	gw.char_selected_window = char_selected_window;

	linked::WindowDiv* s_div1 = new linked::WindowDiv(*char_selected_window, char_window_width, char_window_width, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 0.0f + char_window_width * 0.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* s_div2 = new linked::WindowDiv(*char_selected_window, char_window_width, char_window_width, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 1.0f + char_window_width * 1.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* s_div3 = new linked::WindowDiv(*char_selected_window, char_window_width, char_window_width, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 2.0f + char_window_width * 2.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	s_div1->createBorder(1, 1, 1, 1);
	s_div1->setBorderColor(cyan);
	s_div2->createBorder(1, 1, 1, 1);
	s_div2->setBorderColor(cyan);
	s_div3->createBorder(1, 1, 1, 1);
	s_div3->setBorderColor(cyan);
	char_selected_window->divs.push_back(s_div1);
	char_selected_window->divs.push_back(s_div2);
	char_selected_window->divs.push_back(s_div3);

	linked::WindowDiv* info_div = new linked::WindowDiv(*char_selected_window, 24, 24, 0, 0, hm::vec2(600.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	Texture* space_texture = new Texture("../../../res/textures/spacebar.png");
	info_div->setBackgroundTexture(space_texture);
	char_selected_window->divs.push_back(info_div);

	linked::WindowDiv* info_label_div = new linked::WindowDiv(*char_selected_window, 256, 24, 0, 0, hm::vec2(624.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_selected_window->divs.push_back(info_label_div);
	linked::Label* info_label = new linked::Label(*info_label_div, (u8*)"Information", sizeof("Information"), hm::vec2(5.0f, 2.0f), hm::vec4(1, 1, 1, 1), FONT_OSWALD_LIGHT_16, 0, 0);
	info_label_div->getLabels().push_back(info_label);

	linked::WindowDiv* confirm_div = new linked::WindowDiv(*char_selected_window, 24, 24, 0, 0, hm::vec2(740.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	Texture* enter_texture = new Texture("../../../res/textures/enter.png");
	confirm_div->setBackgroundTexture(enter_texture);
	char_selected_window->divs.push_back(confirm_div);

	linked::WindowDiv* confirm_label_div = new linked::WindowDiv(*char_selected_window, 256, 24, 0, 0, hm::vec2(764.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_selected_window->divs.push_back(confirm_label_div);
	linked::Label* confirm_label = new linked::Label(*confirm_label_div, (u8*)"Confirm", sizeof("Confirm"), hm::vec2(5.0f, 2.0f), hm::vec4(1, 1, 1, 1), FONT_OSWALD_LIGHT_16, 0.0f, 0);
	confirm_label_div->getLabels().push_back(confirm_label);

	linked::WindowDiv* back_div = new linked::WindowDiv(*char_selected_window, 24, 24, 0, 0, hm::vec2(840.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	Texture* esc_texture = new Texture("../../../res/textures/esc.png");
	back_div->setBackgroundTexture(esc_texture);
	char_selected_window->divs.push_back(back_div);

	linked::WindowDiv* back_label_div = new linked::WindowDiv(*char_selected_window, 256, 24, 0, 0, hm::vec2(864.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_selected_window->divs.push_back(back_label_div);
	linked::Label* back_label = new linked::Label(*back_label_div, (u8*)"Back", sizeof("Back"), hm::vec2(5.0f, 2.0f), hm::vec4(1, 1, 1, 1), FONT_OSWALD_LIGHT_16, 0.0f, 0);
	back_label_div->getLabels().push_back(back_label);

	linked::WindowDiv* play_div = new linked::WindowDiv(*char_selected_window, 148, 48, 0, 0, hm::vec2(740.0f, 50.0f), hm::vec4(0.34f, 0.9f, 0.72f, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_selected_window->divs.push_back(play_div);
	linked::Label* play_label = new linked::Label(*play_div, (u8*)"FIGHT", sizeof("FIGHT"),
		hm::vec2(44.0f, 48.0f / 2.0f - get_font_size(FONT_OSWALD_REGULAR_24) / 2.0f), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_24, 0, 0);
	linked::Button* play_button = new linked::Button(*play_div, play_label, hm::vec2(0, 0), 148, 48, hm::vec4(0.34f, 0.73f, 0.62f, 1), 0);
	hm::vec4 hovered_play_div_color(0.24f, 0.63f, 0.52f, 1);
	play_button->setHoveredBGColor(hovered_play_div_color);
	play_button->setHeldBGColor(char_window_held_color);
	play_button->setClickedCallback(button_combat_start_mode);
	play_div->getButtons().push_back(play_button);
	play_div->m_render = false;
	char_sel_state.play_button_div = play_div;

	// Character selection window (top)
	linked::Window* char_selection_window = new linked::Window(6 * char_window_width + 100.0f, 630, hm::vec2(520, 30), hm::vec4(12.0f / 255.0f, 16.0f / 255.0f, 40.0f / 255.0f, 0.55f), 0, 0, linked::W_UNFOCUSABLE);
	gw.char_selection_window = char_selection_window;

	Texture* chars_texture[NUM_CHARS] = {};
	
	chars_texture[0] =  new Texture("../../../res/textures/zero.png");
	chars_texture[1] =  new Texture("../../../res/textures/one.png");
	chars_texture[2] =  new Texture("../../../res/textures/serial.png");
	chars_texture[3] =  new Texture("../../../res/textures/ray.png");
	chars_texture[4] =  new Texture("../../../res/textures/astar.png");
	chars_texture[5] =  new Texture("../../../res/textures/deadlock.png");
	chars_texture[6] =  new Texture("../../../res/textures/norma.png");
	chars_texture[7] =  new Texture("../../../res/textures/hazard.png");
	chars_texture[8] =  new Texture("../../../res/textures/qwerty.png");
	chars_texture[9] =  new Texture("../../../res/textures/bigo.png");
	chars_texture[10] = new Texture("../../../res/textures/new.png");
	chars_texture[11] = new Texture("../../../res/textures/clockboy.png");
	
#if FAST
	chars_texture_big[0] = chars_texture[0];
	chars_texture_big[1] = chars_texture[1];
	chars_texture_big[2] = chars_texture[2];
	chars_texture_big[3] = chars_texture[3];
	chars_texture_big[4] = chars_texture[4];
	chars_texture_big[5] = chars_texture[5];
	chars_texture_big[6] = chars_texture[6];
	chars_texture_big[7] = chars_texture[7];
	chars_texture_big[8] = chars_texture[8];
	chars_texture_big[9] = chars_texture[9];
	chars_texture_big[10] = chars_texture[10];
	chars_texture_big[11] = chars_texture[11];
#else
	chars_texture_big[0] = new Texture("../../../res/char_big/zero.png");
	chars_texture_big[1] = new Texture("../../../res/char_big/one.png");
	chars_texture_big[2] = new Texture("../../../res/char_big/serial-keiller.png");
	chars_texture_big[3] = new Texture("../../../res/char_big/ray-tracey.png");
	chars_texture_big[4] = new Texture("../../../res/char_big/astar.png");
	chars_texture_big[5] = new Texture("../../../res/char_big/deadlock.png");
	chars_texture_big[6] = new Texture("../../../res/char_big/norma.png");
	chars_texture_big[7] = new Texture("../../../res/char_big/hazard.png");
	chars_texture_big[8] = new Texture("../../../res/char_big/qwerty.png");
	chars_texture_big[9] = new Texture("../../../res/char_big/big-o.png");
	chars_texture_big[10] = new Texture("../../../res/char_big/new.png");
	chars_texture_big[11] = new Texture("../../../res/char_big/clockboy.png");
#endif

	linked::WindowDiv* char_divs[NUM_CHARS] = {};
	float char_div_offset_x = 0.0f;
	float char_div_offset_y = 0.0f;
	for (int i = 0; i < NUM_CHARS; ++i) {
		char_divs[i] = new linked::WindowDiv(*char_selection_window, char_window_width, char_window_height, 0.0f, 0.0f,
			hm::vec2(25.0f + 10.0f * char_div_offset_x + char_window_width * char_div_offset_x, 10.0f * (char_div_offset_y + 1.0f) + char_div_offset_y * char_window_height),
			char_window_color, linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		char_selection_window->divs.push_back(char_divs[i]);
		char_divs[i]->createBorder(-5, -5, -5, -5);
		char_divs[i]->setBorderColor(hm::vec4(0, 1, 1, 0));

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
		button->setClickedCallback(button_select_character);
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

	linked::WindowDiv* s_div1 = new linked::WindowDiv(*char_info_window_bot, char_window_width, char_window_width, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 0.0f + char_window_width * 0.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* s_div2 = new linked::WindowDiv(*char_info_window_bot, char_window_width, char_window_width, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 1.0f + char_window_width * 1.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* s_div3 = new linked::WindowDiv(*char_info_window_bot, char_window_width, char_window_width, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 2.0f + char_window_width * 2.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	s_div1->createBorder(1, 1, 1, 1);
	s_div1->setBorderColor(cyan);
	s_div2->createBorder(1, 1, 1, 1);
	s_div2->setBorderColor(cyan);
	s_div3->createBorder(1, 1, 1, 1);
	s_div3->setBorderColor(cyan);
	char_info_window_bot->divs.push_back(s_div1);
	char_info_window_bot->divs.push_back(s_div2);
	char_info_window_bot->divs.push_back(s_div3);

	linked::WindowDiv* back_div = new linked::WindowDiv(*char_info_window_bot, 24, 24, 0, 0, hm::vec2(840.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	Texture* esc_texture = new Texture("../../../res/textures/esc.png");
	back_div->setBackgroundTexture(esc_texture);
	char_info_window_bot->divs.push_back(back_div);

	linked::WindowDiv* back_label_div = new linked::WindowDiv(*char_info_window_bot, 256, 24, 0, 0, hm::vec2(864.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_info_window_bot->divs.push_back(back_label_div);
	linked::Label* back_label = new linked::Label(*back_label_div, (u8*)"Back", sizeof("Back"), hm::vec2(5.0f, 2.0f), hm::vec4(1, 1, 1, 1), FONT_OSWALD_LIGHT_16, 0.0f, 0);
	back_label_div->getLabels().push_back(back_label);
	

	float start_offset_x = 85.0f;

	linked::WindowDiv* skills_divs[NUM_SKILLS] = {};
	float char_div_offset_x = 0.0f;
	float char_div_offset_y = 0.0f;
	float skills_spacing = 30.0f;
	for (int i = 0; i < NUM_SKILLS; ++i) {
		skills_divs[i] = new linked::WindowDiv(*char_info_window, char_window_width, char_window_height, 0.0f, 0.0f,
			hm::vec2(start_offset_x + skills_spacing * char_div_offset_x + char_window_width * char_div_offset_x, skills_spacing * (char_div_offset_y + 1.0f) + char_div_offset_y * char_window_height),
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
	linked::WindowDiv* skill_title_div = new linked::WindowDiv(*char_info_window, 300, 48, 0, 0, hm::vec2(start_offset_x, 140.0f + 55.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_info_window->divs.push_back(skill_title_div);
	linked::Label* skill_title_label = new linked::Label(*skill_title_div, (u8*)"", 0, hm::vec2(0, 0), hm::vec4(1,1,1,1), FONT_OSWALD_REGULAR_18, 0, 0);
	skill_title_div->getLabels().push_back(skill_title_label);

	// gotta be second
	linked::WindowDiv* skill_desc_div = new linked::WindowDiv(*char_info_window, 300, 48, 0, 0, hm::vec2(start_offset_x, 140.0f + 105.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_info_window->divs.push_back(skill_desc_div);
	linked::Label* skill_desc_label = new linked::Label(*skill_desc_div, (u8*)"", 0, hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_LIGHT_16, 0, 0);
	skill_desc_div->getLabels().push_back(skill_desc_label);

	linked::Window* skill_cost = new linked::Window(300, 48, hm::vec2(530 + 580, 40 + char_window_width + 20 * 2), hm::vec4(0, 0, 0, 0), 0, 0, linked::W_UNFOCUSABLE);
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

	linked::WindowDiv* skill_group_div = new linked::WindowDiv(*char_info_window, 640, 48, 0, 0, hm::vec2(start_offset_x, 630.0f - 48.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	gw.skill_group_div = skill_group_div;
	char_info_window->divs.push_back(skill_group_div);
	linked::Label* skill_group_label = new linked::Label(*skill_group_div, (u8*)/*"VIRTUAL, RANGED, INV, STATIC"*/"", 0, hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_LIGHT_12, 0, 0);
	skill_group_div->getLabels().push_back(skill_group_label);

	linked::Label* skill_cooldown_label = new linked::Label(*skill_group_div, (u8*)"", 0, hm::vec2(600, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_LIGHT_12, 0, 0);
	skill_group_div->getLabels().push_back(skill_cooldown_label);
}

void init_combat_mode()
{
	// initialize unknown skill texture
	for (int i = 0; i < NUM_CHARS * 4; i += 4) {
		skill_textures[i + 0] = new Texture("../../../res/skills/unk_skill1.png");
		skill_textures[i + 1] = new Texture("../../../res/skills/unk_skill2.png");
		skill_textures[i + 2] = new Texture("../../../res/skills/unk_skill3.png");
		skill_textures[i + 3] = new Texture("../../../res/skills/unk_skill4.png");
	}

	// Initialize Character square textures
	char_textures[CHAR_ZERO]            = new Texture("../../../res/char_square/zero_sq.png");
	char_textures[CHAR_ONE]             = new Texture("../../../res/char_square/one_sq.png");
	char_textures[CHAR_SERIAL_KEYLLER]  = new Texture("../../../res/char_square/serial_sq.png");
	char_textures[CHAR_RAY_TRACEY]      = new Texture("../../../res/char_square/ray_sq.png");
	char_textures[CHAR_A_STAR]          = new Texture("../../../res/char_square/astar_sq.png");
	char_textures[CHAR_DEADLOCK]        = new Texture("../../../res/char_square/deadlock_sq.png");
	char_textures[CHAR_NORMA]           = new Texture("../../../res/char_square/norma_sq.png");
	char_textures[CHAR_HAZARD]          = new Texture("../../../res/char_square/hazard_sq.png");
	char_textures[CHAR_QWERTY]          = new Texture("../../../res/char_square/qwerty_sq.png");
	char_textures[CHAR_BIG_O]           = new Texture("../../../res/char_square/bigo_sq.png");
	char_textures[CHAR_NEW]             = new Texture("../../../res/char_square/new_sq.png");
	char_textures[CHAR_CLOCKBOY]        = new Texture("../../../res/char_square/clockboy_sq.png");

	// Initialize Skill textures
	skill_textures[CHAR_ZERO * NUM_SKILLS + 0] = new Texture("../../../res/skills/zero/false_rush.png");
	skill_textures[CHAR_ZERO * NUM_SKILLS + 1] = new Texture("../../../res/skills/zero/contradiction.png");
	skill_textures[CHAR_ZERO * NUM_SKILLS + 2] = new Texture("../../../res/skills/zero/requiem_zero.png");
	skill_textures[CHAR_ZERO * NUM_SKILLS + 3] = new Texture("../../../res/skills/zero/void_barrier.png");

	skill_textures[CHAR_ONE * NUM_SKILLS + 0] = new Texture("../../../res/skills/one/truth_slash.png");
	skill_textures[CHAR_ONE * NUM_SKILLS + 1] = new Texture("../../../res/skills/one/tautology.png");
	skill_textures[CHAR_ONE * NUM_SKILLS + 2] = new Texture("../../../res/skills/one/axion_one.png");
	skill_textures[CHAR_ONE * NUM_SKILLS + 3] = new Texture("../../../res/skills/one/true_endurance.png");

	skill_textures[CHAR_SERIAL_KEYLLER * NUM_SKILLS + 0] = new Texture("../../../res/skills/serial_keyller/brute_force.png");
	skill_textures[CHAR_SERIAL_KEYLLER * NUM_SKILLS + 1] = new Texture("../../../res/skills/serial_keyller/buffer_overflow.png");
	skill_textures[CHAR_SERIAL_KEYLLER * NUM_SKILLS + 2] = new Texture("../../../res/skills/serial_keyller/ddos_attack.png");
	skill_textures[CHAR_SERIAL_KEYLLER * NUM_SKILLS + 3] = new Texture("../../../res/skills/serial_keyller/encryption.png");

	skill_textures[CHAR_SERIAL_KEYLLER * NUM_SKILLS + 0] = new Texture("../../../res/skills/serial_keyller/brute_force.png");
	skill_textures[CHAR_SERIAL_KEYLLER * NUM_SKILLS + 1] = new Texture("../../../res/skills/serial_keyller/buffer_overflow.png");
	skill_textures[CHAR_SERIAL_KEYLLER * NUM_SKILLS + 2] = new Texture("../../../res/skills/serial_keyller/ddos_attack.png");
	skill_textures[CHAR_SERIAL_KEYLLER * NUM_SKILLS + 3] = new Texture("../../../res/skills/serial_keyller/encryption.png");

	skill_textures[CHAR_RAY_TRACEY * NUM_SKILLS + 0] = new Texture("../../../res/skills/ray_tracey/particle_rendering.png");
	skill_textures[CHAR_RAY_TRACEY * NUM_SKILLS + 1] = new Texture("../../../res/skills/ray_tracey/diffuse_reflection.png");
	skill_textures[CHAR_RAY_TRACEY * NUM_SKILLS + 2] = new Texture("../../../res/skills/ray_tracey/dynamic_frustum.png");
	skill_textures[CHAR_RAY_TRACEY * NUM_SKILLS + 3] = new Texture("../../../res/skills/ray_tracey/rasterization.png");

	skill_textures[CHAR_A_STAR * NUM_SKILLS + 0] = new Texture("../../../res/skills/astar/q_punch.png");
	skill_textures[CHAR_A_STAR * NUM_SKILLS + 1] = new Texture("../../../res/skills/astar/perceptron.png");
	skill_textures[CHAR_A_STAR * NUM_SKILLS + 2] = new Texture("../../../res/skills/astar/neural_network.png");
	skill_textures[CHAR_A_STAR * NUM_SKILLS + 3] = new Texture("../../../res/skills/astar/hill_climbing.png");

	skill_textures[CHAR_DEADLOCK * NUM_SKILLS + 0] = new Texture("../../../res/skills/deadlock/preemption.png");
	skill_textures[CHAR_DEADLOCK * NUM_SKILLS + 1] = new Texture("../../../res/skills/deadlock/mutex.png");
	skill_textures[CHAR_DEADLOCK * NUM_SKILLS + 2] = new Texture("../../../res/skills/deadlock/thread_scheduling.png");
	skill_textures[CHAR_DEADLOCK * NUM_SKILLS + 3] = new Texture("../../../res/skills/deadlock/fork.png");

	skill_textures[CHAR_NORMA * NUM_SKILLS + 0] = new Texture("../../../res/skills/norma/pumping_up.png");
	skill_textures[CHAR_NORMA * NUM_SKILLS + 1] = new Texture("../../../res/skills/norma/automata_summon.png");
	skill_textures[CHAR_NORMA * NUM_SKILLS + 2] = new Texture("../../../res/skills/norma/turing_machine.png");
	skill_textures[CHAR_NORMA * NUM_SKILLS + 3] = new Texture("../../../res/skills/norma/non_determinism.png");

	skill_textures[CHAR_HAZARD * NUM_SKILLS + 0] = new Texture("../../../res/skills/hazard/tmr.png");
	skill_textures[CHAR_HAZARD * NUM_SKILLS + 1] = new Texture("../../../res/skills/hazard/redundancy.png");
	skill_textures[CHAR_HAZARD * NUM_SKILLS + 2] = new Texture("../../../res/skills/hazard/rollback.png");
	skill_textures[CHAR_HAZARD * NUM_SKILLS + 3] = new Texture("../../../res/skills/hazard/rollforward.png");

	skill_textures[CHAR_QWERTY * NUM_SKILLS + 0] = new Texture("../../../res/skills/qwerty/alt.png");
	skill_textures[CHAR_QWERTY * NUM_SKILLS + 1] = new Texture("../../../res/skills/qwerty/ctrl.png");
	skill_textures[CHAR_QWERTY * NUM_SKILLS + 2] = new Texture("../../../res/skills/qwerty/del.png");
	skill_textures[CHAR_QWERTY * NUM_SKILLS + 3] = new Texture("../../../res/skills/qwerty/esc.png");

	skill_textures[CHAR_BIG_O * NUM_SKILLS + 0] = new Texture("../../../res/skills/big_o/best_bound_fist.png");
	skill_textures[CHAR_BIG_O * NUM_SKILLS + 1] = new Texture("../../../res/skills/big_o/dual_simplex.png");
	skill_textures[CHAR_BIG_O * NUM_SKILLS + 2] = new Texture("../../../res/skills/big_o/graph_coloring.png");
	skill_textures[CHAR_BIG_O * NUM_SKILLS + 3] = new Texture("../../../res/skills/big_o/knapsack_hideout.png");

	skill_textures[CHAR_NEW * NUM_SKILLS + 0] = new Texture("../../../res/skills/new/sprint_burst.png");
	skill_textures[CHAR_NEW * NUM_SKILLS + 1] = new Texture("../../../res/skills/new/inheritance.png");
	skill_textures[CHAR_NEW * NUM_SKILLS + 2] = new Texture("../../../res/skills/new/override.png");
	skill_textures[CHAR_NEW * NUM_SKILLS + 3] = new Texture("../../../res/skills/new/polymorphism.png");

	skill_textures[CHAR_CLOCKBOY * NUM_SKILLS + 0] = new Texture("../../../res/skills/clockboy/clock_pulse.png");
	skill_textures[CHAR_CLOCKBOY * NUM_SKILLS + 1] = new Texture("../../../res/skills/clockboy/pipeline.png");
	skill_textures[CHAR_CLOCKBOY * NUM_SKILLS + 2] = new Texture("../../../res/skills/clockboy/overclock.png");
	skill_textures[CHAR_CLOCKBOY * NUM_SKILLS + 3] = new Texture("../../../res/skills/clockboy/branch_prediction.png");

	orb_textures[ORB_HARD] = new Texture("../../../res/orbs/hard_orb.png");
	orb_textures[ORB_SOFT] = new Texture("../../../res/orbs/soft_orb.png");
	orb_textures[ORB_VR]   = new Texture("../../../res/orbs/vr_orb.png");
	orb_textures[ORB_BIOS] = new Texture("../../../res/orbs/bios_orb.png");
	orb_textures[ORB_NULL] = new Texture("../../../res/orbs/null_orb.png");
	
	{
		// Players Names
		linked::Window* player_name = new linked::Window(window_info.width, 80, hm::vec2(0, 0), hm::vec4(0, 0, 0, 0.6f), 0, 0, linked::W_UNFOCUSABLE);
		linked::WindowDiv* player_name_div = new linked::WindowDiv(*player_name, 300, 32, 0, 0, hm::vec2(0, 0), hm::vec4(1, 0, 0, 0), linked::DIV_CENTER_Y | linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		linked::WindowDiv* enemy_name_div = new linked::WindowDiv(*player_name, 300, 32, 0, 0, hm::vec2(0, 0), hm::vec4(1, 0, 0, 0), linked::DIV_CENTER_Y | linked::DIV_ANCHOR_RIGHT | linked::DIV_ANCHOR_TOP);
		player_name->divs.push_back(player_name_div);
		player_name->divs.push_back(enemy_name_div);
		gw.player_name_window = player_name;

		linked::Label* player_label = new linked::Label(*player_name_div, (u8*)"Player Name", sizeof "Player Name", hm::vec2(50.0f, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_32, 0, 0);
		player_name_div->getLabels().push_back(player_label);

		linked::Label* enemy_label = new linked::Label(*enemy_name_div, (u8*)"Enemy Name", sizeof "Enemy Name", hm::vec2(50.0f, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_32, 0, 0);
		enemy_name_div->getLabels().push_back(enemy_label);

		orb_alive_ally = new Texture("../../../res/orbs/alive_orb.png");
		orb_dead_ally = new Texture("../../../res/orbs/dead_orb.png");;
		orb_alive_enemy = new Texture("../../../res/orbs/enemy_alive_orb.png");;
		orb_dead_enemy = new Texture("../../../res/orbs/enemy_dead_orb.png");;
		float poff_x = 0.0f;
		for (int i = 0; i < NUM_ALLIES; ++i) {
			linked::WindowDiv* player_indicator = new linked::WindowDiv(*player_name, 32, 32, 0, 0, hm::vec2(250 + poff_x, 0), hm::vec4(1, 0, 0, 1), linked::DIV_CENTER_Y | linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
			player_indicator->setBackgroundTexture(orb_alive_ally);
			player_name->divs.push_back(player_indicator);
			poff_x += 32.0f + 5.0f;
			gw.allies_indicator[i] = player_indicator;
		}

		poff_x = 0.0f;
		for (int i = 0; i < NUM_ENEMIES; ++i) {
			linked::WindowDiv* enemy_indicator = new linked::WindowDiv(*player_name, 32, 32, 0, 0, hm::vec2(1200.0f + poff_x, 0), hm::vec4(1, 0, 0, 1), linked::DIV_CENTER_Y | linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
			enemy_indicator->setBackgroundTexture(orb_alive_enemy);
			player_name->divs.push_back(enemy_indicator);
			poff_x += 32.0f + 5.0f;
			gw.enemies_indicator[i] = enemy_indicator;
		}

		// End turn button
		linked::WindowDiv* end_turn = new linked::WindowDiv(*player_name, 200, 45, 0, 0, hm::vec2(0, 20.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP |linked::DIV_CENTER_X);
		linked::Label* end_turn_label = new linked::Label(*end_turn, (u8*)"END TURN", sizeof "END TURN", hm::vec2(55.0f, 12.0f), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_24, 0, 0);
		hm::vec2 end_turn_label_pos((200.0f - end_turn_label->getTextWidth()) / 2.0f, 10.0f);
		end_turn_label->setPosition(end_turn_label_pos);
		linked::Button* end_turn_button = new linked::Button(*end_turn, end_turn_label, hm::vec2(0, 0), 200, 45, greener_cyan - hm::vec4(0.2f, 0.2f, 0.2f, 0.0f), 0);
		combat_state.end_turn_button = end_turn_button;
		end_turn_button->setClickedCallback(button_end_turn);
		player_name->divs.push_back(end_turn);
		end_turn->getButtons().push_back(end_turn_button);
		hm::vec4 end_turn_button_hovered_bgcolor = greener_cyan - hm::vec4(0.4f, 0.35f, 0.4f, 0.0f);
		end_turn_button->setHoveredBGColor(end_turn_button_hovered_bgcolor);
		hm::vec4 end_turn_button_held_bgcolor = hm::vec4(0.4f, 0.65f, 0.45f, 1.0f);
		end_turn_button->setHeldBGColor(end_turn_button_held_bgcolor);
	}
	{
		// Timer window
		linked::Window* timer_window = new linked::Window(window_info.width, 5, hm::vec2(0, 80), greener_cyan, 0, 0, linked::W_UNFOCUSABLE);
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
		gw.allies[i]->setBorderColor(cyan);
		linked::WindowDiv* allies_div = new linked::WindowDiv(*gw.allies[i], size_img, size_img, 0, 0, hm::vec2(0, 0), char_window_color, linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		gw.allies[i]->divs.push_back(allies_div);
		linked::Button* dummy_ally_button = new linked::Button(*allies_div, 0, hm::vec2(0, 0), size_img, size_img, hm::vec4(0, 0, 0, 0), i);
		dummy_ally_button->button_info.id = i;
		dummy_ally_button->setClickedCallback(target_ally);
		allies_div->getButtons().push_back(dummy_ally_button);

		start_pos.x += size_img + x_spacing;
		gw.allies_info[i] = new linked::Window(size_info, size_img, start_pos, char_window_color, 0, 0, linked::W_UNFOCUSABLE | linked::W_BORDER);
		gw.allies_info[i]->setBorderSizeX(1.0f);
		gw.allies_info[i]->setBorderSizeY(1.0f);
		gw.allies_info[i]->setBorderColor(cyan);

		float x_off = 0.0f;
		for (int k = 0; k < NUM_SKILLS; ++k) {
			gw.allies_skills[i * NUM_SKILLS + k] = new linked::Window(skill_img_size, skill_img_size, start_pos + hm::vec2(x_spacing + x_off, hp_bar_height + y_spacing * 2 + 10.0f), hm::vec4(1, 1, 1, 0), 0, 0, linked::W_BORDER | linked::W_UNFOCUSABLE);
			linked::WindowDiv* skill_div = new linked::WindowDiv(*gw.allies_skills[i * NUM_SKILLS + k], skill_img_size, skill_img_size, 0, 0, hm::vec2(0, 0), hm::vec4(0, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
			gw.allies_skills[i * NUM_SKILLS + k]->divs.push_back(skill_div);
			gw.allies_skills[i * NUM_SKILLS + k]->setBorderSizeX(1.0f);
			gw.allies_skills[i * NUM_SKILLS + k]->setBorderSizeY(1.0f);
			gw.allies_skills[i * NUM_SKILLS + k]->setBorderColor(ally_hp_bar_full_color);
			linked::Button* skill_button = new linked::Button(*skill_div, 0, hm::vec2(0, 0), skill_img_size, skill_img_size, hm::vec4(0, 1, 1, 1), k);
			skill_button->button_info.data = (void*)i;
			skill_button->button_info.id = k;
			skill_button->setClickedCallback(button_skill);
			hm::vec4 skill_button_hovered_bgcolor(0, 1, 1, 0.8f);
			skill_button->setHoveredBGColor(skill_button_hovered_bgcolor);
			hm::vec4 skill_button_held_bgcolor(0, 0.8f, 0.9f, 0.7f);
			skill_button->setHeldBGColor(skill_button_held_bgcolor);

			skill_button->setIsToggle(true);
			//skill_button->setToggledNormalBGColor(hm::vec4(0, 1, 1, 0.6f));

			skill_div->getButtons().push_back(skill_button);
			x_off += skill_img_size + x_spacing;
		}

		linked::WindowDiv* hpempty = new linked::WindowDiv(*gw.allies_info[i], size_info - 2 * x_spacing + 2, hp_bar_height + 2.0f, 0, 0, hm::vec2(x_spacing - 1, x_spacing - 1), ally_hp_bar_empty_color, linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		gw.allies_info[i]->divs.push_back(hpempty);

		linked::WindowDiv* hpfilled = new linked::WindowDiv(*gw.allies_info[i], size_info - 2 * x_spacing, hp_bar_height, 0, 0, hm::vec2(x_spacing, x_spacing), ally_hp_bar_full_color, linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		gw.allies_info[i]->divs.push_back(hpfilled);

		linked::WindowDiv* status_info = new linked::WindowDiv(*gw.allies_info[i], size_info - 2 * x_spacing + 2, hp_bar_height + 2.0f, 0, 0, hm::vec2(x_spacing, y_spacing + hp_bar_height), hm::vec4(0, 0, 1, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		gw.allies_info[i]->divs.push_back(status_info);
		linked::Label* hplabel = new linked::Label(*status_info, (u8*)"100/100", sizeof("100/100"), hm::vec2(size_info - sizeof("100/100") * 10.0f, 0.0f), hm::vec4(0, 1, 1, 1), FONT_OSWALD_REGULAR_16, 0, 0);
		status_info->getLabels().push_back(hplabel);

		r32 begin_negative = 0.0f;
		for (int b = 0; b < MAX_STATUS; ++b) {
			r32 b_spacing = (r32)b * hp_bar_height + 5.0f * (r32)b;
			linked::Button* negative_status = new linked::Button(*status_info, 0, hm::vec2(begin_negative + b_spacing, -2.0f), hp_bar_height, hp_bar_height, hm::vec4(1, 0, 0, 0), 0);
			status_info->getButtons().push_back(negative_status);
		}

		start_pos.x = 60;

		hm::vec2 target_pos = start_pos;
		r32 target_icon_size = 40.0f;
		r32 target_spacing = 10.0f;
		target_pos.x += (size_img + x_spacing + size_info + target_spacing);

		for (int k = 0; k < NUM_ALLIES; ++k) {
			linked::Window* target_window = new linked::Window(target_icon_size, target_icon_size, target_pos, char_window_color, 0, 0, linked::W_UNFOCUSABLE | linked::W_BORDER);
			linked::WindowDiv* target_div = new linked::WindowDiv(*target_window, target_icon_size, target_icon_size, 0, 0, hm::vec2(0, 0), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
			target_window->divs.push_back(target_div);
			target_pos.x += 10.0f + target_icon_size;
			gw.ally_target[i][k] = target_window;
		}

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
		linked::Button* dummy_enemy_button = new linked::Button(*enemy_image_div, 0, hm::vec2(0, 0), size_img, size_img, hm::vec4(0, 0, 0, 0), i);
		dummy_enemy_button->button_info.id = i;
		dummy_enemy_button->setClickedCallback(target_enemy);
		enemy_image_div->getButtons().push_back(dummy_enemy_button);

		start_pos.x -= (x_spacing + size_info);
		gw.enemies_info[i] = new linked::Window(size_info, size_enemy_bars, start_pos, char_window_color, 0, 0, linked::W_UNFOCUSABLE | linked::W_BORDER);
		gw.enemies_info[i]->setBorderSizeX(1.0f);
		gw.enemies_info[i]->setBorderSizeY(1.0f);
		gw.enemies_info[i]->setBorderColor(enem_hp_bar_full_color);
		
		linked::WindowDiv* hpempty = new linked::WindowDiv(*gw.enemies_info[i], size_info - 2 * x_spacing + 2, hp_bar_height + 2.0f, 0, 0, hm::vec2(x_spacing - 1, x_spacing - 1), enem_hp_bar_empty_color, linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		gw.enemies_info[i]->divs.push_back(hpempty);
		
		linked::WindowDiv* hpfilled = new linked::WindowDiv(*gw.enemies_info[i], size_info - 2 * x_spacing, hp_bar_height, 0, 0, hm::vec2(-x_spacing, x_spacing), enem_hp_bar_full_color, linked::DIV_ANCHOR_RIGHT | linked::DIV_ANCHOR_TOP);
		gw.enemies_info[i]->divs.push_back(hpfilled);
		
		linked::WindowDiv* status_info = new linked::WindowDiv(*gw.enemies_info[i], size_info - 2 * x_spacing + 2, hp_bar_height + 2.0f, 0, 0, hm::vec2(x_spacing, y_spacing + hp_bar_height), hm::vec4(0, 0, 1, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		gw.enemies_info[i]->divs.push_back(status_info);
		linked::Label* hplabel = new linked::Label(*status_info, (u8*)"100/100", sizeof("100/100"), hm::vec2(/*size_info - sizeof("100/100") * 10.0f*/ 0.0f, 0.0f), enem_hp_bar_full_color, FONT_OSWALD_REGULAR_16, 0, 0);
		status_info->getLabels().push_back(hplabel);

		r32 begin_negative = size_info - 2 * x_spacing + 2 - hp_bar_height;
		for (int b = 0; b < MAX_STATUS; ++b) {
			r32 b_spacing = (r32)b * hp_bar_height + 5.0f * (r32)b;
			linked::Button* negative_status = new linked::Button(*status_info, 0, hm::vec2(begin_negative - b_spacing, 0.0f), hp_bar_height, hp_bar_height, hm::vec4(1, 0, 0, 0), 0);
			status_info->getButtons().push_back(negative_status);
		}

		start_pos.x = 1400.0f;

		hm::vec2 target_pos = start_pos;
		r32 target_icon_size = 40.0f;
		r32 target_spacing = 10.0f;
		target_pos.x -= (x_spacing + size_info + target_icon_size + target_spacing);
		s32 max_ = MAX(NUM_ALLIES, NUM_ENEMIES);
		for (int k = 0; k < max_ ; ++k) {
			linked::Window* target_window = new linked::Window(target_icon_size, target_icon_size, target_pos, char_window_color, 0, 0, linked::W_UNFOCUSABLE | linked::W_BORDER);
			linked::WindowDiv* target_div = new linked::WindowDiv(*target_window, target_icon_size, target_icon_size, 0, 0, hm::vec2(0, 0), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
			target_window->divs.push_back(target_div);
			target_pos.x -= 10.0f + target_icon_size;
			gw.enemy_target[i][k] = target_window;
		}

		start_pos.y += size_img + y_spacing;
	}

	{
		float orbs_size = 64.0f;

		hm::vec4 combat_info_bar_color(15.0f / 255.0f, 17.0f / 255.0f, 42.0f / 255.0f, 0.8f);
		linked::Window* combat_bottom_info = new linked::Window(window_info.width, 200, hm::vec2(0, 660), combat_info_bar_color, 0, 0, linked::W_BORDER | linked::W_UNFOCUSABLE);
		linked::WindowDiv* orbs_div = new linked::WindowDiv(*combat_bottom_info, 500, 100, 0, 0, hm::vec2(45.0f, 0), hm::vec4(1, 0, 0, 0.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP | linked::DIV_CENTER_Y);
		combat_bottom_info->divs.push_back(orbs_div);

		linked::Button* multiple_orb_button = new linked::Button(*orbs_div, 0, hm::vec2(0, 0), orbs_size, orbs_size, hm::vec4(0, 0, 0, 1), 0);
		Texture* multiple_orbs_texture = new Texture("../../../res/orbs/all_orbs.png");
		multiple_orb_button->setAllBGTexture(multiple_orbs_texture);
		orbs_div->getButtons().push_back(multiple_orb_button);

		linked::Button* hard_orb_button = new linked::Button(*orbs_div, 0, hm::vec2((orbs_size + 10) + 18, 0), orbs_size, orbs_size, hm::vec4(0, 0, 0, 1), 0);
		hard_orb_button->setAllBGTexture(orb_textures[0]);
		orbs_div->getButtons().push_back(hard_orb_button);

		linked::Button* soft_orb_button = new linked::Button(*orbs_div, 0, hm::vec2((orbs_size + 10) * 2 + 18, 0), orbs_size, orbs_size, hm::vec4(0, 0, 0, 1), 0);
		soft_orb_button->setAllBGTexture(orb_textures[1]);
		orbs_div->getButtons().push_back(soft_orb_button);

		linked::Button* vr_orb_button = new linked::Button(*orbs_div, 0, hm::vec2((orbs_size + 10) * 3 + 18, 0), orbs_size, orbs_size, hm::vec4(0, 0, 0, 1), 0);
		vr_orb_button->setAllBGTexture(orb_textures[2]);
		orbs_div->getButtons().push_back(vr_orb_button);

		linked::Button* bios_orb_button = new linked::Button(*orbs_div, 0, hm::vec2((orbs_size + 10) * 4 + 18, 0), orbs_size, orbs_size, hm::vec4(0, 0, 0, 1), 0);
		bios_orb_button->setAllBGTexture(orb_textures[3]);
		orbs_div->getButtons().push_back(bios_orb_button);

		linked::Button* exchange_orb_button = new linked::Button(*orbs_div, 0, hm::vec2((orbs_size + 10) * 5 + 18, 0), orbs_size, orbs_size, hm::vec4(0, 0, 0, 1), 0);
		Texture* xchg_orb_texture = new Texture("../../../res/orbs/exchange_orb.png");
		exchange_orb_button->setAllBGTexture(xchg_orb_texture);
		hm::vec4 exchange_orb_button_hovered_bgcolor(0, 1, 1, 0.7f);
		exchange_orb_button->setHoveredBGColor(exchange_orb_button_hovered_bgcolor);
		hm::vec4 exchange_orb_button_held_bgcolor(0, 0.38f, 0.32f, 0.7f);
		exchange_orb_button->setHeldBGColor(exchange_orb_button_held_bgcolor);
		orbs_div->getButtons().push_back(exchange_orb_button);
		exchange_orb_button->setClickedCallback(button_exchange_orb);

		r32 orb_pos_offset = 0;
		linked::Label* multiple_orb_label = new linked::Label(*orbs_div, (u8*)"0", sizeof("0"), hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_24, 0, 0);
		orb_pos_offset = 64.0f / 2.0f - multiple_orb_label->getTextWidth() / 2.0f;
		hm::vec2 multiple_orb_label_position(orb_pos_offset, 74);
		multiple_orb_label->setPosition(multiple_orb_label_position);
		orbs_div->getLabels().push_back(multiple_orb_label);
		combat_state.all_orbs_label = multiple_orb_label;

		r32 base_next_orb = (orbs_size + 10) + 18;
		linked::Label* hard_orb_label = new linked::Label(*orbs_div, (u8*)"0", sizeof("0"), hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_24, 0, 0);
		hard_orb_label->setPosition(hm::vec2(base_next_orb + orb_pos_offset, 74));
		orbs_div->getLabels().push_back(hard_orb_label);
		combat_state.orb_labels[ORB_HARD] = hard_orb_label;
		base_next_orb = (orbs_size + 10) * 2 + 18;
		linked::Label* soft_orb_label = new linked::Label(*orbs_div, (u8*)"0", sizeof("0"), hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_24, 0, 0);
		soft_orb_label->setPosition(hm::vec2(base_next_orb + orb_pos_offset, 74));
		orbs_div->getLabels().push_back(soft_orb_label);
		combat_state.orb_labels[ORB_SOFT] = soft_orb_label;	
		base_next_orb = (orbs_size + 10) * 3 + 18;
		linked::Label* vr_orb_label = new linked::Label(*orbs_div, (u8*)"0", sizeof("0"), hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_24, 0, 0);
		vr_orb_label->setPosition(hm::vec2(base_next_orb + orb_pos_offset, 74));
		orbs_div->getLabels().push_back(vr_orb_label);
		combat_state.orb_labels[ORB_VR] = vr_orb_label;
		base_next_orb = (orbs_size + 10) * 4 + 18;
		linked::Label* bios_orb_label = new linked::Label(*orbs_div, (u8*)"0", sizeof("0"), hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_24, 0, 0);
		bios_orb_label->setPosition(hm::vec2(base_next_orb + orb_pos_offset, 74));
		orbs_div->getLabels().push_back(bios_orb_label);
		combat_state.orb_labels[ORB_BIOS] = bios_orb_label;

		combat_bottom_info->setBorderSizeY(4.0f);
		combat_bottom_info->setBorderColor(greener_cyan);
		gw.combat_bottom_info = combat_bottom_info;

		float skill_desc_height = 40.0f;
		linked::WindowDiv* skill_image_div = new linked::WindowDiv(*combat_bottom_info, 120, 120, 0, 0, hm::vec2(640.0f, skill_desc_height), hm::vec4(0, 1, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		skill_image_div->createBorder(3, 3, 3, 3);
		combat_bottom_info->divs.push_back(skill_image_div);
		skill_image_div->m_render = false;

		linked::WindowDiv* skill_title_div = new linked::WindowDiv(*combat_bottom_info, 300, 48, 0, 0, hm::vec2(780.0f, skill_desc_height), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_title_div);
		linked::Label* skill_title_label = new linked::Label(*skill_title_div, (u8*)"", 0, hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_18, 0, 0);
		skill_title_div->getLabels().push_back(skill_title_label);
		skill_title_div->m_render = false;

		linked::WindowDiv* skill_desc_div = new linked::WindowDiv(*combat_bottom_info, 300, 48, 0, 0, hm::vec2(780.0f, skill_desc_height + 28 + 12), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_desc_div);
		linked::Label* skill_desc_label = new linked::Label(*skill_desc_div, (u8*)"", 0, hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_LIGHT_14, 0, 0);
		skill_desc_div->getLabels().push_back(skill_desc_label);
		skill_desc_div->m_render = false;

		linked::WindowDiv* skill_group_div = new linked::WindowDiv(*combat_bottom_info, 600, 48, 0, 0, hm::vec2(780.0f, skill_desc_height + 126 - 18), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_group_div);
		linked::Label* skill_group_label = new linked::Label(*skill_group_div, (u8*)"", 0, hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_LIGHT_16, 0, 0);
		skill_group_div->getLabels().push_back(skill_group_label);
		linked::Label* skill_cooldown_label = new linked::Label(*skill_group_div, (u8*)"", 0, hm::vec2(540, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_LIGHT_16, 0, 0);
		skill_group_div->getLabels().push_back(skill_cooldown_label);
		skill_desc_div->m_render = false;

		float orb_size = 32.0f;
		linked::WindowDiv* skill_cost_div_1 = new linked::WindowDiv(*combat_bottom_info, orb_size, orb_size, 0, 0, hm::vec2(1300.0f, skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_cost_div_1);
		linked::WindowDiv* skill_cost_div_2 = new linked::WindowDiv(*combat_bottom_info, orb_size, orb_size, 0, 0, hm::vec2(1300.0f + (orb_size + 5), skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_cost_div_2);
		linked::WindowDiv* skill_cost_div_3 = new linked::WindowDiv(*combat_bottom_info, orb_size, orb_size, 0, 0, hm::vec2(1300.0f + (orb_size + 5) * 2, skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_cost_div_3);
		linked::WindowDiv* skill_cost_div_4 = new linked::WindowDiv(*combat_bottom_info, orb_size, orb_size, 0, 0, hm::vec2(1300.0f + (orb_size + 5) * 3, skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_cost_div_4);
		linked::WindowDiv* skill_cost_div_5 = new linked::WindowDiv(*combat_bottom_info, orb_size, orb_size, 0, 0, hm::vec2(1300.0f + (orb_size + 5) * 4, skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
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
		//combat_state.skill_info_image_border = skill_image_div_border;
		combat_state.skill_info_title = skill_title_div;
		combat_state.skill_info_desc = skill_desc_div;
		combat_state.skill_info_group = skill_group_div;
	}
	
	{
		// Sacrifice Null orb Modal
		linked::Window* null_orb_modal = new linked::Window(440, 260, hm::vec2(window_info.width / 2 - 460 / 2, window_info.height / 2 - 260 / 2), char_window_color, (u8*)"  Sacrifice Orb", sizeof "  Sacrifice Orb",
			linked::W_HEADER | linked::W_BORDER | linked::W_MOVABLE);
		null_orb_modal->setBorderColor(greener_cyan);
		null_orb_modal->setTitleColor(char_window_color);
		null_orb_modal->setTitleCentered(true);
		null_orb_modal->setActive(false);
		gw.null_orb_modal = null_orb_modal;
	}

	{
		// Multiple Orb Modal
		linked::Window* exchange_orbs = new linked::Window(420, 260, hm::vec2(window_info.width / 2 - 460 / 2, window_info.height / 2 - 260 / 2), char_window_color, (u8*)"  Exchange Orbs", sizeof "  Exchange Orbs",
			linked::W_HEADER|linked::W_BORDER|linked::W_MOVABLE);
		exchange_orbs->setBorderColor(greener_cyan);
		exchange_orbs->setTitleColor(char_window_color);
		exchange_orbs->setTitleCentered(true);
		gw.exchange_orbs = exchange_orbs;
		exchange_orbs->setActive(false);

		// Close Multiple Orb Modal
		linked::WindowDiv* close_div = new linked::WindowDiv(*exchange_orbs, 360, 40, 0, 0, hm::vec2(0, -20), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_BOTTOM | linked::DIV_CENTER_X);
		exchange_orbs->divs.push_back(close_div);
		linked::Label* close_label = new linked::Label(*close_div, (u8*)"CANCEL", sizeof("CANCEL"), hm::vec2(20, 10), hm::vec4(0, 0, 0.2f, 1.0f), FONT_OSWALD_REGULAR_16, 0, 0);
		close_label->setPosition(hm::vec2((100.0f - close_label->getTextWidth()) / 2.0f, 10));
		linked::Button* close_button = new linked::Button(*close_div, close_label, hm::vec2(200, 0), 100, 40, greener_cyan, 0);
		close_button->setHoveredBGColor(greener_cyan - hm::vec4(0.1f, 0.1f, 0.1f, 0.0f));
		close_button->setHoveredTextColor(char_window_color + hm::vec4(0.2f, 0.2f, 0.2f, 0.0f));
		close_button->setClickedCallback(button_exchange_orbs_close);
		close_div->getButtons().push_back(close_button);

		linked::Label* confirm_label = new linked::Label(*close_div, (u8*)"CONFIRM", sizeof("CONFIRM"), hm::vec2(10, 10), hm::vec4(0, 0, 0.2f, 1.0f), FONT_OSWALD_REGULAR_16, 0, 0);
		confirm_label->setPosition(hm::vec2((100.0f - confirm_label->getTextWidth()) / 2.0f, 10));
		linked::Button* confirm_button = new linked::Button(*close_div, confirm_label, hm::vec2(60, 0), 100, 40, greener_cyan, 0);
		confirm_button->setInactiveAllBGColor(hm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
		confirm_button->setHoveredBGColor(greener_cyan - hm::vec4(0.1f, 0.1f, 0.1f, 0.0f));
		confirm_button->setHoveredTextColor(char_window_color + hm::vec4(0.2f, 0.2f, 0.2f, 0.0f));
		confirm_button->setClickedCallback(button_exchange_orbs_confirm);
		close_div->getButtons().push_back(confirm_button);

		// Multiple Orb Modal Orbs
		linked::WindowDiv* info_div = new linked::WindowDiv(*exchange_orbs, 220, 120, 0, 0, hm::vec2(0, 30), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP|linked::DIV_CENTER_X);
		exchange_orbs->divs.push_back(info_div);
		combat_state.exchange_orbs_state.info_div = info_div;

		float orbs_size = 32.0f;
		float orbs_height = 36.0f;

		linked::Button* multiple_orb_button = new linked::Button(*info_div, 0, hm::vec2(0, orbs_height), orbs_size, orbs_size, hm::vec4(0, 0, 0, 1), 0);
		Texture* multiple_orbs_texture = new Texture("../../../res/orbs/all_orbs.png");
		multiple_orb_button->setAllBGTexture(multiple_orbs_texture);
		info_div->getButtons().push_back(multiple_orb_button);

		linked::Button* hard_orb_button = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) + 18, orbs_height), orbs_size, orbs_size, hm::vec4(0, 0, 0, 1), 0);
		hard_orb_button->setAllBGTexture(orb_textures[0]);
		info_div->getButtons().push_back(hard_orb_button);

		linked::Button* soft_orb_button = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) * 2 + 18, orbs_height), orbs_size, orbs_size, hm::vec4(0, 0, 0, 1), 0);
		soft_orb_button->setAllBGTexture(orb_textures[1]);
		info_div->getButtons().push_back(soft_orb_button);

		linked::Button* vr_orb_button = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) * 3 + 18, orbs_height), orbs_size, orbs_size, hm::vec4(0, 0, 0, 1), 0);
		vr_orb_button->setAllBGTexture(orb_textures[2]);
		info_div->getButtons().push_back(vr_orb_button);

		linked::Button* bios_orb_button = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) * 4 + 18, orbs_height), orbs_size, orbs_size, hm::vec4(0, 0, 0, 1), 0);
		bios_orb_button->setAllBGTexture(orb_textures[3]);
		info_div->getButtons().push_back(bios_orb_button);
		
		linked::Label* multiple_orb_label = new linked::Label(*info_div, (u8*)"0", sizeof("0"), hm::vec2(0, 72), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_14, 0, 0);
		r32 text_width = multiple_orb_label->getTextWidth();
		multiple_orb_label->setPosition(hm::vec2(orbs_size / 2.0f - text_width / 2.0f, 72));
		info_div->getLabels().push_back(multiple_orb_label);

		r32 spacing = (18 + (orbs_size + 10)) + (orbs_size / 2.0f - text_width / 2.0f);
		linked::Label* hard_orb_label = new linked::Label(*info_div, (u8*)"0", sizeof("0"), hm::vec2(spacing, 72), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_14, 0, 0);
		info_div->getLabels().push_back(hard_orb_label);
		spacing += (orbs_size + 10);
		linked::Label* soft_orb_label = new linked::Label(*info_div, (u8*)"0", sizeof("0"), hm::vec2(spacing, 72), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_14, 0, 0);
		info_div->getLabels().push_back(soft_orb_label);
		spacing += (orbs_size + 10);
		linked::Label* vr_orb_label = new linked::Label(*info_div, (u8*)"0", sizeof("0"), hm::vec2(spacing, 72), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_14, 0, 0);
		info_div->getLabels().push_back(vr_orb_label);
		spacing += (orbs_size + 10);
		linked::Label* bios_orb_label = new linked::Label(*info_div, (u8*)"0", sizeof("0"), hm::vec2(spacing, 72), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_14, 0, 0);
		info_div->getLabels().push_back(bios_orb_label);		

		Texture* arrow_up = new Texture("../../../res/orbs/arrow_up.png");
		Texture* arrow_down = new Texture("../../../res/orbs/arrow_down.png");
		float arrow_up_size = 24.0f;
		hm::vec4 arrow_hovered_color = hm::vec4(1, 1, 1, 0.1f);
		hm::vec4 arrow_normal_color = greener_cyan;
		arrow_normal_color.z = 0.0f;
		// up
		linked::Button* hard_orb_arrow_up = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) + 22, 8), arrow_up_size, arrow_up_size, arrow_normal_color, ORB_HARD);
		hard_orb_arrow_up->setAllBGTexture(arrow_up);
		hard_orb_arrow_up->setHoveredBGColor(arrow_hovered_color);
		hard_orb_arrow_up->setInactiveAllBGColor(darker_gray);
		hard_orb_arrow_up->setClickedCallback(button_exchange_orb_arrow);
		hard_orb_arrow_up->button_info.data = (void*)ARROW_UP;
		info_div->getButtons().push_back(hard_orb_arrow_up);

		linked::Button* soft_orb_arrow_up = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) * 2 + 22, 8), arrow_up_size, arrow_up_size, arrow_normal_color, ORB_SOFT);
		soft_orb_arrow_up->setAllBGTexture(arrow_up);
		soft_orb_arrow_up->setHoveredBGColor(arrow_hovered_color);
		soft_orb_arrow_up->setInactiveAllBGColor(darker_gray);
		soft_orb_arrow_up->setClickedCallback(button_exchange_orb_arrow);
		soft_orb_arrow_up->button_info.data = (void*)ARROW_UP;
		info_div->getButtons().push_back(soft_orb_arrow_up);

		linked::Button* vr_orb_arrow_up = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) * 3 + 22, 8), arrow_up_size, arrow_up_size, arrow_normal_color, ORB_VR);
		vr_orb_arrow_up->setAllBGTexture(arrow_up);
		vr_orb_arrow_up->setHoveredBGColor(arrow_hovered_color);
		vr_orb_arrow_up->setInactiveAllBGColor(darker_gray);
		vr_orb_arrow_up->setClickedCallback(button_exchange_orb_arrow);
		vr_orb_arrow_up->button_info.data = (void*)ARROW_UP;
		info_div->getButtons().push_back(vr_orb_arrow_up);

		linked::Button* bios_orb_arrow_up = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) * 4 + 22, 8), arrow_up_size, arrow_up_size, arrow_normal_color, ORB_BIOS);
		bios_orb_arrow_up->setAllBGTexture(arrow_up);
		bios_orb_arrow_up->setHoveredBGColor(arrow_hovered_color);
		bios_orb_arrow_up->setInactiveAllBGColor(darker_gray);
		bios_orb_arrow_up->setClickedCallback(button_exchange_orb_arrow);
		bios_orb_arrow_up->button_info.data = (void*)ARROW_UP;
		info_div->getButtons().push_back(bios_orb_arrow_up);

		// down
		linked::Button* hard_orb_arrow_down = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) + 22, 72 + 18), arrow_up_size, arrow_up_size, arrow_normal_color, ORB_HARD);
		hard_orb_arrow_down->setAllBGTexture(arrow_down);
		hard_orb_arrow_down->setHoveredBGColor(arrow_hovered_color);
		hard_orb_arrow_down->setInactiveAllBGColor(darker_gray);
		hard_orb_arrow_down->setClickedCallback(button_exchange_orb_arrow);
		hard_orb_arrow_down->button_info.data = (void*)ARROW_DOWN;
		info_div->getButtons().push_back(hard_orb_arrow_down);

		linked::Button* soft_orb_arrow_down = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) * 2 + 22, 72 + 18), arrow_up_size, arrow_up_size, arrow_normal_color, ORB_SOFT);
		soft_orb_arrow_down->setAllBGTexture(arrow_down);
		soft_orb_arrow_down->setHoveredBGColor(arrow_hovered_color);
		soft_orb_arrow_down->setInactiveAllBGColor(darker_gray);
		soft_orb_arrow_down->setClickedCallback(button_exchange_orb_arrow);
		soft_orb_arrow_down->button_info.data = (void*)ARROW_DOWN;
		info_div->getButtons().push_back(soft_orb_arrow_down);

		linked::Button* vr_orb_arrow_down = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) * 3 + 22, 72 + 18), arrow_up_size, arrow_up_size, arrow_normal_color, ORB_VR);
		vr_orb_arrow_down->setAllBGTexture(arrow_down);
		vr_orb_arrow_down->setHoveredBGColor(arrow_hovered_color);
		vr_orb_arrow_down->setInactiveAllBGColor(darker_gray);
		vr_orb_arrow_down->setClickedCallback(button_exchange_orb_arrow);
		vr_orb_arrow_down->button_info.data = (void*)ARROW_DOWN;
		info_div->getButtons().push_back(vr_orb_arrow_down);

		linked::Button* bios_orb_arrow_down = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) * 4 + 22, 72 + 18), arrow_up_size, arrow_up_size, arrow_normal_color, ORB_BIOS);
		bios_orb_arrow_down->setAllBGTexture(arrow_down);
		bios_orb_arrow_down->setHoveredBGColor(arrow_hovered_color);
		bios_orb_arrow_down->setInactiveAllBGColor(darker_gray);
		bios_orb_arrow_down->setClickedCallback(button_exchange_orb_arrow);
		bios_orb_arrow_down->button_info.data = (void*)ARROW_DOWN;
		info_div->getButtons().push_back(bios_orb_arrow_down);

		Texture* reset_button_texture = new Texture("../../../res/orbs/reset.png");
		linked::Button* reset_button = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) * 5 + 22, 72 + 12), 32, 32, hm::vec4(0, 1, 0.7f, 0), 10);
		reset_button->setAllBGTexture(reset_button_texture);
		reset_button->setHoveredBGColor(hm::vec4(0, 0.7f, 0.5f, 0));
		info_div->getButtons().push_back(reset_button);
	}
	{
		// Status stuff
		Texture* default_status = new Texture("../../../res/status/status_default.png");
		g_layout_status.status_images[SKILL_CONDITION_BURN] = new Texture("../../../res/status/burn.png");
		g_layout_status.status_images[SKILL_CONDITION_FREEZE] = new Texture("../../../res/status/freeze.png");
		g_layout_status.status_images[SKILL_CONDITION_POISON] = new Texture("../../../res/status/poison.png");
		g_layout_status.status_images[SKILL_CONDITION_PARALYZE] = new Texture("../../../res/status/paralyze.png");
		g_layout_status.status_images[SKILL_CONDITION_SLEEP] = new Texture("../../../res/status/sleep.png");
		g_layout_status.status_images[SKILL_CONDITION_STUN] = new Texture("../../../res/status/stun.png");
	}
}

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

	// @Temporary
	gw.null_orb_modal->setActive(true);
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
		for (int k = 0; k < MAX(NUM_ALLIES, NUM_ENEMIES); ++k) {
			s32 t = combat_state.player.targets[i].enemy_target_index[k];
			if (t != -1)
				target = k;
		}
		if (target == -1) {
			for (int k = 0; k < NUM_ALLIES; ++k) {
				s32 t = combat_state.player.targets[i].ally_target_index[k];
				if (t != -1)
					target = k;
			}
		}

		#if MULTIPLAYER
			if (combat_state.player_turn)
				execute_skill(combat_state.player.targets[i].skill_used, target, combat_state.player.targets[i].attacking_character, &combat_state, false, true);
			else
				execute_skill(combat_state.player.targets[i].skill_used, target, combat_state.player.targets[i].attacking_character, &combat_state, true, false);
		#else
			execute_skill(combat_state.player.targets[i].skill_used, target, combat_state.player.targets[i].attacking_character, &combat_state, false, true);
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

// Gameplay functions
void end_turn() {
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
			combat_state.orbs_amount[orb_generated] += 1;

			combat_state.total_orbs += 1;
			layout_change_orb_amount(ORB_ALL, combat_state.total_orbs);
			layout_change_orb_amount((Orb_ID)orb_generated, combat_state.orbs_amount[orb_generated]);
			printf("%d ");
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
			layout_set_timer_percentage(turn_time / TURN_DURATION);

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
					combat_state.skill_info_image->setBorderColor(hm::vec4(enemy_color));
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
	int x = 0;
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
	layout_set_enemy_image_opacity(ally_index, 0.5f, hm::vec4(0, 0, 0, 1));
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

static void layout_change_exchange_orb_amount(Orb_ID id, int amt) {
	static char all_orbs_buffer[16] = { 0 };
	static char hard_orb_buffer[16] = { 0 };
	static char soft_orb_buffer[16] = { 0 };
	static char vr_orb_buffer[16] = { 0 };
	static char bios_orb_buffer[16] = { 0 };

	char* selected = 0;
	linked::Label* label = 0;
	linked::WindowDiv* info_div = combat_state.exchange_orbs_state.info_div;
	

	switch (id) {
	case ORB_HARD: selected = (char*)&hard_orb_buffer; label = info_div->getLabels()[ORB_HARD + 1]; break;
	case ORB_SOFT: selected = (char*)&soft_orb_buffer; label = info_div->getLabels()[ORB_SOFT + 1]; break;
	case ORB_VR:   selected = (char*)&vr_orb_buffer;   label = info_div->getLabels()[ORB_VR + 1]; break;
	case ORB_BIOS: selected = (char*)&bios_orb_buffer; label = info_div->getLabels()[ORB_BIOS + 1]; break;
	case ORB_ALL:  selected = (char*)&all_orbs_buffer; label = info_div->getLabels()[0]; break;
	case ORB_NULL: return;
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
		combat_state.end_turn_button->setNormalBGColor(greener_cyan - hm::vec4(0.2f, 0.2f, 0.2f, 0.0f));
		combat_state.end_turn_button->setHoveredBGColor(hm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		combat_state.end_turn_button->setHoveredBGColor(greener_cyan - hm::vec4(0.4f, 0.35f, 0.4f, 0.0f));
		combat_state.end_turn_button->setHeldBGColor(hm::vec4(0.4f, 0.65f, 0.45f, 1.0f));
	} else {
		end_turn_label->setText((u8*)"ENEMY TURN", sizeof("ENEMY TURN"));
		combat_state.end_turn_button->setNormalBGColor(hm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		combat_state.end_turn_button->setHoveredBGColor(hm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		combat_state.end_turn_button->setHeldBGColor(hm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
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