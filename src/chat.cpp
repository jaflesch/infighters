#include "chat.h"
#include "input.h"
#include "debug_table.h"
#include "util.h"

extern Window_State win_state;
extern Keyboard_State keyboard_state;
extern DebugTable debug_table;

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
	chatWindow = new Window(500, 140, hm::vec2(10, win_state.win_height - 150), hm::vec4(0.16f, 0.15f, 0.2f, 0.5f), (unsigned char*)std::string("Chat").c_str(), 5,
		W_BORDER | W_MOVABLE);
	chatWindow->setBorderColor(hm::vec4(0, 0, 0.1f, 0.6f));
	chatWindow->setBorderSizeX(1);
	chatWindow->setBorderSizeY(1);

	chatDiv = new WindowDiv(*chatWindow, 500, 25, 0, 0, hm::vec2(0, 0), hm::vec4(0, 0, 0, 0.3f),
		DIV_ANCHOR_BOTTOM | DIV_ANCHOR_LEFT);
	chatWindow->divs.push_back(chatDiv);

	chatLabel = new Label(*chatDiv, (unsigned char*)chatString.c_str(), chatString.size(), hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), 30, 5, 0);
	chatDiv->getLabels().push_back(chatLabel);

	messagesDiv = new WindowDiv(*chatWindow, 500, 125, 0, 0, hm::vec2(0, 0), hm::vec4(0, 0, 0, 0),
		DIV_ANCHOR_TOP | DIV_ANCHOR_LEFT);
	messagesDiv->m_render = false;
	chatWindow->divs.push_back(messagesDiv);

	for (unsigned int i = 0; i < CHAT_MAX_MSGS; i++)
		messagesDiv->getLabels().push_back(new Label(*messagesDiv, nullptr, 0, hm::vec2(0, 105 - ((i + 1) * 15)), hm::vec4(1, 1, 1, 1), 30, 5, 0));

	return chatWindow;
}

void Chat::update() {
	if (msg != "") {
		set_next_message(msg);
		msg = "";
	}

	chatString = ss.str();

	if (m_enabled)
		chatDiv->setBackgroundColor(hm::vec4(0, 0, 0, 0.3f));
	else
		chatDiv->setBackgroundColor(hm::vec4(0, 0, 0, 0));

	chatLabel->setText((unsigned char*)chatString.c_str(), chatString.size());
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
		l->setText((unsigned char*)messages[i].c_str(), messages[i].size());
	}
}