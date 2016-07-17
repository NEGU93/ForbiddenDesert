#include "MeteorologistCharacter.h"

MeteorologistCharacter::MeteorologistCharacter(int rowSet, int columnSet, Container info, ALL* allegro) : Character(rowSet, columnSet, info, allegro) {
	if (adventurerImage = al_load_bitmap_resized("Resources/Players/meteorologistImage.png", allegro)) {
		if (image = al_load_bitmap_resized("Resources/Players/Pawns/MeteorologistPawnImage.png", allegro)) {
			seeStormDeckButton = new Button(allegro->screenWidth / 8, allegro->screenHeight / 2 - 50, "Resources/Players/MeteorologistButton/PeepButtonImage.png", "Resources/Players/MeteorologistButton/PeepButtonImage.png", allegro);	//TODO: cambiar las imagenes! XD
		}
		else { cout << "Failed to load Meteorologist Pawn Image" << endl; }
	}
	else { cout << "Failed to load meteorologistImage.png" << endl; }
	//Sounds
	if (gettingCloseSound = al_load_sample("Resources/Sounds/MustBeGettingClose(Boy).wav")) {}
	else { cout << "Failed to load Must be getting close Sound" << endl; }
}
MeteorologistCharacter::~MeteorologistCharacter(){
	delete seeStormDeckButton;
	for (int i = 0; i < MAXOFWATERS; i++) { al_destroy_bitmap(watersImage[i]); }
	al_destroy_bitmap(meteorologistImage);
	al_destroy_sample(gettingCloseSound);
}
bool MeteorologistCharacter::peep() { 
	if (numberOfMoves > 0) {
		decreseMoves();
		cout << "peep" << endl;
		return true;
	}
	else { return false; }
}

//	TODO: game se fija si le quedan movimientos y si es así roba menos cartas.
//  TODO: lo de ordenar las storm cards lo hace game... se fija si apretó el boton de ordenar las cartas y ahí corre su función.