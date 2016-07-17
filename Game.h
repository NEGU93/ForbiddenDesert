
#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "def.h"
#include "allegro.h"
#include "Button.h"
#include "Tile.h"
#include "Character.h"
/* ESTO NO ME GUSTA */
#include "MeteorologistCharacter.h"
#include "ClimberCharacter.h"
#include "ArcheologistCharacter.h"
#include "ExplorerCharacter.h"
#include "WaterCarrierCharacter.h"
#include "NavigatorCharacter.h"

#include "StartMenu.h"
#include "fsmInit.h"
#include "Part.h"
#include "StormCard.h"
#include <algorithm>		//Used to shuffle the array of tiles
#include <array>

#define MODEBUTTONSNUMBER 7
#define TILEDECKNUMBER 25
#define MAXWATERS 5

enum ModeEnum { NORMAL, REMOVESAND, //Normal Use
	PEEPMODE, MOVEOTHER,			//Characters special
	DUNEBLASTERMODE, JETPACKMODE, TERRASCOPEMODE,	//Equipments
	OFFEREQUIPMENTMODE, REQUESTEQUIPMENTMODE, OFFERWATERMODE, REQUESTWATERMODE };

class Game {
public:
	Game(ALL *allegro, FSMI *gameNetwork);
	~Game();
	bool eventHandler(ALL *allegro);
	bool receiveHandler(ALL* allegro);
	bool getPlayAgain() { return playAgain; }
	bool getGameOver() { return gameOver; }

private:
	//Methods
	pos getSpot(int row, int column, ALL* allegro, ALLEGRO_BITMAP* image);		//Gets the pos of the image you must put giving the row and column where the item is.
	void updateScreen(ALL* allegro);
	void updateTilesButton(ALL* allegro);
	void pickUpAction();
	//End Of Game
	bool win(ALL *allegro);
	bool lose(ALL *allegro);
	void wannaPlay(ALL *allegro, char ID);
	//Mouse Methods
	void click(ALL* allegro);
	void clickOnPeepMode(ALL* allegro);
	void clickOnJetPackMode(ALL* allegro);
	void clickOnTerrascopeMode(ALL* allegro);
	void clickOnRequestWaterMode(ALL*  allegro);
	void clickOnOfferEquipmentMode(ALL* allegro);
	void clickOnRequestEquipmentMode(ALL* allegro);
	void clickOnOfferWaterMode(ALL* allegro);
	void clickOnDefaultMode(ALL* allegro);
	void checkMouse(ALL* allegro);
	void checkTilesButtons(double volume);
	void clickTilesButtons(ALL *allegro);
	void clickModesButtons(ALL* allegro);
	void clickEquipmentButtons(ALL* allegro);
	//Perform Action
	void performTileAction(TilesEnum tileEnum, int index, ALL* allegro);
	void performEquipmentsAction(EquipmentsEnum equipEnum, ALL *allegro);
	//Tile Deck methods
	void swapTiles(int i, int j);		//Lets use swap from STL (algorithm)
	void shuffleTiles();		//Lets use shuffle from STL
	//Equipments methods
	void drawEquipment(EquipmentsEnum equipEnum);
	void chooseOption(ALL* allegro, Container temp);
	//StormCards methods
	void performStormCardAction(ALL *allegro, StormCardsEnum cardEnum);
	void drawStormCards(ALL *allegro);
	void swapStormCards(int i, int j);
	//Initialize Methods
	bool initializeAllegro(ALL* allegro);
	void initializeBoard(ALL* allegro);
	void initializeStormCards(ALL* allegro);
	void initializeCharacters(Container info, ALL* allegro);
	void initializeButtons(ALL* allegro);
	void initializeWaterButtons(ALL* allegro);
	void initializePeepButtons(ALL* allegro);
	void Game::initializeEquipmentCardButton(ALL* allegro);
	//Keys Pressed
	void keyUp(ALL *allegro);
	void keyDown(ALL *allegro);
	void keyLeft(ALL *allegro);
	void keyRight(ALL *allegro);
	void keyE(ALL* allegro);
	void keyP(ALL *allegro);
	void keyR(ALL *allegro);
	void keyEscape();
	//Peep Methods
	void nextPeepStormCard();
	void previousPeepStormCard();
	void resetPeepOffset() { peepOffset = 0; }
	//Offer equipment Methods
	void nextEquipCard();
	void previousEquipCard();
	// ---------------------- Variables ---------------------
	pos mouse;
	bool redraw;
	FSMI *gameNetwork;
	ModeEnum modeEnum;				//ModeEnum is used to know in wich mode I am... say.. have to move or remove sand
	int SandMarkersLeft;
	int sandStormLevel;
	Pack gamePack;
	//double volume;
	//Decks
	std::array< StormCard *, CANTOFSTORM> stormCardsDeck;
	std::array<Tile *, TILEDECKNUMBER> tilesDeck;		//Important! its a deck of cards seen as an array, we will use algorithm library to make it work.
	//int stormCardIndex;			//Indexes for the decks
	int equipIndex;
	int peepOffset;
	EquipmentsEnum equipOffset;
	int firstTimeUpdateScreenChooseEquipment;		//ChooseEquipment Counter								
	int firstTimeUpdateScreenPeep;					//Peep Counter
	// Character Variables
	bool turn;							//Used to know which turn is it... true = this computer turn. False = companion turn.
	bool playAgain;
	bool gameOver;
	Character *character;
	Character *character2;
	//Parts
	Part *engine;
	Part *solarCrystal;
	Part *propeller;
	Part *navigationDeck;
	//BUTTONS
	Button *endTurnButton;
	Button *modesButtons[MODEBUTTONSNUMBER];
	Button *watersButton[MAXWATERS];
	//Peep Buttons
	Button *nextButton;
	Button *previousButton;
	Button *selectPeepCard;
	Button *selectEquipmentCard;
	//VolumeButtons
	Button *muteButton;
	Button *noMuteButton;
};

#endif
