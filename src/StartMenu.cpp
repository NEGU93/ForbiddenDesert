
#include <include/StartMenu.hpp>

#include "StartMenu.hpp"

StartMenu::StartMenu(ALL* allegro) {
    al_set_target_backbuffer(allegro->startMenuDisplay);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(allegro->backgroundStartMenuImage, 0, 0, 0);
    initializeButtons(allegro);
}
bool StartMenu::eventHandler(ALL *allegro) {
	bool chooseMode = true;
	ALLEGRO_EVENT ev;
	al_wait_for_event(allegro->events_queue, &ev);
	if (ev.type == ALLEGRO_EVENT_TIMER) { redraw = true; }
	else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		/*gamePack.quit();
		gamePack.sendPack();
		gamePack.waitForAck(allegro);*/
	}
	else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
		mouse.x = ev.mouse.x;
		mouse.y = ev.mouse.y;
		checkMouse(allegro);
	}
	else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
		if (ev.mouse.button == SECONDARY_BUTTON) {
			// Do Nothing
		}
		else {
		    chooseMode = click(allegro);
		}
	}
	if (redraw && al_is_event_queue_empty(allegro->events_queue)) {
		updateScreen(allegro);
	}

	return chooseMode;
}
bool StartMenu::click(ALL *allegro) {
	if (singlePlayerButton->checkMouse(mouse.x, mouse.y, allegro->volume)) {
	    singlePlayer = true;
	}
	else if(multiPlayerButton->checkMouse(mouse.x, mouse.y, allegro->volume)) {
	    singlePlayer = false;
	}
	else { return true; }
	return false;
}
void StartMenu::initializeButtons(ALL *allegro) {
	singlePlayerButton = new Button(WIDTH * allegro->screenWidth / MAXRESOLUTION_X / 2 - al_get_bitmap_width(al_load_bitmap("Resources/Buttons/muteButton.png")) /2,
	        HEIGHT * allegro->screenHeight / MAXRESOLUTION_Y * 1 / 3,
	        "Resources/Buttons/muteButton.png", "Resources/Buttons/muteButton.png", allegro);
	multiPlayerButton = new Button(WIDTH * allegro->screenWidth / MAXRESOLUTION_X / 2 - al_get_bitmap_width(al_load_bitmap("Resources/Buttons/muteButton.png")) / 2,
	        HEIGHT * allegro->screenHeight / MAXRESOLUTION_Y * 2 / 3,
	        "Resources/Buttons/noMuteButton.png", "Resources/Buttons/noMuteButton.png", allegro);
}
void StartMenu::checkMouse(ALL *allegro) {
	singlePlayerButton->checkMouse(mouse.x, mouse.y, allegro->volume);
	multiPlayerButton->checkMouse(mouse.x, mouse.y, allegro->volume);
}
void StartMenu::updateScreen(ALL *allegro) {
	al_set_target_backbuffer(allegro->startMenuDisplay);
	al_draw_textf(allegro->statusFont, al_map_rgb(WHITE), WIDTH * allegro->screenWidth / MAXRESOLUTION_X / 2, HEIGHT * allegro->screenHeight / MAXRESOLUTION_Y - 12, ALLEGRO_ALIGN_CENTRE, "%s", "Welcome to Forbidden Desert");
	singlePlayerButton->updateButton(allegro);
	multiPlayerButton->updateButton(allegro);
	al_flip_display();
}

//StartMenu::~StartMenu(){
//	al_destroy_bitmap(background);
//}
