#include "ExplorerCharacter.hpp"

ExplorerCharacter::ExplorerCharacter(int rowSet, int columnSet, Container info, ALL *allegro) : Character(rowSet, columnSet, info, allegro){
	if(adventurerImage = al_load_bitmap_resized("Resources/Players/explorerImage.png", allegro)){
		if (image = al_load_bitmap_resized("Resources/Players/Pawns/ExplorerPawnImage.png", allegro)) {
		}
		else { cout << "Failed to load Explorer Pawn Image" << endl; }
	}
	else { cout << "Failed to load explorerImage.png" << endl; }
	//Sounds
	if (gettingCloseSound = al_load_sample("Resources/Sounds/MustBeGettingClose(Boy).wav")) {}
	else { cout << "Failed to load Must be getting close Sound" << endl; }
}
void ExplorerCharacter::moveUpRightDiagonally() {
	if (column < 4 && row > 0 && numberOfMoves > 0) {
		column++;
		row--;
		decreseMoves();
	}
}
void ExplorerCharacter::moveUpLeftDiagonally() {
	if (column > 0 && row > 0 && numberOfMoves > 0) {
		column--;
		row--;
		decreseMoves();
	}
}
void ExplorerCharacter::moveDownRightDiagonally() {
	if (column < 4 && row < 4 && numberOfMoves > 0) {
		column++;
		row++;
		decreseMoves();
	}
}
void ExplorerCharacter::moveDownLeftDiagonally() {
	if (column > 0 && row < 4 && numberOfMoves > 0) {
		column--;
		row++;
		decreseMoves();
	}
}
void ExplorerCharacter::useDuneBlasterDiagonally(){
	//TODO: movimiento especial, hay que hacerlo todo, lo declaro ahora como para que est�.
}
ExplorerCharacter::~ExplorerCharacter(){
	for (int i = 0; i < MAXOFWATERS; i++) { al_destroy_bitmap(watersImage[i]); }
	al_destroy_bitmap(explorerImage);
	al_destroy_sample(gettingCloseSound);
}
