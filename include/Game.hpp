
#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "def.hpp"
#include "allegro.hpp"
#include "Button.hpp"
#include "Tile.hpp"
#include "Character.hpp"
/* ESTO NO ME GUSTA */
#include "MeteorologistCharacter.hpp"
#include "ClimberCharacter.hpp"
#include "ArcheologistCharacter.hpp"
#include "ExplorerCharacter.hpp"
#include "WaterCarrierCharacter.hpp"
#include "NavigatorCharacter.hpp"

#include "StartMenu.hpp"
#include "fsmInit.h"
#include "Part.hpp"
#include "StormCard.hpp"
#include <algorithm>		//Used to shuffle the array of tiles
#include <array>

#define DEBUG_GAME
//#define DEBUG_INIT
//#define DEBUG_UPDATE_SCREEN

#define MODEBUTTONSNUMBER 7
#define TILEDECKNUMBER 25
#define MAXWATERS 5

enum ModeEnum { NORMAL, REMOVESAND, //Normal Use
	PEEPMODE, MOVEOTHER,			//Characters special
	DUNEBLASTERMODE, JETPACKMODE, TERRASCOPEMODE,	//Equipments
	OFFEREQUIPMENTMODE, REQUESTEQUIPMENTMODE, OFFERWATERMODE, REQUESTWATERMODE };

class Game {
public:
    //! Methods
	Game() = default;
	~Game();
	virtual bool eventHandler(ALL *allegro) = 0;
	bool getPlayAgain() { return playAgain; }
	bool getGameOver() { return gameOver; }

	//! Variables
    Container info;
    pos mouse{};
    bool redraw{};

    ModeEnum modeEnum;				//ModeEnum is used to know in wich mode I am... say.. have to move or remove sand
    int SandMarkersLeft{};
    int sandStormLevel{};
    //double volume;
    //Decks
    std::array< StormCard *, CANTOFSTORM> stormCardsDeck{};
    std::array<Tile *, TILEDECKNUMBER> tilesDeck{};		//Important! its a deck of cards seen as an array, we will use algorithm library to make it work.
    //int stormCardIndex;			//Indexes for the decks
    int equipIndex{};
    int peepOffset{};
    EquipmentsEnum equipOffset;
    int firstTimeUpdateScreenChooseEquipment{};		//ChooseEquipment Counter
    int firstTimeUpdateScreenPeep{};				//Peep Counter
    // Character Variables
    bool playAgain{};
    bool gameOver{};
    Character *character{};
    Character *character2{};
    //Parts
    Part *engine{};
    Part *solarCrystal{};
    Part *propeller{};
    Part *navigationDeck{};
    //BUTTONS
    Button *endTurnButton{};
    Button *modesButtons[MODEBUTTONSNUMBER]{};
    Button *watersButton[MAXWATERS]{};
    //Peep Buttons
    Button *nextButton{};
    Button *previousButton{};
    Button *selectPeepCard{};
    Button *selectEquipmentCard{};
    //VolumeButtons
    Button *muteButton{};
    Button *noMuteButton{};

	//Methods
	pos getSpot(int row, int column, ALL* allegro, ALLEGRO_BITMAP* image);		//Gets the pos of the image you must put giving the row and column where the item is.
    virtual void updateScreen(ALL* allegro) = 0;
    virtual void updateTilesButton(ALL* allegro) = 0;
	virtual void pickUpAction() = 0;
	//End Of Game
	virtual bool win(ALL *allegro) = 0;
	virtual bool lose(ALL *allegro) = 0;
	virtual void wannaPlay(ALL *allegro, char ID) = 0;
	//Mouse Methods
	virtual void click(ALL* allegro) = 0;
    virtual void clickOnPeepMode(ALL* allegro) = 0;
    virtual void clickOnJetPackMode(ALL* allegro) = 0;
    virtual void clickOnTerrascopeMode(ALL* allegro) = 0;
    virtual void clickOnRequestWaterMode(ALL*  allegro) = 0;
    virtual void clickOnOfferEquipmentMode(ALL* allegro) = 0;
    virtual void clickOnRequestEquipmentMode(ALL* allegro) = 0;
    virtual void clickOnOfferWaterMode(ALL* allegro) = 0;
    virtual void clickOnDefaultMode(ALL* allegro) = 0;
	void checkMouse(ALL* allegro);
	virtual void checkTilesButtons(double volume) = 0;
    virtual void clickTilesButtons(ALL *allegro) = 0;
    virtual void clickModesButtons(ALL* allegro) = 0;
	void clickEquipmentButtons(ALL* allegro);
	//Perform Action
	void performTileAction(TilesEnum tileEnum, int index, ALL* allegro);
	virtual void performEquipmentsAction(EquipmentsEnum equipEnum, ALL *allegro) = 0;
	//Tile Deck methods
	void swapTiles(int i, int j);		//Lets use swap from STL (algorithm)
	void shuffleTiles();		//Lets use shuffle from STL
	//Equipments methods
	virtual void drawEquipment(EquipmentsEnum equipEnum) = 0;

	//StormCards methods
	void performStormCardAction(ALL *allegro, StormCardsEnum cardEnum);
	virtual void drawStormCards(ALL *allegro) = 0;
	void swapStormCards(int i, int j);
	//Initialize Methods
	bool initializeAllegro(ALL* allegro);
	void initializeBoard(ALL* allegro);
	void initializeStormCards(ALL* allegro);
	void initializeCharacters(Container info, ALL* allegro);
	void initializeButtons(ALL* allegro);
	void initializeWaterButtons(ALL* allegro);
	void initializePeepButtons(ALL* allegro);
	void initializeEquipmentCardButton(ALL* allegro);
	//Keys Pressed
	virtual void keyUp(ALL *allegro) = 0;
    virtual void keyDown(ALL *allegro) = 0;
    virtual void keyLeft(ALL *allegro) = 0;
    virtual void keyRight(ALL *allegro) = 0;
    virtual void keyE(ALL* allegro) = 0;
    virtual void keyP(ALL *allegro) = 0;
    virtual void keyR(ALL *allegro) = 0;
    void keyEscape();
	//Peep Methods
	void nextPeepStormCard();
	void previousPeepStormCard();
	void resetPeepOffset() { peepOffset = 0; }
	//Offer equipment Methods
	void nextEquipCard();
	void previousEquipCard();
};

#endif
