#ifndef CLIMBERBUTTON_H_INCLUDED
#define CLIMBERBUTTON_H_INCLUDED

#include "Button.h"

class ClimberButton : public Button {
public:
	ClimberButton();
	ClimberButton(int x0, int y0, const char *imageName, const char * nonSelectedImageName, ALL* allegro);
	bool buttonAction(int x, int y);
};

#endif