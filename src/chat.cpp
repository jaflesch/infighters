#include "chat.h"
#include "input.h"
#include "util.h"
#include "font_render/os.h"

extern Window_Info window_info;
extern Keyboard_State keyboard_state;


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
	SKILL_BRANCH_DAMAGE,
	SKILL_NUMBER
};

struct Combat_State;

Chat::Chat()
{
	m_enabled = false;
	m_active = true;
}

linked::Window* Chat::init_chat()
{
	history_index = 0;
#define CHAT_MAX_MSGS 7
	using namespace linked;
	Window* result = 0;

	// Chat Window
	chatWindow = new Window(500, 140, hm::vec2(10, window_info.height - 150), hm::vec4(0.16f, 0.15f, 0.2f, 0.5f), (unsigned char*)std::string("Chat").c_str(), 5,
		W_BORDER | W_MOVABLE);
	chatWindow->setBorderColor(hm::vec4(0, 0, 0.1f, 0.6f));
	chatWindow->setBorderSizeX(1);
	chatWindow->setBorderSizeY(1);

	chatDiv = new WindowDiv(*chatWindow, 500, 25, 0, 0, hm::vec2(0, 0), hm::vec4(0, 0, 0, 0.3f),
		DIV_ANCHOR_BOTTOM | DIV_ANCHOR_LEFT);
	chatWindow->divs.push_back(chatDiv);

	chatLabel = new Label(*chatDiv, (unsigned char*)chatString.c_str(), chatString.size(), hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_LIGHT_14, 5, 0);
	chatDiv->getLabels().push_back(chatLabel);

	messagesDiv = new WindowDiv(*chatWindow, 500, 125, 0, 0, hm::vec2(0, 0), hm::vec4(0, 0, 0, 0),
		DIV_ANCHOR_TOP | DIV_ANCHOR_LEFT);
	messagesDiv->m_render = true;
	chatWindow->divs.push_back(messagesDiv);

	for (unsigned int i = 0; i < CHAT_MAX_MSGS; i++)
		messagesDiv->getLabels().push_back(new Label(*messagesDiv, nullptr, 0, hm::vec2(0, 105 - ((i + 1) * 15)), hm::vec4(1, 1, 1, 1), FONT_OSWALD_LIGHT_14, 5, 0));

	return chatWindow;
}

static s32 next_int(const char* str, int* advance) {
	char* start = (char*)str + *advance;
	str = start;
	int length = 0;
	while (is_number(*str)) {
		length++;
		str++;
	}
	*advance += length;
	return str_to_s32(start, length);
}

s32 execute_skill(Skill_ID id, int target_index, int source_index, Combat_State* combat_state);
extern Combat_State combat_state;

void execute_instruction(std::string& msg) {
	// use 1 2
	const char* str = msg.c_str();
	int at = 0;
	if (!str_equal(str, sizeof("use") - 1, "use", sizeof("use") - 1))
		return;
	else {
		at = sizeof("use") - 1;
		if (str[at] != ' ')
			return;
		while (str[at] == ' ') at += 1;
		int skill_index = next_int(str, &at);
		if (str[at] != ' ')
			return;
		while (str[at] == ' ') at += 1;
		int source_index = next_int(str, &at);
		if (str[at] != ' ')
			return;
		while (str[at] == ' ') at += 1;
		int target_index = next_int(str, &at);

		execute_skill((Skill_ID)skill_index, target_index, source_index, &combat_state);
	}
}

void Chat::update() {
	if (msg != "") {
		execute_instruction(msg);
		set_next_message(msg);
		msg = "";
	}

	chatString = ss.str();

	if (m_enabled)
		chatDiv->setBackgroundColor(hm::vec4(0, 0, 0, 0.3f));
	else
		chatDiv->setBackgroundColor(hm::vec4(0, 0, 0, 0));

	chatLabel->setText((unsigned char*)chatString.c_str(), chatString.size() + 1);
}

void Chat::handle_keystroke(int key, int params)
{
	if (m_enabled) {
		// clear the key events
		for (int i = 0; i < MAX_KEYS; ++i) {
			keyboard_state.key_event[i] = false;
		}
	}
	// Enter
	if (key == 13) {
		if (m_enabled)
		{	
			msg = ss.str();
			ss.str("");
		}
		m_enabled = !m_enabled;
	}
	else {
		if (key == 8)
		{
			std::string s = ss.str();
			if (s.size() > 0)
			{
				ss.clear();
				s.erase(s.end() - 1, s.end());
				ss.str(s);
				ss.seekp(0, std::ios_base::end);
			}
		}
		if (m_enabled && key >= 10)
		{
			ss << (char)key;
		}
	}
}

void Chat::next_history()
{
	int num_msg = messages.size();
	if (num_msg == 0) return;
	if (history_index == CHAT_MAX_MSGS) {
		history_index = 0;
	} else {
		history_index++;
	}
	history_index = (history_index == num_msg) ? 0 : history_index;
	ss.str(messages[num_msg - 1 - history_index]);
}

void Chat::previous_history()
{
	int num_msg = messages.size();
	if (num_msg == 0) return;
	if (history_index == 0) {
		history_index = num_msg - 1;
	} else {
		history_index--;
	}
	ss.str(messages[num_msg - 1 - history_index]);
}

void Chat::set_next_message(std::string& msg) {
	linked::Label* l = messagesDiv->getLabels()[0];
	if (l->getYAdvance() > 0) {
		for (int i = 0; i < l->getYAdvance(); i++) {
			if (messages.size() < CHAT_MAX_MSGS) {
				//messages.insert(messages.begin(), "");
			} else {
				messages.erase(messages.begin() + messages.size() - 1);
				//messages.insert(messages.begin(), "");
			}
		}
	}

	if (messages.size() < CHAT_MAX_MSGS) {
		messages.insert(messages.begin(), msg);
	} else {
		messages.erase(messages.begin() + messages.size() - 1);
		messages.insert(messages.begin(), msg);
	}

	for (unsigned int i = 0; i < messages.size(); i++) {
		linked::Label* l = messagesDiv->getLabels()[i];
		l->setText((unsigned char*)messages[i].c_str(), messages[i].size() + 1);
	}
}