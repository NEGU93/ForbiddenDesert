#include "Character.hpp"

Character::Character() {
		row = 0;
		column = 0;
		numberOfMoves = 4;
		waters = 4;
}
Character::Character(int rowSet, int columnSet, Container info, ALL* allegro) {
	
	int yoffset = allegro->screenHeight * 16 / 19 + al_get_bitmap_height(al_load_bitmap_resized("Resources/Equipment/haveDuneBlasterImage.png", allegro)) / 2.0;
	int xoffset = allegro->screenWidth / 3;
	int i = 0;
	if (gettingCloseSound = al_load_sample("Resources/Sounds/MustBeGettingClose(Boy).wav")) {

	}
	else { cerr << "Failed to load Must be getting close Sound" << endl; }
	if (image) {
		if (watersImages[0] = al_load_bitmap_resized("Resources/Water Level/0WaterLevelImage.png", allegro)) {
			if (watersImages[1] = al_load_bitmap_resized("Resources/Water Level/1WaterLevelImage.png", allegro)) {
				if (watersImages[2] = al_load_bitmap_resized("Resources/Water Level/2WaterLevelImage.png", allegro)) {
					if (watersImages[3] = al_load_bitmap_resized("Resources/Water Level/3WaterLevelImage.png", allegro)) {
						if (watersImages[4] = al_load_bitmap_resized("Resources/Water Level/4WaterLevelImage.png", allegro)) {
							if (watersImages[5] = watersImages[6] = al_load_bitmap_resized("Resources/Water Level/DWaterLevelImage.png", allegro)) {
								getClose = true;
								row = rowSet;
								column = columnSet;
								numberOfMoves = 4;
								waters = 4;
								shield = false;
								solarShield = 0;
								duneBlaster = 0;
								jetPack = 0;
								secretWaterReserve = 0; 
								terrascope = 0;
								timeThrottle = 0;
								duneBlasterButton = new Button(xoffset, yoffset, "Resources/Equipment/haveDuneBlasterImage.png", "Resources/Equipment/haveDuneBlasterImage.png", allegro);
								jetPackButton = new Button( xoffset + (al_get_bitmap_width(allegro->noEquipments[4]) + 1), yoffset, "Resources/Equipment/haveJetPackImage.png","Resources/Equipment/haveJetPackImage.png", allegro);
								secretWaterReserveButton = new Button(xoffset + 2 * (al_get_bitmap_width(allegro->noEquipments[0]) + 1), yoffset,"Resources/Equipment/haveSecretWaterReserveImage.png","Resources/Equipment/haveSecretWaterReserveImage.png", allegro);
								solarShieldButton = new Button(xoffset + 3 * (al_get_bitmap_width(allegro->noEquipments[4]) + 1), yoffset,"Resources/Equipment/haveSolarShieldImage.png","Resources/Equipment/haveSolarShieldImage.png", allegro);
								terrascopeButton = new Button( xoffset + 4 * (al_get_bitmap_width(allegro->noEquipments[4]) + 1), yoffset, "Resources/Equipment/haveTerrascopeImage.png","Resources/Equipment/haveTerrascopeImage.png", allegro);
								timeThrottleButton = new Button(xoffset + 5 * (al_get_bitmap_width(allegro->noEquipments[4]) + 1), yoffset,"Resources/Equipment/haveTimeThrottleImage.png","Resources/Equipment/haveTimeThrottleImage.png", allegro);
							}
							else { cerr << "Unable to load Dead Water Level Image" << endl; }
						}
						else { cerr << "Unable to load 4 water level Image" << endl; }
					}
					else { cerr << "Unable to load 3 water level Image" << endl; }
				}
				else { cerr << "Unable to load 2 water level Image" << endl; }
			}
			else { cerr << "Unable to load 1 water level Image" << endl; }
		}
		else { cerr << "Unable to load 0 water level Image" << endl; }
	}
}
void Character::drinkWater() {
	if (waters > 0 && waters != 6) { waters--; }
	else { waters = 6; } //I use the sixth image to show the dead Image
}
void Character::addWater(){
	if(waters < 4)
		waters++;
}
bool Character::haveWater(){
	if (waters == 6) { return false; }
	return true;
}
void Character::updateChar(ALL *allegro, int numofplayer, int x, int y) {
	if (numofplayer == 1) { al_draw_textf(allegro->font, al_map_rgb(WHITE), x, 2 * y, ALLEGRO_ALIGN_CENTRE, "Your moves Left: %d", numberOfMoves); }
	else { al_draw_textf(allegro->font, al_map_rgb(WHITE), x, 2 * y, ALLEGRO_ALIGN_CENTRE, "Partner moves Left: %d", numberOfMoves); }
	updateChar(allegro, numofplayer);
}
void Character::updateChar(ALL *allegro, int numofplayer) {
	pos charCoords = updateCoords(allegro);
	al_draw_bitmap(image, charCoords.x, charCoords.y, 0);
	//al_draw_bitmap(waterImage, allegro->screenWidth - al_get_bitmap_width(waterImage)*3/2, allegro->screenHeight / 2, 0);
	if(numofplayer == 1){
		al_draw_textf(allegro->gameFont, al_map_rgb(WHITE), allegro->screenWidth/16, allegro->screenHeight * 0.05, ALLEGRO_ALIGN_CENTRE,"%s",name);
		if (waters >= 0) { al_draw_bitmap(watersImages[waters], 0, allegro->screenHeight / 2 - al_get_bitmap_height(watersImages[waters]), 0); }
		al_draw_bitmap(adventurerImage, allegro->screenWidth / 8, allegro->screenHeight * 0.1, 0);
		//al_draw_bitmap(allegro->roles[(int)rol-48], allegro->screenWidth/8, allegro->screenHeight/8, 0);
		updateEquipments(allegro);
	}
	else if(numofplayer == 2){
		al_draw_textf(allegro->gameFont, al_map_rgb(WHITE), allegro->screenWidth/16, allegro->screenHeight * 0.55, ALLEGRO_ALIGN_CENTRE,"%s",name);
		if (waters >= 0) { al_draw_bitmap(watersImages[waters], 0, allegro->screenHeight - al_get_bitmap_height(watersImages[waters]), 0); }
		al_draw_bitmap(adventurerImage, allegro->screenWidth / 8, allegro->screenHeight * 0.6, 0);
		//al_draw_bitmap(allegro->roles[(int)rol-48], allegro->screenWidth/8, allegro->screenHeight *0.625, 0);
	}
	
}
bool Character::checkButton(EquipmentsEnum equipEnum, int mousex, int mousey, double volume){
	switch (equipEnum)
	{
	case DUNEBLAST:
		if(duneBlasterButton->checkMouse(mousex, mousey, volume))
			return true;
		else return false;
	case JETPACK:
		if(jetPackButton->checkMouse(mousex, mousey, volume))
			return true;
		else return false;
	case SOLARSHIELD:
		if(solarShieldButton->checkMouse(mousex, mousey, volume))
			return true;
		else return false;
	case TERRASCOPE:
		if(terrascopeButton->checkMouse(mousex, mousey, volume))
			return true;
		else return false;
	case SECRETWATERRESERVE:
		if(secretWaterReserveButton->checkMouse(mousex, mousey, volume))
			return true;
		else return false;
	case TIMETHROTTLE:
		if(timeThrottleButton->checkMouse(mousex, mousey, volume))
			return true;
		else return false;
	default:
		return false;
	}
}
void Character::updateEquipments(ALL* allegro) {
	//Update Equipments
	int yoffset = allegro->screenHeight * 16 / 19;
	int xoffset = allegro->screenWidth / 3;
	int i = 0;
	if (duneBlaster > 0) {
		duneBlasterButton->updateButton(allegro);
		al_draw_textf(allegro->font, al_map_rgb(BLACK), xoffset + i * (al_get_bitmap_width(allegro->noEquipments[4]) + 1), yoffset, 0, "%d", duneBlaster);
	}
	else { al_draw_bitmap(allegro->noEquipments[i], xoffset + i * (al_get_bitmap_width(allegro->noEquipments[4]) + 1), yoffset, 0); }
	i++;
	if (jetPack > 0) {
		jetPackButton->updateButton(allegro);
		al_draw_textf(allegro->font, al_map_rgb(BLACK), xoffset + i * (al_get_bitmap_width(allegro->noEquipments[4]) + 1), yoffset, 0, "%d", jetPack);
	}
	else { al_draw_bitmap(allegro->noEquipments[i], xoffset + i * (al_get_bitmap_width(allegro->noEquipments[4]) + 1), yoffset, 0); }
	i++;
	if (secretWaterReserve > 0) {
		secretWaterReserveButton->updateButton(allegro);
		al_draw_textf(allegro->font, al_map_rgb(BLACK), xoffset + i * (al_get_bitmap_width(allegro->noEquipments[0]) + 1), yoffset, 0, "%d", secretWaterReserve);
	}
	else { al_draw_bitmap(allegro->noEquipments[i], xoffset + i * (al_get_bitmap_width(allegro->noEquipments[4]) + 1), yoffset, 0); }
	i++;
	if (solarShield > 0) {
		solarShieldButton->updateButton(allegro);
		al_draw_textf(allegro->font, al_map_rgb(BLACK), xoffset + i * (al_get_bitmap_width(allegro->noEquipments[4]) + 1), yoffset, 0, "%d", solarShield);
	}
	else { al_draw_bitmap(allegro->noEquipments[i], xoffset + i * (al_get_bitmap_width(allegro->noEquipments[4]) + 1), yoffset, 0); }
	i++;
	if (terrascope > 0) {
		terrascopeButton->updateButton(allegro);
		al_draw_textf(allegro->font, al_map_rgb(BLACK), xoffset + i * (al_get_bitmap_width(allegro->noEquipments[4]) + 1), yoffset, 0, "%d", terrascope);
	}
	else { al_draw_bitmap(allegro->noEquipments[i], xoffset + i * (al_get_bitmap_width(allegro->noEquipments[4]) + 1), yoffset, 0); }
	i++;
	if (timeThrottle > 0) {
		timeThrottleButton->updateButton(allegro);
		al_draw_textf(allegro->font, al_map_rgb(BLACK), xoffset + i * (al_get_bitmap_width(allegro->noEquipments[4]) + 1), yoffset, 0, "%d", timeThrottle);
	}
	else { al_draw_bitmap(allegro->noEquipments[i], xoffset + i * (al_get_bitmap_width(allegro->noEquipments[4]) + 1), yoffset, 0); }
}

bool Character::decreseMoves() {
	if (numberOfMoves > 0) { 
		numberOfMoves--; 
		return true;
	}
	else { return false; }
}
pos Character::updateCoords(ALL *allegro) {
	int n = allegro->screenWidth / 3.0;
	int yoffset = (allegro->screenHeight - n) / 2.0;
	int xoffset = n;
	pos tempPos;
	if(image){
		int l = abs((n - al_get_bitmap_width(image) * 5) / 4);	//If the number is negative it souldn't run anyway... but at least it won't explote.
		tempPos.x = (column)*(al_get_bitmap_width(image) + l) + xoffset;
		tempPos.y = (row)*(al_get_bitmap_width(image) + l) + yoffset;
	}
	else {
		cout << "Can't update coords" << endl;
		tempPos.x = 0;
		tempPos.y = 0;
	}
	return tempPos;
}
bool Character::moveUp() {
	if (row > 0 && numberOfMoves > 0) {
		row--;
		decreseMoves();
		return true;
	}
	else { return false; }
}
bool Character::moveDown() {
	if (row < 4 && numberOfMoves > 0) {
		row++;
		decreseMoves();
		return true;
	}
	else { return false; }
}
bool Character::moveLeft() {
	if (column > 0 && numberOfMoves > 0) {
		column--;
		decreseMoves();
		return true;
	}
	else { return false; }
}
bool Character::moveRight() {
	if (column < 4 && numberOfMoves > 0) {
		column++;
		decreseMoves();
		return true;
	}
	else { return false; }
}
bool Character::removeSand() {
	if (numberOfMoves > 0) {
#ifdef DEBUG_GAME
		cout << "Remove Sand" << endl;
#endif
		decreseMoves();
		return true;
	}
	else { return false; }
}
bool Character::dig() {
	if (numberOfMoves > 0) {
#ifdef DEBUG_GAME
		cout << "Dig" << endl;
#endif
		decreseMoves();
		return true;
	}
	else { return false; }
}
bool Character::pickUpPart() {
	if (numberOfMoves > 0) {
#ifdef DEBUG_GAME
		cout << "Pick Up Part" << endl;
#endif
		decreseMoves();
		return true;
	}
	else { return false; }
}
void Character::resetNumberOfMoves() {
	numberOfMoves = 4;
}
uint Character::getCardIndex() {
	return 5 * column + row;
}
bool Character::setPos(int row, int column) {
	if (this->row != row || this->column != column) {		//if already there its no point on doing this
		//cout << "set pos" << endl;
		if (!decreseMoves()) { return false; }
		this->row = row;
		this->column = column;
		return true;
	}
	else { 
		cerr << "They wanted me to move where I already was" << endl;
		return false;
	} //Normally this should never happen so I print for it to be fixed
}
void Character::setName(char* name){
	strncpy(this->name,name,SIZEOFNAME);
}

bool Character::haveDuneBlaster(){
	if(duneBlaster > 0)
		return true;
	else return false;
}
bool Character::haveJetPack(){
	if(jetPack > 0)
		return true;
	else return false;
}
bool Character::haveWaterReserve(){
	if(secretWaterReserve > 0)
		return true;
	else return false;
}
bool Character::haveSolarShield(){
	if(solarShield > 0)
		return true;
	else return false;
}
bool Character::haveTerrascope(){
	if(terrascope > 0)
		return true;
	else return false;
}
bool Character::haveTimeThrottle(){
	if(timeThrottle > 0)
		return true;
	else return false;
}
Character::~Character(){
	al_destroy_bitmap(image);
	al_destroy_bitmap(adventurerImage);
	delete duneBlasterButton;
	delete solarShieldButton;
	delete jetPackButton;
	delete secretWaterReserveButton;
	delete terrascopeButton;
	delete timeThrottleButton;
}
void Character::playGettingCloseSound() {
	if (getClose) {
		al_play_sample(gettingCloseSound, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		getClose = false;
	}
}