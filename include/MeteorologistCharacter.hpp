#ifndef METEOROLOGIST_H_INCLUDED
#define METEOROLOGIST_H_INCLUDED

#include "Character.hpp"
#include "Button.hpp"

class MeteorologistCharacter : public Character {
public:
	MeteorologistCharacter(int rowSet, int columnSet, Container info, ALL* allegro);
	~MeteorologistCharacter();
	bool checkMouseOverButton(int x, int y, double volume) { return seeStormDeckButton->checkMouse(x, y, volume); }
	void updateMeteorologistButton(ALL* allegro) { seeStormDeckButton->updateButton(allegro); }
	bool peep();
private:
	ALLEGRO_BITMAP *watersImage[MAXOFWATERS];
	ALLEGRO_BITMAP *meteorologistImage;
	Button *seeStormDeckButton;
};

#endif
