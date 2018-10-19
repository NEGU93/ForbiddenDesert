#include "StormCard.hpp"
#define STORMCARD_X 0.8375
#define STORMCARD_Y 0.3722
StormCard::StormCard() {}
StormCard::StormCard(StormCardsEnum cardEnum, ALL *allegro) {
	switch (cardEnum)
	{
	case SUNBEATSDOWN:
		this->cardImage = al_load_bitmap_resized("Resources/Storm Cards/sunBeatsDownImage.png", allegro);
		break;
	case STORMUP:
		this->cardImage = al_load_bitmap_resized("Resources/Storm Cards/stormPickUpImage.png", allegro);
		break;
	case RIGHT1:
		this->cardImage = al_load_bitmap_resized("Resources/Storm Cards/1stormRightImage.png", allegro);
		break;
	case LEFT1:
		this->cardImage = al_load_bitmap_resized("Resources/Storm Cards/1stormLeftImage.png", allegro);
		break;
	case UP1:
		this->cardImage = al_load_bitmap_resized("Resources/Storm Cards/1stormUpImage.png", allegro);
		break;
	case DOWN1:
		this->cardImage = al_load_bitmap_resized("Resources/Storm Cards/1stormDownImage.png", allegro);
		break;
	case RIGHT2:
		this->cardImage = al_load_bitmap_resized("Resources/Storm Cards/2stormRightImage.png", allegro);
		break;
	case LEFT2:
		this->cardImage = al_load_bitmap_resized("Resources/Storm Cards/2stormLeftImage.png", allegro);
		break;
	case UP2:
		this->cardImage = al_load_bitmap_resized("Resources/Storm Cards/2stormUpImage.png", allegro);
		break;
	case DOWN2:
		this->cardImage = al_load_bitmap_resized("Resources/Storm Cards/2stormDownImage.png", allegro);
		break;
	case RIGHT3:
		this->cardImage = al_load_bitmap_resized("Resources/Storm Cards/3stormRightImage.png", allegro);
		break;
	case LEFT3:
		this->cardImage = al_load_bitmap_resized("Resources/Storm Cards/3stormLeftImage.png", allegro);
		break;
	case UP3:
		this->cardImage = al_load_bitmap_resized("Resources/Storm Cards/3stormUpImage.png", allegro);
		break;
	case DOWN3:
		this->cardImage = al_load_bitmap_resized("Resources/Storm Cards/3stormDownImage.png", allegro);
		break;
	}

	if( this->cardImage)
		stormCardEnum = cardEnum;
	else { cout << "Failed to load " << cardEnum << " image" << endl;}
}
void StormCard::showImage(ALL *allegro){
	al_draw_bitmap(cardImage, allegro->screenWidth * STORMCARD_X, allegro->screenHeight * STORMCARD_Y, 0);
	al_flip_display();
}
StormCard::~StormCard(){
	al_destroy_bitmap(cardImage);
}