#ifndef TILES_H_INCLUDED
#define TILES_H_INCLUDED

#include "def.hpp"
#include "allegro.hpp"
#include "Button.hpp"

enum TilesEnum { STORM, CRASHSITE, WATER, DEFAULT, TUNNEL, LAUNCHPAD, 
	GEAR1, GEAR2, GEAR3, GEAR4, GEAR5, GEAR6, GEAR7, GEAR8, 
	PROPELLERROW, PROPELLERCOLUMN, SOLARCRYSROW, SOLARCRYSCOLUMN, NAVIGATIONROW, NAVIGATIONCOLUMN, ENGINEROW, ENGINECOLUMN };

class Tile {
public:
	Tile();
	Tile(TilesEnum tileType, ALL* allegro);
	~Tile();
	void updateTile(ALL *allegro);
	//Buttons Methods
	void updateButton(ALL* allegro);
	bool checkButton(int mousex, int mousey, double volume);
	//Setters
	void turnTile();
	void turnTerrascopeTile();
	void setSand(int i);
	void increaseSand();
	void setCoord(pos coords);
	// Getters
	TilesEnum gettileType() { return tileEnum; }
	bool isTurned() {return turned;}
	void setTurned(bool turned) { this->turned = turned; }
	bool canIMove(bool climber);
	int getSand(void) { return sand; }
	bool canIMoveClimber();
	bool haveSand();
	void removeSand() { if(sand>0) sand--; }
	ALLEGRO_BITMAP* getTileBackImage() { return backOfCardImage; }
private:
	ALLEGRO_BITMAP *backOfCardImage;
	ALLEGRO_BITMAP *frontOfCardImage;
	Button *frontCardButton;
	Button *backCardButton;
	ALLEGRO_BITMAP *oneSandImage;
	ALLEGRO_BITMAP *multipleSandImage;
	pos tileCoords;
	bool turned;
	TilesEnum tileEnum;
	int sand;
};

#endif