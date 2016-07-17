#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED	//Se incluyen definiciones y estructuras que usan todos los .cpp

#include "def.h"
#include "allegro.h"

enum buttonEnum{REMOVEBUTTON, EXCAVATEBUTTON, PICKUPBUTTON, OFFERWBUTTON, OFFEREBUTTON, REQUESTWBUTTON,REQUESTEBUTTON};

typedef struct {
	pos initialPos;
	pos endPos;
} ButtonPos;

class Button {
public:
	Button();
	Button(int x0, int y0, const char *imageName, const char * selectedImageName, ALL* allegro);	//Receives the initial position of the button
	~Button();
	bool checkMouse(int x, int y, double volume);									//See if the mouse is over the button
	void testButtonAction(int x, int y);
	void updateButton(ALL *allegro);
	int getMiddleOfX();
	int getMiddleOfY();
	pos getInitialPos();
	void changeCoords(int x, int y);

protected:
	bool mouseOverButton;
	ButtonPos buttonPos;
	ALLEGRO_BITMAP *buttonImage;
	ALLEGRO_BITMAP *nonSelectedImage;
	ALLEGRO_SAMPLE *selectCharSample;
private:
	ALLEGRO_SAMPLE *mouseOverButtonSample;
};

#endif