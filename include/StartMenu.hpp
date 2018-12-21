#ifndef STARTMENU_H_INCLUDED
#define STARTMENU_H_INCLUDED	//Se incluyen definiciones y estructuras que usan todos los .cpp

#include "def.hpp"
#include "allegro.hpp"
#include "fsmInit.h"

class StartMenu {
public:
	StartMenu(ALL *allegro);
	//~StartMenu();

	bool eventHandler(ALL *allegro);

    bool get_player_mode() { return singlePlayer; }

private:
	//! Variables
	pos mouse;
	bool redraw;
	bool singlePlayer;

	//ALLEGRO_BITMAP	*background;
	// Buttons
	Button *singlePlayerButton;
	Button *multiPlayerButton;

	//! Methods
	void checkMouse(ALL* allegro);
	bool click(ALL* allegro);
	void initializeButtons(ALL* allegro);

	void updateScreen(ALL* allegro);


};

#endif