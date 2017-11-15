#pragma once
#include <hmath.h>
#include "shader.h"
#include "quaternion.h"
#include "ho_gl.h"

#include "WindowApi/Window.h"
#include "ResourceLoad/Texture.h"

struct Camera {
	mat4 view_matrix;
	mat4 projection_matrix;

	hm::vec3 cam_position;
	hm::vec3 cam_orientation;
	hm::vec3 cam_up;

	bool locked;

	float current_speed;
	float current_turn_speed;

	float fov;
	float far_plane;
	float near_plane;

	void set_cam_position(hm::vec3 newpos);
	void set_cam_orientation(hm::vec3 newori);

	void move_forward_and_back(float amt);
	void move_sideways(float amt);
	void rotate_x(float amt);
	void rotate_y(float amt);
};

struct Vertex3D {
	float pos[3];
	float normal[3];
	float tex[2];
};

struct Texture;

struct IndexedModel3D {
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	Vertex3D* vertices;
	u16* indices;

	int num_vertices;
	int num_indices;

	bool is_colliding;
	int colliding_with_index = -1;

	hm::vec3 position;
	hm::vec3 last_pos;
	hm::vec3 orientation;
	Quaternion rotation = Quaternion(0,0,0,1);

	float scale;
	mat4 model_matrix;

	bool simulating = false;
	bool static_object = false;
	hm::vec3 velocity = hm::vec3(0.0f, 0.0f, 0.0f);
	float time = 0.0f;
	Texture* texture;
};

void init_application();
void update_and_render(double frametime);
void input();

enum Orb_ID {
	ORB_NONE = -1,
	ORB_HARD = 0,
	ORB_SOFT = 1,
	ORB_VR = 2,
	ORB_BIOS = 3,
	ORB_NULL = 4,
	ORB_NUMBER,
	ORB_ALL,
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

// Game layout functions
static void layout_enemy_die(u32 enemy_index);
static void layout_ally_die(u32 ally_index);
static void layout_set_skill_group_from_skill(int skill_index, linked::Label* label);
static void layout_set_cooldown_from_skill(int skill_index, linked::Label* label);
static void layout_change_orb_amount(Orb_ID id, int amt);
static void layout_change_exchange_orb_amount(Orb_ID id, int amt);
static void layout_toggle_char_selection(int id, std::vector<linked::WindowDiv*>* divs);

// Gameplay structures
#define NUM_CHARS 12
#define NUM_SKILLS 4
#define NUM_ALLIES 3
#define NUM_ENEMIES 3
#define TURN_DURATION 60.0
#define FAST 1

struct GameState {
	Camera camera;
	GLuint shader;

	IndexedModel3D* models;
	int num_models;

	Game_Mode mode;
	Game_Mode last_mode;
};

struct Char_Selection_State {
	int num_selected;
	int selections[NUM_ALLIES];
	linked::WindowDiv* play_button_div;
	Character_ID last_hovered;
};

struct Orb_Exchange_State {
	bool active = false;
	bool state_changed = false;
	int num_lost = 0;
	int num_gain = 0;
	int accumulated = 0;
	int orbs_amount_added_subtracted[ORB_NUMBER];
	int orbs_start_amount[ORB_NUMBER];

	// Visual
	linked::WindowDiv* info_div;
};

struct Combat_State {
	// Gameplay
	bool player_turn;
	int orbs_amount[ORB_NUMBER];
	int total_orbs;

	Orb_Exchange_State exchange_orbs_state;

	// Visual
	Skill_ID last_hovered;

	linked::WindowDiv* skill_info_image;
	linked::WindowDiv* skill_info_image_border;
	linked::WindowDiv* skill_info_title;
	linked::WindowDiv* skill_info_desc;
	linked::WindowDiv* skill_info_group;
	linked::WindowDiv* skill_costs[ORB_NUMBER];

	linked::Label* orb_labels[ORB_NUMBER];
	linked::Label* all_orbs_label;

	linked::Button* end_turn_button;
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
	linked::Window* exchange_orbs;
};

// Gameplay functions

void change_game_mode(Game_Mode mode);
void end_turn();