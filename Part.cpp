#include "Part.h"

Part::Part() {
	if (partImage = al_load_bitmap("Resources/Parts/engineImage.png")) {
		column = -1;
		row = -1;
		found = false;
	}
	else{ cout << "Failed to run the Part's default constructor correctly" << endl; }
}
Part::Part(const char* partImageName) {
	if (this->partImage = al_load_bitmap(partImageName)) {
		if (pickUpSound = al_load_sample("Resources/Sounds/pickUpPart.wav")) {
			column = -1;
			row = -1;
			found = false;
			picked = false;
		}
	}
	else{ cout << "Failed to load " << partImageName << endl; }
}
void Part::updatePart(ALL* allegro) {
	if (found) {
		al_draw_bitmap(partImage, partCoords.x, partCoords.y, 0);
	}
}
void Part::findColum(int newColumn, ALL* allegro) {
	column = newColumn;
	if (row != -1) {
		found = true;
		partCoords = updateCoords(allegro);						//This actualy create the pos of the part for it to be displayed
	}
}
void Part::findRow(int newRow, ALL* allegro) {
	row = newRow;
	if (column != -1) {
		found = true;
		partCoords = updateCoords(allegro);
	}
}
void Part::pickUpPart(){
	found = false;
	picked = true;
	al_play_sample(pickUpSound, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
}
void Part::setCardIndex(int index, ALL* allegro) {
	column = index / 5;
	row = index % 5;
	partCoords = updateCoords(allegro);
}
pos Part::updateCoords(ALL* allegro) {
	int n = allegro->screenWidth / 3.0;
	int yoffset = (allegro->screenHeight - n) / 2.0;
	int xoffset = n;
	pos tempPos;
	if(	partImage){
		int l = abs((n - al_get_bitmap_width(partImage) * 5) / 4);	//If the number is negative it souldn't run anyway... but at least it won't explote.
		tempPos.x = (column)*(al_get_bitmap_width(partImage) + l) + xoffset;
		tempPos.y = (row)*(al_get_bitmap_width(partImage) + l) + yoffset;
	}
	else {
		cout << "Failed to updateCoords, Part.cpp" << endl;
		tempPos.x=0;
		tempPos.y=0;
	}

		return tempPos;
}
Part::~Part(){
	al_destroy_bitmap(partImage);
	//al_destroy_sample(pickUpSound);
}