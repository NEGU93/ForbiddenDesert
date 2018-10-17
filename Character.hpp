#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include "def.hpp"
#include "Packages.hpp"
#include "allegro.hpp"

#define MAXOFWATERS 7 //for the array of watersImage for each character

class Character {
public:
	Character();
	Character(int rowSet, int columnSet, Container info, ALL* allegro);
	~Character();
	//Update Allegro
	void updateChar(ALL* allegro, int numofplayer);		//TODO: necesito que la clase sea virtual para el dynamimc_cast :S
	void updateChar(ALL *allegro, int numofplayer, int x, int y);
	void updateEquipments(ALL* allegro);
	bool checkButton(EquipmentsEnum equipEnum, int mousex, int mousey, double volume);
	//Actions
	bool removeSand();
	bool dig();
	bool pickUpPart();
	//Waters
	void drinkWater();
	virtual void addWater();
	bool haveWater();
	int getWaters() { return waters; }
	//Equipments
	bool shield;		//TODO: no me gusta :P
	bool haveSolarShield();
	void increaseSolarShield() { solarShield++; }
	void decreaseSolarShield() { solarShield--; }
	bool haveDuneBlaster();
	void increaseDuneBlaster() { duneBlaster++; }
	void decreaseDuneBlaster() { duneBlaster--; }
	bool haveJetPack();
	void increaseJetPack() { jetPack++; }
	void decreaseJetPack() { jetPack--; }
	bool haveWaterReserve();
	void increaseWaterReserve() { secretWaterReserve++; }
	void decreaseWaterReserve() { secretWaterReserve--; }
	bool haveTerrascope();
	void increaseTerrascope() { terrascope++; }
	void decreaseTerrascope() { terrascope--; }
	bool haveTimeThrottle();
	void increaseTimeThrottle() { timeThrottle++; }
	void decreaseTimeThrottle() { timeThrottle--; }
	//Setter
	bool setPos(int row, int column);
	void setName(char* name);
	//Getters
	uint getCardIndex();
	int getNumberOfMoves() { return numberOfMoves; }
	int getRow() { return row; }
	int getColumn() { return column; }
	int getDuneBlast() { return duneBlaster; }
	int getJetPack() { return jetPack; }
	int getSolarShield() { return solarShield; }
	int getTerrascope() { return terrascope; }
	int getSecretWaterReserve() { return secretWaterReserve; }
	int getTimeThrottle() { return timeThrottle; }
	//Moves Left
	void resetNumberOfMoves();
	void increaseMoves(){ numberOfMoves++; }
	//Movements
	bool moveUp();
	bool moveDown();
	bool moveLeft();
	bool moveRight();
	//Sounds
	void playGettingCloseSound();
protected:
	//methods
	bool decreseMoves();
	//variables
	char name[SIZEOFNAME];
	pos updateCoords(ALL *allegro);
	int numberOfMoves;	//Number of moves left before end of turn
	int row;			//Location of the player
	int column;
	int waters;
	//Images
	ALLEGRO_BITMAP *image;
	ALLEGRO_BITMAP* watersImages[MAXOFWATERS];
	ALLEGRO_BITMAP* adventurerImage;
	//Equipments
	int solarShield;
	int duneBlaster;
	int jetPack;
	int secretWaterReserve;
	int terrascope;
	int timeThrottle;
	//Buttons (Equipments)
	Button *duneBlasterButton; 
	Button *solarShieldButton;
	Button *jetPackButton;
	Button *secretWaterReserveButton;
	Button *terrascopeButton;
	Button *timeThrottleButton;
	//Sounds
	bool getClose;
	ALLEGRO_SAMPLE *gettingCloseSound;
};

#endif