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

char** skill_desc = 0;
int* skill_desc_length = 0;

char* skill_desc_pt[NUM_SKILLS * NUM_CHARS] = {
	// Zer0
	"Ataca um oponente e realiza 20 de dano.",
	"Marca um adversário e, se no próximo turno ele realizar um ataque em Zer0, seu golpe é negado, \nsofre 20 de dano e recebe status Paralyze por 1 turno.",
	"Cria uma atmosfera negativa no campo de batalha. False Rush e Contradiction acertam todos os adversários\ndurante 3 turnos.",
	"Utilizando uma barreira criada após um vácuo, Zer0 fica invulnerável por 1 turno.",

	// On1
	"Ataca um oponente e realiza 30 de dano.",
	"Ataca um inimigo e realiza 15 de dano piercing. Se, no próximo turno, o adversário utilizar\nqualquer habilidade, sofrerá 15 de dano crushing.",
	"On1 canaliza sua positividade e aumenta suas habilidades. Por 3 turnos, On1 tem redução de dano de 15%\ne Truth Slash e Tautology gastam uma energia a menos.",
	"Concentrando toda sua energia positiva, On1 obtém uma defesa absoluta e fica invulnerável por 1 turno.",

	// Serial
	"Ataca um oponente e realiza 20 de dano. Este golpe ignora INVUL. Porém, o adversário recebe 10 de dano\nse estiver nesse status.",
	"Infringe 15 de dano piercing a um oponente e adiciona status STUN por 1 turno.",
	"Todos os adversários recebem status PARALYZE por 3 turnos.",
	"Envolto por uma densa camada encriptada, Serial K3yll3R fica invulnerável por 1 turno.",

	// Ray
	"Ataca um oponente, realiza 15 de dano e R4y Tr4c3y fica invulnerável a golpes PHYSICAL por 1 turno.",
	"Por 1 turno, todos os aliados recebem REFLEX DEFENSE.",
	"Aplica um golpe em todos os inimigos e realiza 35 de dano normal.",
	"Usando suas técnicas de computação gráfica, R4y Tr4c3y fica invulnerável por 1 turno.",

	// AStar
	"Ataca um oponente e realiza 20 de dano. Para cada vez que um oponente recebe novamente este golpe, o dano\né aumentado em 5 pontos. O efeito é acumulativo.",
	"Produz 25 de dano crushing a um oponente.",
	"Durante 4 turnos, todos os adversários atingidos por Neural Network podem receber o ataque Perceptron.\nA-St4r recebe 30 pontos de ABSORPTION.",
	"Para se proteger, A-St4r corre até as colinas e fica invulnerável por 1 turno.",

	// Deadlock
	"Ataca um oponente e infringe 25 de dano. Caso ele tenha alguma habilidade com duração CONTROL, elimina a\nhabilidade e este sofre status SLEEP por 2 turnos.",
	"Atinge todos os adversários e, no próximo turno, apenas um pode utilizar alguma habilidade.",
	"Acerta um inimigo aleatório e coloca o status SLEEP por 3 turnos.",
	"D3ADL0CK cria um clone seu para receber golpe no seu lugar, engana o adversário e fica invulnerável por 1 turno.",

	// Norma
	"Ataca um oponente, realiza 25 de dano piercing e há 50% de chance de causar status BURN por 3 turnos.",
	"Invoca um aliado, garantindo 15 pontos de defesa ABSORPTION. O adversário que atacar N0rma recebe o status\nPOISON por 2 turnos.",
	"Acerta todos os adversários com 30 de dano. Só pode ser utilizado enquanto a habilidade Automata Summon estiver ativa.",
	"Aproveitando o universo não-determinístico, N0rma se esconde num estado protegido e fica invulnerável por 1 turno.",

	// Hazard
	"Ataca todos os oponentes e realiza 15 de dano. Haz4rd recebe 5 pontos de defesa ABSORPTION por cada ataque bem sucedido.",
	"Cria uma barreira redundante para os aliados. Cada personagem recebe 15 pontos de redução de defesa por 3 turnos.",
	"Cura qualquer status negativo de um aliado.",
	"Para escapar de uma situação adversa, Haz4rd utiliza sua técnica de tolerância a falhas para avançar a um estado seguro,\nficando invulnerável por 1 turno.",

	// Qwerty
	"Qwerty altera sua forma física e ganha redução de dano de 40 pontos por 4 turnos.",
	"Controla um inimigo por dois turnos. DELETE pode ser usado no adversário controlado.",
	"Mata instantaneamente qualquer adversário que recebe este golpe, desde que esteja sob efeito de CTRL.",
	"Em meio ao perigo, Qwerty utiliza seus instintos para escapar da batalha e fica invulnerável por 1 turno.",

	// Big O
	"Ataca o oponente com menor HP e realiza 20 de dano. Por 2 turnos, o alvo fica com status BURN.\nSe o alvo já possui status BURN, este sofre 10 de dano crushing adicional.",
	"Se utilizado em um adversário, gera status BURN por 3 turnos. Se, em um aliado, recupera 25 de energia\ne atribui status FROZEN por 2 turnos.",
	"No próximo turno, recebe 4 orbs de energia: uma de cada categoria.",
	"=Big-O= cria uma ilusão nos adversários, possibilitando uma fuga estratégica que o torna invulnerável por 1 turno.",

	// New
	"Ataca um oponente e realiza 25 de dano. New() recebe redução de dano de 10 pontos no próximo turno.",
	"Seleciona um inimigo e se, no próximo turno, ele utilizar um ataque que não seja UNIQUE nem exija uma\ntécnica de preparação, sua habilidade será copiada e poderá ser usada por New() também.",
	"Cura todos os status negativos de um aliado.",
	"Alterando sua estrutura física de forma dinâmica, New() fica invulnerável por 1 turno.",

	// Clockboy
	"Desfera 20 de dano a um adversário. Atribui status PARALYZE por 2 turnos.",
	"Durante 2 turnos, nenhum golpe de Cl0ckB0y sofre COUNTER.",
	"Cl0ckb0y fica invulnerável por 2 turnos, perde 50% de sua energia e seus ataques causam 500% a mais de dano.",
	"Assumindo que a batalha levou a uma condição de dano, Cl0ckB0y faz um pulo condicional que o torna invulnerável por 1 turno."
};

int skill_desc_pt_length[sizeof(skill_desc_pt) / sizeof(char*)] = {
	// Zer0
	sizeof "Ataca um oponente e realiza 20 de dano.",
	sizeof "Marca um adversário e, se no próximo turno ele realizar um ataque em Zer0, seu golpe é negado, \nsofre 20 de dano e recebe status Paralyze por 1 turno.",
	sizeof "Cria uma atmosfera negativa no campo de batalha. False Rush e Contradiction acertam todos os adversários\ndurante 3 turnos.",
	sizeof "Utilizando uma barreira criada após um vácuo, Zer0 fica invulnerável por 1 turno.",

	// On1
	sizeof "Ataca um oponente e realiza 30 de dano.",
	sizeof "Ataca um inimigo e realiza 15 de dano piercing. Se, no próximo turno, o adversário utilizar\nqualquer habilidade, sofrerá 15 de dano crushing.",
	sizeof "On1 canaliza sua positividade e aumenta suas habilidades. Por 3 turnos, On1 tem redução de dano de 15%\ne Truth Slash e Tautology gastam uma energia a menos.",
	sizeof "Concentrando toda sua energia positiva, On1 obtém uma defesa absoluta e fica invulnerável por 1 turno.",

	// Serial
	sizeof "Ataca um oponente e realiza 20 de dano. Este golpe ignora INVUL. Porém, o adversário recebe 10 de dano\nse estiver nesse status.",
	sizeof "Infringe 15 de dano piercing a um oponente e adiciona status STUN por 1 turno.",
	sizeof "Todos os adversários recebem status PARALYZE por 3 turnos.",
	sizeof "Envolto por uma densa camada encriptada, Serial K3yll3R fica invulnerável por 1 turno.",

	// Ray
	sizeof "Ataca um oponente, realiza 15 de dano e R4y Tr4c3y fica invulnerável a golpes PHYSICAL por 1 turno.",
	sizeof "Por 1 turno, todos os aliados recebem REFLEX DEFENSE.",
	sizeof "Aplica um golpe em todos os inimigos e realiza 35 de dano normal.",
	sizeof "Usando suas técnicas de computação gráfica, R4y Tr4c3y fica invulnerável por 1 turno.",

	// AStar
	sizeof "Ataca um oponente e realiza 20 de dano. Para cada vez que um oponente recebe novamente este golpe, o dano\né aumentado em 5 pontos. O efeito é acumulativo.",
	sizeof "Produz 25 de dano crushing a um oponente.",
	sizeof "Durante 4 turnos, todos os adversários atingidos por Neural Network podem receber o ataque Perceptron.\nA-St4r recebe 30 pontos de ABSORPTION.",
	sizeof "Para se proteger, A-St4r corre até as colinas e fica invulnerável por 1 turno.",

	// Deadlock
	sizeof "Ataca um oponente e infringe 25 de dano. Caso ele tenha alguma habilidade com duração CONTROL, elimina a\nhabilidade e este sofre status SLEEP por 2 turnos.",
	sizeof "Atinge todos os adversários e, no próximo turno, apenas um pode utilizar alguma habilidade.",
	sizeof "Acerta um inimigo aleatório e coloca o status SLEEP por 3 turnos.",
	sizeof "D3ADL0CK cria um clone seu para receber golpe no seu lugar, engana o adversário e fica invulnerável por 1 turno.",

	// Norma
	sizeof "Ataca um oponente, realiza 25 de dano piercing e há 50% de chance de causar status BURN por 3 turnos.",
	sizeof "Invoca um aliado, garantindo 15 pontos de defesa ABSORPTION. O adversário que atacar N0rma recebe o status\nPOISON por 2 turnos.",
	sizeof "Acerta todos os adversários com 30 de dano. Só pode ser utilizado enquanto a habilidade Automata Summon estiver ativa.",
	sizeof "Aproveitando o universo não-determinístico, N0rma se esconde num estado protegido e fica invulnerável por 1 turno.",

	// Hazard
	sizeof "Ataca todos os oponentes e realiza 15 de dano. Haz4rd recebe 5 pontos de defesa ABSORPTION por cada ataque bem sucedido.",
	sizeof "Cria uma barreira redundante para os aliados. Cada personagem recebe 15 pontos de redução de defesa por 3 turnos.",
	sizeof "Cura qualquer status negativo de um aliado.",
	sizeof "Para escapar de uma situação adversa, Haz4rd utiliza sua técnica de tolerância a falhas para avançar a um estado seguro,\nficando invulnerável por 1 turno.",

	// Qwerty
	sizeof "Qwerty altera sua forma física e ganha redução de dano de 40 pontos por 4 turnos.",
	sizeof "Controla um inimigo por dois turnos. DELETE pode ser usado no adversário controlado.",
	sizeof "Mata instantaneamente qualquer adversário que recebe este golpe, desde que esteja sob efeito de CTRL.",
	sizeof "Em meio ao perigo, Qwerty utiliza seus instintos para escapar da batalha e fica invulnerável por 1 turno.",

	// Big O
	sizeof "Ataca o oponente com menor HP e realiza 20 de dano. Por 2 turnos, o alvo fica com status BURN.\nSe o alvo já possui status BURN, este sofre 10 de dano crushing adicional.",
	sizeof "Se utilizado em um adversário, gera status BURN por 3 turnos. Se, em um aliado, recupera 25 de energia\ne atribui status FROZEN por 2 turnos.",
	sizeof "No próximo turno, recebe 4 orbs de energia: uma de cada categoria.",
	sizeof "=Big-O= cria uma ilusão nos adversários, possibilitando uma fuga estratégica que o torna invulnerável por 1 turno.",

	// New
	sizeof "Ataca um oponente e realiza 25 de dano. New() recebe redução de dano de 10 pontos no próximo turno.",
	sizeof "Seleciona um inimigo e se, no próximo turno, ele utilizar um ataque que não seja UNIQUE nem exija uma\ntécnica de preparação, sua habilidade será copiada e poderá ser usada por New() também.",
	sizeof "Cura todos os status negativos de um aliado.",
	sizeof "Alterando sua estrutura física de forma dinâmica, New() fica invulnerável por 1 turno.",

	// Clockboy
	sizeof "Desfera 20 de dano a um adversário. Atribui status PARALYZE por 2 turnos.",
	sizeof "Durante 2 turnos, nenhum golpe de Cl0ckB0y sofre COUNTER.",
	sizeof "Cl0ckb0y fica invulnerável por 2 turnos, perde 50% de sua energia e seus ataques causam 500% a mais de dano.",
	sizeof "Assumindo que a batalha levou a uma condição de dano, Cl0ckB0y faz um pulo condicional que o torna invulnerável por 1 turno."
};

char* skill_desc_en[NUM_SKILLS * NUM_CHARS] = {
	// Zer0
	//"Ataca um oponente e realiza 20 de dano.",
	"Attacks an opponent dealing 20 damage.",
	//"Marca um adversário e, se no próximo turno ele realizar um ataque em Zer0, seu golpe é negado, \nsofre 20 de dano e recebe status Paralyze por 1 turno.",
	"Marks an enemy and, if in the next turn they make an attack on Zer0, their attack is denied, \nsuffer 20 damage and receive Paralyze status for 1 turn.",
	//"Cria uma atmosfera negativa no campo de batalha. False Rush e Contradiction acertam todos os adversários\ndurante 3 turnos.",
	"Creates a negative atmosphere in the battle field. False Rush and Contradiction hit all the enemies for 3 turns.",
	//"Utilizando uma barreira criada após um vácuo, Zer0 fica invulnerável por 1 turno.",
	"Utilizing a shield created after a vacuum, Zer0 stays invulnerable for 1 turn.",

	// On1
	//"Ataca um oponente e realiza 30 de dano.",
	"Attacks an opponent and deals 30 damage.",
	//"Ataca um inimigo e realiza 15 de dano piercing. Se, no próximo turno, o adversário utilizar\nqualquer habilidade, sofrerá 15 de dano crushing.",
	"Attacks an enemy and deals 15 piercing damage. If, in the next turn, the enemy utilize any ability,\n they will receive 15 crushing damage.",
	//"On1 canaliza sua positividade e aumenta suas habilidades. Por 3 turnos, On1 tem redução de dano de 15%\ne Truth Slash e Tautology gastam uma energia a menos.",
	"On1 channels his positivity and increases his abilities. For 3 turns, On1 has 15% damage reduction,\nTruth Slash and Tautology cost one energy less.",
	//"Concentrando toda sua energia positiva, On1 obtém uma defesa absoluta e fica invulnerável por 1 turno.",
	"Concentrating all his positive energy, On1 acquires an absolute defense and stays invulnerable for 1 turn.",

	// Serial
	//"Ataca um oponente e realiza 20 de dano. Este golpe ignora INVUL. Porém, o adversário recebe 10 de dano\nse estiver nesse status.",
	"Attacks an opponent and deals 20 damage. This ability ignores INVUL. But, the enemy receives 10 damage\nif they are in that state.",
	//Infringe 15 de dano piercing a um oponente e adiciona status STUN por 1 turno.",
	"Inflicts 15 piercing damage to an opponent and applies STUN status for 1 turn.",
	//"Todos os adversários recebem status PARALYZE por 3 turnos.",
	"All the enemies receive PARALYZE status for 3 turns.",
	//"Envolto por uma densa camada encriptada, Serial K3yll3R fica invulnerável por 1 turno.",
	"Surrounded by a dense encrypted layer, Serial K3yll3R stays invulnerable for 1 turn.",

	// Ray
	//"Ataca um oponente, realiza 15 de dano e R4y Tr4c3y fica invulnerável a golpes PHYSICAL por 1 turno.",
	"Attacks an opponent, deals 15 damage and R4y Tr4c3y stays invulnerable to PHYSICAL attacks for 1 turn.",
	//"Por 1 turno, todos os aliados recebem REFLEX DEFENSE.",
	"For 1 turn, all the allies receive REFLEX DEFENSE.",
	//"Aplica um golpe em todos os inimigos e realiza 35 de dano normal.",
	"Apply a hit to all enemies and deals 35 normal damage.",
	//"Usando suas técnicas de computação gráfica, R4y Tr4c3y fica invulnerável por 1 turno.",
	"Using her computer graphics techniques, R4y Tr4c3y stays invulnerable for 1 turn.",

	// AStar
	//"Ataca um oponente e realiza 20 de dano. Para cada vez que um oponente recebe novamente este golpe, o dano\né aumentado em 5 pontos. O efeito é acumulativo.",
	"Attacks an opponent and deals 20 damage. For each time that the opponent receives this ability, the damage\nis increased in 5 points. The effect is cumulative.",
	//"Produz 25 de dano crushing a um oponente.",
	"Produces 25 crushing damage to an opponent.",
	//"Durante 4 turnos, todos os adversários atingidos por Neural Network podem receber o ataque Perceptron.\nA-St4r recebe 30 pontos de ABSORPTION.",
	"During 4 turns, all the enemies hit by Neural Network can be hit by Perceptron.\nA-St4r receives 30 points of ABSORPTION.",
	//"Para se proteger, A-St4r corre até as colinas e fica invulnerável por 1 turno.",
	"To protect herself, A-St4r runs to the hills and stays invulnerable for 1 turn.",

	// Deadlock
	//"Ataca um oponente e infringe 25 de dano. Caso ele tenha alguma habilidade com duração CONTROL, elimina a\nhabilidade e este sofre status SLEEP por 2 turnos.",
	"Attacks an opponent and deals 25 damage. In case they are under any CONTROL duration abilities, eliminates the\nability and undergoes SLEEP status for 2 turns.",
	//"Atinge todos os adversários e, no próximo turno, apenas um pode utilizar alguma habilidade.",
	"Hits all the enemies and, in the next turn, only one can utilize an ability.",
	//"Acerta um inimigo aleatório e coloca o status SLEEP por 3 turnos.",
	"Hits a random enemy and puts them into SLEEP status for 3 turns.",
	//"D3ADL0CK cria um clone seu para receber golpe no seu lugar, engana o adversário e fica invulnerável por 1 turno.",
	"D3ADL0CK creates a clone of himself to receive the hit in his place, tricks the enemy and stays invulnerable for 1 turn.",

	// Norma
	//"Ataca um oponente, realiza 25 de dano piercing e há 50% de chance de causar status BURN por 3 turnos.",
	"Attacks an opponent, deals 25 piercing damage and there is 50% chance of inflicting BURN status for 3 turns.",
	//"Invoca um aliado, garantindo 15 pontos de defesa ABSORPTION. O adversário que atacar N0rma recebe o status\nPOISON por 2 turnos.",
	"Summon an ally, guaranteeing 15 points of ABSORPTION defense. The enemy that attacks N0rma receives the status\nPOISON for 2 turns.",
	//"Acerta todos os adversários com 30 de dano. Só pode ser utilizado enquanto a habilidade Automata Summon estiver ativa.",
	"Hits all the enemies dealing 30 damage. Can only be used when the ability Automata Summon is active.",
	//"Aproveitando o universo não-determinístico, N0rma se esconde num estado protegido e fica invulnerável por 1 turno.",
	"Taking advantage of a non-deterministic universe, N0rma hides in a protected state and stays invulnerable for 1 turn.",

	// Hazard
	//"Ataca todos os oponentes e realiza 15 de dano. Haz4rd recebe 5 pontos de defesa ABSORPTION por cada ataque bem sucedido.",
	"Attacks all the enemies and deals 15 damage. Haz4rd receives 5 points of ABSORPTION defense for each attack that succeeded.",
	//"Cria uma barreira redundante para os aliados. Cada personagem recebe 15 pontos de redução de defesa por 3 turnos.",
	"Creates a redundant barrier for the allies. Each character receives 15 points of damage reduction for 3 turns.",
	//"Cura qualquer status negativo de um aliado.",
	"Removes any negative status of an ally.",
	//"Para escapar de uma situação adversa, Haz4rd utiliza sua técnica de tolerância a falhas para avançar a um estado seguro,\nficando invulnerável por 1 turno.",
	"In order to escape an adverse situation, Haz4rd utilizes his technique of fault tolerance to advance to a safe state,\nstaying invulnerable for 1 turn.",

	// Qwerty
	//"Qwerty altera sua forma física e ganha redução de dano de 40 pontos por 4 turnos.",
	"Qwerty alters his physical form and obtains 40 damage reduction for 4 turns.",
	//"Controla um inimigo por dois turnos. DELETE pode ser usado no adversário controlado.",
	"Controls an enemy for two turns. DELETE can be used in the enemy controlled.",
	//"Mata instantaneamente qualquer adversário que recebe este golpe, desde que esteja sob efeito de CTRL.",
	"Kills any enemy that gets hit by this ability instantly, inasmuch as they are under the effect of CTRL.",
	//"Em meio ao perigo, Qwerty utiliza seus instintos para escapar da batalha e fica invulnerável por 1 turno.",
	"Among danger, Qwerty utilizes his instincts to escape the battle and stays invulnerable for 1 turn.",

	// Big O
	//"Ataca o oponente com menor HP e realiza 20 de dano. Por 2 turnos, o alvo fica com status BURN.\nSe o alvo já possui status BURN, este sofre 10 de dano crushing adicional.",
	"Attacks the opponent with less HP and deals 20 damage. For 2 turns, the target stays with BURN status.\nIf the target is already in BURN status, they suffer an additional 10 crushing damage.",
	//"Se utilizado em um adversário, gera status BURN por 3 turnos. Se, em um aliado, recupera 25 de energia\ne atribui status FROZEN por 2 turnos.",
	"If utilzed in an enemy, generates BURN status for 3 turns. If, in an ally, restores 25 energy and attributes them with FROZEN\nstatus for 2 turns.",
	//"No próximo turno, recebe 4 orbs de energia: uma de cada categoria.",
	"In the next turn, receive 4 orbs of energy: one of each category.",
	//"=Big-O= cria uma ilusão nos adversários, possibilitando uma fuga estratégica que o torna invulnerável por 1 turno.",
	"=Big-O= creates an illusion for the enemies, allowing an strategic escape that makes him invulnerable for 1 turn.",

	// New
	//"Ataca um oponente e realiza 25 de dano. New() recebe redução de dano de 10 pontos no próximo turno.",
	"Attacks an opponent and deals 25 damage. New() receives 10 damage reduction points in the next turn.",
	//"Seleciona um inimigo e se, no próximo turno, ele utilizar um ataque que não seja UNIQUE nem exija uma\ntécnica de preparação, sua habilidade será copiada e poderá ser usada por New() também.",
	"Selects and enemy and if, in the next turn, they utilize an ability that is'nt UNIQUE and does not require\na preparation technique, their ability is copied and may be used by New() also.",
	//"Cura todos os status negativos de um aliado.",
	"Removes any negative status of an ally.",
	//"Alterando sua estrutura física de forma dinâmica, New() fica invulnerável por 1 turno.",
	"Altering her basic structure in a dynamic way, New() stays invulnerable for 1 turn.",

	// Clockboy
	//"Desfera 20 de dano a um adversário. Atribui status PARALYZE por 2 turnos.",
	"Deals 20 damage to an enemy. Attributes the PARALYZE status for 2 turns.",
	//"Durante 2 turnos, nenhum golpe de Cl0ckB0y sofre COUNTER.",
	"During 2 turns, any of the Cl0ckB0y's abilities suffer COUNTER.",
	//"Cl0ckb0y fica invulnerável por 2 turnos, perde 50% de sua energia e seus ataques causam 500% a mais de dano.",
	"Cl0ckb0y stays invulnerable for 2 turns, loses 50% of his energy and his abilities deal 500% more damage.",
	//"Assumindo que a batalha levou a uma condição de dano, Cl0ckB0y faz um pulo condicional que o torna invulnerável por 1 turno."
	"Assuming that the battle got into a damage condition, Cl0ckB0y makes a conditional jump that makes him invulnerable for 1 turn."
};

int skill_desc_en_length[sizeof(skill_desc_en) / sizeof(char*)] = {
	// Zer0
	//"Ataca um oponente e realiza 20 de dano.",
	sizeof ("Attacks an opponent dealing 20 damage."),
	//"Marca um adversário e, se no próximo turno ele realizar um ataque em Zer0, seu golpe é negado, \nsofre 20 de dano e recebe status Paralyze por 1 turno.",
	sizeof "Marks an enemy and, if in the next turn they make an attack on Zer0, their attack is denied, \nsuffer 20 damage and receive Paralyze status for 1 turn.",
	//"Cria uma atmosfera negativa no campo de batalha. False Rush e Contradiction acertam todos os adversários\ndurante 3 turnos.",
	sizeof "Creates a negative atmosphere in the battle field. False Rush and Contradiction hit all the enemies for 3 turns.",
	//"Utilizando uma barreira criada após um vácuo, Zer0 fica invulnerável por 1 turno.",
	sizeof "Utilizing a shield created after a vacuum, Zer0 stays invulnerable for 1 turn.",

	// On1
	//"Ataca um oponente e realiza 30 de dano.",
	sizeof "Attacks an opponent and deals 30 damage.",
	//"Ataca um inimigo e realiza 15 de dano piercing. Se, no próximo turno, o adversário utilizar\nqualquer habilidade, sofrerá 15 de dano crushing.",
	sizeof "Attacks an enemy and deals 15 piercing damage. If, in the next turn, the enemy utilize any ability,\n they will receive 15 crushing damage.",
	//"On1 canaliza sua positividade e aumenta suas habilidades. Por 3 turnos, On1 tem redução de dano de 15%\ne Truth Slash e Tautology gastam uma energia a menos.",
	sizeof "On1 channels his positivity and increases his abilities. For 3 turns, On1 has 15% damage reduction,\nTruth Slash and Tautology cost one energy less.",
	//"Concentrando toda sua energia positiva, On1 obtém uma defesa absoluta e fica invulnerável por 1 turno.",
	sizeof "Concentrating all his positive energy, On1 acquires an absolute defense and stays invulnerable for 1 turn.",

	// Serial
	//"Ataca um oponente e realiza 20 de dano. Este golpe ignora INVUL. Porém, o adversário recebe 10 de dano\nse estiver nesse status.",
	sizeof "Attacks an opponent and deals 20 damage. This ability ignores INVUL. But, the enemy receives 10 damage\nif they are in that state.",
	//Infringe 15 de dano piercing a um oponente e adiciona status STUN por 1 turno.",
	sizeof "Inflicts 15 piercing damage to an opponent and applies STUN status for 1 turn.",
	//"Todos os adversários recebem status PARALYZE por 3 turnos.",
	sizeof "All the enemies receive PARALYZE status for 3 turns.",
	//"Envolto por uma densa camada encriptada, Serial K3yll3R fica invulnerável por 1 turno.",
	sizeof "Surrounded by a dense encrypted layer, Serial K3yll3R stays invulnerable for 1 turn.",

	// Ray
	//"Ataca um oponente, realiza 15 de dano e R4y Tr4c3y fica invulnerável a golpes PHYSICAL por 1 turno.",
	sizeof "Attacks an opponent, deals 15 damage and R4y Tr4c3y stays invulnerable to PHYSICAL attacks for 1 turn.",
	//"Por 1 turno, todos os aliados recebem REFLEX DEFENSE.",
	sizeof "For 1 turn, all the allies receive REFLEX DEFENSE.",
	//"Aplica um golpe em todos os inimigos e realiza 35 de dano normal.",
	sizeof "Apply a hit to all enemies and deals 35 normal damage.",
	//"Usando suas técnicas de computação gráfica, R4y Tr4c3y fica invulnerável por 1 turno.",
	sizeof "Using her computer graphics techniques, R4y Tr4c3y stays invulnerable for 1 turn.",

	// AStar
	//"Ataca um oponente e realiza 20 de dano. Para cada vez que um oponente recebe novamente este golpe, o dano\né aumentado em 5 pontos. O efeito é acumulativo.",
	sizeof "Attacks an opponent and deals 20 damage. For each time that the opponent receives this ability, the damage\nis increased in 5 points. The effect is cumulative.",
	//"Produz 25 de dano crushing a um oponente.",
	sizeof "Produces 25 crushing damage to an opponent.",
	//"Durante 4 turnos, todos os adversários atingidos por Neural Network podem receber o ataque Perceptron.\nA-St4r recebe 30 pontos de ABSORPTION.",
	sizeof "During 4 turns, all the enemies hit by Neural Network can be hit by Perceptron.\nA-St4r receives 30 points of ABSORPTION.",
	//"Para se proteger, A-St4r corre até as colinas e fica invulnerável por 1 turno.",
	sizeof "To protect herself, A-St4r runs to the hills and stays invulnerable for 1 turn.",

	// Deadlock
	//"Ataca um oponente e infringe 25 de dano. Caso ele tenha alguma habilidade com duração CONTROL, elimina a\nhabilidade e este sofre status SLEEP por 2 turnos.",
	sizeof "Attacks an opponent and deals 25 damage. In case they are under any CONTROL duration abilities, eliminates the\nability and undergoes SLEEP status for 2 turns.",
	//"Atinge todos os adversários e, no próximo turno, apenas um pode utilizar alguma habilidade.",
	sizeof "Hits all the enemies and, in the next turn, only one can utilize an ability.",
	//"Acerta um inimigo aleatório e coloca o status SLEEP por 3 turnos.",
	sizeof "Hits a random enemy and puts them into SLEEP status for 3 turns.",
	//"D3ADL0CK cria um clone seu para receber golpe no seu lugar, engana o adversário e fica invulnerável por 1 turno.",
	sizeof "D3ADL0CK creates a clone of himself to receive the hit in his place, tricks the enemy and stays invulnerable for 1 turn.",

	// Norma
	//"Ataca um oponente, realiza 25 de dano piercing e há 50% de chance de causar status BURN por 3 turnos.",
	sizeof "Attacks an opponent, deals 25 piercing damage and there is 50% chance of inflicting BURN status for 3 turns.",
	//"Invoca um aliado, garantindo 15 pontos de defesa ABSORPTION. O adversário que atacar N0rma recebe o status\nPOISON por 2 turnos.",
	sizeof "Summon an ally, guaranteeing 15 points of ABSORPTION defense. The enemy that attacks N0rma receives the status\nPOISON for 2 turns.",
	//"Acerta todos os adversários com 30 de dano. Só pode ser utilizado enquanto a habilidade Automata Summon estiver ativa.",
	sizeof "Hits all the enemies dealing 30 damage. Can only be used when the ability Automata Summon is active.",
	//"Aproveitando o universo não-determinístico, N0rma se esconde num estado protegido e fica invulnerável por 1 turno.",
	sizeof "Taking advantage of a non-deterministic universe, N0rma hides in a protected state and stays invulnerable for 1 turn.",

	// Hazard
	//"Ataca todos os oponentes e realiza 15 de dano. Haz4rd recebe 5 pontos de defesa ABSORPTION por cada ataque bem sucedido.",
	sizeof "Attacks all the enemies and deals 15 damage. Haz4rd receives 5 points of ABSORPTION defense for each attack that succeeded.",
	//"Cria uma barreira redundante para os aliados. Cada personagem recebe 15 pontos de redução de defesa por 3 turnos.",
	sizeof "Creates a redundant barrier for the allies. Each character receives 15 points of damage reduction for 3 turns.",
	//"Cura qualquer status negativo de um aliado.",
	sizeof "Removes any negative status of an ally.",
	//"Para escapar de uma situação adversa, Haz4rd utiliza sua técnica de tolerância a falhas para avançar a um estado seguro,\nficando invulnerável por 1 turno.",
	sizeof "In order to escape an adverse situation, Haz4rd utilizes his technique of fault tolerance to advance to a safe state,\nstaying invulnerable for 1 turn.",

	// Qwerty
	//"Qwerty altera sua forma física e ganha redução de dano de 40 pontos por 4 turnos.",
	sizeof "Qwerty alters his physical form and obtains 40 damage reduction for 4 turns.",
	//"Controla um inimigo por dois turnos. DELETE pode ser usado no adversário controlado.",
	sizeof "Controls an enemy for two turns. DELETE can be used in the enemy controlled.",
	//"Mata instantaneamente qualquer adversário que recebe este golpe, desde que esteja sob efeito de CTRL.",
	sizeof "Kills any enemy that gets hit by this ability instantly, inasmuch as they are under the effect of CTRL.",
	//"Em meio ao perigo, Qwerty utiliza seus instintos para escapar da batalha e fica invulnerável por 1 turno.",
	sizeof "Among danger, Qwerty utilizes his instincts to escape the battle and stays invulnerable for 1 turn.",

	// Big O
	//"Ataca o oponente com menor HP e realiza 20 de dano. Por 2 turnos, o alvo fica com status BURN.\nSe o alvo já possui status BURN, este sofre 10 de dano crushing adicional.",
	sizeof "Attacks the opponent with less HP and deals 20 damage. For 2 turns, the target stays with BURN status.\nIf the target is already in BURN status, they suffer an additional 10 crushing damage.",
	//"Se utilizado em um adversário, gera status BURN por 3 turnos. Se, em um aliado, recupera 25 de energia\ne atribui status FROZEN por 2 turnos.",
	sizeof "If utilzed in an enemy, generates BURN status for 3 turns. If, in an ally, restores 25 energy and attributes them with FROZEN\nstatus for 2 turns.",
	//"No próximo turno, recebe 4 orbs de energia: uma de cada categoria.",
	sizeof "In the next turn, receive 4 orbs of energy: one of each category.",
	//"=Big-O= cria uma ilusão nos adversários, possibilitando uma fuga estratégica que o torna invulnerável por 1 turno.",
	sizeof "=Big-O= creates an illusion for the enemies, allowing an strategic escape that makes him invulnerable for 1 turn.",

	// New
	//"Ataca um oponente e realiza 25 de dano. New() recebe redução de dano de 10 pontos no próximo turno.",
	sizeof "Attacks an opponent and deals 25 damage. New() receives 10 damage reduction points in the next turn.",
	//"Seleciona um inimigo e se, no próximo turno, ele utilizar um ataque que não seja UNIQUE nem exija uma\ntécnica de preparação, sua habilidade será copiada e poderá ser usada por New() também.",
	sizeof "Selects and enemy and if, in the next turn, they utilize an ability that is'nt UNIQUE and does not require\na preparation technique, their ability is copied and may be used by New() also.",
	//"Cura todos os status negativos de um aliado.",
	sizeof "Removes any negative status of an ally.",
	//"Alterando sua estrutura física de forma dinâmica, New() fica invulnerável por 1 turno.",
	sizeof "Altering her basic structure in a dynamic way, New() stays invulnerable for 1 turn.",

	// Clockboy
	//"Desfera 20 de dano a um adversário. Atribui status PARALYZE por 2 turnos.",
	sizeof "Deals 20 damage to an enemy. Attributes the PARALYZE status for 2 turns.",
	//"Durante 2 turnos, nenhum golpe de Cl0ckB0y sofre COUNTER.",
	sizeof "During 2 turns, any of the Cl0ckB0y's abilities suffer COUNTER.",
	//"Cl0ckb0y fica invulnerável por 2 turnos, perde 50% de sua energia e seus ataques causam 500% a mais de dano.",
	sizeof "Cl0ckb0y stays invulnerable for 2 turns, loses 50% of his energy and his abilities deal 500% more damage.",
	//"Assumindo que a batalha levou a uma condição de dano, Cl0ckB0y faz um pulo condicional que o torna invulnerável por 1 turno."
	sizeof "Assuming that the battle got into a damage condition, Cl0ckB0y makes a conditional jump that makes him invulnerable for 1 turn."
};

char** char_descriptions = 0;
int* char_descriptions_length = 0;

char* char_descriptions_pt[NUM_CHARS] = {
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

int char_descriptions_pt_length[NUM_CHARS] = {
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

char* char_descriptions_en[NUM_CHARS] = {
	"The villan that never has his conditions met!",
	"The most optimistic superhero!",
	"Has a parrot called Cracken.",
	"Photogenic, loves cameras and always walk through the shadows.",
	"In a systematic search to find the starring path.",
	"Graduated in law, master of processes.",
	"A machine in art of combat: simple and effective.",
	"Catastrophe codename.",
	"I/OLO",
	"Moody, complex to have a relationship.\n",
	"Elegant, filled with class.",
	"Was never late to ENEM."
};

int char_descriptions_en_length[NUM_CHARS] = {
	sizeof "The villan that never has his conditions met!",
	sizeof "The most optimistic superhero!",
	sizeof "Has a parrot called Cracken.",
	sizeof "Photogenic, loves cameras and always walk through the shadows.",
	sizeof "In a systematic search to find the starring path.",
	sizeof "Graduated in law, master of processes.",
	sizeof "A machine in art of combat: simple and effective.",
	sizeof "Catastrophe codename.",
	sizeof "I/OLO",
	sizeof "Moody, complex to have a relationship.\n",
	sizeof "Elegant, filled with class.",
	sizeof "Was never late to ENEM."
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
	1, 3, 2, 4, // ray
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
//hm::vec4 enem_hp_bar_full_color(1, 0.3529f, 0, 1.0f);
//hm::vec4 enem_hp_bar_empty_color(0.61f, 0.21f, 0.0f, 1.0f);
hm::vec4 enem_hp_bar_full_color(1, 0, 0.4f, 1.0f);			//#ff0066
hm::vec4 enem_hp_bar_empty_color(0.6f, 0.0f, 0.2392f, 1.0f);	// #99003d

static SOCKET * connection;
static client_info * player;

//#include "gamepad.cpp"

//xbox controller support
/*
static const char* button_names[] = {
"d-pad up",
"d-pad down",
"d-pad left",
"d-pad right",
"start",
"back",
"left thumb",
"right thumb",
"left shoulder",
"right shoulder",
"???",
"???",
"A",
"B",
"X",
"Y"
};

static int line = 0;

static void update(GAMEPAD_DEVICE dev) {
float lx, ly, rx, ry;

if (!GamepadIsConnected(dev)) {
//printw("%d) n/a\n", dev);
return;
}

GamepadStickNormXY(dev, STICK_LEFT, &lx, &ly);
GamepadStickNormXY(dev, STICK_RIGHT, &rx, &ry);

/*
printw("%d) L:(%+.3f,%+.3f :: %+.3f,%+.3f) R:(%+.3f, %+.3f :: %+.3f,%+.3f) LT:%+.3f RT:%+.3f ",
dev,
lx, ly,
GamepadStickAngle(dev, STICK_LEFT),
GamepadStickLength(dev, STICK_LEFT),
rx, ry,
GamepadStickAngle(dev, STICK_RIGHT),
GamepadStickLength(dev, STICK_RIGHT),
GamepadTriggerLength(dev, TRIGGER_LEFT),
GamepadTriggerLength(dev, TRIGGER_RIGHT));
printw("U:%d D:%d L:%d R:%d ",
GamepadButtonDown(dev, BUTTON_DPAD_UP),
GamepadButtonDown(dev, BUTTON_DPAD_DOWN),
GamepadButtonDown(dev, BUTTON_DPAD_LEFT),
GamepadButtonDown(dev, BUTTON_DPAD_RIGHT));
printw("A:%d B:%d X:%d Y:%d Bk:%d St:%d ",
GamepadButtonDown(dev, BUTTON_A),
GamepadButtonDown(dev, BUTTON_B),
GamepadButtonDown(dev, BUTTON_X),
GamepadButtonDown(dev, BUTTON_Y),
GamepadButtonDown(dev, BUTTON_BACK),
GamepadButtonDown(dev, BUTTON_START));
printw("LB:%d RB:%d LS:%d RS:%d\n",
GamepadButtonDown(dev, BUTTON_LEFT_SHOULDER),
GamepadButtonDown(dev, BUTTON_RIGHT_SHOULDER),
GamepadButtonDown(dev, BUTTON_LEFT_THUMB),
GamepadButtonDown(dev, BUTTON_RIGHT_THUMB));

}
*/

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
	if (combat_state.player.targeting)
		return;
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
	layout_update_cooldowns(true);
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
		else
			gw.exchange_orb_ui->arrows_right[i]->setActive(false, false);
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
	s32 sum = 0;
	for (s32 i = 0; i < ORB_NUMBER - 1; ++i) {
		sum += combat_state.sacrifice_orbs_state.orb_right_amount[i];
	}

	if (sum == combat_state.total_null_orbs_in_temp_use) {
		for (s32 i = 0; i < ORB_NUMBER - 1; ++i) {
			combat_state.orbs_amount[i] -= combat_state.sacrifice_orbs_state.orb_right_amount[i];
			layout_change_orb_amount((Orb_ID)i, combat_state.orbs_amount[i]);
		}
		combat_state.total_orbs -= sum;
		layout_change_orb_amount(ORB_ALL, combat_state.total_orbs);
		combat_state.total_null_orbs_in_temp_use = 0;

		button_callback_sacrifice_orb_cancel(0);
		end_turn();
	}
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

static bool is_on_cooldown(Skill_ID skill_used) {
	if (skill_state_ally.cooldown_counter[skill_used] > 0)
		return true;
	return false;
}

static void reset_selections() {
	for (int i = 0; i < NUM_ALLIES; ++i) {
		for (int j = 0; j < NUM_SKILLS; ++j) {
			linked::Button* b = gw.allies_skills[i * NUM_SKILLS + j]->divs[0]->getButtons()[0];
			b->setActive(true);
			if (!is_on_cooldown((Skill_ID)(i * NUM_SKILLS + j))) {
				b->setOpacity(1.0f);
			}
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
	s32 total_orbs = combat_state.total_orbs - combat_state.total_orbs_temp_added;
	for (int i = 0; i < ORB_NUMBER - 1; ++i) {
		total_orbs -= skill_costs[skill_used][i];
		if (skill_costs[skill_used][i] > combat_state.orbs_amount[i] - combat_state.orbs_amount_temp_added[i]) {
			return false;
		}
	}
	s32 null_orb_cost = skill_costs[skill_used][ORB_NULL];
	if (null_orb_cost > 0) {
		if (total_orbs - combat_state.total_null_orbs_in_temp_use < null_orb_cost) {
			//printf("Not enough orbs to sacrifice\n");
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
		combat_state.orbs_amount_temp_added[i] += add * cost;
		combat_state.total_orbs_temp_added += add * cost;

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
		if (!has_enough_orbs(skill_used) || is_on_cooldown(skill_used)) {
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
		for(int j = 0; j < MAX(NUM_ALLIES, NUM_ENEMIES); ++j)
			combat_state.enemy.receiving_skill[i][j] = SKILL_NONE;
	}
	for (int i = 0; i < NUM_ALLIES; ++i) {
		int index = char_sel_state.selections[i];
		combat_state.player.skill_in_use[i] = SKILL_NONE;
		combat_state.player.char_id[i] = (Character_ID)index;
		combat_state.player.max_hp[i] = 100;
		combat_state.player.hp[i] = 100;
		combat_state.player.targeting = false;
		combat_state.player.targets[i].skill_used = SKILL_NONE;
		for (int j = 0; j < MAX(NUM_ALLIES, NUM_ENEMIES); ++j)
			combat_state.player.receiving_skill[i][j] = SKILL_NONE;
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

void bind_framebuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, gw.framebuffer);
}

void bind_framebuffer_texture() {
	glBindTexture(GL_TEXTURE_2D, gw.framebuffer_texture);
}

void init_application()
{
	using namespace linked;

	u32 FrameBuffer;
	glGenFramebuffers(1, &FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);

	glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	gw.framebuffer = FrameBuffer;

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1600, 900, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	gw.framebuffer_texture = texture;

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	init_animations();

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
	for(int i = 0; i < ORB_NUMBER; ++i)
		combat_state.orbs_amount_temp_added[i] = 0;
	combat_state.total_orbs_temp_added = 0;

	combat_state.orbs_amount[ORB_HARD] = 8;
	combat_state.orbs_amount[ORB_SOFT] = 3;
	combat_state.orbs_amount[ORB_VR] = 4;
	combat_state.orbs_amount[ORB_BIOS] = 0;
	combat_state.total_orbs = 8 + 3 + 4 + 0;
	layout_change_orb_amount(ORB_HARD, 8);
	layout_change_orb_amount(ORB_SOFT, 3);
	layout_change_orb_amount(ORB_VR, 4);
	layout_change_orb_amount(ORB_BIOS, 0);
	layout_change_orb_amount(ORB_ALL, 8 + 3 + 4 + 0);
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

	char_descriptions = char_descriptions_en;
	char_descriptions_length = char_descriptions_en_length;

	skill_desc = skill_desc_en;
	skill_desc_length = skill_desc_en_length;

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

static void print_character(Character_ID char_id) {
	printf("%s", char_names[char_id]);
}

static void print_skill(Skill_ID skill_id) {
	printf("%s", skill_names[skill_id]);
}

static void print_targets() {
	for (int i = 0; i < NUM_ALLIES; ++i) {
		if (combat_state.player.targets[i].skill_used != SKILL_NONE) {
			print_character((Character_ID)i);
			printf(" is attacking using ");
			print_skill(combat_state.player.targets[i].skill_used);
			printf(" on");
			for (int e = 0; e < NUM_ENEMIES; ++e) {
				if (combat_state.player.targets[i].enemy_target_index[e] != -1) {
					printf(" [%d][E]", e);
					print_character((Character_ID)char_sel_state.enemy_selections[e]);
				}
			}
			for (int a = 0; a < NUM_ALLIES; ++a) {
				if (combat_state.player.targets[i].ally_target_index[a] != -1) {
					printf(" [%d][A]", a);
					print_character((Character_ID)char_sel_state.selections[a]);
				}
			}
		}
		printf("\n");
	}
}

static void apply_skills_and_send() {
	print_targets();

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

	for (int i = 0; i < ORB_NUMBER; ++i) {
		combat_state.orbs_amount_temp_added[i] = 0;
	}
	combat_state.total_orbs_temp_added = 0;

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
	layout_update_cooldowns(true);
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
			layout_update_hp_animations(frametime);
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

			layout_update_cooldowns();

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
					Character_ID hover_char_id = (Character_ID)char_sel_state.selections[i];
					layout_set_char_orb_types_description(hover_char_id, combat_state.skill_info_group->getLabels()[0]);
					combat_state.skill_info_group->getLabels()[1]->setTextLength(0);
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
							// Update skill costs on layout
							for (int n = ORB_NUMBER - 1, orb_index = 0; n >= 0; --n) {
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
					Character_ID hover_char_id = (Character_ID)char_sel_state.enemy_selections[i];
					layout_set_char_orb_types_description(hover_char_id, combat_state.skill_info_group->getLabels()[0]);
					combat_state.skill_info_group->getLabels()[1]->setTextLength(0);
					is_hovering_char = true;
					break;
				}
			}
			
			static bool render_info = false;

			if (is_hovering_skill | is_hovering_char)
				render_info = true;

			if (is_hovering_char) {
				for (int i = 0; i < ORB_NUMBER; ++i) {
					combat_state.skill_costs[i]->m_render = false;
				}
			}

			combat_state.skill_info_image->m_render = render_info;
			combat_state.skill_info_title->m_render = render_info;
			combat_state.skill_info_desc->m_render = render_info;
			combat_state.skill_info_group->m_render = render_info;

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

static int global_animating = 0;

void render_overlay(double frametime) {
	if (ggs.mode != MODE_COMBAT)
		return;
	linked::Window::m_animationShader->useShader();
	linked::Window::m_animationShader->activateAlphaBlend();
	
	for (int i = 0; i < NUM_ALLIES; ++i) {
		for (int j = 0; j < MAX(NUM_ALLIES, NUM_ENEMIES); ++j) {
			Skill_ID receiving = combat_state.player.receiving_skill[i][j];
			if (receiving != SKILL_NONE) {
				if (gw.skills_animations[receiving] == 0) {
					combat_state.player.receiving_skill[i][j] = SKILL_NONE;
					return;
				}
				hm::vec2 pos = gw.allies[i]->getPosition();
				linked::Window::m_animationShader->update(pos, gw.skills_animations[receiving]);
				gw.animation->render();
				bool ended = gw.skills_animations[receiving]->animate();
				if (ended) {
					combat_state.player.receiving_skill[i][j] = SKILL_NONE;
				}
				return;
			}
		}
		
	}
	for (int i = 0; i < NUM_ENEMIES; ++i) {
		for (int j = 0; j < MAX(NUM_ALLIES, NUM_ENEMIES); ++j) {
			Skill_ID receiving = combat_state.enemy.receiving_skill[i][j];
			if (receiving != SKILL_NONE) {
				if (gw.skills_animations[receiving] == 0) {
					combat_state.enemy.receiving_skill[i][j] = SKILL_NONE;
					return;
				}
				hm::vec2 pos = gw.enemies[i]->getPosition();
				linked::Window::m_animationShader->update(pos, gw.skills_animations[receiving]);
				gw.animation->render();
				bool ended = gw.skills_animations[receiving]->animate();
				if (ended) {
					combat_state.enemy.receiving_skill[i][j] = SKILL_NONE;
				}
				return;
			}
		}
	}
	linked::Window::m_animationShader->deactivateAlphaBlend();
}

void update_and_render(double frametime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	input();

	if (chat.m_active)
		chat.update();

	update_game_mode(frametime);
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
	if (keyboard_state.key_event[VK_F3]) {
		keyboard_state.key_event[VK_F3] = false;

		if (gw.language == LANGUAGE_EN) {
			char_descriptions = char_descriptions_pt;
			char_descriptions_length = char_descriptions_pt_length;

			skill_desc = skill_desc_pt;
			skill_desc_length = skill_desc_pt_length;
			gw.language = LANGUAGE_PT;
		} else {
			char_descriptions = char_descriptions_en;
			char_descriptions_length = char_descriptions_en_length;

			skill_desc = skill_desc_en;
			skill_desc_length = skill_desc_en_length;
			gw.language = LANGUAGE_EN;
		}
	}

	static int turn = 0;

	if (keyboard_state.key_event[VK_F4]) {
		keyboard_state.key_event[VK_F4] = false;
		switch (turn) {
			case 0: {
				execute_skill(SKILL_DUAL_SIMPLEX, 0, 0, &combat_state, true, false);
				execute_skill(SKILL_FALSE_RUSH, 2, 2, &combat_state, true, false);
			}break;
			case 1: {
				execute_skill(SKILL_TRUTH_SLASH, 2, 1, &combat_state, true, false);
				execute_skill(SKILL_REQUIEM_ZERO, 2, 2, &combat_state, true, true);
			}break;
			case 2: {
				execute_skill(SKILL_FALSE_RUSH, 0, 2, &combat_state, true, false);
				execute_skill(SKILL_BEST_BOUND_FIST, 0, 0, &combat_state, true, false);
			}break;
			case 3: {
				execute_skill(SKILL_FALSE_RUSH, 0, 2, &combat_state, true, false);
			}break;
		}
		turn++;
		end_turn();
	}

	if (keyboard_state.key_event['R']) {
		keyboard_state.key_event['R'] = false;
		linked::Window::m_animationShader->reloadShader();
	}
	if (keyboard_state.key_event['V']) {
		keyboard_state.key_event['V'] = false;
		global_animating += 1;
		while (!gw.skills_animations[global_animating]) {
			global_animating += 1;
			if (global_animating >= NUM_SKILLS * NUM_CHARS) {
				global_animating = 0;
			}
		}
		if (global_animating >= NUM_SKILLS * NUM_CHARS) {
			global_animating = 0;
		}
	}
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

static char skill_group_layout_label[512] = {0};
static void layout_set_char_orb_types_description(Character_ID id, linked::Label* label) {
	int length = 0;
	switch (id) {
		case CHAR_ZERO:				 PUT_STR("SOFT, VR, BIOS", skill_group_layout_label, length); break;
		case CHAR_ONE:				 PUT_STR("HARD, VR, BIOS", skill_group_layout_label, length); break;
		case CHAR_SERIAL_KEYLLER:	 PUT_STR("HARD, SOFT", skill_group_layout_label, length); break;
		case CHAR_RAY_TRACEY:		 PUT_STR("VR", skill_group_layout_label, length); break;
		case CHAR_A_STAR:			 PUT_STR("BIOS,	VR", skill_group_layout_label, length); break;
		case CHAR_DEADLOCK:			 PUT_STR("SOFT, ", skill_group_layout_label, length); break;
		case CHAR_NORMA:			 PUT_STR("NULL", skill_group_layout_label, length); break;
		case CHAR_HAZARD:			 PUT_STR("BIOS, HARD", skill_group_layout_label, length); break;
		case CHAR_QWERTY:			 PUT_STR("HARD", skill_group_layout_label, length); break;
		case CHAR_BIG_O:			 PUT_STR("HARD, VR", skill_group_layout_label, length); break;
		case CHAR_NEW:				 PUT_STR("SOFT, BIOS", skill_group_layout_label, length); break;
		case CHAR_CLOCKBOY:			 PUT_STR("BIOS", skill_group_layout_label, length); break;
	}
	label->setText((u8*)skill_group_layout_label, length + 1);
}

static void layout_set_skill_group_from_skill(int skill_index, linked::Label* label) {
	int length = 0;
	bool insert_space = false;

	switch (skill_groups[skill_index].type) {
	case SKILL_TYPE_NONE: break;
	case SKILL_TYPE_PHYSICAL:	PUT_STR("PHYSICAL", skill_group_layout_label, length); insert_space = true; break;
	case SKILL_TYPE_MENTAL:		PUT_STR("MENTAL", skill_group_layout_label, length);   insert_space = true; break;
	case SKILL_TYPE_VIRTUAL:	PUT_STR("VIRTUAL", skill_group_layout_label, length);  insert_space = true; break;
	default: break;
	}

	switch (skill_groups[skill_index].mode) {
	case SKILL_MODE_NONE: break;
	case SKILL_MODE_MELEE:  if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("MELEE", skill_group_layout_label, length);  insert_space = true;  break;
	case SKILL_MODE_RANGED: if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("RANGED", skill_group_layout_label, length); insert_space = true;  break;
	default: break;
	}

	switch (skill_groups[skill_index].category) {
	case SKILL_CATEGORY_NORMAL: break;
	case SKILL_CATEGORY_COUNTER: if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("COUNTER", skill_group_layout_label, length); insert_space = true; break;
	case SKILL_CATEGORY_STATUS:  if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("STATUS", skill_group_layout_label, length);  insert_space = true; break;
	default: break;
	}

	switch (skill_groups[skill_index].condition) {
	case SKILL_CONDITION_NONE:
	case SKILL_CONDITION_NORMAL: break;
	case SKILL_CONDITION_BURN: if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("BURN", skill_group_layout_label, length); insert_space = true; break;
	case SKILL_CONDITION_FREEZE: if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("FREEZE", skill_group_layout_label, length); insert_space = true; break;
	case SKILL_CONDITION_POISON: if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("POISON", skill_group_layout_label, length); insert_space = true; break;
	case SKILL_CONDITION_PARALYZE: if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("PARALYZE", skill_group_layout_label, length); insert_space = true; break;
	case SKILL_CONDITION_SLEEP: if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("SLEEP", skill_group_layout_label, length); insert_space = true; break;
	default: break;
	}

	switch (skill_groups[skill_index].damage) {
	case SKILL_DMG_NONE: break;
	case SKILL_DMG_NORMAL: if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("DMG NORMAL", skill_group_layout_label, length); insert_space = true; break;
	case SKILL_DMG_PIERCING: if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("PIERCING", skill_group_layout_label, length); insert_space = true; break;
	case SKILL_DMG_CRUSHING: if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("CRUSHING", skill_group_layout_label, length); insert_space = true; break;
	default: break;
	}

	switch (skill_groups[skill_index].defense) {
	case SKILL_DEF_NONE: break;
	case SKILL_DEF_REDUCTION: if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("REDUCTION", skill_group_layout_label, length); insert_space = true; break;
	case SKILL_DEF_ABSORPTION: if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("ABSORPTION", skill_group_layout_label, length); insert_space = true; break;
	case SKILL_DEF_RELECTION: if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("REFLECTION", skill_group_layout_label, length); insert_space = true; break;
	case SKILL_DEF_INVULNERABILITY: if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("INVUL", skill_group_layout_label, length); insert_space = true; break;
	default: break;
	}

	switch (skill_groups[skill_index].duration) {
	case SKILL_DURATION_NONE: break;
	case SKILL_DURATION_STATIC: if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("STATIC", skill_group_layout_label, length); insert_space = true; break;
	case SKILL_DURATION_CONTROL: if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("CONTROL", skill_group_layout_label, length); insert_space = true; break;
	case SKILL_DURATION_CONSTANT: if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("CONSTANT", skill_group_layout_label, length); insert_space = true; break;
	}

	switch (skill_groups[skill_index].unique) {
	case SKILL_NOT_UNIQUE: break;
	case SKILL_UNIQUE: if (insert_space) put_space(&length, skill_group_layout_label); PUT_STR("UNIQUE", skill_group_layout_label, length); break;
	default: break;
	}

	label->setText((u8*)skill_group_layout_label, length + 1);
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

struct HP_Bar_Animation {
	bool ally_hp_animating[NUM_ALLIES];
	bool enemy_hp_animating[NUM_ENEMIES];

	int ally_start_hp[NUM_ALLIES];
	int enemy_start_hp[NUM_ALLIES];

	int ally_hp_to_set[NUM_ALLIES];
	int enemy_hp_to_set[NUM_ENEMIES];

	r32 ally_current_animating_hp[NUM_ALLIES];
	r32 enemy_current_animating_hp[NUM_ENEMIES];
};

static HP_Bar_Animation hp_bar_animation = {};

static void layout_set_ally_hp(int ally_index, int max_hp, int hp_to_set)
{
	// hp_empty
	// hp_full
	// hp_label

	hp_bar_animation.ally_hp_animating[ally_index] = true;
	hp_bar_animation.ally_hp_to_set[ally_index] = hp_to_set;
	hp_bar_animation.ally_start_hp[ally_index] = combat_state.player.hp[ally_index];
	hp_bar_animation.ally_current_animating_hp[ally_index] = combat_state.player.hp[ally_index];

	static char hp_buffer[NUM_ALLIES][32] = {};
	memset(hp_buffer[ally_index], 0, sizeof(hp_buffer[NUM_ALLIES]));
	int count = s32_to_str_base10(hp_to_set, hp_buffer[ally_index]);
	hp_buffer[ally_index][count++] = '/';
	count += s32_to_str_base10(max_hp, hp_buffer[ally_index] + count);

	//int max_width = gw.allies_info[ally_index]->divs[0]->getWidth() - 2;
	//int new_width = (int)roundf((r32)(hp_to_set * max_width) / (r32)max_hp);
	//gw.allies_info[ally_index]->divs[1]->setWidth(new_width);

	linked::Label* hp_label = gw.allies_info[ally_index]->divs[2]->getLabels()[0];
	hp_label->setText((u8*)hp_buffer[ally_index], count + 1);
}

static void layout_set_enemy_hp(int enemy_index, int max_hp, int hp_to_set)
{
	// hp_empty
	// hp_full
	// hp_label

	hp_bar_animation.enemy_hp_animating[enemy_index] = true;
	hp_bar_animation.enemy_hp_to_set[enemy_index] = hp_to_set;
	hp_bar_animation.enemy_start_hp[enemy_index] = combat_state.enemy.hp[enemy_index];
	hp_bar_animation.enemy_current_animating_hp[enemy_index] = combat_state.enemy.hp[enemy_index];

	static char hp_buffer[NUM_ENEMIES][32] = {};
	memset(hp_buffer[enemy_index], 0, sizeof(hp_buffer[NUM_ENEMIES]));
	int count = s32_to_str_base10(hp_to_set, hp_buffer[enemy_index]);
	hp_buffer[enemy_index][count++] = '/';
	count += s32_to_str_base10(max_hp, hp_buffer[enemy_index] + count);

	//int max_width = gw.enemies_info[enemy_index]->divs[0]->getWidth() - 2;
	//int new_width = (int)roundf((r32)(hp_to_set * max_width) / (r32)max_hp);
	//gw.enemies_info[enemy_index]->divs[1]->setWidth(new_width);

	linked::Label* hp_label = gw.enemies_info[enemy_index]->divs[2]->getLabels()[0];
	hp_label->setText((u8*)hp_buffer[enemy_index], count + 1);
}

// layout_set_enemy_hp &  layout_set_ally_hp must be set before updating combat_state always
// in order to enable animation on hp bar
static void layout_update_hp_animations(r32 delta) {
	const r32 seconds_to_animate = 0.8f;
	delta /= seconds_to_animate;

	for (int i = 0; i < NUM_ENEMIES; ++i) {
		if (!hp_bar_animation.enemy_hp_animating[i])
			continue;
		int max_width = gw.enemies_info[i]->divs[0]->getWidth() - 2;
		
		r32 update = (hp_bar_animation.enemy_start_hp[i] - hp_bar_animation.enemy_hp_to_set[i]);
		r32 move = hp_bar_animation.enemy_current_animating_hp[i] - delta * update;
		hp_bar_animation.enemy_current_animating_hp[i] = move;

		if (update > 0 && move <= hp_bar_animation.enemy_hp_to_set[i]) {
			move = hp_bar_animation.enemy_hp_to_set[i];
			hp_bar_animation.enemy_hp_animating[i] = false;
		} else if (update <= 0 && move >= hp_bar_animation.enemy_hp_to_set[i]) {
			move = hp_bar_animation.enemy_hp_to_set[i];
			hp_bar_animation.enemy_hp_animating[i] = false;
		}
		int new_width = (int)roundf((r32)(move * max_width) / (r32)combat_state.enemy.max_hp[i]);
		gw.enemies_info[i]->divs[1]->setWidth(new_width);
	}

	for (int i = 0; i < NUM_ALLIES; ++i) {
		if (!hp_bar_animation.ally_hp_animating[i])
			continue;
		int max_width = gw.allies_info[i]->divs[0]->getWidth() - 2;
		
		r32 update = (hp_bar_animation.ally_start_hp[i] - hp_bar_animation.ally_hp_to_set[i]);
		r32 move = hp_bar_animation.ally_current_animating_hp[i] - delta * update;
		hp_bar_animation.ally_current_animating_hp[i] = move;

		if (update > 0 && move <= hp_bar_animation.ally_hp_to_set[i]) {
			move = hp_bar_animation.ally_hp_to_set[i];
			hp_bar_animation.ally_hp_animating[i] = false;
		} else if(update <= 0 && move >= hp_bar_animation.ally_hp_to_set[i]){
			move = hp_bar_animation.ally_hp_to_set[i];
			hp_bar_animation.ally_hp_animating[i] = false;
		}
		int new_width = (int)roundf((r32)(move * max_width) / (r32)combat_state.player.max_hp[i]);
		gw.allies_info[i]->divs[1]->setWidth(new_width);
	}
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

static void layout_update_cooldowns(bool update_to_full_opacity) {

	for (int i = 0; i < NUM_ALLIES; ++i) {
		for (int j = 0; j < NUM_SKILLS; ++j) {
			linked::Label* label = gw.allies_skills[i * NUM_SKILLS + j]->divs[0]->getLabels()[0];
			Skill_ID skill_index = (Skill_ID)((int)char_sel_state.selections[i] * NUM_SKILLS + j);
			if (skill_state_ally.cooldown_counter[skill_index] == 0) {
				label->getText()[0] = ' ';
				if (update_to_full_opacity) {
					gw.allies_skills[i * NUM_SKILLS + j]->divs[0]->getButtons()[0]->setOpacity(1.0f);
					gw.allies_skills[i * NUM_SKILLS + j]->divs[0]->getButtons()[0]->setAllBGColor(hm::vec4(0x0f112aff));
				}
				if (!has_enough_orbs(skill_index)) {
					gw.allies_skills[i * NUM_SKILLS + j]->divs[0]->getButtons()[0]->setOpacity(0.25f);
				}
			}
			else {
				label->getText()[0] = skill_state_ally.cooldown_counter[skill_index] + 0x30;
				gw.allies_skills[i * NUM_SKILLS + j]->divs[0]->getButtons()[0]->setOpacity(0.25f);
				//0x0f112aff
				gw.allies_skills[i * NUM_SKILLS + j]->divs[0]->getButtons()[0]->setAllBGColor(hm::vec4(0x0f112ac8));
			}
		}
	}

}