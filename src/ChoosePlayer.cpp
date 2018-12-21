#include "ChoosePlayer.hpp"

ChoosePlayer::ChoosePlayer(ALL *allegro) {
	initializeAllegro(allegro);
	player = 1;
	// Create all 6 characterButtons
	archeologistButton = new ArcheologistButton(0, allegro->screenHeight / 2.0, "Resources/Buttons/ArcheologistButton.png", "Resources/Buttons/SelectedArcheologistButton.png", allegro);
	explorerButton = new ExplorerButton(allegro->screenWidth / 6.0, allegro->screenHeight / 2.0, "Resources/Buttons/ExplorerButton.png", "Resources/Buttons/SelectedExplorerButton.png", allegro);
	navigatorButton = new NavigatorButton(allegro->screenWidth / 3.0, allegro->screenHeight / 2.0, "Resources/Buttons/NavigatorButton.png", "Resources/Buttons/SelectedNavigatorButton.png", allegro);
	waterCarrierButton = new WaterCarrierButton(allegro->screenWidth / 2.0, allegro->screenHeight / 2.0, "Resources/Buttons/WaterCarrierButton.png", "Resources/Buttons/SelectedWaterCarrierButton.png", allegro);
	climberButton = new ClimberButton(allegro->screenWidth * 2.0 / 3.0, allegro->screenHeight / 2.0, "Resources/Buttons/ClimberButton.png", "Resources/Buttons/SelectedClimberButton.png", allegro);
	meteorologistButton = new MeteorologistButton(allegro->screenWidth * 5.0 / 6.0, allegro->screenHeight / 2.0, "Resources/Buttons/MeteorologistButton.png", "Resources/Buttons/SelectedMeteorologistButton.png", allegro);
	
	mouse.x = allegro->screenWidth / 2.0;
	mouse.y = allegro->screenHeight / 2.0;
	redraw = true;
}
bool ChoosePlayer::eventHandler(ALL *allegro) {
	bool stillPlaying = true;
	ALLEGRO_EVENT ev;
	al_wait_for_event(allegro->events_queue, &ev);

	if (ev.type == ALLEGRO_EVENT_TIMER) {
		redraw = true;
	}
	else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		stillPlaying = false;
	}
	else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
		mouse.x = ev.mouse.x;
		mouse.y = ev.mouse.y;
		archeologistButton->checkMouse(ev.mouse.x, ev.mouse.y, allegro->volume);
		explorerButton->checkMouse(ev.mouse.x, ev.mouse.y, allegro->volume);
		navigatorButton->checkMouse(ev.mouse.x, ev.mouse.y, allegro->volume);
		meteorologistButton->checkMouse(ev.mouse.x, ev.mouse.y, allegro->volume);
		climberButton->checkMouse(ev.mouse.x, ev.mouse.y, allegro->volume);
		waterCarrierButton->checkMouse(ev.mouse.x, ev.mouse.y, allegro->volume);
	}
	else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
		//If there whas a selected character, then return true, else return false
		if (archeologistButton->buttonAction(ev.mouse.x, ev.mouse.y)) {
			if (player == 1) {
				rol1 = ARCHEOLOGIST_ROLE;
				player = 2;
				return true;
			}
			else if (rol1 != ARCHEOLOGIST_ROLE) {	//If im player 2, I must check player 1 is not the same character.
				rol2 = ARCHEOLOGIST_ROLE;
				return false;
			}
			return true;		//Player 1 is already this type
		}
		if (explorerButton->buttonAction(ev.mouse.x, ev.mouse.y)) { 
			if (player == 1) {
				rol1 = EXPLORER_ROLE;
				player = 2;
				return true;
			}
			else if (rol1 != EXPLORER_ROLE) {	//If im player 2, I must check player 1 is not the same character.
				rol2 = EXPLORER_ROLE;
				return false;
			}
			return true;		//Player 1 is already this type
		}
		if (navigatorButton->buttonAction(ev.mouse.x, ev.mouse.y)) { 
			if (player == 1) {
				rol1 = NAVIGATOR_ROLE;
				player = 2;
				return true;
			}
			else if (rol1 != NAVIGATOR_ROLE) {	//If im player 2, I must check player 1 is not the same character.
				rol2 = NAVIGATOR_ROLE;
				return false;
			}
			return true;		//Player 1 is already this type	 
		}
		if (meteorologistButton->buttonAction(ev.mouse.x, ev.mouse.y)) { 
			if (player == 1) {
				rol1 = METEOROLOGIST_ROLE;
				player = 2;
				return true;
			}
			else if (rol1 != METEOROLOGIST_ROLE) {	//If im player 2, I must check player 1 is not the same character.
				rol2 = METEOROLOGIST_ROLE;
				return false;
			}
			return true;		//Player 1 is already this type
		}
		if (climberButton->buttonAction(ev.mouse.x, ev.mouse.y)) { 
			if (player == 1) {
				rol1 = CLIMBER_ROLE;
				player = 2;
				return true;
			}
			else if (rol1 != CLIMBER_ROLE) {	//If im player 2, I must check player 1 is not the same character.
				rol2 = CLIMBER_ROLE;
				return false;
			}
			return true;		//Player 1 is already this type
		}
		if (waterCarrierButton->buttonAction(ev.mouse.x, ev.mouse.y)) {
			if (player == 1) {
				rol1 = WATER_CARRIER_ROLE;
				player = 2;
				return true;
			}
			else if (rol1 != WATER_CARRIER_ROLE) {	//If im player 2, I must check player 1 is not the same character.
				rol2 = WATER_CARRIER_ROLE;
				return false;
			}
			return true;		//Player 1 is already this type 
		}
		return true;
	}

	if (redraw && al_is_event_queue_empty(allegro->events_queue)) {
		updateScreen(allegro);
	}
	return stillPlaying;
}
void ChoosePlayer::updateScreen(ALL* allegro) {
	redraw = false;
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_bitmap(allegro->fondo, 0, 0, 0);
	//Texts
	if (player == 1) { al_draw_text(allegro->gameFont, al_map_rgb(WHITE), 0, 0, ALLEGRO_ALIGN_LEFT, "Choose your adventurer: "); }
	else { al_draw_text(allegro->gameFont, al_map_rgb(WHITE), 0, 0, ALLEGRO_ALIGN_LEFT, "Choose your partner: "); }
	//Player Buttons
	archeologistButton->updateButton(allegro);
	explorerButton->updateButton(allegro);
	navigatorButton->updateButton(allegro);
	meteorologistButton->updateButton(allegro);
	climberButton->updateButton(allegro);
	waterCarrierButton->updateButton(allegro);
	if (archeologistButton->checkMouse(mouse.x, mouse.y, allegro->volume)) {
		al_draw_text(allegro->gameFont, al_map_rgb(WHITE), allegro->screenWidth / 2, allegro->screenHeight * 0.1, ALLEGRO_ALIGN_CENTRE, "Archeologist");
		al_draw_bitmap(allegro->rectangleImage[0], (allegro->screenWidth - al_get_bitmap_width(allegro->rectangleImage[0])) / 2, allegro->screenHeight * 16 / 20, 0);
	}
	if (climberButton->checkMouse(mouse.x, mouse.y, allegro->volume)) {
		al_draw_text(allegro->gameFont, al_map_rgb(WHITE), allegro->screenWidth / 2, allegro->screenHeight * 0.1, ALLEGRO_ALIGN_CENTRE, "Climber");
		al_draw_bitmap(allegro->rectangleImage[1], (allegro->screenWidth - al_get_bitmap_width(allegro->rectangleImage[1])) / 2, allegro->screenHeight * 16 / 20, 0);
	}
	if (explorerButton->checkMouse(mouse.x, mouse.y, allegro->volume)) {
		al_draw_text(allegro->gameFont, al_map_rgb(WHITE), allegro->screenWidth / 2, allegro->screenHeight * 0.1, ALLEGRO_ALIGN_CENTRE, "Explorer");
		al_draw_bitmap(allegro->rectangleImage[2], (allegro->screenWidth - al_get_bitmap_width(allegro->rectangleImage[2])) / 2, allegro->screenHeight * 16 / 20, 0);
	}
	if (navigatorButton->checkMouse(mouse.x, mouse.y, allegro->volume)) {
		al_draw_text(allegro->gameFont, al_map_rgb(WHITE), allegro->screenWidth / 2, allegro->screenHeight * 0.1, ALLEGRO_ALIGN_CENTRE, "Navigator");
		al_draw_bitmap(allegro->rectangleImage[3], (allegro->screenWidth - al_get_bitmap_width(allegro->rectangleImage[3])) / 2, allegro->screenHeight * 16 / 20, 0);
	}
	if (waterCarrierButton->checkMouse(mouse.x, mouse.y, allegro->volume)) {
		al_draw_text(allegro->gameFont, al_map_rgb(WHITE), allegro->screenWidth / 2, allegro->screenHeight * 0.1, ALLEGRO_ALIGN_CENTRE, "Water Carrier");
		al_draw_bitmap(allegro->rectangleImage[4], (allegro->screenWidth - al_get_bitmap_width(allegro->rectangleImage[4])) / 2, allegro->screenHeight * 16 / 20, 0);
	}
	if (meteorologistButton->checkMouse(mouse.x, mouse.y, allegro->volume)) {
		al_draw_text(allegro->gameFont, al_map_rgb(WHITE), allegro->screenWidth / 2, allegro->screenHeight * 0.1, ALLEGRO_ALIGN_CENTRE, "Meteorologist");
		al_draw_bitmap(allegro->rectangleImage[5], (allegro->screenWidth - al_get_bitmap_width(allegro->rectangleImage[5])) / 2, allegro->screenHeight * 16 / 20, 0);
	}
	al_flip_display();
}

bool ChoosePlayer::initializeAllegro(ALL* allegro) {
	bool state = true;
	//! Display
	//! CODE TO FULLSCREEN
#ifndef WINDOWED
	ALLEGRO_DISPLAY_MODE   disp_data;
	al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
	al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	allegro->display = al_create_display(disp_data.width, disp_data.height);
	allegro->screenWidth = disp_data.width;
	allegro->screenHeight = disp_data.height;
#endif
	//! Code for windowed mode
#ifdef WINDOWED
	if (state) {
		state = false;
		if (allegro->display = al_create_display(SCREEN_X, SCREEN_Y)) {
			allegro->screenWidth = SCREEN_X;
			allegro->screenHeight = SCREEN_Y;
			//al_destroy_display(allegro->startMenuDisplay);
			al_register_event_source(allegro->events_queue, al_get_display_event_source(allegro->display));
			al_set_window_title(allegro->display, "Choose Character");
			state = true;
		}
		else { cout << "Failed to create Display" << endl; }
	}
#endif
	//Load Images
	if (state) {
		state = false;
		al_set_target_backbuffer(allegro->display);
		if (allegro->fondo = al_load_bitmap_resized("Resources/Scenario.jpg", allegro)) {
			if (allegro->mouse = al_load_bitmap_resized("Resources/mouse.png", allegro)) {
				state = true;
			}
			else { cout << "Failed to load mouse.png" << endl; }
		}
		else { cout << "Failed to load scenario.png" << endl; }
	}

	//Audio
	if (allegro->backgroundMusic != nullptr) {
		al_play_sample(allegro->backgroundMusic, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &allegro->backgroundMusicID);
	}

	//Mouse Initialization
	if (allegro->mouse = al_load_bitmap_resized("Resources/mouse.png", allegro)) {
		if (allegro->cursor = al_create_mouse_cursor(allegro->mouse, 0, 0)) {
			al_set_mouse_cursor(allegro->display, allegro->cursor);
		}
	}
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_bitmap(allegro->fondo, 0, 0, 0);
	al_flip_display();

	return state;
}

ChoosePlayer::~ChoosePlayer() {
	delete archeologistButton;
	delete explorerButton;
	delete navigatorButton;
	delete waterCarrierButton;
	delete meteorologistButton;
	delete climberButton;
}