#include "ArcheologistCharacter.h"


ArcheologistCharacter::ArcheologistCharacter(int rowSet, int columnSet, Container info, ALL* allegro) : Character(rowSet, columnSet, info, allegro) {
	if(adventurerImage = al_load_bitmap_resized("Resources/Players/archeologistImage.png", allegro)){
		if (image = al_load_bitmap_resized("Resources/Players/Pawns/ArcheologistPawnImage.png", allegro)) {
			if (gettingCloseSound = al_load_sample("Resources/Sounds/MustBeGettingClose(Girl).wav")) {
			}
			else { cout << "Failed to load Must be getting close Sound" << endl; }
		}
		else { cout << "Failed to load Archeologist Pawn Image" << endl; }
	}
	else { cout << "Failed to load archeologistImage.png" << endl; }
}
ArcheologistCharacter::~ArcheologistCharacter()
{
	for (int i = 0; i < MAXOFWATERS; i++) { al_destroy_bitmap(watersImage[i]); }
	al_destroy_bitmap(archeologistImage);
	al_destroy_sample(gettingCloseSound);
}
