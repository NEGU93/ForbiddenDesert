#include "WaterCarrierCharacter.h"

WaterCarrierCharacter::WaterCarrierCharacter(int rowSet, int columnSet, Container info, ALL *allegro) : Character(rowSet, columnSet, info, allegro) {
	if(adventurerImage = al_load_bitmap_resized("Resources/Players/waterCarrierImage.png", allegro)){
		if (watersImages[0] = al_load_bitmap_resized("Resources/Water Level/WaterCarrier/0WaterLevelImage.png", allegro)) {
			if (watersImages[1] = al_load_bitmap_resized("Resources/Water Level/WaterCarrier/1WaterLevelImage.png", allegro)) {
				if (watersImages[2] = al_load_bitmap_resized("Resources/Water Level/WaterCarrier/2WaterLevelImage.png", allegro)) {
					if (watersImages[3] = al_load_bitmap_resized("Resources/Water Level/WaterCarrier/3WaterLevelImage.png", allegro)) {
						if (watersImages[4] = al_load_bitmap_resized("Resources/Water Level/WaterCarrier/4WaterLevelImage.png", allegro)) {
							if (watersImages[5] = al_load_bitmap_resized("Resources/Water Level/WaterCarrier/5WaterLevelImage.png", allegro)) {
								if (watersImages[6] = al_load_bitmap_resized("Resources/Water Level/WaterCarrier/DWaterLevelImage.png", allegro)) {
									if (image = al_load_bitmap_resized("Resources/Players/Pawns/WaterCarrierPawnImage.png", allegro)) {
										if (disabledGetWatersFromWellImage = al_load_bitmap_resized("Resources/Players/WaterCarrierButton/disabledGetWaterImage.png", allegro)) {
											waters = 5;
											getWatersFromWellButton = new Button(allegro->screenWidth / 7 + al_get_bitmap_width(disabledGetWatersFromWellImage), allegro->screenHeight / 2, "Resources/Players/WaterCarrierButton/noMouseOverGetWater.png", "Resources/Players/WaterCarrierButton/mouseOverGetWater.png", allegro);
										}
										else { cout << "Failed to load dislabled get waters from well image" << endl; }
									}
									else { cout << "Failed to load Water Carrier Pawn Image" << endl; }
								}
								else { cout << "Unable to load Dead Water Level Image" << endl; }
							}
							else { cout << "Unable to load 5 Water Level Image" << endl; }
						}
						else { cout << "Unable to load 4 water level Image" << endl; }
					}
					else { cout << "Unable to load 3 water level Image" << endl; }
				}
				else { cout << "Unable to load 2 water level Image" << endl; }
			}
			else { cout << "Unable to load 1 water level Image" << endl; }
		}
		else { cout << "Unable to load 0 water level Image" << endl; }
	}
	else { cout << "Failed to load archeologistImage.png" << endl; }
	//Sounds
	if (gettingCloseSound = al_load_sample("Resources/Sounds/MustBeGettingClose(Girl).wav")) {}
	else { cout << "Failed to load Must be getting close Sound" << endl; }
}
void WaterCarrierCharacter::addWater() {
	if (waters < 5) {
		waters++;
	}
}
void WaterCarrierCharacter::updateGetWatersFromWell(bool imOnDiscoveredWell, ALL* allegro) {
	if (imOnDiscoveredWell) { getWatersFromWellButton->updateButton(allegro); }
	else { al_draw_bitmap(disabledGetWatersFromWellImage, allegro->screenWidth / 7 + al_get_bitmap_width(disabledGetWatersFromWellImage), (allegro->screenHeight - al_get_bitmap_height(disabledGetWatersFromWellImage)) / 2, 0); }
}
bool WaterCarrierCharacter::getWatersFromWell() {
	if (waters < 5 && numberOfMoves > 0) {
		addWater();
		addWater();
		decreseMoves();
		return true;
	}
	else { return false; }
}
WaterCarrierCharacter::~WaterCarrierCharacter(){
	for (int i = 0; i < MAXOFWATERS; i++) { al_destroy_bitmap(watersImage[i]); }
	al_destroy_bitmap(waterCarrierImage);
	al_destroy_bitmap(disabledGetWatersFromWellImage);
	al_destroy_sample(gettingCloseSound);
	delete getWatersFromWellButton;
}
