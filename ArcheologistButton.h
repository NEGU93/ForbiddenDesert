#ifndef ARCHEOLOGISTBUTTON_H_INCLUDED
#define ARCHEOLOGISTBUTTON_H_INCLUDED

#include "Button.h"

class ArcheologistButton : public Button {
public:
	ArcheologistButton();
	ArcheologistButton(int x0, int y0, const char *imageName, const char * nonSelectedImageName, ALL* allegro);
	bool buttonAction(int x, int y);
};

#endif
