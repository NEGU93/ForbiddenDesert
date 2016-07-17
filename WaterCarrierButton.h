#ifndef WATERCARRIERBUTTON_H_INCLUDED
#define WATERCARRIERBUTTON_H_INCLUDED

#include "Button.h"

class WaterCarrierButton : public Button {
public:
	WaterCarrierButton();
	WaterCarrierButton(int x0, int y0, const char *imageName, const char * nonSelectedImageName, ALL* allegro);
	bool buttonAction(int x, int y);
};

#endif