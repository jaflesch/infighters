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
		float orbs_size = 64.0f;

		hm::vec4 combat_info_bar_color(15.0f / 255.0f, 17.0f / 255.0f, 42.0f / 255.0f, 0.8f);
		linked::Window* combat_bottom_info = new linked::Window(window_info.width, 200, hm::vec2(0, 660), combat_info_bar_color, 0, 0, linked::W_BORDER | linked::W_UNFOCUSABLE);
		linked::WindowDiv* orbs_div = new linked::WindowDiv(*combat_bottom_info, 500, 100, 0, 0, hm::vec2(45.0f, 0), hm::vec4(1, 0, 0, 0.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP | linked::DIV_CENTER_Y);
		combat_bottom_info->divs.push_back(orbs_div);

		linked::Button* multiple_orb_button = new linked::Button(*orbs_div, 0, hm::vec2(0, 0), (int)orbs_size, (int)orbs_size, hm::vec4(0, 0, 0, 1), 0);
		Texture* multiple_orbs_texture = new Texture("../../../res/orbs/all_orbs.png");
		multiple_orb_button->setAllBGTexture(multiple_orbs_texture);
		orbs_div->getButtons().push_back(multiple_orb_button);

		linked::Button* hard_orb_button = new linked::Button(*orbs_div, 0, hm::vec2((orbs_size + 10) + 18, 0), (int)orbs_size, (int)orbs_size, hm::vec4(0, 0, 0, 1), 0);
		hard_orb_button->setAllBGTexture(orb_textures[0]);
		orbs_div->getButtons().push_back(hard_orb_button);

		linked::Button* soft_orb_button = new linked::Button(*orbs_div, 0, hm::vec2((orbs_size + 10) * 2 + 18, 0), (int)orbs_size, (int)orbs_size, hm::vec4(0, 0, 0, 1), 0);
		soft_orb_button->setAllBGTexture(orb_textures[1]);
		orbs_div->getButtons().push_back(soft_orb_button);

		linked::Button* vr_orb_button = new linked::Button(*orbs_div, 0, hm::vec2((orbs_size + 10) * 3 + 18, 0), (int)orbs_size, (int)orbs_size, hm::vec4(0, 0, 0, 1), 0);
		vr_orb_button->setAllBGTexture(orb_textures[2]);
		orbs_div->getButtons().push_back(vr_orb_button);

		linked::Button* bios_orb_button = new linked::Button(*orbs_div, 0, hm::vec2((orbs_size + 10) * 4 + 18, 0), (int)orbs_size, (int)orbs_size, hm::vec4(0, 0, 0, 1), 0);
		bios_orb_button->setAllBGTexture(orb_textures[3]);
		orbs_div->getButtons().push_back(bios_orb_button);

		linked::Button* exchange_orb_button = new linked::Button(*orbs_div, 0, hm::vec2((orbs_size + 10) * 5 + 18, 0), (int)orbs_size, (int)orbs_size, hm::vec4(0, 0, 0, 1), 0);
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
		linked::WindowDiv* skill_cost_div_1 = new linked::WindowDiv(*combat_bottom_info, (int)orb_size, (int)orb_size, 0, 0, hm::vec2(1300.0f, skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_cost_div_1);
		linked::WindowDiv* skill_cost_div_2 = new linked::WindowDiv(*combat_bottom_info, (int)orb_size, (int)orb_size, 0, 0, hm::vec2(1300.0f + (orb_size + 5), skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_cost_div_2);
		linked::WindowDiv* skill_cost_div_3 = new linked::WindowDiv(*combat_bottom_info, (int)orb_size, (int)orb_size, 0, 0, hm::vec2(1300.0f + (orb_size + 5) * 2, skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_cost_div_3);
		linked::WindowDiv* skill_cost_div_4 = new linked::WindowDiv(*combat_bottom_info, (int)orb_size, (int)orb_size, 0, 0, hm::vec2(1300.0f + (orb_size + 5) * 3, skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
		combat_bottom_info->divs.push_back(skill_cost_div_4);
		linked::WindowDiv* skill_cost_div_5 = new linked::WindowDiv(*combat_bottom_info, (int)orb_size, (int)orb_size, 0, 0, hm::vec2(1300.0f + (orb_size + 5) * 4, skill_desc_height), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
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
#if 1
		// Sacrifice Null orb Modal
		linked::Window* null_orb_modal = new linked::Window(300, 140, hm::vec2((r32)window_info.width / 2 - 300 / 2, (r32)window_info.height / 2 - 120 / 2), char_window_color, (u8*)"Choose an orb to sacrifice", sizeof "Choose an orb to sacrifice",
			linked::W_HEADER | linked::W_BORDER | linked::W_MOVABLE);
		null_orb_modal->setBorderColor(greener_cyan);
		null_orb_modal->setTitleColor(char_window_color);
		null_orb_modal->setTitleCentered(true);
		null_orb_modal->setActive(false);
		gw.null_orb_modal = null_orb_modal;

		linked::WindowDiv* null_orb_modal_div = new linked::WindowDiv(*null_orb_modal, 230, 50, 0, 0, hm::vec2(0, 30), hm::vec4(1, 0, 0, 1), linked::DIV_CENTER_X | linked::DIV_ANCHOR_TOP);
		null_orb_modal->divs.push_back(null_orb_modal_div);
		linked::Button* orb_hard = new linked::Button(*null_orb_modal_div, 0, hm::vec2(0, 0), 50, 50, hm::vec4(0, 0, 0, 1), 0);
		orb_hard->setAllBGTexture(orb_textures[ORB_HARD]);
		null_orb_modal_div->getButtons().push_back(orb_hard);
		linked::Button* orb_soft = new linked::Button(*null_orb_modal_div, 0, hm::vec2(50 * 1 + 10 * 1, 0), 50, 50, hm::vec4(0, 0, 0, 1), 0);
		orb_soft->setAllBGTexture(orb_textures[ORB_SOFT]);
		null_orb_modal_div->getButtons().push_back(orb_soft);
		linked::Button* orb_vr = new linked::Button(*null_orb_modal_div, 0, hm::vec2(50 * 2 + 10 * 2, 0), 50, 50, hm::vec4(0, 0, 0, 1), 0);
		orb_vr->setAllBGTexture(orb_textures[ORB_VR]);
		null_orb_modal_div->getButtons().push_back(orb_vr);
		linked::Button* orb_bios = new linked::Button(*null_orb_modal_div, 0, hm::vec2(50 * 3 + 10 * 3, 0), 50, 50, hm::vec4(0, 0, 0, 1), 0);
		orb_bios->setAllBGTexture(orb_textures[ORB_BIOS]);
		null_orb_modal_div->getButtons().push_back(orb_bios);

		linked::WindowDiv* null_orb_modal_div_2 = new linked::WindowDiv(*null_orb_modal, 230, 50, 0, 0, hm::vec2(0, 80), hm::vec4(1, 0, 0, 1), linked::DIV_CENTER_X | linked::DIV_ANCHOR_TOP);
		null_orb_modal->divs.push_back(null_orb_modal_div_2);
#endif
	}

	{
		// Multiple Orb Modal
		linked::Window* exchange_orbs = new linked::Window(420, 260, hm::vec2((r32)window_info.width / 2 - 460 / 2, (r32)window_info.height / 2 - 260 / 2), char_window_color, (u8*)"  Exchange Orbs", sizeof "  Exchange Orbs",
			linked::W_HEADER | linked::W_BORDER | linked::W_MOVABLE);
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
		linked::WindowDiv* info_div = new linked::WindowDiv(*exchange_orbs, 220, 120, 0, 0, hm::vec2(0, 30), hm::vec4(1, 0, 0, 0), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP | linked::DIV_CENTER_X);
		exchange_orbs->divs.push_back(info_div);
		combat_state.exchange_orbs_state.info_div = info_div;

		float orbs_size = 32.0f;
		float orbs_height = 36.0f;

		linked::Button* multiple_orb_button = new linked::Button(*info_div, 0, hm::vec2(0, orbs_height), (int)orbs_size, (int)orbs_size, hm::vec4(0, 0, 0, 1), 0);
		Texture* multiple_orbs_texture = new Texture("../../../res/orbs/all_orbs.png");
		multiple_orb_button->setAllBGTexture(multiple_orbs_texture);
		info_div->getButtons().push_back(multiple_orb_button);

		linked::Button* hard_orb_button = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) + 18, orbs_height), (int)orbs_size, (int)orbs_size, hm::vec4(0, 0, 0, 1), 0);
		hard_orb_button->setAllBGTexture(orb_textures[0]);
		info_div->getButtons().push_back(hard_orb_button);

		linked::Button* soft_orb_button = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) * 2 + 18, orbs_height), (int)orbs_size, (int)orbs_size, hm::vec4(0, 0, 0, 1), 0);
		soft_orb_button->setAllBGTexture(orb_textures[1]);
		info_div->getButtons().push_back(soft_orb_button);

		linked::Button* vr_orb_button = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) * 3 + 18, orbs_height), (int)orbs_size, (int)orbs_size, hm::vec4(0, 0, 0, 1), 0);
		vr_orb_button->setAllBGTexture(orb_textures[2]);
		info_div->getButtons().push_back(vr_orb_button);

		linked::Button* bios_orb_button = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) * 4 + 18, orbs_height), (int)orbs_size, (int)orbs_size, hm::vec4(0, 0, 0, 1), 0);
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
		linked::Button* hard_orb_arrow_up = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) + 22, 8), (int)arrow_up_size, (int)arrow_up_size, arrow_normal_color, ORB_HARD);
		hard_orb_arrow_up->setAllBGTexture(arrow_up);
		hard_orb_arrow_up->setHoveredBGColor(arrow_hovered_color);
		hard_orb_arrow_up->setInactiveAllBGColor(darker_gray);
		hard_orb_arrow_up->setClickedCallback(button_exchange_orb_arrow);
		hard_orb_arrow_up->button_info.data = (void*)ARROW_UP;
		info_div->getButtons().push_back(hard_orb_arrow_up);

		linked::Button* soft_orb_arrow_up = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) * 2 + 22, 8), (int)arrow_up_size, (int)arrow_up_size, arrow_normal_color, ORB_SOFT);
		soft_orb_arrow_up->setAllBGTexture(arrow_up);
		soft_orb_arrow_up->setHoveredBGColor(arrow_hovered_color);
		soft_orb_arrow_up->setInactiveAllBGColor(darker_gray);
		soft_orb_arrow_up->setClickedCallback(button_exchange_orb_arrow);
		soft_orb_arrow_up->button_info.data = (void*)ARROW_UP;
		info_div->getButtons().push_back(soft_orb_arrow_up);

		linked::Button* vr_orb_arrow_up = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) * 3 + 22, 8), (int)arrow_up_size, (int)arrow_up_size, arrow_normal_color, ORB_VR);
		vr_orb_arrow_up->setAllBGTexture(arrow_up);
		vr_orb_arrow_up->setHoveredBGColor(arrow_hovered_color);
		vr_orb_arrow_up->setInactiveAllBGColor(darker_gray);
		vr_orb_arrow_up->setClickedCallback(button_exchange_orb_arrow);
		vr_orb_arrow_up->button_info.data = (void*)ARROW_UP;
		info_div->getButtons().push_back(vr_orb_arrow_up);

		linked::Button* bios_orb_arrow_up = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) * 4 + 22, 8), (int)arrow_up_size, (int)arrow_up_size, arrow_normal_color, ORB_BIOS);
		bios_orb_arrow_up->setAllBGTexture(arrow_up);
		bios_orb_arrow_up->setHoveredBGColor(arrow_hovered_color);
		bios_orb_arrow_up->setInactiveAllBGColor(darker_gray);
		bios_orb_arrow_up->setClickedCallback(button_exchange_orb_arrow);
		bios_orb_arrow_up->button_info.data = (void*)ARROW_UP;
		info_div->getButtons().push_back(bios_orb_arrow_up);

		// down
		linked::Button* hard_orb_arrow_down = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) + 22, 72 + 18), (int)arrow_up_size, (int)arrow_up_size, arrow_normal_color, ORB_HARD);
		hard_orb_arrow_down->setAllBGTexture(arrow_down);
		hard_orb_arrow_down->setHoveredBGColor(arrow_hovered_color);
		hard_orb_arrow_down->setInactiveAllBGColor(darker_gray);
		hard_orb_arrow_down->setClickedCallback(button_exchange_orb_arrow);
		hard_orb_arrow_down->button_info.data = (void*)ARROW_DOWN;
		info_div->getButtons().push_back(hard_orb_arrow_down);

		linked::Button* soft_orb_arrow_down = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) * 2 + 22, 72 + 18), (int)arrow_up_size, (int)arrow_up_size, arrow_normal_color, ORB_SOFT);
		soft_orb_arrow_down->setAllBGTexture(arrow_down);
		soft_orb_arrow_down->setHoveredBGColor(arrow_hovered_color);
		soft_orb_arrow_down->setInactiveAllBGColor(darker_gray);
		soft_orb_arrow_down->setClickedCallback(button_exchange_orb_arrow);
		soft_orb_arrow_down->button_info.data = (void*)ARROW_DOWN;
		info_div->getButtons().push_back(soft_orb_arrow_down);

		linked::Button* vr_orb_arrow_down = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) * 3 + 22, 72 + 18), (int)arrow_up_size, (int)arrow_up_size, arrow_normal_color, ORB_VR);
		vr_orb_arrow_down->setAllBGTexture(arrow_down);
		vr_orb_arrow_down->setHoveredBGColor(arrow_hovered_color);
		vr_orb_arrow_down->setInactiveAllBGColor(darker_gray);
		vr_orb_arrow_down->setClickedCallback(button_exchange_orb_arrow);
		vr_orb_arrow_down->button_info.data = (void*)ARROW_DOWN;
		info_div->getButtons().push_back(vr_orb_arrow_down);

		linked::Button* bios_orb_arrow_down = new linked::Button(*info_div, 0, hm::vec2((orbs_size + 10) * 4 + 22, 72 + 18), (int)arrow_up_size, (int)arrow_up_size, arrow_normal_color, ORB_BIOS);
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
		g_layout_status.status_images[SKILL_CONDITION_FREEZE] = new Texture("../../../res/status/frozen.png");
		g_layout_status.status_images[SKILL_CONDITION_POISON] = new Texture("../../../res/status/poison.png");
		g_layout_status.status_images[SKILL_CONDITION_PARALYZE] = new Texture("../../../res/status/paralyze.png");
		g_layout_status.status_images[SKILL_CONDITION_SLEEP] = new Texture("../../../res/status/sleep.png");
		g_layout_status.status_images[SKILL_CONDITION_STUN] = new Texture("../../../res/status/stun.png");
	}
}
