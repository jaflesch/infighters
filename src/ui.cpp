// Layout initialization functions
void hide_all_windows() {
	//gw.bgwindow->setActive(false);

	gw.settings_window->setActive(false);

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
	gw.exchange_orb_ui->window->setActive(false);
	gw.sacrifice_orb_ui->window->setActive(false);
}

void init_intro_mode() {
	Texture* logo = new Texture("../../../res/logo_clean.png");
	Texture* playbutton_normal = new Texture("../../../res/textures/playbutton_normal.png");
	Texture* playbutton_hover = new Texture("../../../res/textures/playbutton_hover.png");

	linked::Window* logo_window = new linked::Window(logo->width, logo->height, hm::vec2((r32)(window_info.width / 2 - logo->width / 2), (r32)(window_info.height / 2 - logo->height / 2)), hm::vec4(0, 0, 0, 0), 0, 0, linked::W_UNFOCUSABLE);
	linked::WindowDiv* logo_div = new linked::WindowDiv(*logo_window, logo->width, logo->height, 0, 0, hm::vec2(0, 0), hm::vec4(0, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	//logo_div->setBackgroundTexture(logo);
	logo_window->divs.push_back(logo_div);
	gw.intro_logo = logo_window;

	linked::Label* play_label = new linked::Label(*logo_div, (u8*)"PLAY", sizeof("PLAY"), hm::vec2(90.0f, 15.0f), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_24, 0, 0);
	linked::Button* play_button = new linked::Button(*logo_div, play_label, hm::vec2((r32)(logo->width / 2 - 230 / 2), (r32)logo->height - 140.0f), 230, 60, hm::vec4(0.91f, 0.23f, 0.88f, 1), 0);
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
	linked::Window* char_selected_window = new linked::Window(6 * (int)char_window_width + 100, 200, hm::vec2(520, 670), hm::vec4(12.0f / 255.0f, 16.0f / 255.0f, 40.0f / 255.0f, 0.55f), 0, 0, linked::W_UNFOCUSABLE);
	gw.char_selected_window = char_selected_window;

	linked::WindowDiv* s_div1 = new linked::WindowDiv(*char_selected_window, (int)char_window_width, (int)char_window_width, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 0.0f + char_window_width * 0.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* s_div2 = new linked::WindowDiv(*char_selected_window, (int)char_window_width, (int)char_window_width, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 1.0f + char_window_width * 1.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* s_div3 = new linked::WindowDiv(*char_selected_window, (int)char_window_width, (int)char_window_width, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 2.0f + char_window_width * 2.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
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
	Texture* space_texture = new Texture("../../../res/keys/spacebar.png");
	info_div->setBackgroundTexture(space_texture);
	char_selected_window->divs.push_back(info_div);

	linked::WindowDiv* info_label_div = new linked::WindowDiv(*char_selected_window, 256, 24, 0, 0, hm::vec2(624.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_selected_window->divs.push_back(info_label_div);
	linked::Label* info_label = new linked::Label(*info_label_div, (u8*)"Information", sizeof("Information"), hm::vec2(5.0f, 2.0f), hm::vec4(1, 1, 1, 1), FONT_OSWALD_LIGHT_16, 0, 0);
	info_label_div->getLabels().push_back(info_label);

	linked::WindowDiv* confirm_div = new linked::WindowDiv(*char_selected_window, 24, 24, 0, 0, hm::vec2(740.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	Texture* enter_texture = new Texture("../../../res/keys/enter.png");
	confirm_div->setBackgroundTexture(enter_texture);
	char_selected_window->divs.push_back(confirm_div);

	linked::WindowDiv* confirm_label_div = new linked::WindowDiv(*char_selected_window, 256, 24, 0, 0, hm::vec2(764.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_selected_window->divs.push_back(confirm_label_div);
	linked::Label* confirm_label = new linked::Label(*confirm_label_div, (u8*)"Confirm", sizeof("Confirm"), hm::vec2(5.0f, 2.0f), hm::vec4(1, 1, 1, 1), FONT_OSWALD_LIGHT_16, 0.0f, 0);
	confirm_label_div->getLabels().push_back(confirm_label);

	linked::WindowDiv* back_div = new linked::WindowDiv(*char_selected_window, 24, 24, 0, 0, hm::vec2(840.0f, 20.0f + 140.0f), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	Texture* esc_texture = new Texture("../../../res/keys/esc.png");
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
	linked::Window* char_selection_window = new linked::Window(6 * (int)char_window_width + 100, 630, hm::vec2(520, 30), hm::vec4(12.0f / 255.0f, 16.0f / 255.0f, 40.0f / 255.0f, 0.55f), 0, 0, linked::W_UNFOCUSABLE);
	gw.char_selection_window = char_selection_window;

	Texture* chars_texture[NUM_CHARS] = {};

	chars_texture[0] = new Texture("../../../res/textures/zero.png");
	chars_texture[1] = new Texture("../../../res/textures/one.png");
	chars_texture[2] = new Texture("../../../res/textures/serial.png");
	chars_texture[3] = new Texture("../../../res/textures/ray.png");
	chars_texture[4] = new Texture("../../../res/textures/astar.png");
	chars_texture[5] = new Texture("../../../res/textures/deadlock.png");
	chars_texture[6] = new Texture("../../../res/textures/norma.png");
	chars_texture[7] = new Texture("../../../res/textures/hazard.png");
	chars_texture[8] = new Texture("../../../res/textures/qwerty.png");
	chars_texture[9] = new Texture("../../../res/textures/bigo.png");
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
		char_divs[i] = new linked::WindowDiv(*char_selection_window, (int)char_window_width, (int)char_window_height, 0.0f, 0.0f,
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
		linked::Button* button = new linked::Button(*char_divs[i], 0, hm::vec2(0, 0), (int)char_window_width, (int)char_window_height, char_window_color, i);
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

	linked::Window* char_info_window = new linked::Window(6 * (int)char_window_width + 100 - 20, 630, hm::vec2(530, 30), hm::vec4(12.0f / 255.0f, 16.0f / 255.0f, 40.0f / 255.0f, 0.9f), 0, 0, linked::W_BORDER | linked::W_UNFOCUSABLE);
	char_info_window->setBorderSizeX(10.0f);
	char_info_window->setBorderSizeY(0.0f);
	hm::vec4 border_color = hm::vec4(1.0f, 0.71f, 0.29f, 1.0f);
	char_info_window->setBorderColor(border_color);
	gw.char_info_window = char_info_window;

	linked::Window* char_info_window_bot = new linked::Window(6 * (int)char_window_width + 100, 200, hm::vec2(520, 670), hm::vec4(12.0f / 255.0f, 16.0f / 255.0f, 40.0f / 255.0f, 0.55f), 0, 0, 0);
	gw.char_info_window_bot = char_info_window_bot;

	linked::WindowDiv* s_div1 = new linked::WindowDiv(*char_info_window_bot, (int)char_window_width, (int)char_window_width, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 0.0f + char_window_width * 0.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* s_div2 = new linked::WindowDiv(*char_info_window_bot, (int)char_window_width, (int)char_window_width, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 1.0f + char_window_width * 1.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* s_div3 = new linked::WindowDiv(*char_info_window_bot, (int)char_window_width, (int)char_window_width, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 2.0f + char_window_width * 2.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
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
	Texture* esc_texture = new Texture("../../../res/keys/esc.png");
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
		skills_divs[i] = new linked::WindowDiv(*char_info_window, (int)char_window_width, (int)char_window_height, 0.0f, 0.0f,
			hm::vec2(start_offset_x + skills_spacing * char_div_offset_x + char_window_width * char_div_offset_x, skills_spacing * (char_div_offset_y + 1.0f) + char_div_offset_y * char_window_height),
			char_window_color, linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		char_info_window->divs.push_back(skills_divs[i]);

		linked::Button* button = new linked::Button(*skills_divs[i], 0, hm::vec2(0, 0), (int)char_window_width, (int)char_window_height, char_window_color, i);
		button->setHoveredBGColor(char_window_hover_color);
		button->setHeldBGColor(char_window_held_color);
		skills_divs[i]->getButtons().push_back(button);
		button->button_info.data = &char_info_window->divs;
		char_div_offset_x += 1.0f;
	}

	// gotta be first
	linked::WindowDiv* skill_title_div = new linked::WindowDiv(*char_info_window, 300, 48, 0, 0, hm::vec2(start_offset_x, 140.0f + 55.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_info_window->divs.push_back(skill_title_div);
	linked::Label* skill_title_label = new linked::Label(*skill_title_div, (u8*)"", 0, hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_18, 0, 0);
	skill_title_div->getLabels().push_back(skill_title_label);

	// gotta be second
	linked::WindowDiv* skill_desc_div = new linked::WindowDiv(*char_info_window, 300, 48, 0, 0, hm::vec2(start_offset_x, 140.0f + 105.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	char_info_window->divs.push_back(skill_desc_div);
	linked::Label* skill_desc_label = new linked::Label(*skill_desc_div, (u8*)"", 0, hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_LIGHT_16, 0, 0);
	skill_desc_div->getLabels().push_back(skill_desc_label);

	linked::Window* skill_cost = new linked::Window(300, 48, hm::vec2(530 + 580, 40 + char_window_width + 20 * 2), hm::vec4(0, 0, 0, 0), 0, 0, linked::W_UNFOCUSABLE);
	gw.char_info_skill_cost = skill_cost;

	float orb_size = 32.0f;
	linked::WindowDiv* skill_cost_div_1 = new linked::WindowDiv(*skill_cost, (int)orb_size, (int)orb_size, 0, 0, hm::vec2(0, 0), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	skill_cost->divs.push_back(skill_cost_div_1);
	linked::WindowDiv* skill_cost_div_2 = new linked::WindowDiv(*skill_cost, (int)orb_size, (int)orb_size, 0, 0, hm::vec2(0 + (orb_size + 5), 0), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	skill_cost->divs.push_back(skill_cost_div_2);
	linked::WindowDiv* skill_cost_div_3 = new linked::WindowDiv(*skill_cost, (int)orb_size, (int)orb_size, 0, 0, hm::vec2(0 + (orb_size + 5) * 2, 0), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	skill_cost->divs.push_back(skill_cost_div_3);
	linked::WindowDiv* skill_cost_div_4 = new linked::WindowDiv(*skill_cost, (int)orb_size, (int)orb_size, 0, 0, hm::vec2(0 + (orb_size + 5) * 3, 0), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	skill_cost->divs.push_back(skill_cost_div_4);
	linked::WindowDiv* skill_cost_div_5 = new linked::WindowDiv(*skill_cost, (int)orb_size, (int)orb_size, 0, 0, hm::vec2(0 + (orb_size + 5) * 4, 0), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	skill_cost->divs.push_back(skill_cost_div_5);

	linked::WindowDiv* skill_group_div = new linked::WindowDiv(*char_info_window, 640, 48, 0, 0, hm::vec2(start_offset_x, 630.0f - 48.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	gw.skill_group_div = skill_group_div;
	char_info_window->divs.push_back(skill_group_div);
	linked::Label* skill_group_label = new linked::Label(*skill_group_div, (u8*)/*"VIRTUAL, RANGED, INV, STATIC"*/"", 0, hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_LIGHT_12, 0, 0);
	skill_group_div->getLabels().push_back(skill_group_label);

	linked::Label* skill_cooldown_label = new linked::Label(*skill_group_div, (u8*)"", 0, hm::vec2(600, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_LIGHT_12, 0, 0);
	skill_group_div->getLabels().push_back(skill_cooldown_label);
}

static Exchange_Orbs_UI exchange_orb_modal_ui = {};

static void layout_set_exchange_modal_quantity(bool orb_left, Orb_ID orb_id, s32 quantity) {
	char buffer[16] = {};
	if (quantity > 999) quantity = 999;
	s32 length = s32_to_str_base10(quantity, buffer);
	if (orb_left) {
		u8* t = exchange_orb_modal_ui.orbs_left->getLabels()[orb_id + 1]->getText();
		for (int i = 0; i < 3; ++i)
			t[i] = ' ';
		for (int i = 0; i < length; ++i)
			t[i] = buffer[i];
	} else {
		u8* t = exchange_orb_modal_ui.orbs_new->getLabels()[orb_id + 1]->getText();
		for (int i = 0; i < 3; ++i)
			t[i] = ' ';
		for (int i = 2, index = length - 1; index >= 0; --i, --index) {
			t[i] = buffer[index];
		}
	}
}

static void layout_set_exchange_modal_upper_orbs(Orb_ID id, s32 index) {
	if (id == ORB_NONE) {
		gw.exchange_orb_ui->upper_orbs_div->getButtons()[index]->setAllBGTexture(gw.exchange_orb_ui->empty_orb_texture);
	} else {
		gw.exchange_orb_ui->upper_orbs_div->getButtons()[index]->setAllBGTexture(orb_textures[id]);
	}
}

void init_exchange_orbs_modal() {
	using namespace linked;

	gw.exchange_orb_ui = &exchange_orb_modal_ui;

	linked::Window* exchange_orbs = new linked::Window(450, 380, hm::vec2((r32)window_info.width / 2 - 450 / 2, (r32)window_info.height / 2 - 380 / 2), char_window_color, (u8*)"Exchange Orbs", sizeof "Exchange Orbs",
		linked::W_HEADER | linked::W_BORDER | linked::W_MOVABLE);
	exchange_orbs->setBorderColor(cyan);
	exchange_orbs->setTitleColor(hm::vec4(0x0B8383ff));
	exchange_orbs->setTitleCentered(true);
	exchange_orb_modal_ui.window = exchange_orbs;

	// Orbs filling up
	WindowDiv* upper_orbs = new WindowDiv(*exchange_orbs, 140, 40, 0, 0, hm::vec2(0, 20), hm::vec4(1, 0, 0, 0), DIV_CENTER_X | DIV_ANCHOR_TOP);
	exchange_orbs->divs.push_back(upper_orbs);
	exchange_orb_modal_ui.upper_orbs_div = upper_orbs;

	Button* upper_orb_buttons[3] = {};
	Texture* orb_empty = new Texture("../../../res/orbs/exchange_empty_orb_modal.png");
	exchange_orb_modal_ui.empty_orb_texture = orb_empty;
	for (int i = 0; i < 3; ++i) {
		const r32 spacing = 10.0f;
		upper_orb_buttons[i] = new Button(*upper_orbs, 0, hm::vec2((r32)i * (spacing + 40.0f), 0), 40, 40, hm::vec4(0, 1, 0, 1), i);
		upper_orb_buttons[i]->setAllBGTexture(orb_empty);
		upper_orbs->getButtons().push_back(upper_orb_buttons[i]);
	}
	
	// Orbs Left side (Orbs Left)
	WindowDiv* orbs_left = new WindowDiv(*exchange_orbs, 140, 200, 0, 0, hm::vec2(30, 0), hm::vec4(1, 0, 0, 0), DIV_ANCHOR_LEFT | DIV_CENTER_Y | DIV_ANCHOR_TOP);
	exchange_orbs->divs.push_back(orbs_left);
	Label* orbs_left_label = new Label(*orbs_left, (u8*)"ORBS LEFT", sizeof("ORBS LEFT"), hm::vec2(0, 0), hm::vec4(0x0B8383ff), FONT_OSWALD_REGULAR_18, 0, 0);
	orbs_left->getLabels().push_back(orbs_left_label);
	exchange_orb_modal_ui.orbs_left = orbs_left;

	for (int i = 0; i < ORB_NUMBER - 1; ++i) {
		const r32 y_spacing = 10.0f;
		Button* orb = new Button(*orbs_left, 0, hm::vec2(0.0f, 50.0f + (r32)i * (30.0f + y_spacing)), 30, 30, hm::vec4(1, 1, 0, 1), i);
		orb->setAllBGTexture(orb_textures[i]);
		orbs_left->getButtons().push_back(orb);
		u8* text = (u8*)calloc(3, sizeof(u8));
		text[0] = '0';
		text[1] = ' ';
		text[2] = ' ';
		Label* quantity = new Label(*orbs_left, text, 4, hm::vec2(40, 55.0f + (r32)i * (30.0f + y_spacing)), hm::vec4(1,1,1,1), FONT_OSWALD_REGULAR_18, 0, 0);
		orbs_left->getLabels().push_back(quantity);
	}

	// Orbs Right side (New Orbs)
	WindowDiv* orbs_new = new WindowDiv(*exchange_orbs, 140, 200, 0, 0, hm::vec2(-30, 0), hm::vec4(1, 0, 0, 0), DIV_ANCHOR_RIGHT | DIV_CENTER_Y | DIV_ANCHOR_TOP);
	exchange_orbs->divs.push_back(orbs_new);
	Label* new_orbs_label = new Label(*orbs_new, (u8*)"NEW ORBS", sizeof("NEW ORBS"), hm::vec2(60, 0), hm::vec4(0x0B8383ff), FONT_OSWALD_REGULAR_18, 0, 0);
	orbs_new->getLabels().push_back(new_orbs_label);
	exchange_orb_modal_ui.orbs_new = orbs_new;

	for (int i = 0; i < ORB_NUMBER - 1; ++i) {
		const r32 y_spacing = 10.0f;
		Button* orb = new Button(*orbs_new, 0, hm::vec2(108.0f, 50.0f + (r32)i * (30.0f + y_spacing)), 30, 30, hm::vec4(1, 1, 0, 1), i);
		orb->setAllBGTexture(orb_textures[i]);
		orbs_new->getButtons().push_back(orb);
		u8* text = (u8*)calloc(3, sizeof(u8));
		text[0] = ' ';
		text[1] = ' ';
		text[2] = '0';
		Label* quantity = new Label(*orbs_new, text, 4, hm::vec2(70, 55.0f + (r32)i * (30.0f + y_spacing)), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_18, 0, 0);
		orbs_new->getLabels().push_back(quantity);
	}

	// Arrows
	WindowDiv* arrows_div = new WindowDiv(*exchange_orbs, 60, 200, 0, 0, hm::vec2(0, 140), hm::vec4(1, 0, 0, 0), DIV_ANCHOR_TOP | DIV_CENTER_X);
	exchange_orbs->divs.push_back(arrows_div);

	Texture* arrow_left_texture = new Texture("../../../res/orbs/arrow_normal_left.png");
	Texture* arrow_right_texture = new Texture("../../../res/orbs/arrow_normal_right.png");
	Texture* arrow_left_texture_hover = new Texture("../../../res/orbs/arrow_active_left.png");
	Texture* arrow_right_texture_hover = new Texture("../../../res/orbs/arrow_active_right.png");
	Texture* arrow_inactive_left = new Texture("../../../res/orbs/arrow_inactive_left.png");
	Texture* arrow_inactive_right = new Texture("../../../res/orbs/arrow_inactive_right.png");
	for (int i = 0; i < ORB_NUMBER - 1; ++i) {
		const r32 y_spacing = 10.0f;
		// left arrows
		Button* left = new Button(*arrows_div, 0, hm::vec2(0, (r32)i * (30.0f + y_spacing)), 16, 24, hm::vec4(0, 1, 0, 1), i);
		left->setAllBGTexture(arrow_left_texture_hover);
		left->setNormalBGTexture(arrow_left_texture);
		left->setInactiveTexture(arrow_inactive_left);
		left->setClickedCallback(button_callback_exchange_orb_arrow_left);
		arrows_div->getButtons().push_back(left);
		exchange_orb_modal_ui.arrows_left[i] = left;

		Button* right = new Button(*arrows_div, 0, hm::vec2(45, (r32)i * (30.0f + y_spacing)), 16, 24, hm::vec4(0, 0, 1, 1), i);
		right->setAllBGTexture(arrow_right_texture_hover);
		right->setNormalBGTexture(arrow_right_texture);
		right->setInactiveTexture(arrow_inactive_right);
		right->setClickedCallback(button_callback_exchange_orb_arrow_right);
		arrows_div->getButtons().push_back(right);
		exchange_orb_modal_ui.arrows_right[i] = right;
	}

	// Confirm and cancel buttons
	WindowDiv* confirm_cancel_button = new WindowDiv(*exchange_orbs, 390, 70, 0, 0, hm::vec2(0, 0), hm::vec4(1, 0, 0, 0), DIV_ANCHOR_BOTTOM | DIV_CENTER_X);
	exchange_orbs->divs.push_back(confirm_cancel_button);

	Button* confirm = new Button(*confirm_cancel_button, new Label(*confirm_cancel_button, (u8*)"EXCHANGE", sizeof("EXCHANGE"), hm::vec2(56, 15), hm::vec4(1,1,1,1), FONT_OSWALD_BOLD_18, 0, 0),
		hm::vec2(0, 0), 185, 55, hm::vec4(1, 1, 1, 1), 0);
	Texture* confirm_button_texture = new Texture("../../../res/textures/playbutton_normal.png");
	Texture* confirm_button_hover_texture = new Texture("../../../res/textures/playbutton_hover.png");
	confirm->setAllBGTexture(confirm_button_hover_texture);
	confirm->setNormalBGTexture(confirm_button_texture);
	confirm_cancel_button->getButtons().push_back(confirm);
	exchange_orb_modal_ui.confirm = confirm;
	confirm->setClickedCallback(button_callback_exchange_orb_confirm);

	Button* cancel = new Button(*confirm_cancel_button, new Label(*confirm_cancel_button, (u8*)"CANCEL", sizeof("CANCEL"), hm::vec2(66, 15), cyan, FONT_OSWALD_REGULAR_18, 0, 0),
		hm::vec2(204, 0), 185, 55, hm::vec4(1, 1, 1, 1), 0);
	Texture* cancel_button_texture = new Texture("../../../res/textures/cancel_action_modal.png");
	cancel->setAllBGTexture(cancel_button_texture);
	confirm_cancel_button->getButtons().push_back(cancel);
	exchange_orb_modal_ui.cancel = cancel;
	cancel->setClickedCallback(button_callback_exchange_orb_cancel);
}

static Sacrifice_Orbs_UI sacrifice_orb_modal_ui = {};

static void layout_set_sacrifice_modal_quantity(bool orb_left, Orb_ID orb_id, s32 quantity) {
	char buffer[16] = {};
	if (quantity > 999) quantity = 999;
	s32 length = s32_to_str_base10(quantity, buffer);
	if (orb_left) {
		u8* t = sacrifice_orb_modal_ui.orbs_left->getLabels()[orb_id + 1]->getText();
		for (int i = 0; i < 3; ++i)
			t[i] = ' ';
		for (int i = 0; i < length; ++i)
			t[i] = buffer[i];
	}
	else {
		u8* t = sacrifice_orb_modal_ui.orbs_right->getLabels()[orb_id + 1]->getText();
		for (int i = 0; i < 3; ++i)
			t[i] = ' ';
		for (int i = 2, index = length - 1; index >= 0; --i, --index) {
			t[i] = buffer[index];
		}
	}
}

static void init_sacrifice_orbs_modal() {
	using namespace linked;

	gw.sacrifice_orb_ui = &sacrifice_orb_modal_ui;

	// Sacrifice orb window
	Window* null_orb_modal = new Window(450, 430, hm::vec2((r32)window_info.width / 2 - 450 / 2, (r32)window_info.height / 2 - 420 / 2), char_window_color, 
		(u8*)"End turn", sizeof "End turn",	W_HEADER | W_BORDER | W_MOVABLE);
	null_orb_modal->setBorderColor(cyan);
	null_orb_modal->setTitleColor(hm::vec4(0x0B8383ff));
	null_orb_modal->setTitleCentered(true);
	null_orb_modal->setActive(true);
	gw.sacrifice_orb_ui->window = null_orb_modal;

	WindowDiv* upper_text = new WindowDiv(*null_orb_modal, 280, 40, 0, 0, hm::vec2(0, 10), hm::vec4(1, 0, 0, 0), DIV_ANCHOR_TOP | DIV_CENTER_X);
	null_orb_modal->divs.push_back(upper_text);
	Label* upper_text_label = new Label(*upper_text, (u8*)"CHOOSE          RANDOM ORBS", sizeof("CHOOSE          RANDOM ORBS"), hm::vec2(0, 10), hm::vec4(1,1,1,1), FONT_OSWALD_REGULAR_24, 0, 0);
	upper_text->getLabels().push_back(upper_text_label);
	u8* text = (u8*)calloc(1, sizeof(u8));
	text[0] = '0';
	Label* upper_text_number_label = new Label(*upper_text, text, 2, hm::vec2(96, 4), hm::vec4(1, 0, 1, 1), FONT_OSWALD_REGULAR_32, 0, 0);
	upper_text->getLabels().push_back(upper_text_number_label);
	gw.sacrifice_orb_ui->orb_number_label = upper_text_number_label;

	// Orbs Left side (Orbs Left)
	WindowDiv* orbs_left = new WindowDiv(*null_orb_modal, 140, 200, 0, 0, hm::vec2(30, 70), hm::vec4(1, 0, 0, 0), DIV_ANCHOR_LEFT | DIV_ANCHOR_TOP);
	null_orb_modal->divs.push_back(orbs_left);
	Label* orbs_left_label = new Label(*orbs_left, (u8*)"ORBS LEFT", sizeof("ORBS LEFT"), hm::vec2(0, 0), hm::vec4(0x0B8383ff), FONT_OSWALD_REGULAR_18, 0, 0);
	orbs_left->getLabels().push_back(orbs_left_label);
	gw.sacrifice_orb_ui->orbs_left = orbs_left;

	for (int i = 0; i < ORB_NUMBER - 1; ++i) {
		const r32 y_spacing = 10.0f;
		Button* orb = new Button(*orbs_left, 0, hm::vec2(0.0f, 50.0f + (r32)i * (30.0f + y_spacing)), 30, 30, hm::vec4(1, 1, 0, 1), i);
		orb->setAllBGTexture(orb_textures[i]);
		orbs_left->getButtons().push_back(orb);
		u8* text = (u8*)calloc(3, sizeof(u8));
		text[0] = '0';
		text[1] = ' ';
		text[2] = ' ';
		Label* quantity = new Label(*orbs_left, text, 4, hm::vec2(40, 55.0f + (r32)i * (30.0f + y_spacing)), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_18, 0, 0);
		orbs_left->getLabels().push_back(quantity);
	}

	// Orbs Right side (New Orbs)
	WindowDiv* orbs_right = new WindowDiv(*null_orb_modal, 140, 200, 0, 0, hm::vec2(-30, 70), hm::vec4(1, 0, 0, 0), DIV_ANCHOR_RIGHT | DIV_ANCHOR_TOP);
	null_orb_modal->divs.push_back(orbs_right);
	Label* right_orbs_label = new Label(*orbs_right, (u8*)"SACRIFICED ORBS", sizeof("SACRIFICED ORBS"), hm::vec2(8, 0), hm::vec4(0x0B8383ff), FONT_OSWALD_REGULAR_18, 0, 0);
	orbs_right->getLabels().push_back(right_orbs_label);
	gw.sacrifice_orb_ui->orbs_right = orbs_right;

	for (int i = 0; i < ORB_NUMBER - 1; ++i) {
		const r32 y_spacing = 10.0f;
		Button* orb = new Button(*orbs_right, 0, hm::vec2(108.0f, 50.0f + (r32)i * (30.0f + y_spacing)), 30, 30, hm::vec4(1, 1, 0, 1), i);
		orb->setAllBGTexture(orb_textures[i]);
		orbs_right->getButtons().push_back(orb);
		u8* text = (u8*)calloc(3, sizeof(u8));
		text[0] = ' ';
		text[1] = ' ';
		text[2] = '0';
		Label* quantity = new Label(*orbs_right, text, 4, hm::vec2(78, 55.0f + (r32)i * (30.0f + y_spacing)), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_18, 0, 0);
		orbs_right->getLabels().push_back(quantity);
	}

	// Arrows
	WindowDiv* arrows_div = new WindowDiv(*null_orb_modal, 60, 200, 0, 0, hm::vec2(0, 122), hm::vec4(1, 0, 0, 0), DIV_ANCHOR_TOP | DIV_CENTER_X);
	null_orb_modal->divs.push_back(arrows_div);

	Texture* arrow_left_texture = new Texture("../../../res/orbs/arrow_normal_left.png");
	Texture* arrow_right_texture = new Texture("../../../res/orbs/arrow_normal_right.png");
	Texture* arrow_left_texture_hover = new Texture("../../../res/orbs/arrow_active_left.png");
	Texture* arrow_right_texture_hover = new Texture("../../../res/orbs/arrow_active_right.png");
	Texture* arrow_inactive_left = new Texture("../../../res/orbs/arrow_inactive_left.png");
	Texture* arrow_inactive_right = new Texture("../../../res/orbs/arrow_inactive_right.png");
	for (int i = 0; i < ORB_NUMBER - 1; ++i) {
		const r32 y_spacing = 10.0f;
		// left arrows
		Button* left = new Button(*arrows_div, 0, hm::vec2(0, (r32)i * (30.0f + y_spacing)), 16, 24, hm::vec4(0, 1, 0, 1), i);
		left->setAllBGTexture(arrow_left_texture_hover);
		left->setNormalBGTexture(arrow_left_texture);
		left->setInactiveTexture(arrow_inactive_left);
		left->setClickedCallback(button_callback_sacrifice_orb_arrow_left);
		arrows_div->getButtons().push_back(left);
		gw.sacrifice_orb_ui->arrows_left[i] = left;

		Button* right = new Button(*arrows_div, 0, hm::vec2(45, (r32)i * (30.0f + y_spacing)), 16, 24, hm::vec4(0, 0, 1, 1), i);
		right->setAllBGTexture(arrow_right_texture_hover);
		right->setNormalBGTexture(arrow_right_texture);
		right->setInactiveTexture(arrow_inactive_right);
		right->setClickedCallback(button_callback_sacrifice_orb_arrow_right);
		arrows_div->getButtons().push_back(right);
		gw.sacrifice_orb_ui->arrows_right[i] = right;
	}

	// Miniature skills
	WindowDiv* skills_div = new WindowDiv(*null_orb_modal, 42 * NUM_ALLIES + ((NUM_ALLIES - 1) * 10), 42, 0, 0, hm::vec2(0, -90), hm::vec4(1, 0, 0, 0), DIV_ANCHOR_BOTTOM | DIV_CENTER_X);
	null_orb_modal->divs.push_back(skills_div);
	Texture* empty_skill_texture = new Texture("../../../res/orbs/empty_square.png");
	gw.sacrifice_orb_ui->empty_skill_texture = empty_skill_texture;
	gw.sacrifice_orb_ui->skills_miniatures = skills_div;
	for (int i = 0; i < NUM_ALLIES; ++i) {
		const float x_spacing = 10.0f;
		Button* skill = new Button(*skills_div, 0, hm::vec2((r32)i * (42.0f + x_spacing), 0), 42, 42, hm::vec4(1, 1, 1, 1), i);
		skill->setAllBGTexture(empty_skill_texture);
		skills_div->getButtons().push_back(skill);
	}

	// Confirm and cancel buttons
	WindowDiv* confirm_cancel_button = new WindowDiv(*null_orb_modal, 390, 70, 0, 0, hm::vec2(0, 0), hm::vec4(1, 0, 0, 0), DIV_ANCHOR_BOTTOM | DIV_CENTER_X);
	null_orb_modal->divs.push_back(confirm_cancel_button);

	Button* confirm = new Button(*confirm_cancel_button, new Label(*confirm_cancel_button, (u8*)"END TURN", sizeof("END TURN"), hm::vec2(56, 15), hm::vec4(1, 1, 1, 1), FONT_OSWALD_BOLD_18, 0, 0),
		hm::vec2(0, 0), 185, 55, hm::vec4(1, 1, 1, 1), 0);
	Texture* confirm_button_texture = new Texture("../../../res/textures/playbutton_normal.png");
	Texture* confirm_button_hover_texture = new Texture("../../../res/textures/playbutton_hover.png");
	confirm->setAllBGTexture(confirm_button_hover_texture);
	confirm->setNormalBGTexture(confirm_button_texture);
	confirm_cancel_button->getButtons().push_back(confirm);
	gw.sacrifice_orb_ui->endturn = confirm;
	confirm->setClickedCallback(button_callback_sacrifice_orb_endturn);

	Button* cancel = new Button(*confirm_cancel_button, new Label(*confirm_cancel_button, (u8*)"CANCEL", sizeof("CANCEL"), hm::vec2(66, 15), cyan, FONT_OSWALD_REGULAR_18, 0, 0),
		hm::vec2(204, 0), 185, 55, hm::vec4(1, 1, 1, 1), 0);
	Texture* cancel_button_texture = new Texture("../../../res/textures/cancel_action_modal.png");
	cancel->setAllBGTexture(cancel_button_texture);
	confirm_cancel_button->getButtons().push_back(cancel);
	gw.sacrifice_orb_ui->cancel = cancel;
	cancel->setClickedCallback(button_callback_sacrifice_orb_cancel);
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
	char_textures[CHAR_ZERO] = new Texture("../../../res/char_square/zero_sq.png");
	char_textures[CHAR_ONE] = new Texture("../../../res/char_square/one_sq.png");
	char_textures[CHAR_SERIAL_KEYLLER] = new Texture("../../../res/char_square/serial_sq.png");
	char_textures[CHAR_RAY_TRACEY] = new Texture("../../../res/char_square/ray_sq.png");
	char_textures[CHAR_A_STAR] = new Texture("../../../res/char_square/astar_sq.png");
	char_textures[CHAR_DEADLOCK] = new Texture("../../../res/char_square/deadlock_sq.png");
	char_textures[CHAR_NORMA] = new Texture("../../../res/char_square/norma_sq.png");
	char_textures[CHAR_HAZARD] = new Texture("../../../res/char_square/hazard_sq.png");
	char_textures[CHAR_QWERTY] = new Texture("../../../res/char_square/qwerty_sq.png");
	char_textures[CHAR_BIG_O] = new Texture("../../../res/char_square/bigo_sq.png");
	char_textures[CHAR_NEW] = new Texture("../../../res/char_square/new_sq.png");
	char_textures[CHAR_CLOCKBOY] = new Texture("../../../res/char_square/clockboy_sq.png");

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
	orb_textures[ORB_VR] = new Texture("../../../res/orbs/vr_orb.png");
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
		gw.end_turn_button_player_turn = new Texture("../../../res/textures/endturn_button_ally_normal.png");
		gw.end_turn_button_enemy_turn = new Texture("../../../res/textures/endturn_button_enemy_normal.png");
		gw.end_turn_button_player_turn_hover = new Texture("../../../res/textures/endturn_button_ally_hover.png");
		gw.end_turn_button_enemy_turn_hover = new Texture("../../../res/textures/endturn_button_enemy_hover.png");

		linked::WindowDiv* end_turn = new linked::WindowDiv(*player_name, 200, 45, 0, 0, hm::vec2(0, 20.0f), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP | linked::DIV_CENTER_X);
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
		gw.allies[i] = new linked::Window((int)size_img, (int)size_img, start_pos, char_window_color, 0, 0, linked::W_UNFOCUSABLE | linked::W_BORDER);
		gw.allies[i]->setBorderSizeX(1.0f);
		gw.allies[i]->setBorderSizeY(1.0f);
		gw.allies[i]->setBorderColor(cyan);
		linked::WindowDiv* allies_div = new linked::WindowDiv(*gw.allies[i], (int)size_img, (int)size_img, 0, 0, hm::vec2(0, 0), char_window_color, linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		gw.allies[i]->divs.push_back(allies_div);
		linked::Button* dummy_ally_button = new linked::Button(*allies_div, 0, hm::vec2(0, 0), (int)size_img, (int)size_img, hm::vec4(0, 0, 0, 0), i);
		dummy_ally_button->button_info.id = i;
		dummy_ally_button->setClickedCallback(target_ally);
		allies_div->getButtons().push_back(dummy_ally_button);

		start_pos.x += size_img + x_spacing;
		gw.allies_info[i] = new linked::Window((int)size_info, (int)size_img, start_pos, char_window_color, 0, 0, linked::W_UNFOCUSABLE | linked::W_BORDER);
		gw.allies_info[i]->setBorderSizeX(1.0f);
		gw.allies_info[i]->setBorderSizeY(1.0f);
		gw.allies_info[i]->setBorderColor(cyan);

		float x_off = 0.0f;
		for (int k = 0; k < NUM_SKILLS; ++k) {
			gw.allies_skills[i * NUM_SKILLS + k] = new linked::Window((int)skill_img_size, (int)skill_img_size, start_pos + hm::vec2(x_spacing + x_off, hp_bar_height + y_spacing * 2 + 10.0f), hm::vec4(1, 1, 1, 0), 0, 0, linked::W_BORDER | linked::W_UNFOCUSABLE);
			linked::WindowDiv* skill_div = new linked::WindowDiv(*gw.allies_skills[i * NUM_SKILLS + k], (int)skill_img_size, (int)skill_img_size, 0, 0, hm::vec2(0, 0), hm::vec4(0, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
			gw.allies_skills[i * NUM_SKILLS + k]->divs.push_back(skill_div);
			gw.allies_skills[i * NUM_SKILLS + k]->setBorderSizeX(1.0f);
			gw.allies_skills[i * NUM_SKILLS + k]->setBorderSizeY(1.0f);
			gw.allies_skills[i * NUM_SKILLS + k]->setBorderColor(ally_hp_bar_full_color);
			u8* text = (u8*)calloc(1, sizeof(u8));
			text[0] = '1';
			linked::Label* skill_cooldown_label = new linked::Label(*skill_div, text, 2, hm::vec2(30, 16), cyan, FONT_OSWALD_REGULAR_38, 0, 0);
			skill_div->getLabels().push_back(skill_cooldown_label);
			linked::Button* skill_button = new linked::Button(*skill_div, 0, hm::vec2(0, 0), (int)skill_img_size, (int)skill_img_size, hm::vec4(0, 1, 1, 1), k);
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

		linked::WindowDiv* hpempty = new linked::WindowDiv(*gw.allies_info[i], int(size_info - 2 * x_spacing + 2), (int)hp_bar_height + 2, 0, 0, hm::vec2(x_spacing - 1, x_spacing - 1), ally_hp_bar_empty_color, linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		gw.allies_info[i]->divs.push_back(hpempty);

		linked::WindowDiv* hpfilled = new linked::WindowDiv(*gw.allies_info[i], int(size_info - 2 * x_spacing), (int)hp_bar_height, 0, 0, hm::vec2(x_spacing, x_spacing), ally_hp_bar_full_color, linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		gw.allies_info[i]->divs.push_back(hpfilled);

		linked::WindowDiv* status_info = new linked::WindowDiv(*gw.allies_info[i], int(size_info - 2 * x_spacing + 2), (int)hp_bar_height + 2, 0, 0, hm::vec2(x_spacing, y_spacing + hp_bar_height), hm::vec4(0, 0, 1, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		gw.allies_info[i]->divs.push_back(status_info);
		linked::Label* hplabel = new linked::Label(*status_info, (u8*)"100/100", sizeof("100/100"), hm::vec2(size_info - sizeof("100/100") * 10.0f, 0.0f), hm::vec4(0, 1, 1, 1), FONT_OSWALD_REGULAR_16, 0, 0);
		status_info->getLabels().push_back(hplabel);

		r32 begin_negative = 0.0f;
		for (int b = 0; b < MAX_STATUS; ++b) {
			r32 b_spacing = (r32)b * hp_bar_height + 5.0f * (r32)b;
			linked::Button* negative_status = new linked::Button(*status_info, 0, hm::vec2(begin_negative + b_spacing, -2.0f), (int)hp_bar_height, (int)hp_bar_height, hm::vec4(1, 0, 0, 0), 0);
			status_info->getButtons().push_back(negative_status);
		}

		start_pos.x = 60;

		hm::vec2 target_pos = start_pos;
		r32 target_icon_size = 40.0f;
		r32 target_spacing = 10.0f;
		target_pos.x += (size_img + x_spacing + size_info + target_spacing);

		for (int k = 0; k < NUM_ALLIES; ++k) {
			linked::Window* target_window = new linked::Window((int)target_icon_size, (int)target_icon_size, target_pos, char_window_color, 0, 0, linked::W_UNFOCUSABLE | linked::W_BORDER);
			linked::WindowDiv* target_div = new linked::WindowDiv(*target_window, (int)target_icon_size, (int)target_icon_size, 0, 0, hm::vec2(0, 0), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
			target_window->divs.push_back(target_div);
			target_pos.x += 10.0f + target_icon_size;
			gw.ally_target[i][k] = target_window;
		}

		start_pos.y += size_img + y_spacing;
	}

	start_pos.x = 1400.0f;
	start_pos.y = 120.0f;

	for (int i = 0; i < NUM_ENEMIES; ++i) {
		gw.enemies[i] = new linked::Window((int)size_img, (int)size_img, start_pos, char_window_color, 0, 0, linked::W_UNFOCUSABLE | linked::W_BORDER);
		gw.enemies[i]->setBorderSizeX(1.0f);
		gw.enemies[i]->setBorderSizeY(1.0f);
		gw.enemies[i]->setBorderColor(enem_hp_bar_full_color);

		linked::WindowDiv* enemy_image_div = new linked::WindowDiv(*gw.enemies[i], (int)size_img, (int)size_img, 0, 0, hm::vec2(0, 0), hm::vec4(0, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		enemy_image_div->setBackgroundTexture(char_textures[i]);
		gw.enemies[i]->divs.push_back(enemy_image_div);
		linked::Button* dummy_enemy_button = new linked::Button(*enemy_image_div, 0, hm::vec2(0, 0), (int)size_img, (int)size_img, hm::vec4(0, 0, 0, 0), i);
		dummy_enemy_button->button_info.id = i;
		dummy_enemy_button->setClickedCallback(target_enemy);
		enemy_image_div->getButtons().push_back(dummy_enemy_button);

		start_pos.x -= (x_spacing + size_info);
		gw.enemies_info[i] = new linked::Window((int)size_info, (int)size_enemy_bars, start_pos, char_window_color, 0, 0, linked::W_UNFOCUSABLE | linked::W_BORDER);
		gw.enemies_info[i]->setBorderSizeX(1.0f);
		gw.enemies_info[i]->setBorderSizeY(1.0f);
		gw.enemies_info[i]->setBorderColor(enem_hp_bar_full_color);

		linked::WindowDiv* hpempty = new linked::WindowDiv(*gw.enemies_info[i], int(size_info - 2 * x_spacing + 2), (int)hp_bar_height + 2, 0, 0, hm::vec2(x_spacing - 1, x_spacing - 1), enem_hp_bar_empty_color, linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		gw.enemies_info[i]->divs.push_back(hpempty);

		linked::WindowDiv* hpfilled = new linked::WindowDiv(*gw.enemies_info[i], int(size_info - 2 * x_spacing), (int)hp_bar_height, 0, 0, hm::vec2(-x_spacing, x_spacing), enem_hp_bar_full_color, linked::DIV_ANCHOR_RIGHT | linked::DIV_ANCHOR_TOP);
		gw.enemies_info[i]->divs.push_back(hpfilled);

		linked::WindowDiv* status_info = new linked::WindowDiv(*gw.enemies_info[i], int(size_info - 2 * x_spacing + 2), (int)hp_bar_height + 2, 0, 0, hm::vec2(x_spacing, y_spacing + hp_bar_height), hm::vec4(0, 0, 1, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		gw.enemies_info[i]->divs.push_back(status_info);
		linked::Label* hplabel = new linked::Label(*status_info, (u8*)"100/100", sizeof("100/100"), hm::vec2(/*size_info - sizeof("100/100") * 10.0f*/ 0.0f, 0.0f), enem_hp_bar_full_color, FONT_OSWALD_REGULAR_16, 0, 0);
		status_info->getLabels().push_back(hplabel);

		r32 begin_negative = size_info - 2 * x_spacing + 2 - hp_bar_height;
		for (int b = 0; b < MAX_STATUS; ++b) {
			r32 b_spacing = (r32)b * hp_bar_height + 5.0f * (r32)b;
			linked::Button* negative_status = new linked::Button(*status_info, 0, hm::vec2(begin_negative - b_spacing, 0.0f), (int)hp_bar_height, (int)hp_bar_height, hm::vec4(1, 0, 0, 0), 0);
			status_info->getButtons().push_back(negative_status);
		}

		start_pos.x = 1400.0f;

		hm::vec2 target_pos = start_pos;
		r32 target_icon_size = 40.0f;
		r32 target_spacing = 10.0f;
		target_pos.x -= (x_spacing + size_info + target_icon_size + target_spacing);
		s32 max_ = MAX(NUM_ALLIES, NUM_ENEMIES);
		for (int k = 0; k < max_; ++k) {
			linked::Window* target_window = new linked::Window((int)target_icon_size, (int)target_icon_size, target_pos, char_window_color, 0, 0, linked::W_UNFOCUSABLE | linked::W_BORDER);
			linked::WindowDiv* target_div = new linked::WindowDiv(*target_window, (int)target_icon_size, (int)target_icon_size, 0, 0, hm::vec2(0, 0), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
			target_window->divs.push_back(target_div);
			target_pos.x -= 10.0f + target_icon_size;
			gw.enemy_target[i][k] = target_window;
		}

		start_pos.y += size_img + y_spacing;
	}

	{
		float orbs_size = 48.0f;

		hm::vec4 combat_info_bar_color(15.0f / 255.0f, 17.0f / 255.0f, 42.0f / 255.0f, 0.8f);
		linked::Window* combat_bottom_info = new linked::Window(window_info.width, 200, hm::vec2(0, 660), combat_info_bar_color, 0, 0, linked::W_BORDER | linked::W_UNFOCUSABLE);
		linked::WindowDiv* orbs_div = new linked::WindowDiv(*combat_bottom_info, 500, 100, 0, 0, hm::vec2(45.0f, 0), hm::vec4(1, 0, 0, 0.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP | linked::DIV_CENTER_Y);
		combat_bottom_info->divs.push_back(orbs_div);

		linked::Button* multiple_orb_button = new linked::Button(*orbs_div, 0, hm::vec2(0, 0), (int)orbs_size, (int)orbs_size, hm::vec4(0, 0, 0, 1), 0);
		Texture* multiple_orbs_texture = new Texture("../../../res/orbs/all_orbs.png");
		multiple_orb_button->setAllBGTexture(multiple_orbs_texture);
		orbs_div->getButtons().push_back(multiple_orb_button);

		linked::Button* hard_orb_button = new linked::Button(*orbs_div, 0, hm::vec2((orbs_size + 14) + 18, 0), (int)orbs_size, (int)orbs_size, hm::vec4(0, 0, 0, 1), 0);
		hard_orb_button->setAllBGTexture(orb_textures[0]);
		orbs_div->getButtons().push_back(hard_orb_button);

		linked::Button* soft_orb_button = new linked::Button(*orbs_div, 0, hm::vec2((orbs_size + 14) * 2 + 18, 0), (int)orbs_size, (int)orbs_size, hm::vec4(0, 0, 0, 1), 0);
		soft_orb_button->setAllBGTexture(orb_textures[1]);
		orbs_div->getButtons().push_back(soft_orb_button);

		linked::Button* vr_orb_button = new linked::Button(*orbs_div, 0, hm::vec2((orbs_size + 14) * 3 + 18, 0), (int)orbs_size, (int)orbs_size, hm::vec4(0, 0, 0, 1), 0);
		vr_orb_button->setAllBGTexture(orb_textures[2]);
		orbs_div->getButtons().push_back(vr_orb_button);

		linked::Button* bios_orb_button = new linked::Button(*orbs_div, 0, hm::vec2((orbs_size + 14) * 4 + 18, 0), (int)orbs_size, (int)orbs_size, hm::vec4(0, 0, 0, 1), 0);
		bios_orb_button->setAllBGTexture(orb_textures[3]);
		orbs_div->getButtons().push_back(bios_orb_button);

		linked::Button* exchange_orb_button = new linked::Button(*orbs_div, 0, hm::vec2((orbs_size + 14) * 5 + 18, 0), (int)orbs_size, (int)orbs_size, hm::vec4(0, 0, 0, 1), 0);
		Texture* xchg_orb_texture = new Texture("../../../res/orbs/exchange_orb.png");
		exchange_orb_button->setAllBGTexture(xchg_orb_texture);
		hm::vec4 exchange_orb_button_hovered_bgcolor(0, 1, 1, 0.7f);
		exchange_orb_button->setHoveredBGColor(exchange_orb_button_hovered_bgcolor);
		hm::vec4 exchange_orb_button_held_bgcolor(0, 0.38f, 0.32f, 0.7f);
		exchange_orb_button->setHeldBGColor(exchange_orb_button_held_bgcolor);
		orbs_div->getButtons().push_back(exchange_orb_button);
		exchange_orb_button->setClickedCallback(button_callback_exchange_orb);

		r32 orb_pos_offset = 0;
		linked::Label* multiple_orb_label = new linked::Label(*orbs_div, (u8*)"0", sizeof("0"), hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_24, 0, 0);
		orb_pos_offset = 48.0f / 2.0f - multiple_orb_label->getTextWidth() / 2.0f;
		hm::vec2 multiple_orb_label_position(orb_pos_offset, 74);
		multiple_orb_label->setPosition(multiple_orb_label_position);
		orbs_div->getLabels().push_back(multiple_orb_label);
		combat_state.all_orbs_label = multiple_orb_label;

		r32 base_next_orb = (orbs_size + 14) + 18;
		linked::Label* hard_orb_label = new linked::Label(*orbs_div, (u8*)"0", sizeof("0"), hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_24, 0, 0);
		hard_orb_label->setPosition(hm::vec2(base_next_orb + orb_pos_offset, 74));
		orbs_div->getLabels().push_back(hard_orb_label);
		combat_state.orb_labels[ORB_HARD] = hard_orb_label;
		base_next_orb = (orbs_size + 14) * 2 + 18;
		linked::Label* soft_orb_label = new linked::Label(*orbs_div, (u8*)"0", sizeof("0"), hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_24, 0, 0);
		soft_orb_label->setPosition(hm::vec2(base_next_orb + orb_pos_offset, 74));
		orbs_div->getLabels().push_back(soft_orb_label);
		combat_state.orb_labels[ORB_SOFT] = soft_orb_label;
		base_next_orb = (orbs_size + 14) * 3 + 18;
		linked::Label* vr_orb_label = new linked::Label(*orbs_div, (u8*)"0", sizeof("0"), hm::vec2(0, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_24, 0, 0);
		vr_orb_label->setPosition(hm::vec2(base_next_orb + orb_pos_offset, 74));
		orbs_div->getLabels().push_back(vr_orb_label);
		combat_state.orb_labels[ORB_VR] = vr_orb_label;
		base_next_orb = (orbs_size + 14) * 4 + 18;
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
		linked::WindowDiv* skill_cost_div_1 = new linked::WindowDiv(*combat_bottom_info, (int)orb_size, (int)orb_size, 0, 0, hm::vec2(1220.0f, skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_cost_div_1);
		linked::WindowDiv* skill_cost_div_2 = new linked::WindowDiv(*combat_bottom_info, (int)orb_size, (int)orb_size, 0, 0, hm::vec2(1220.0f + (orb_size + 5), skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_cost_div_2);
		linked::WindowDiv* skill_cost_div_3 = new linked::WindowDiv(*combat_bottom_info, (int)orb_size, (int)orb_size, 0, 0, hm::vec2(1220.0f + (orb_size + 5) * 2, skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_cost_div_3);
		linked::WindowDiv* skill_cost_div_4 = new linked::WindowDiv(*combat_bottom_info, (int)orb_size, (int)orb_size, 0, 0, hm::vec2(1220.0f + (orb_size + 5) * 3, skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_cost_div_4);
		linked::WindowDiv* skill_cost_div_5 = new linked::WindowDiv(*combat_bottom_info, (int)orb_size, (int)orb_size, 0, 0, hm::vec2(1220.0f + (orb_size + 5) * 4, skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_cost_div_5);

		skill_cost_div_1->m_render = false;
		skill_cost_div_2->m_render = false;
		skill_cost_div_3->m_render = false;
		skill_cost_div_4->m_render = false;
		skill_cost_div_5->m_render = false;

		combat_state.skill_costs[0] = skill_cost_div_5;
		combat_state.skill_costs[1] = skill_cost_div_4;
		combat_state.skill_costs[2] = skill_cost_div_3;
		combat_state.skill_costs[3] = skill_cost_div_2;
		combat_state.skill_costs[4] = skill_cost_div_1;

		combat_state.skill_info_image = skill_image_div;
		//combat_state.skill_info_image_border = skill_image_div_border;
		combat_state.skill_info_title = skill_title_div;
		combat_state.skill_info_desc = skill_desc_div;
		combat_state.skill_info_group = skill_group_div;
	}

	init_sacrifice_orbs_modal();
	init_exchange_orbs_modal();
	{
		// Status stuff
		Texture* default_status = new Texture("../../../res/status/status_default.png");
		g_layout_status.status_images[SKILL_CONDITION_BURN] = new Texture("../../../res/status/burn.png");
		g_layout_status.status_images[SKILL_CONDITION_FREEZE] = new Texture("../../../res/status/frozen.png");
		g_layout_status.status_images[SKILL_CONDITION_POISON] = new Texture("../../../res/status/poison.png");
		g_layout_status.status_images[SKILL_CONDITION_PARALYZE] = new Texture("../../../res/status/paralyze.png");
		g_layout_status.status_images[SKILL_CONDITION_SLEEP] = new Texture("../../../res/status/sleep.png");
		g_layout_status.status_images[SKILL_CONDITION_STUN] = new Texture("../../../res/status/stun.png");
	}
}

static void init_animation_indices(Skill_ID skill) {
	int maxx = gw.skills_animations[skill]->m_num_cols;
	int maxy = gw.skills_animations[skill]->m_num_rows;
	int i = 0;
	for (int y = maxy - 1; y >= 0; --y) {
		for (int x = 0; x < maxx; ++x) {
			gw.skills_animations[skill]->state_offsets[i] = hm::vec2(x, y);
			++i;
			if (i >= gw.skills_animations[skill]->m_num_indices)
				return;
		}
	}
}

static void init_status_animation_indices(Skill_Condition skill) {
	int maxx = gw.status_animations[skill]->m_num_cols;
	int maxy = gw.status_animations[skill]->m_num_rows;
	int i = 0;
	for (int y = maxy - 1; y >= 0; --y) {
		for (int x = 0; x < maxx; ++x) {
			gw.status_animations[skill]->state_offsets[i] = hm::vec2(x, y);
			++i;
			if (i >= gw.status_animations[skill]->m_num_indices)
				return;
		}
	}
}

void init_animations() {
	gw.animation = new Mesh(new Quad(hm::vec3(0, 0, 0), 160.0f, 160.0f, 1, 0));

	gw.status_animations[SKILL_CONDITION_BURN] = new Animation("../../../res/status_animations/burn.png", 3, 5, 15, 15);
	init_status_animation_indices(SKILL_CONDITION_BURN);
	gw.status_animations[SKILL_CONDITION_FREEZE] = new Animation("../../../res/status_animations/frozen.png", 3, 5, 13, 17);
	init_status_animation_indices(SKILL_CONDITION_FREEZE);
	gw.status_animations[SKILL_CONDITION_PARALYZE] = new Animation("../../../res/status_animations/paralyze.png", 3, 5, 15, 15);
	init_status_animation_indices(SKILL_CONDITION_PARALYZE);
	gw.status_animations[SKILL_CONDITION_POISON] = new Animation("../../../res/status_animations/poison.png", 3, 5, 14, 15);
	init_status_animation_indices(SKILL_CONDITION_POISON);
	gw.status_animations[SKILL_CONDITION_SLEEP] = new Animation("../../../res/status_animations/sleep.png", 3, 5, 14, 15);
	init_status_animation_indices(SKILL_CONDITION_SLEEP);
	gw.status_animations[SKILL_CONDITION_STUN] = new Animation("../../../res/status_animations/stun.png", 1, 5, 5, 20);
	init_status_animation_indices(SKILL_CONDITION_STUN);

	// Zer0
	gw.skills_animations[SKILL_FALSE_RUSH] = new Animation("../../../res/skills_animations/zero/false_rush.png", 3, 5, 15, 15);
	init_animation_indices(SKILL_FALSE_RUSH);
	gw.skills_animations[SKILL_CONTRADICTION] = new Animation("../../../res/skills_animations/zero/contradiction.png", 1, 5, 5, 30);
	init_animation_indices(SKILL_CONTRADICTION);
	gw.skills_animations[SKILL_REQUIEM_ZERO] = new Animation("../../../res/skills_animations/zero/requiem_zero.png", 2, 5, 10, 18);
	init_animation_indices(SKILL_REQUIEM_ZERO);
	gw.skills_animations[SKILL_VOID_BARRIER] = new Animation("../../../res/skills_animations/zero/void_barrier.png", 1, 5, 5, 25);
	init_animation_indices(SKILL_VOID_BARRIER);

	// On1
	gw.skills_animations[SKILL_TRUTH_SLASH] = new Animation("../../../res/skills_animations/one/truth_slash.png", 2, 5, 10, 18);
	init_animation_indices(SKILL_TRUTH_SLASH);
	gw.skills_animations[SKILL_TAUTOLOGY] = new Animation("../../../res/skills_animations/one/tautology.png", 1, 5, 5, 25);
	init_animation_indices(SKILL_TAUTOLOGY);
	gw.skills_animations[SKILL_AXIOM_ONE] = new Animation("../../../res/skills_animations/one/axiom_one.png", 3, 5, 15, 18);
	init_animation_indices(SKILL_AXIOM_ONE);
	gw.skills_animations[SKILL_TRUE_ENDURANCE] = new Animation("../../../res/skills_animations/one/true_endurance.png", 2, 5, 10, 20);
	init_animation_indices(SKILL_TRUE_ENDURANCE);

	// Serial Keyller
	gw.skills_animations[SKILL_BRUTE_FORCE] = new Animation("../../../res/skills_animations/serial_keyller/brute_force.png", 2, 5, 8, 20);
	init_animation_indices(SKILL_BRUTE_FORCE);
	gw.skills_animations[SKILL_BUFFER_OVERFLOW] = new Animation("../../../res/skills_animations/serial_keyller/buffer_overflow.png", 1, 5, 5, 25);
	gw.skills_animations[SKILL_BUFFER_OVERFLOW]->animate_reverse = true;
	init_animation_indices(SKILL_BUFFER_OVERFLOW);
	gw.skills_animations[SKILL_DDOS_ATTACK] = new Animation("../../../res/skills_animations/serial_keyller/ddos_attack.png", 3, 5, 15, 10);
	init_animation_indices(SKILL_DDOS_ATTACK);
	gw.skills_animations[SKILL_ENCRYPTION] = new Animation("../../../res/skills_animations/serial_keyller/encryption.png", 1, 5, 5, 25);
	init_animation_indices(SKILL_ENCRYPTION);

	// Ray Tracey
	gw.skills_animations[SKILL_PARTICLE_RENDERING] = new Animation("../../../res/skills_animations/ray_tracey/particle_rendering.png", 1, 5, 5, 25);
	init_animation_indices(SKILL_PARTICLE_RENDERING);
	gw.skills_animations[SKILL_DIFFUSE_REFLECTION] = new Animation("../../../res/skills_animations/ray_tracey/diffuse_reflection.png", 3, 5, 15, 10);
	init_animation_indices(SKILL_DIFFUSE_REFLECTION);
	gw.skills_animations[SKILL_DYNAMIC_FRUSTUM_ATTACK] = new Animation("../../../res/skills_animations/ray_tracey/dynamic_frustum.png", 2, 5, 10, 18);
	init_animation_indices(SKILL_DYNAMIC_FRUSTUM_ATTACK);
	gw.skills_animations[SKILL_RASTERIZATION] = new Animation("../../../res/skills_animations/ray_tracey/rasterization.png", 1, 5, 5, 25);
	init_animation_indices(SKILL_RASTERIZATION);

	// A-Star
	gw.skills_animations[SKILL_Q_PUNCH] = new Animation("../../../res/skills_animations/astar/q_punch.png", 1, 3, 3, 30);
	init_animation_indices(SKILL_Q_PUNCH);
	gw.skills_animations[SKILL_PERCEPTRON] = new Animation("../../../res/skills_animations/astar/perceptron.png", 2, 5, 7, 23);
	init_animation_indices(SKILL_PERCEPTRON);
	gw.skills_animations[SKILL_NEURAL_NETWORK] = new Animation("../../../res/skills_animations/astar/neural_network.png", 2, 5, 8, 20);
	init_animation_indices(SKILL_NEURAL_NETWORK);
	gw.skills_animations[SKILL_HILL_CLIMBING] = new Animation("../../../res/skills_animations/astar/hill_climbing.png", 1, 5, 5, 25);
	init_animation_indices(SKILL_HILL_CLIMBING);

	// Deadlock
	gw.skills_animations[SKILL_PREEMPTION] = new Animation("../../../res/skills_animations/deadlock/preemption.png", 2, 5, 10, 18);
	init_animation_indices(SKILL_PREEMPTION);
	gw.skills_animations[SKILL_MUTEX] = new Animation("../../../res/skills_animations/deadlock/mutex.png", 3, 5, 15, 15);
	init_animation_indices(SKILL_MUTEX);
	gw.skills_animations[SKILL_THREAD_SCHEDULING] = new Animation("../../../res/skills_animations/deadlock/thread_scheduling.png", 3, 5, 11, 30);
	init_animation_indices(SKILL_THREAD_SCHEDULING);
	gw.skills_animations[SKILL_FORK] = new Animation("../../../res/skills_animations/deadlock/fork.png", 1, 5, 5, 18);
	init_animation_indices(SKILL_FORK);

	// Norma
	gw.skills_animations[SKILL_PUMPING_UP] = new Animation("../../../res/skills_animations/norma/pumping_up.png", 4, 5, 20, 10);
	init_animation_indices(SKILL_PUMPING_UP);
	gw.skills_animations[SKILL_AUTOMATA_SUMMON] = new Animation("../../../res/skills_animations/norma/automata_summon.png", 3, 5, 15, 15);
	init_animation_indices(SKILL_AUTOMATA_SUMMON);
	gw.skills_animations[SKILL_TURING_MACHINE] = new Animation("../../../res/skills_animations/norma/turing_machine.png", 4, 5, 17, 18);
	init_animation_indices(SKILL_TURING_MACHINE);
	gw.skills_animations[SKILL_NON_DETERMINISM] = new Animation("../../../res/skills_animations/norma/non_determinism.png", 2, 5, 7, 18);
	init_animation_indices(SKILL_NON_DETERMINISM);

	// Hazard
	gw.skills_animations[SKILL_TMR] = new Animation("../../../res/skills_animations/hazard/tmr.png", 2, 5, 10, 18);
	init_animation_indices(SKILL_TMR);
	gw.skills_animations[SKILL_REDUNDANCY] = new Animation("../../../res/skills_animations/hazard/redundancy.png", 2, 5, 6, 15);
	init_animation_indices(SKILL_REDUNDANCY);
	gw.skills_animations[SKILL_ROLLBACK] = new Animation("../../../res/skills_animations/hazard/rollback.png", 2, 5, 7, 15);
	init_animation_indices(SKILL_ROLLBACK);
	gw.skills_animations[SKILL_ROLLFORWARD] = new Animation("../../../res/skills_animations/hazard/rollforward.png", 2, 5, 7, 15);
	init_animation_indices(SKILL_ROLLFORWARD);

	// Qwerty
	gw.skills_animations[SKILL_ALT] = new Animation("../../../res/skills_animations/qwerty/alt.png", 2, 5, 8, 20);
	init_animation_indices(SKILL_ALT);
	gw.skills_animations[SKILL_CTRL] = new Animation("../../../res/skills_animations/qwerty/ctrl.png", 2, 5, 10, 15);
	init_animation_indices(SKILL_CTRL);
	gw.skills_animations[SKILL_DELETE] = new Animation("../../../res/skills_animations/qwerty/del.png", 3, 5, 11, 18);
	init_animation_indices(SKILL_DELETE);
	gw.skills_animations[SKILL_ESC] = new Animation("../../../res/skills_animations/qwerty/esc.png", 1, 5, 3, 30);
	init_animation_indices(SKILL_ESC);

	// Big 0
	gw.skills_animations[SKILL_BEST_BOUND_FIST] = new Animation("../../../res/skills_animations/big_o/best_bound_fist.png", 2, 5, 9, 20);
	init_animation_indices(SKILL_BEST_BOUND_FIST);
	gw.skills_animations[SKILL_DUAL_SIMPLEX] = new Animation("../../../res/skills_animations/big_o/dual_simplex.png", 3, 5, 12, 15);
	init_animation_indices(SKILL_DUAL_SIMPLEX);
	gw.skills_animations[SKILL_GRAPH_COLORING] = new Animation("../../../res/skills_animations/big_o/graph_coloring.png", 3, 5, 13, 18);
	init_animation_indices(SKILL_GRAPH_COLORING);
	gw.skills_animations[SKILL_KNAPSACK_HIDEOUT] = new Animation("../../../res/skills_animations/big_o/knapsack_hideout.png", 1, 5, 5, 18);
	init_animation_indices(SKILL_KNAPSACK_HIDEOUT);

	// New
	gw.skills_animations[SKILL_SPRINT_BURST] = new Animation("../../../res/skills_animations/new/sprint_burst.png", 2, 5, 10, 20);
	init_animation_indices(SKILL_BEST_BOUND_FIST);
	gw.skills_animations[SKILL_INHERITANCE] = new Animation("../../../res/skills_animations/new/inheritance.png", 4, 5, 16, 8);
	init_animation_indices(SKILL_INHERITANCE);
	gw.skills_animations[SKILL_OVERRIDE] = new Animation("../../../res/skills_animations/new/override.png", 4, 5, 20, 6);
	init_animation_indices(SKILL_OVERRIDE);
	gw.skills_animations[SKILL_POLIMORPHISM] = new Animation("../../../res/skills_animations/new/polymorphism.png", 3, 5, 15, 10);
	init_animation_indices(SKILL_POLIMORPHISM);

	// Clockboy
	gw.skills_animations[SKILL_CLOCK_PULSE] = new Animation("../../../res/skills_animations/clockboy/clock_pulse.png", 2, 5, 9, 15);
	init_animation_indices(SKILL_CLOCK_PULSE);
	gw.skills_animations[SKILL_PIPELINE] = new Animation("../../../res/skills_animations/clockboy/pipeline.png", 3, 5, 12, 18);
	init_animation_indices(SKILL_PIPELINE);
	gw.skills_animations[SKILL_OVERCLOCK] = new Animation("../../../res/skills_animations/clockboy/overclock.png", 3, 5, 13, 18);
	init_animation_indices(SKILL_OVERCLOCK);
	gw.skills_animations[SKILL_BRANCH_DAMAGE] = new Animation("../../../res/skills_animations/clockboy/branch_prediction.png", 1, 5, 5, 20);
	init_animation_indices(SKILL_BRANCH_DAMAGE);
}

static Settings_UI settings_ui;

static void init_settings_mode()
{
	using namespace linked;
	Texture* bg_settings = new Texture("../../../res/textures/bg_settings.jpg");
	gw.bg_settings = bg_settings;

	static u8 config_label[] = "SETTINGS";
	static u8 idioma_label[] = "LANGUAGE";
	static u8 animations_label[] = "ANIMATIONS";

	hm::vec4 transparent_window_color = char_window_color;
	transparent_window_color.w = 0.9f;

	gw.settings_ui = &settings_ui;

	Window* settings = new Window(600, 350, hm::vec2((r32)window_info.width / 2.0f - 300.0f, window_info.height / 2.0f - 150.0f), transparent_window_color, config_label, sizeof(config_label), W_UNFOCUSABLE | W_BORDER | W_HEADER);
	settings->setTitleCentered(true);
	settings->setTitleColor(hm::vec4(11.0f/255.0f, 114.0f/255.0f, 121.0f/255.0f, 1.0f));
	settings->setBorderColor(cyan);
	gw.settings_window = settings;
	settings_ui.settings_window = settings;

	WindowDiv* settings_div = new WindowDiv(*settings, 400, 280, 0, 0, hm::vec2(0, 0), hm::vec4(0,0,0,0), DIV_CENTER_X| DIV_CENTER_Y);
	settings->divs.push_back(settings_div);
	settings_ui.settings_div = settings_div;
	Label* idioma = new Label(*settings_div, (u8*)idioma_label, sizeof(idioma_label), hm::vec2(0, 0), hm::vec4(11.0f / 255.0f, 114.0f / 255.0f, 121.0f / 255.0f, 1.0f), FONT_OSWALD_REGULAR_18, 0, 0);
	settings_div->getLabels().push_back(idioma);
	settings_ui.idioma = idioma;
	Label* volume_sfx = new Label(*settings_div, (u8*)"VOLUME SFX", sizeof("VOLUME SFX"), hm::vec2(0, 40), hm::vec4(11.0f / 255.0f, 114.0f / 255.0f, 121.0f / 255.0f, 1.0f), FONT_OSWALD_REGULAR_18, 0, 0);
	settings_div->getLabels().push_back(volume_sfx);
	settings_ui.vol_sfx = volume_sfx;
	Label* volume_bgm= new Label(*settings_div, (u8*)"VOLUME BGM", sizeof("VOLUME BGM"), hm::vec2(0, 80), hm::vec4(11.0f / 255.0f, 114.0f / 255.0f, 121.0f / 255.0f, 1.0f), FONT_OSWALD_REGULAR_18, 0, 0);
	settings_div->getLabels().push_back(volume_bgm);
	settings_ui.vol_bmg = volume_bgm;
	Label* animacoes = new Label(*settings_div, (u8*)animations_label, sizeof(animations_label), hm::vec2(0, 120), hm::vec4(11.0f / 255.0f, 114.0f / 255.0f, 121.0f / 255.0f, 1.0f), FONT_OSWALD_REGULAR_18, 0, 0);
	settings_div->getLabels().push_back(animacoes);
	settings_ui.animations = animacoes;

	static u8 salvar_label[] = "SAVE\0\0";
	static u8 cancel_label[] = "CANCEL\0\0";

	Button* confirm = new Button(*settings_div, new Label(*settings_div, (u8*)salvar_label, sizeof(salvar_label), hm::vec2(68, 15), hm::vec4(1, 1, 1, 1), FONT_OSWALD_BOLD_18, 0, 0),
		hm::vec2(0, 200), 185, 55, hm::vec4(1, 1, 1, 1), 0);
	settings_ui.confirm = confirm;
	confirm->setClickedCallback(button_settings_save);

	Texture* confirm_button_texture = new Texture("../../../res/textures/playbutton_normal.png");
	Texture* confirm_button_hover_texture = new Texture("../../../res/textures/playbutton_hover.png");
	confirm->setAllBGTexture(confirm_button_hover_texture);
	confirm->setNormalBGTexture(confirm_button_texture);
	settings_div->getButtons().push_back(confirm);
	gw.sacrifice_orb_ui->endturn = confirm;

	Button* cancel = new Button(*settings_div, new Label(*settings_div, (u8*)cancel_label, sizeof(cancel_label), hm::vec2(66, 15), cyan, FONT_OSWALD_REGULAR_18, 0, 0),
		hm::vec2(204, 200), 185, 55, hm::vec4(1, 1, 1, 1), 0);
	cancel->setClickedCallback(button_settings_cancel);
	Texture* cancel_button_texture = new Texture("../../../res/textures/cancel_action_modal.png");
	cancel->setAllBGTexture(cancel_button_texture);
	settings_div->getButtons().push_back(cancel);
	gw.sacrifice_orb_ui->cancel = cancel;
	settings_ui.cancel = cancel;

	static u8 english_label[] = "PORTUGUÊS";
	static u8 volume_sfx_value[] = "100";
	static u8 volume_bgm_value[] = "100";
	static u8 animacoes_value[] = "ATIVO\0\0\0\0";

	Label* lang_label_value = new Label(*settings_div, (u8*)english_label, sizeof(english_label), hm::vec2(260.0f, 0), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_18, 0, 0);
	Label* volume_sfx_label_value = new Label(*settings_div, (u8*)volume_sfx_value, sizeof(volume_sfx_value), hm::vec2(285.0f, 40), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_18, 0, 0);
	Label* volume_bgm_label_value = new Label(*settings_div, (u8*)volume_bgm_value, sizeof(volume_bgm_value), hm::vec2(285.0f, 80), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_18, 0, 0);
	Label* animacoes_label_value = new Label(*settings_div, (u8*)animacoes_value, sizeof(animacoes_value), hm::vec2(275.0f, 120), hm::vec4(1, 1, 1, 1), FONT_OSWALD_REGULAR_18, 0, 0);
	settings_div->getLabels().push_back(lang_label_value);
	settings_div->getLabels().push_back(volume_sfx_label_value);
	settings_div->getLabels().push_back(volume_bgm_label_value);
	settings_div->getLabels().push_back(animacoes_label_value);
	settings_ui.idioma_value = lang_label_value;
	settings_ui.vol_sfx_value = volume_sfx_label_value;
	settings_ui.vol_bmg_value = volume_bgm_label_value;
	settings_ui.animations_value = animacoes_label_value;

	Texture* arrow_left_normal = new Texture("../../../res/orbs/arrow_normal_left.png");
	Texture* arrow_left_hover = new Texture("../../../res/orbs/arrow_active_left.png");
	Texture* arrow_left_inactive = new Texture("../../../res/orbs/arrow_inactive_left.png");

	Texture* arrow_right_normal = new Texture("../../../res/orbs/arrow_normal_right.png");
	Texture* arrow_right_hover = new Texture("../../../res/orbs/arrow_active_right.png");
	Texture* arrow_right_inactive = new Texture("../../../res/orbs/arrow_inactive_right.png");

	Button* arrow_left_idioma = new Button(*settings_div, 0, hm::vec2(205.0f, 0.0f), 16, 24, hm::vec4(0, 0, 0, 1), 0);
	arrow_left_idioma->setAllBGTexture(arrow_left_hover);
	arrow_left_idioma->setNormalBGTexture(arrow_left_normal);
	arrow_left_idioma->setInactiveTexture(arrow_left_inactive);
	arrow_left_idioma->setClickedCallback(button_arrows_settings);

	Button* arrow_right_idioma = new Button(*settings_div, 0, hm::vec2(375.0f, 0.0f), 16, 24, hm::vec4(0, 0, 0, 1), 1);
	arrow_right_idioma->setAllBGTexture(arrow_right_hover);
	arrow_right_idioma->setNormalBGTexture(arrow_right_normal);
	arrow_right_idioma->setInactiveTexture(arrow_right_inactive);
	arrow_right_idioma->setClickedCallback(button_arrows_settings);

	settings_div->getButtons().push_back(arrow_left_idioma);
	settings_div->getButtons().push_back(arrow_right_idioma);

	Button* arrow_left_vsfx = new Button(*settings_div, 0, hm::vec2(205.0f, 40.0f), 16, 24, hm::vec4(0, 0, 0, 1), 2);
	arrow_left_vsfx->setAllBGTexture(arrow_left_hover);
	arrow_left_vsfx->setNormalBGTexture(arrow_left_normal);
	arrow_left_vsfx->setInactiveTexture(arrow_left_inactive);
	arrow_left_vsfx->setClickedCallback(button_arrows_settings);

	Button* arrow_right_vsfx = new Button(*settings_div, 0, hm::vec2(375.0f, 40.0f), 16, 24, hm::vec4(0, 0, 0, 1), 3);
	arrow_right_vsfx->setAllBGTexture(arrow_right_hover);
	arrow_right_vsfx->setNormalBGTexture(arrow_right_normal);
	arrow_right_vsfx->setInactiveTexture(arrow_right_inactive);
	arrow_right_vsfx->setClickedCallback(button_arrows_settings);

	settings_div->getButtons().push_back(arrow_left_vsfx);
	settings_div->getButtons().push_back(arrow_right_vsfx);

	Button* arrow_left_bsfx = new Button(*settings_div, 0, hm::vec2(205.0f, 80.0f), 16, 24, hm::vec4(0, 0, 0, 1), 4);
	arrow_left_bsfx->setAllBGTexture(arrow_left_hover);
	arrow_left_bsfx->setNormalBGTexture(arrow_left_normal);
	arrow_left_bsfx->setInactiveTexture(arrow_left_inactive);
	arrow_left_bsfx->setClickedCallback(button_arrows_settings);

	Button* arrow_right_bsfx = new Button(*settings_div, 0, hm::vec2(375.0f, 80.0f), 16, 24, hm::vec4(0, 0, 0, 1), 5);
	arrow_right_bsfx->setAllBGTexture(arrow_right_hover);
	arrow_right_bsfx->setNormalBGTexture(arrow_right_normal);
	arrow_right_bsfx->setInactiveTexture(arrow_right_inactive);
	arrow_right_bsfx->setClickedCallback(button_arrows_settings);

	settings_div->getButtons().push_back(arrow_left_bsfx);
	settings_div->getButtons().push_back(arrow_right_bsfx);

	Button* arrow_left_animacoes = new Button(*settings_div, 0, hm::vec2(205.0f, 120.0f), 16, 24, hm::vec4(0, 0, 0, 1), 6);
	arrow_left_animacoes->setAllBGTexture(arrow_left_hover);
	arrow_left_animacoes->setNormalBGTexture(arrow_left_normal);
	arrow_left_animacoes->setInactiveTexture(arrow_left_inactive);
	arrow_left_animacoes->setClickedCallback(button_arrows_settings);

	Button* arrow_right_animacoes = new Button(*settings_div, 0, hm::vec2(375.0f, 120.0f), 16, 24, hm::vec4(0, 0, 0, 1), 7);
	arrow_right_animacoes->setAllBGTexture(arrow_right_hover);
	arrow_right_animacoes->setNormalBGTexture(arrow_right_normal);
	arrow_right_animacoes->setInactiveTexture(arrow_right_inactive);
	arrow_right_animacoes->setClickedCallback(button_arrows_settings);

	settings_div->getButtons().push_back(arrow_left_animacoes);
	settings_div->getButtons().push_back(arrow_right_animacoes);
}