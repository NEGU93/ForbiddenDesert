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

//enum roles {ARCHEOLOGIST=0x30, CLIMBER, EXPLORER, METEOROLOGIST, NAVIGATOR, WATER_CARRIER};
// TODO: have to basically repeat the enum here.
enum Role {ARCHEOLOGIST_ROLE=0x30, CLIMBER_ROLE, EXPLORER_ROLE, METEOROLOGIST_ROLE, NAVIGATOR_ROLE, WATER_CARRIER_ROLE};

//I made this class static because I'll only have one game
/*static*/ class ChoosePlayer {
public:
	ChoosePlayer(ALL *allegro);
	~ChoosePlayer();
	bool eventHandler(ALL *allegro);
	bool initializeAllegro(ALL* allegro);

	Role get_p1_role() { return rol1; }
	Role get_p2_role() { return rol2; }
private:
	//Buffer
	int player;
	Role rol1;
	Role rol2;

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