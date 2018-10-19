#ifndef METEOROLOGISTBUTTON_H_INCLUDED
#define METEOROLOGISTBUTTON_H_INCLUDED

#include "Button.hpp"

class MeteorologistButton : public Button {
public:
	MeteorologistButton();
	MeteorologistButton(int x0, int y0, const char *imageName, const char * nonSelectedImageName, ALL* allegro);
	bool buttonAction(int x, int y);
};

#endif