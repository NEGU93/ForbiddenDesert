#ifndef STORMCARD_H_INCLUDED
#define STORMCARD_H_INCLUDED

#include "def.h"
#include "allegro.h"

enum StormCardsEnum { SUNBEATSDOWN, STORMUP,
	RIGHT1, LEFT1, UP1, DOWN1,
	RIGHT2, LEFT2, UP2, DOWN2,
	RIGHT3, LEFT3, UP3, DOWN3};

class StormCard {
public:
	StormCard();
	~StormCard();
	StormCard(StormCardsEnum cardEnum, ALL *allegro);
	//Getter
	StormCardsEnum getStormCardType() { return stormCardEnum; }
	void showImage(ALL *allegro);
	void peepShowImage(ALL* allegro) { al_draw_bitmap(cardImage, (allegro->screenWidth - al_get_bitmap_width(cardImage)) / 2, (allegro->screenHeight - al_get_bitmap_height(cardImage)) / 2, 0); }
private:
	ALLEGRO_BITMAP *cardImage;
	StormCardsEnum stormCardEnum;
};

#endif