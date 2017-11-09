#pragma once
#include "WindowApi\Window.h"
#include <string>
#include <sstream>
#include <vector>

struct Chat {
	std::string chatString;
	std::stringstream ss;
	std::string msg;

	Chat();
	void update();
	void set_next_message(std::string& msg);

	linked::Window* init_chat();
	void handle_keystroke(int key, int params);
	void next_history();
	void previous_history();
	int history_index;

	bool m_enabled;
	bool m_active;

	linked::WindowDiv* chatDiv;
	linked::Window* chatWindow;
	linked::Label* chatLabel;
	linked::WindowDiv* messagesDiv;

	std::vector<std::string> messages;

	//Parser parser;
};