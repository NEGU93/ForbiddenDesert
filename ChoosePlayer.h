#ifndef CHOOSEPLAYER_H_INCLUDED
#define CHOOSEPLAYER_H_INCLUDED	//Se incluyen definiciones y estructuras que usan todos los .cpp

#include "def.h"
#include "allegro.h"
#include "Packages.h"
#include "ArcheologistButton.h"
#include "ExplorerButton.h"
#include "NavigatorButton.h"
#include "WaterCarrierButton.h"
#include "MeteorologistButton.h"
#include "ClimberButton.h"

//I made this class static because I'll only have one game
/*static*/ class ChoosePlayer {
public:
	ChoosePlayer(ALL *allegro, char* rol1, char* rol2);
	~ChoosePlayer();
	bool eventHandler(ALL *allegro);
	bool initializeAllegro(ALL* allegro);
private:
	//Buffer
	int player;
	char *rol1;
	char *rol2;
	//Methods
	void updateScreen(ALL *allegro);
	//Variables
	pos mouse;
	bool redraw;
	ArcheologistButton *archeologistButton;
	ExplorerButton *explorerButton;
	NavigatorButton *navigatorButton;
	WaterCarrierButton *waterCarrierButton;
	MeteorologistButton *meteorologistButton;
	ClimberButton *climberButton;
};

#endif