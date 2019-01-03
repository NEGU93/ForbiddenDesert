#include "NavigatorCharacter.hpp"

NavigatorCharacter::NavigatorCharacter(int rowSet, int columnSet, Container info, ALL *allegro) : Character(rowSet, columnSet, info, allegro){
	if(adventurerImage = al_load_bitmap_resized("Resources/Players/navigatorImage.png", allegro)){
		if (image = al_load_bitmap_resized("Resources/Players/Pawns/NavigatorPawnImage.png", allegro)) {
			partnerMovesLeft = 3;
			doneButton = new Button(allegro->screenWidth / 8, allegro->screenHeight / 2 - 50, "Resources/Players/NavigatorButton/DoneButtonImage.png", "Resources/Players/NavigatorButton/moseOverButtonDoneButtonImage.png", allegro);
			moveOtherPlayerButton = new Button(allegro->screenWidth / 8, allegro->screenHeight / 2 - 50, "Resources/Players/NavigatorButton/noMouseOverButtonMoveOther.png", "Resources/Players/NavigatorButton/moveOverButtonMoveOther.png", allegro);
		}
		else { cout << "Failed to load Navigator Pawn Image" << endl; }
	}
	else { cout << "Failed to load navigatorImage.png" << endl; }
	//Sounds
	if (gettingCloseSound = al_load_sample("Resources/Sounds/MustBeGettingClose(Girl).wav")) {}
	else { cout << "Failed to load Must be getting close Sound" << endl; }
}
NavigatorCharacter::~NavigatorCharacter(){
	for (int i = 0; i < MAXOFWATERS; i++) { al_destroy_bitmap(watersImage[i]); }
	al_destroy_bitmap(navigatorImage);
	al_destroy_sample(gettingCloseSound);
	delete moveOtherPlayerButton;
	delete doneButton;
}

bool NavigatorCharacter::checkMouseOverButton(int x, int y, bool moveState, double volume) {
	if (moveState) { return doneButton->checkMouse(x, y, volume); }
	else { return moveOtherPlayerButton->checkMouse(x, y, volume); }
}
void NavigatorCharacter::updateNavigator(ALL* allegro, bool moveState) {
	if (moveState) {
		doneButton->updateButton(allegro);
		al_draw_textf(allegro->font, al_map_rgb(WHITE), allegro->screenWidth / 8, allegro->screenHeight / 2, ALLEGRO_ALIGN_CENTRE, "Number of Moves for the other Player: %d", partnerMovesLeft);
	}
	else { moveOtherPlayerButton->updateButton(allegro); }
}
void NavigatorCharacter::resetPartnerMoves() { partnerMovesLeft = 3; }

bool NavigatorCharacter::usedPartnerMoves() { return !(partnerMovesLeft == 3); }

bool NavigatorCharacter::decresePartnerMoves() { 
	if (partnerMovesLeft > 0) { 
		partnerMovesLeft--; 
		return true;
	}
	else { return false; }
}