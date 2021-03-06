#pragma once
#include <hmath.h>
#include "shader.h"
#include "quaternion.h"
#include "ho_gl.h"

#include "WindowApi/Window.h"
#include "ResourceLoad/Texture.h"
#include "Audio/AudioController.h"
#include "WindowApi/Animation.h"


void init_application();
void update_and_render(double frametime);
void render_overlay(double frametime);
void input();

enum Language {
	LANGUAGE_PT = 0,
	LANGUAGE_EN,
	LANGUAGE_NUMBER
};

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
	SKILL_TYPE_NONE     = 0,
	SKILL_TYPE_PHYSICAL = FLAG(0),
	SKILL_TYPE_VIRTUAL  = FLAG(1),
	SKILL_TYPE_MENTAL   = FLAG(2),
};
enum Skill_Mode {
	SKILL_MODE_NONE    = 0,
	SKILL_MODE_MELEE   = FLAG(0),
	SKILL_MODE_RANGED  = FLAG(1),
};
enum Skill_Category {
	SKILL_CATEGORY_NORMAL,
	SKILL_CATEGORY_COUNTER,
	SKILL_CATEGORY_STATUS,
};
enum Skill_Condition {
	SKILL_CONDITION_NONE        = 0,
	SKILL_CONDITION_NORMAL		= FLAG(0),
	SKILL_CONDITION_BURN		= FLAG(1),
	SKILL_CONDITION_FREEZE		= FLAG(2),
	SKILL_CONDITION_POISON		= FLAG(3),
	SKILL_CONDITION_PARALYZE	= FLAG(4),
	SKILL_CONDITION_SLEEP		= FLAG(5),
	SKILL_CONDITION_STUN		= FLAG(6),
	SKILL_CONDITION_SKILL_STATUS,
	SKILL_CONDITION_NUMBER,
};
enum Skill_Damage {
	SKILL_DMG_NONE,
	SKILL_DMG_NORMAL,
	SKILL_DMG_PIERCING,
	SKILL_DMG_CRUSHING,
};
enum Skill_Defense {
	SKILL_DEF_NONE = 0,
	SKILL_DEF_REDUCTION			= FLAG(0),
	SKILL_DEF_ABSORPTION		= FLAG(1),
	SKILL_DEF_RELECTION			= FLAG(2),
	SKILL_DEF_INVULNERABILITY	= FLAG(3),
	SKILL_DEF_NUMBER,			// needed for reduction points
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
	SKILL_CTRL,
	SKILL_DELETE,
	SKILL_ESC,

	SKILL_BEST_BOUND_FIST,
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
	SKILL_BRANCH_DAMAGE,
	SKILL_NUMBER
};

enum Game_Mode {
	MODE_NONE = -1,
	MODE_INTRO = 0,
	MODE_CHAR_SELECT,
	MODE_CHAR_INFO,
	MODE_COMBAT,
	MODE_SETTINGS,
};

// Game layout functions
static void layout_enemy_die(u32 enemy_index);
static void layout_ally_die(u32 ally_index);
static void layout_set_skill_group_from_skill(int skill_index, linked::Label* label);
static void layout_set_cooldown_from_skill(int skill_index, linked::Label* label);
static void layout_change_orb_amount(Orb_ID id, int amt);
static void layout_toggle_char_selection(int id, std::vector<linked::WindowDiv*>* divs, bool selected);
static void layout_set_ally_hp(int ally_index, int max_hp, int hp_to_set);
static void layout_set_enemy_hp(int enemy_index, int max_hp, int hp_to_set);
static void layout_set_timer_percentage(r32 percentage);
static void layout_set_enemy_image_opacity(s32 index, r32 percentage, hm::vec4 color = hm::vec4(0, 0, 0, 1));
static void layout_set_ally_image_opacity(s32 index, r32 percentage, hm::vec4 color = hm::vec4(0, 0, 0, 1));
static void layout_update_endturn_button();
static void layout_apply_status_ally(s32 index, s32 stat_index, Texture* status_image);
static void layout_apply_status_enemy(s32 index, s32 stat_index, Texture* status_image);
static void layout_set_char_orb_types_description(Character_ID id, linked::Label* label);

static void layout_set_exchange_modal_quantity(bool orb_left, Orb_ID orb_id, s32 quantity);
static void layout_set_exchange_modal_upper_orbs(Orb_ID id, s32 index);

static void layout_set_sacrifice_modal_quantity(bool orb_left, Orb_ID orb_id, s32 quantity);

static void layout_update_hp_animations(r32 delta);

static void layout_update_cooldowns(bool update_to_full_opacity = false);

// Gameplay structures
#define NUM_CHARS 12
#define NUM_SKILLS 4
#define NUM_ALLIES 3
#define NUM_ENEMIES 3
#define MAX_STATUS 6
#define TURN_DURATION 260.0

struct GameState {
	Game_Mode mode;
	Game_Mode last_mode;
};

struct Char_Selection_State {
	int num_selected;
	int selections[NUM_ALLIES];
	linked::WindowDiv* play_button_div;
	Character_ID last_hovered;

	int enemy_selections[NUM_ENEMIES];
};

struct Orb_Exchange_State {
	bool		active;
	s32			orb_left_amount[ORB_NUMBER - 1];
	s32			orb_new_amount[ORB_NUMBER - 1];
	s32			orb_changes[ORB_NUMBER - 1];
	s32			exchanging_count;
	Orb_ID		exchanging[3];
};

struct Orb_Sacrifice_State {
	bool		active;
	s32			orb_left_amount[ORB_NUMBER - 1];
	s32			orb_right_amount[ORB_NUMBER - 1];
};

struct Target {
	Character_ID	attacking_character;
	Skill_ID		skill_used;
	s32				ally_target_index[NUM_ALLIES];	 // -1 if none
	s32				enemy_target_index[NUM_ENEMIES]; // -1 if none
	linked::Window* ally_target_image[NUM_ALLIES];
	linked::Window* enemy_target_image[NUM_ENEMIES];
};

struct Settings {
	s32 volume_sfx;
	s32 volume_bmg;
	bool animations;
	Language language;
};

struct Player {
	Character_ID	char_id[NUM_ALLIES];
	s32				hp[NUM_ALLIES];
	s32				max_hp[NUM_ALLIES];

	bool targeting;
	Target targeting_info;
	Target targets[NUM_ALLIES];
	Skill_ID skill_in_use[NUM_ALLIES];
	
	u32 reduction[NUM_ALLIES];			// Skill_Defense or'd together
	u32 reduction_type[NUM_ALLIES];		// Skill_Type or'd together
	u32 reduction_points[NUM_ALLIES][SKILL_DEF_NUMBER];
	s32 reduction_duration[NUM_ALLIES][SKILL_DEF_NUMBER];
	bool reduction_percentile[NUM_ALLIES][SKILL_DEF_NUMBER];

	u32 status[NUM_ALLIES];				// Skill_Condition or'd together
	s32 status_duration[NUM_ALLIES][SKILL_CONDITION_NUMBER];

	s32 skill_status[NUM_ALLIES][NUM_SKILLS * NUM_CHARS];	// Duration

	s32 cumulative_skill[NUM_ALLIES][SKILL_NUMBER];

	// Skill animation
	Skill_ID receiving_skill[NUM_ALLIES][MAX(NUM_ALLIES, NUM_ENEMIES)];
	// Status animation
	Skill_Condition receiving_status[NUM_ALLIES][MAX(NUM_ALLIES, NUM_ENEMIES)];
};

struct Combat_State {
	// Gameplay
	bool player_turn;
	int orbs_amount[ORB_NUMBER];
	int orbs_amount_temp_added[ORB_NUMBER];
	int total_orbs;
	int total_orbs_temp_added;
	int total_null_orbs_in_temp_use;

	Player player;
	Player enemy;

	Orb_Exchange_State exchange_orbs_state;
	Orb_Sacrifice_State sacrifice_orbs_state;

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

struct Exchange_Orbs_UI {
	linked::Window* window;
	linked::WindowDiv* upper_orbs_div;	// 3 buttons here in order

	linked::WindowDiv* orbs_left;	// 4 buttons and 4 labels
	linked::WindowDiv* orbs_new;	// 4 buttons and 4 labels

	linked::Button* arrows_left[ORB_NUMBER - 1];
	linked::Button* arrows_right[ORB_NUMBER - 1];

	linked::Button* confirm;
	linked::Button* cancel;

	Texture* empty_orb_texture;
};

struct Sacrifice_Orbs_UI {
	linked::Window* window;

	linked::Label* orb_number_label;

	linked::WindowDiv* orbs_left;	// 4 buttons and 4 labels
	linked::WindowDiv* orbs_right;	// 4 buttons and 4 labels

	linked::Button* arrows_left[ORB_NUMBER - 1];
	linked::Button* arrows_right[ORB_NUMBER - 1];

	linked::Button* endturn;
	linked::Button* cancel;

	linked::WindowDiv* skills_miniatures;	// 3 buttons here in order

	Texture* empty_skill_texture;
};

struct Settings_UI {
	linked::Window* settings_window;
	linked::WindowDiv* settings_div;

	linked::Label* idioma;			// LANGUAGE / IDIOMA
	linked::Label* vol_sfx;
	linked::Label* vol_bmg;
	linked::Label* animations;		// ANIMATIONS / ANIMA��ES

	linked::Label* idioma_value;	// ENGLISH / PORTUGU�S
	linked::Label* vol_sfx_value;	// 0 - 100
	linked::Label* vol_bmg_value;	// 0 - 100
	linked::Label* animations_value;// ACTIVE / INACTIVE / ATIVO / INATIVO

	linked::Button* confirm;
	linked::Button* cancel;
};

struct Game_Windows {
	Language language;

	u32 framebuffer;
	u32 framebuffer_texture;
	// background window
	linked::Window* bgwindow;
	Texture* bg_settings;
	Texture* bg_logo;
	Texture* bg_normal;
	Mesh* animation;
	Animation* skills_animations[NUM_SKILLS * NUM_CHARS];
	Animation* status_animations[SKILL_CONDITION_NUMBER];

	// Settings window
	linked::Window* settings_window;

	// intro window
	linked::Window* intro_logo;

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
	linked::Window* historico;
	double historico_timer;

	linked::WindowDiv* allies_indicator[NUM_ALLIES];
	linked::WindowDiv* enemies_indicator[NUM_ENEMIES];

	// Modais
	Exchange_Orbs_UI* exchange_orb_ui;
	Sacrifice_Orbs_UI* sacrifice_orb_ui;

	Settings_UI* settings_ui;

	Texture* end_turn_button_player_turn;
	Texture* end_turn_button_enemy_turn;
	Texture* end_turn_button_player_turn_hover;
	Texture* end_turn_button_enemy_turn_hover;

	// combat target
	linked::Window* enemy_target[NUM_ALLIES][MAX(NUM_ALLIES, NUM_ENEMIES)];
	linked::Window* ally_target[NUM_ALLIES][NUM_ALLIES];
};

// Gameplay functions

void change_game_mode(Game_Mode mode);
void end_turn();

void bind_framebuffer();
void bind_framebuffer_texture();