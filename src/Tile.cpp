#include "Tile.hpp"

Tile::Tile() {}
Tile::Tile(TilesEnum tileType, ALL* allegro) {
	switch (tileType) {
	case STORM:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/stormImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/stormImage.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/crashSiteImage.png", "Resources/Buttons/Tiles/crashSiteImage.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/crashSiteImage.png", "Resources/Buttons/Tiles/crashSiteImage.png", allegro);
		break;
	case CRASHSITE:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/crashSiteImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/crashSiteImage.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noCrashSiteImage.png", "Resources/Buttons/Tiles/crashSiteImage.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noCrashSiteImage.png", "Resources/Buttons/Tiles/crashSiteImage.png", allegro);
		break;
	case LAUNCHPAD:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/backOfCardImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/launchPadImage.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noLaunchPadImage.png", "Resources/Buttons/Tiles/launchPadImage.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noBackOfCardImage.png", "Resources/Buttons/Tiles/backOfCardImage.png", allegro);
		break;
	case DEFAULT:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/waterTileImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/mirageImage.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noMirageImage.png", "Resources/Buttons/Tiles/mirageImage.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noWaterTileImage.png", "Resources/Buttons/Tiles/waterTileImage.png", allegro);
		break;
	case WATER:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/waterTileImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/wellImage.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noWellImage.png", "Resources/Buttons/Tiles/wellImage.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noWaterTileImage.png", "Resources/Buttons/Tiles/waterTileImage.png", allegro);
		break;
	case TUNNEL:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/backOfCardImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/tunnelImage.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noTunnelImage.png", "Resources/Buttons/Tiles/tunnelImage.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noBackOfCardImage.png", "Resources/Buttons/Tiles/backOfCardImage.png", allegro);
		break;
	case ENGINECOLUMN:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/backOfCardImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/engineColumnImage.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noEngineColumnImage.png", "Resources/Buttons/Tiles/engineColumnImage.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noBackOfCardImage.png", "Resources/Buttons/Tiles/backOfCardImage.png", allegro);
		break;
	case ENGINEROW:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/backOfCardImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/engineRowImage.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noEngineRowImage.png", "Resources/Buttons/Tiles/engineRowImage.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noBackOfCardImage.png", "Resources/Buttons/Tiles/backOfCardImage.png", allegro);
		break;
	case SOLARCRYSCOLUMN:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/backOfCardImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/solarCrystalColumnImage.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noSolarCrystalColumnImage.png", "Resources/Buttons/Tiles/solarCrystalColumnImage.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noBackOfCardImage.png", "Resources/Buttons/Tiles/backOfCardImage.png", allegro);
		break;
	case SOLARCRYSROW:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/backOfCardImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/solarCrystalRowImage.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noSolarCrystalRowImage.png", "Resources/Buttons/Tiles/solarCrystalRowImage.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noBackOfCardImage.png", "Resources/Buttons/Tiles/backOfCardImage.png", allegro);
		break;
	case PROPELLERCOLUMN:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/backOfCardImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/propellerColumnImage.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noPropellerColumnImage.png", "Resources/Buttons/Tiles/propellerColumnImage.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noBackOfCardImage.png", "Resources/Buttons/Tiles/backOfCardImage.png", allegro);
		break;
	case PROPELLERROW:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/backOfCardImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/propellerRowImage.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noPropellerRowImage.png", "Resources/Buttons/Tiles/propellerRowImage.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noBackOfCardImage.png", "Resources/Buttons/Tiles/backOfCardImage.png", allegro);
		break;
	case NAVIGATIONCOLUMN:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/backOfCardImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/navigationDeckColumnImage.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noNavigationDeckColumnImage.png", "Resources/Buttons/Tiles/navigationDeckColumnImage.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noBackOfCardImage.png", "Resources/Buttons/Tiles/backOfCardImage.png", allegro);
		break;
	case NAVIGATIONROW:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/backOfCardImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/navigationDeckRowImage.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noNavigationDeckRowImage.png", "Resources/Buttons/Tiles/navigationDeckRowImage.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noBackOfCardImage.png", "Resources/Buttons/Tiles/backOfCardImage.png", allegro);
		break;
	case GEAR1:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/backOfCardImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/gear1Image.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noGear1Image.png", "Resources/Buttons/Tiles/gear1Image.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noBackOfCardImage.png", "Resources/Buttons/Tiles/backOfCardImage.png", allegro);
		break;
	case GEAR2:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/backOfCardImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/gear2Image.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noGear2Image.png", "Resources/Buttons/Tiles/gear2Image.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noBackOfCardImage.png", "Resources/Buttons/Tiles/backOfCardImage.png", allegro);
		break;
	case GEAR3:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/backOfCardImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/gear3Image.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noGear3Image.png", "Resources/Buttons/Tiles/gear3Image.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noBackOfCardImage.png", "Resources/Buttons/Tiles/backOfCardImage.png", allegro);
		break;
	case GEAR4:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/backOfCardImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/gear4Image.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noGear4Image.png", "Resources/Buttons/Tiles/gear4Image.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noBackOfCardImage.png", "Resources/Buttons/Tiles/backOfCardImage.png", allegro);
		break;
	case GEAR5:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/backOfCardImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/gear5Image.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noGear5Image.png", "Resources/Buttons/Tiles/gear5Image.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noBackOfCardImage.png", "Resources/Buttons/Tiles/backOfCardImage.png", allegro);
		break;
	case GEAR6:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/backOfCardImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/gear6Image.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noGear6Image.png", "Resources/Buttons/Tiles/gear6Image.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noBackOfCardImage.png", "Resources/Buttons/Tiles/backOfCardImage.png", allegro);
		break;
	case GEAR7:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/backOfCardImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/gear7Image.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noGear7Image.png", "Resources/Buttons/Tiles/gear7Image.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noBackOfCardImage.png", "Resources/Buttons/Tiles/backOfCardImage.png", allegro);
		break;
	case GEAR8:
		this->backOfCardImage = al_load_bitmap_resized("Resources/Tiles/backOfCardImage.png", allegro);
		this->frontOfCardImage = al_load_bitmap_resized("Resources/Tiles/gear8Image.png", allegro);
		frontCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noGear8Image.png", "Resources/Buttons/Tiles/gear8Image.png", allegro);
		backCardButton = new Button(0, 0, "Resources/Buttons/Tiles/noBackOfCardImage.png", "Resources/Buttons/Tiles/backOfCardImage.png", allegro);
		break;
	}
	if (this->backOfCardImage && this->frontOfCardImage) {
		turned = false;
		tileEnum = tileType;
		if(oneSandImage = al_load_bitmap_resized("Resources/Tiles/sandMarkerImage.png", allegro)){
			if (multipleSandImage = al_load_bitmap_resized("Resources/Tiles/highSandMarkerImage.png", allegro)) {
				sand = 0;
				tileCoords.x = 0;
				tileCoords.y = 0;
			}
		}
		else { cerr << "Failed to load sandMarkerImage.png" << endl; }
	}
	else { cerr << "Failed to load front of card Image for " << TilesNames[tileType] << endl; }
}
void Tile::setCoord(pos coords) { 
	tileCoords = coords;
	frontCardButton->changeCoords(coords.x - 4, coords.y + al_get_bitmap_height(backOfCardImage) / 2 + 1);
	backCardButton->changeCoords(coords.x - 4, coords.y + al_get_bitmap_height(backOfCardImage) / 2 + 1);
}
void Tile::setSand(int i) { sand = i; }
void Tile::increaseSand() { sand++; }
void Tile::updateTile(ALL *allegro) {
	if (turned) {
		al_draw_bitmap(frontOfCardImage, tileCoords.x, tileCoords.y, 0);
	}
	else {
		al_draw_bitmap(backOfCardImage, tileCoords.x, tileCoords.y, 0);
	}
	if (sand == 1) {
		al_draw_bitmap(oneSandImage, tileCoords.x, tileCoords.y, 0);
	}
	else if (sand > 1) {
		al_draw_bitmap(multipleSandImage, tileCoords.x, tileCoords.y, 0);
		if (sand > 2) {		//Les gusta asi? cosa que cuando hay 2 no dice nada y te avisa solo si hay mas? saquen el if si prefieren que siempre avise
			al_draw_textf(allegro->font, al_map_rgb(WHITE), tileCoords.x + al_get_bitmap_width(multipleSandImage) / 2, tileCoords.y + al_get_bitmap_height(multipleSandImage) / 4, ALLEGRO_ALIGN_CENTER, "%d", sand);
		}
	}
}
bool Tile::canIMove(bool climber) {
	if (climber == false) {
		if (sand > 1) { return false; }
	}
	return (tileEnum != STORM);			//If there are 1 or 0 sands I can move only if the tile its not the storm.
}
bool Tile::canIMoveClimber() {
	return (tileEnum != STORM);
}
bool Tile::haveSand() {
	if (sand != 0) { return true; } 
	else { return false; }
}
void Tile::turnTile(){
	if (!haveSand()) {
		turned = true;
	}
}
void Tile::turnTerrascopeTile()
{
	turned=true;
	sand=0;
}
//Buttons methods
void Tile::updateButton(ALL* allegro) {
	if (turned) { frontCardButton->updateButton(allegro); }
	else { backCardButton->updateButton(allegro); }
	if (sand == 1) { al_draw_bitmap(oneSandImage, tileCoords.x, tileCoords.y, 0); }
	else if (sand > 1) {
		al_draw_bitmap(multipleSandImage, tileCoords.x, tileCoords.y, 0);
		if (sand > 2) {		//Les gusta asi? cosa que cuando hay 2 no dice nada y te avisa solo si hay mas? saquen el if si prefieren que siempre avise
			al_draw_textf(allegro->font, al_map_rgb(WHITE), tileCoords.x + al_get_bitmap_width(multipleSandImage) / 2, tileCoords.y + al_get_bitmap_height(multipleSandImage) / 4, ALLEGRO_ALIGN_CENTER, "%d", sand);
		}
	}
}
bool Tile::checkButton(int mousex, int mousey, double volume) {
	if (turned) { return frontCardButton->checkMouse(mousex, mousey, volume); }
	else { return backCardButton->checkMouse(mousex,mousey, volume); }
}
Tile::~Tile(){
	delete frontCardButton;
	delete backCardButton;
	al_destroy_bitmap(backOfCardImage);
	al_destroy_bitmap(frontOfCardImage);
	al_destroy_bitmap(oneSandImage);
	al_destroy_bitmap(multipleSandImage);
}
