#ifndef STARTMENU_H_INCLUDED
#define STARTMENU_H_INCLUDED	//Se incluyen definiciones y estructuras que usan todos los .cpp

#include "def.h"
#include "allegro.h"
#include "fsmInit.h"

class StartMenu {
public:
	StartMenu(ALL *allegro, FSMI *gameConnection);
	//~StartMenu();

private:
	ALLEGRO_BITMAP	*background;

};

#endif