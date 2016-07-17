#ifndef WATERCARRIERCHARACTER_H_INCLUDED
#define WATERCARRIERCHARACTER_H_INCLUDED

#include "Character.h"

class WaterCarrierCharacter : public Character {
public:
	WaterCarrierCharacter(int rowSet, int columnSet, Container info, ALL *allegro);
	~WaterCarrierCharacter();
	virtual void addWater();		//As in the father its virtual it will be virtual anyway but for clarity I define it virtual
	bool checkMouseOverButton(int x, int y, double volume) { return getWatersFromWellButton->checkMouse(x, y, volume); }
	void updateGetWatersFromWell(bool imOnDiscoveredWell, ALL* allegro);
	bool getWatersFromWell();

private:
	ALLEGRO_BITMAP *watersImage[MAXOFWATERS];
	ALLEGRO_BITMAP *waterCarrierImage;
	Button *getWatersFromWellButton;
	ALLEGRO_BITMAP* disabledGetWatersFromWellImage;
};

#endif