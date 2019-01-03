#include "ClimberCharacter.hpp"

ClimberCharacter::ClimberCharacter(int rowSet, int columnSet, Container info, ALL* allegro) : Character(rowSet, columnSet, info, allegro) {
	if(adventurerImage = al_load_bitmap_resized("Resources/Players/climberImage.png", allegro)){
		if (image = al_load_bitmap_resized("Resources/Players/Pawns/ClimberPawnImage.png", allegro)) {
			if (selectedImage = al_load_bitmap_resized("Resources/Players/ClimberButton/selectedImage.png", allegro)) {
				if(takeWithMeButton = new Button(allegro->screenWidth / 8, allegro->screenHeight / 2, "Resources/Players/ClimberButton/noMouseOverCheckBoxImage.png", "Resources/Players/ClimberButton/mouseOverCheckBoxImage.png", allegro)) {
					takeWithMe = false;
				} else { cerr << "Failed to load takeWithMeButton" << endl;}
			} else { cerr << "Failed to init selectedImage" << endl;}
		} else { cerr << "Failed to load Climber Pawn Image" << endl; }
	} else { cerr << "Failed to load climberImage.png" << endl; }
	//Sounds
	if (gettingCloseSound = al_load_sample("Resources/Sounds/MustBeGettingClose(Boy).wav")) {}
	else { cerr << "Failed to load Must be getting close Sound" << endl; }
}
ClimberCharacter::~ClimberCharacter(){
	al_destroy_bitmap(climberImage);
	al_destroy_bitmap(selectedImage);
	al_destroy_bitmap(unableToTakeCharacterWithMe);
	al_destroy_sample(gettingCloseSound);
	for (int i = 0; i < MAXOFWATERS; i++) { al_destroy_bitmap(watersImage[i]); }
	delete takeWithMeButton;
}
void ClimberCharacter::updateClimber(ALL* allegro) {
	if (takeWithMe) {
		al_draw_bitmap(selectedImage, allegro->screenWidth / 8, (allegro->screenHeight- al_get_bitmap_height(selectedImage)) / 2, 0); //Here, X and Y are the same as in the constructor of takeWithMeButton
	}
	takeWithMeButton->updateButton(allegro);
}
void ClimberCharacter::clickOnButton(int x, int y, double volume) {
	if (takeWithMeButton->checkMouse(x, y, volume)) {
		toogleTakeWithMe();
		cout << "Toogle take with me button to " << getTakeWithMe() << endl;
	}
}