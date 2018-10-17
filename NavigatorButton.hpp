#ifndef NAVIGATORBUTTON_H_INCLUDED
#define NAVIGATORBUTTON_H_INCLUDED

#include "Button.hpp"

class NavigatorButton : public Button {
public:
	NavigatorButton();
	NavigatorButton(int x0, int y0, const char *imageName, const char * nonSelectedImageName, ALL* allegro);
	bool buttonAction(int x, int y);
};

#endif
