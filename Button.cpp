#include "Button.h"

Button::Button() {}
Button::Button(int x0, int y0, const char * imageName, const char * nonSelectedImageName, ALL* allegro) {
	if (this->buttonImage = al_load_bitmap_resized(imageName, allegro) ) {
		if (this->nonSelectedImage = al_load_bitmap_resized(nonSelectedImageName, allegro)) {
			buttonPos.initialPos.x = x0;
			buttonPos.initialPos.y = y0;
			buttonPos.endPos.x = buttonPos.initialPos.x + al_get_bitmap_width(buttonImage);
			buttonPos.endPos.y = buttonPos.initialPos.y + al_get_bitmap_height(buttonImage);

			selectCharSample = al_load_sample("Resources/Sounds/ClickOnButton.wav");
			mouseOverButtonSample = al_load_sample("Resources/Sounds/MOUSEOVERBUTTON.wav");

			mouseOverButton = false;
		}
		else { cout << "Failed to load No selected Image, it seams this file is missing:" << nonSelectedImageName << endl; }
	}
	else { cout << "Couldn't load button image, it seams this file is missing:" << imageName << endl; }
}
void Button::changeCoords(int x, int y) {
	buttonPos.initialPos.x = x;
	buttonPos.initialPos.y = y;
	buttonPos.endPos.x = buttonPos.initialPos.x + al_get_bitmap_width(buttonImage);
	buttonPos.endPos.y = buttonPos.initialPos.y + al_get_bitmap_height(buttonImage);
}
void Button::updateButton(ALL *allegro) {
	if (mouseOverButton) { 
		//cout << buttonPos.initialPos.x << endl;
		al_draw_bitmap(nonSelectedImage, buttonPos.initialPos.x, buttonPos.initialPos.y - al_get_bitmap_height(buttonImage) / 2.0, 0);
	}
	else { 
		//cout << buttonPos.initialPos.x << endl;
		al_draw_bitmap(buttonImage, buttonPos.initialPos.x, buttonPos.initialPos.y - al_get_bitmap_height(buttonImage) / 2.0, 0);
	}
}
bool Button::checkMouse(int x, int y, double volume) {
	if ((x > buttonPos.initialPos.x) && (x < buttonPos.endPos.x)) {
		if ((y > buttonPos.initialPos.y - al_get_bitmap_height(buttonImage) / 2.0) && (y < buttonPos.endPos.y - al_get_bitmap_height(buttonImage) / 2.0)) {
			if (!mouseOverButton) {
				al_play_sample(mouseOverButtonSample, volume, 0.0, 3.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
			mouseOverButton = true;
		}
		else { mouseOverButton = false; }
	}
	else { mouseOverButton = false; }
	return mouseOverButton;
}
void Button::testButtonAction(int x, int y) {
	if (mouseOverButton) {
		//Here I set the Button callback
		cout << "Button Click" << endl;
	}
}
int Button::getMiddleOfX() {
	return (buttonPos.initialPos.x + buttonPos.endPos.x) / 2;
}
int Button::getMiddleOfY() {
	return (buttonPos.initialPos.y + buttonPos.endPos.y) / 2;
}
pos Button::getInitialPos(void){
	return buttonPos.initialPos;
}
Button::~Button(){
	al_destroy_bitmap(buttonImage);
	al_destroy_bitmap(nonSelectedImage);
	//al_destroy_sample(selectCharSample);
	//al_destroy_sample(mouseOverButtonSample);
}
