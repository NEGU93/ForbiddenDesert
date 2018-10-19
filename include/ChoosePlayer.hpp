#ifndef CHOOSEPLAYER_H_INCLUDED
#define CHOOSEPLAYER_H_INCLUDED	//Se incluyen definiciones y estructuras que usan todos los .cpp

#include "def.hpp"
#include "allegro.hpp"
#include "Packages.hpp"
#include "ArcheologistButton.hpp"
#include "ExplorerButton.hpp"
#include "NavigatorButton.hpp"
#include "WaterCarrierButton.hpp"
#include "MeteorologistButton.hpp"
#include "ClimberButton.hpp"

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