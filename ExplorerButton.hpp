#ifndef EXPLORERBUTTON_H_INCLUDED
#define EXPLORERBUTTON_H_INCLUDED

#include "Button.hpp"

class ExplorerButton : public Button {
public:
	ExplorerButton();
	ExplorerButton(int x0, int y0, const char *imageName, const char * nonSelectedImageName, ALL* allegro);
	bool buttonAction(int x, int y);
};

#endif
