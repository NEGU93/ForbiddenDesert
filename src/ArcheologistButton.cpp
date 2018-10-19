#include "ArcheologistButton.hpp"

ArcheologistButton::ArcheologistButton() {}
ArcheologistButton::ArcheologistButton(int x0, int y0, const char *imageName, const char * nonSelectedImageName, ALL* allegro) : Button(x0, y0, imageName, nonSelectedImageName, allegro) {}

bool ArcheologistButton::buttonAction(int x, int y) {
	if (mouseOverButton) {
		cout << "Click on Archeologist Button" << endl;
		al_play_sample(selectCharSample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		return true;
	}
	return false;
}