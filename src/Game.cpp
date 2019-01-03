#include "Game.hpp"
#include <cstdlib>
#include <array>
//Used to shuffle
#include <random>
#include <chrono>
#include <include/Game.hpp>

#define DRAW_CARD_SLEEP 600000

pos Game::getSpot(int row, int column, ALL *allegro, ALLEGRO_BITMAP *image) {
    /* returns the position (in pixels) according to the row and column.
	it uses the size of the image to make it flexible to diferent sizes of bitmaps.
	allegro is needed to know the size of the screen
	*/
    int n = allegro->screenWidth / 3.0;
    int yoffset = (allegro->screenHeight - n) / 2.0;
    int xoffset = n;
    pos tempPos;
    int l = abs((n - al_get_bitmap_width(image) * 5) /
                4);    //If the number is negative it souldn't run enyway... but at least it won't explote.
    tempPos.x = (column) * (al_get_bitmap_width(image) + l) + xoffset;
    tempPos.y = (row) * (al_get_bitmap_width(image) + l) + yoffset;
    return tempPos;
}

//! Decks Swap/Shuffle Methods
void Game::shuffleTiles() {
    /* This function automatically shuffles the tile Deck*/
    uint seed = static_cast<uint>(chrono::system_clock::now().time_since_epoch().count());    //Magic, its to make the suffle work, something about using a counter to set the value.
    shuffle(tilesDeck.begin(), tilesDeck.end(), default_random_engine(seed));

    for (uint i = 0; i < tilesDeck.size(); i++) { //Here I set the storm in the middle
        if (tilesDeck[i]->gettileType() == STORM) { swapTiles(i, 12); }
    }
}
void Game::swapTiles(int i, int j) {
    /* swap tile i with j */
    swap(tilesDeck[i], tilesDeck[j]);
}
void Game::swapStormCards(int i, int j) {
    /* swap storm card i with storm card j */
    swap(stormCardsDeck[i], stormCardsDeck[j]);
}
//! Perform Actions
void Game::performTileAction(TilesEnum tileEnum, int index, ALL *allegro) {
    /* When the tile gets excavated, this function is runned for it to perform the action of this particular tile
	Thats why an tileEnum must be passed to tell the function which tile was turned up.
	*/
    switch (tileEnum) {
        case WATER:
            al_play_sample(allegro->waterSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            if (character->getCardIndex() == index) {
                character->addWater();
                character->addWater();
            }
            if (character2->getCardIndex() == index) {
                character2->addWater();
                character2->addWater();
            }
            break;
        case GEAR1:
            drawEquipment(info.equipment[equipIndex]);
            break;
        case GEAR2:
            drawEquipment(info.equipment[equipIndex]);
            break;
        case GEAR3:
            drawEquipment(info.equipment[equipIndex]);
            break;
        case GEAR4:
            drawEquipment(info.equipment[equipIndex]);
            break;
        case GEAR5:
            drawEquipment(info.equipment[equipIndex]);
            break;
        case GEAR6:
            drawEquipment(info.equipment[equipIndex]);
            break;
        case GEAR7:
            drawEquipment(info.equipment[equipIndex]);
            break;
        case GEAR8:
            drawEquipment(info.equipment[equipIndex]);
            break;
        case PROPELLERCOLUMN:
            propeller->findColum(index / 5, allegro);
            break;
        case PROPELLERROW:
            propeller->findRow(index % 5, allegro);
            break;
        case SOLARCRYSCOLUMN:
            solarCrystal->findColum(index / 5, allegro);
            break;
        case SOLARCRYSROW:
            solarCrystal->findRow(index % 5, allegro);
            break;
        case NAVIGATIONCOLUMN:
            navigationDeck->findColum(index / 5, allegro);
            break;
        case NAVIGATIONROW:
            navigationDeck->findRow(index % 5, allegro);
            break;
        case ENGINECOLUMN:
            engine->findColum(index / 5, allegro);
            break;
        case ENGINEROW:
            engine->findRow(index % 5, allegro);
            break;
        case TUNNEL:
            drawEquipment(info.equipment[equipIndex]);
            break;
    }
}

void Game::performStormCardAction(ALL *allegro, StormCardsEnum cardEnum) {
    /* Similar to performTileAction. Gets the enum of the storm card and acts accordingly. */
    int i;
    switch (cardEnum) {
        case RIGHT3:
            cout << "Right" << endl;
            i = 0;
            while (tilesDeck[i]->gettileType() != STORM) { i++; }
            if (i < 20) {
                if (character->getCardIndex() == i + 5) {
                    character->moveLeft();
                    character->resetNumberOfMoves();
                }
                if (character2->getCardIndex() == i + 5) {
                    character2->moveLeft();
                    character2->resetNumberOfMoves();
                }
                if (propeller->getCardIndex() == i + 5) { propeller->setCardIndex(i, allegro); }
                if (engine->getCardIndex() == i + 5) { engine->setCardIndex(i, allegro); }
                if (solarCrystal->getCardIndex() == i + 5) { solarCrystal->setCardIndex(i, allegro); }
                if (navigationDeck->getCardIndex() == i + 5) { navigationDeck->setCardIndex(i, allegro); }
                swapTiles(i, i + 5);
                tilesDeck[i]->increaseSand();
                SandMarkersLeft--;
            }
            updateScreen(allegro);
            stormCardsDeck[0]->showImage(allegro);
            usleep(DRAW_CARD_SLEEP);
        case RIGHT2:
            cout << "Right" << endl;
            i = 0;
            while (tilesDeck[i]->gettileType() != STORM) { i++; }
            if (i < 20) {
                if (character->getCardIndex() == i + 5) {
                    character->moveLeft();
                    character->resetNumberOfMoves();
                }
                if (character2->getCardIndex() == i + 5) {
                    character2->moveLeft();
                    character2->resetNumberOfMoves();
                }
                if (propeller->getCardIndex() == i + 5) { propeller->setCardIndex(i, allegro); }
                if (engine->getCardIndex() == i + 5) { engine->setCardIndex(i, allegro); }
                if (solarCrystal->getCardIndex() == i + 5) { solarCrystal->setCardIndex(i, allegro); }
                if (navigationDeck->getCardIndex() == i + 5) { navigationDeck->setCardIndex(i, allegro); }
                swapTiles(i,
                          i + 5);            //NO need to use break right now, I make the iteration by not puting break
                tilesDeck[i]->increaseSand();
                SandMarkersLeft--;
            }
            updateScreen(allegro);
            stormCardsDeck[0]->showImage(allegro);
            usleep(DRAW_CARD_SLEEP);
        case RIGHT1:
            cout << "Right" << endl;
            i = 0;
            while (tilesDeck[i]->gettileType() != STORM) { i++; }
            if (i < 20) {
                if (character->getCardIndex() == i + 5) {
                    character->moveLeft();
                    character->resetNumberOfMoves();
                }
                if (character2->getCardIndex() == i + 5) {
                    character2->moveLeft();
                    character2->resetNumberOfMoves();
                }
                if (propeller->getCardIndex() == i + 5) { propeller->setCardIndex(i, allegro); }
                if (engine->getCardIndex() == i + 5) { engine->setCardIndex(i, allegro); }
                if (solarCrystal->getCardIndex() == i + 5) { solarCrystal->setCardIndex(i, allegro); }
                if (navigationDeck->getCardIndex() == i + 5) { navigationDeck->setCardIndex(i, allegro); }
                swapTiles(i, i + 5);
                tilesDeck[i]->increaseSand();
                SandMarkersLeft--;
            }
            updateScreen(allegro);
            stormCardsDeck[0]->showImage(allegro);
            usleep(DRAW_CARD_SLEEP);
            break;
        case UP3:
            cout << "Up" << endl;
            i = 0;
            while (tilesDeck[i]->gettileType() != STORM) { i++; }
            if (i % 5 != 0) {
                if (character->getCardIndex() == i - 1) {
                    character->moveDown();
                    character->resetNumberOfMoves();
                }
                if (character2->getCardIndex() == i - 1) {
                    character2->moveDown();
                    character2->resetNumberOfMoves();
                }
                if (propeller->getCardIndex() == i - 1) { propeller->setCardIndex(i, allegro); }
                if (engine->getCardIndex() == i - 1) { engine->setCardIndex(i, allegro); }
                if (solarCrystal->getCardIndex() == i - 1) { solarCrystal->setCardIndex(i, allegro); }
                if (navigationDeck->getCardIndex() == i - 1) { navigationDeck->setCardIndex(i, allegro); }
                swapTiles(i, i - 1);
                tilesDeck[i]->increaseSand();
                SandMarkersLeft--;
            }
            updateScreen(allegro);
            stormCardsDeck[0]->showImage(allegro);
            usleep(DRAW_CARD_SLEEP);
        case UP2:
            cout << "Up" << endl;
            i = 0;
            while (tilesDeck[i]->gettileType() != STORM) { i++; }
            if (i % 5 != 0) {
                if (character->getCardIndex() == i - 1) {
                    character->moveDown();
                    character->resetNumberOfMoves();
                }
                if (character2->getCardIndex() == i - 1) {
                    character2->moveDown();
                    character2->resetNumberOfMoves();
                }
                if (propeller->getCardIndex() == i - 1) { propeller->setCardIndex(i, allegro); }
                if (engine->getCardIndex() == i - 1) { engine->setCardIndex(i, allegro); }
                if (solarCrystal->getCardIndex() == i - 1) { solarCrystal->setCardIndex(i, allegro); }
                if (navigationDeck->getCardIndex() == i - 1) { navigationDeck->setCardIndex(i, allegro); }
                swapTiles(i, i - 1);
                tilesDeck[i]->increaseSand();
                SandMarkersLeft--;
            }
            updateScreen(allegro);
            stormCardsDeck[0]->showImage(allegro);
            usleep(DRAW_CARD_SLEEP);
        case UP1:
            cout << "Up" << endl;
            i = 0;
            while (tilesDeck[i]->gettileType() != STORM) { i++; }        //Search for the storm
            if (i % 5 != 0) {
                if (character->getCardIndex() == i - 1) {
                    character->moveDown();
                    character->resetNumberOfMoves();
                }
                if (character2->getCardIndex() == i - 1) {
                    character2->moveDown();
                    character2->resetNumberOfMoves();
                }
                if (propeller->getCardIndex() == i - 1) { propeller->setCardIndex(i, allegro); }
                if (engine->getCardIndex() == i - 1) { engine->setCardIndex(i, allegro); }
                if (solarCrystal->getCardIndex() == i - 1) { solarCrystal->setCardIndex(i, allegro); }
                if (navigationDeck->getCardIndex() == i - 1) { navigationDeck->setCardIndex(i, allegro); }
                swapTiles(i, i - 1);
                tilesDeck[i]->increaseSand();
                SandMarkersLeft--;
            }
            updateScreen(allegro);
            stormCardsDeck[0]->showImage(allegro);
            usleep(DRAW_CARD_SLEEP);
            break;
        case LEFT3:
            cout << "Left" << endl;
            i = 0;
            while (tilesDeck[i]->gettileType() != STORM) { i++; }
            if (i > 4) {
                if (character->getCardIndex() == i - 5) {
                    character->moveRight();
                    character->resetNumberOfMoves();
                }
                if (character2->getCardIndex() == i - 5) {
                    character2->moveRight();
                    character2->resetNumberOfMoves();
                }
                if (propeller->getCardIndex() == i - 5) { propeller->setCardIndex(i, allegro); }
                if (engine->getCardIndex() == i - 5) { engine->setCardIndex(i, allegro); }
                if (solarCrystal->getCardIndex() == i - 5) { solarCrystal->setCardIndex(i, allegro); }
                if (navigationDeck->getCardIndex() == i - 5) { navigationDeck->setCardIndex(i, allegro); }
                swapTiles(i, i - 5);
                tilesDeck[i]->increaseSand();
                SandMarkersLeft--;
            }
            updateScreen(allegro);
            stormCardsDeck[0]->showImage(allegro);
            usleep(DRAW_CARD_SLEEP);
        case LEFT2:
            cout << "Left" << endl;
            i = 0;
            while (tilesDeck[i]->gettileType() != STORM) { i++; }
            if (i > 4) {
                if (character->getCardIndex() == i - 5) {
                    character->moveRight();
                    character->resetNumberOfMoves();
                }
                if (character2->getCardIndex() == i - 5) {
                    character2->moveRight();
                    character2->resetNumberOfMoves();
                }
                if (propeller->getCardIndex() == i - 5) { propeller->setCardIndex(i, allegro); }
                if (engine->getCardIndex() == i - 5) { engine->setCardIndex(i, allegro); }
                if (solarCrystal->getCardIndex() == i - 5) { solarCrystal->setCardIndex(i, allegro); }
                if (navigationDeck->getCardIndex() == i - 5) { navigationDeck->setCardIndex(i, allegro); }
                swapTiles(i, i - 5);
                tilesDeck[i]->increaseSand();
                SandMarkersLeft--;
            }
            updateScreen(allegro);
            stormCardsDeck[0]->showImage(allegro);
            usleep(DRAW_CARD_SLEEP);
        case LEFT1:
            cout << "Left" << endl;
            i = 0;
            while (tilesDeck[i]->gettileType() != STORM) { i++; }
            if (i > 4) {
                if (character->getCardIndex() == i - 5) {
                    character->moveRight();
                    character->resetNumberOfMoves();
                }
                if (character2->getCardIndex() == i - 5) {
                    character2->moveRight();
                    character2->resetNumberOfMoves();
                }
                if (propeller->getCardIndex() == i - 5) { propeller->setCardIndex(i, allegro); }
                if (engine->getCardIndex() == i - 5) { engine->setCardIndex(i, allegro); }
                if (solarCrystal->getCardIndex() == i - 5) { solarCrystal->setCardIndex(i, allegro); }
                if (navigationDeck->getCardIndex() == i - 5) { navigationDeck->setCardIndex(i, allegro); }
                swapTiles(i, i - 5);
                tilesDeck[i]->increaseSand();
                SandMarkersLeft--;
            }
            updateScreen(allegro);
            stormCardsDeck[0]->showImage(allegro);
            usleep(DRAW_CARD_SLEEP);
            break;
        case DOWN3:
            cout << "Down" << endl;
            i = 0;
            while (tilesDeck[i]->gettileType() != STORM) { i++; }
            if (i % 5 < 4) {
                if (character->getCardIndex() == i + 1) {
                    character->moveUp();
                    character->resetNumberOfMoves();
                }
                if (character2->getCardIndex() == i + 1) {
                    character2->moveUp();
                    character2->resetNumberOfMoves();
                }
                if (propeller->getCardIndex() == i + 1) { propeller->setCardIndex(i, allegro); }
                if (engine->getCardIndex() == i + 1) { engine->setCardIndex(i, allegro); }
                if (solarCrystal->getCardIndex() == i + 1) { solarCrystal->setCardIndex(i, allegro); }
                if (navigationDeck->getCardIndex() == i + 1) { navigationDeck->setCardIndex(i, allegro); }
                swapTiles(i, i + 1);
                tilesDeck[i]->increaseSand();
                SandMarkersLeft--;
            }
            updateScreen(allegro);
            stormCardsDeck[0]->showImage(allegro);
            usleep(DRAW_CARD_SLEEP);
        case DOWN2:
            cout << "Down" << endl;
            i = 0;
            while (tilesDeck[i]->gettileType() != STORM) { i++; }
            if (i % 5 < 4) {
                if (character->getCardIndex() == i + 1) {
                    character->moveUp();
                    character->resetNumberOfMoves();
                }
                if (character2->getCardIndex() == i + 1) {
                    character2->moveUp();
                    character2->resetNumberOfMoves();
                }
                if (propeller->getCardIndex() == i + 1) { propeller->setCardIndex(i, allegro); }
                if (engine->getCardIndex() == i + 1) { engine->setCardIndex(i, allegro); }
                if (solarCrystal->getCardIndex() == i + 1) { solarCrystal->setCardIndex(i, allegro); }
                if (navigationDeck->getCardIndex() == i + 1) { navigationDeck->setCardIndex(i, allegro); }
                swapTiles(i, i + 1);
                tilesDeck[i]->increaseSand();
                SandMarkersLeft--;
            }
            updateScreen(allegro);
            stormCardsDeck[0]->showImage(allegro);
            usleep(DRAW_CARD_SLEEP);
        case DOWN1:
            cout << "Down" << endl;
            i = 0;
            while (tilesDeck[i]->gettileType() != STORM) { i++; }
            if (i % 5 < 4) {
                if (character->getCardIndex() == i + 1) {
                    character->moveUp();
                    character->resetNumberOfMoves();
                }
                if (character2->getCardIndex() == i + 1) {
                    character2->moveUp();
                    character2->resetNumberOfMoves();
                }
                if (propeller->getCardIndex() == i + 1) { propeller->setCardIndex(i, allegro); }
                if (engine->getCardIndex() == i + 1) { engine->setCardIndex(i, allegro); }
                if (solarCrystal->getCardIndex() == i + 1) { solarCrystal->setCardIndex(i, allegro); }
                if (navigationDeck->getCardIndex() == i + 1) { navigationDeck->setCardIndex(i, allegro); }
                swapTiles(i, i + 1);
                tilesDeck[i]->increaseSand();
                SandMarkersLeft--;
            }
            updateScreen(allegro);
            stormCardsDeck[0]->showImage(allegro);
            usleep(DRAW_CARD_SLEEP);
            break;
        case SUNBEATSDOWN:
            cout << "Sun Beats Down" << endl;
            al_play_sample(allegro->sunBeatsDownSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            character->drinkWater();
            character2->drinkWater();
            if (character->shield) { //If player is being protected by the shield card or is inside a tunnel
                character->addWater();
                if (character->getColumn() == character2->getColumn() && character->getRow() == character2->getRow())
                    character2->addWater();
            } else if (tilesDeck[character->getCardIndex()]->gettileType() == TUNNEL &&
                       tilesDeck[character->getCardIndex()]->isTurned())
                character->addWater();
            if (character2->shield) {
                character2->addWater();
                if (character2->getRow() == character->getRow() && character2->getColumn() == character->getColumn())
                    character->addWater();
            } else if (tilesDeck[character2->getCardIndex()]->gettileType() == TUNNEL &&
                       tilesDeck[character2->getCardIndex()]->isTurned())
                character2->addWater();
            updateScreen(allegro);
            stormCardsDeck[0]->showImage(allegro);
            usleep(DRAW_CARD_SLEEP);
            break;
        case STORMUP:
            cout << "Storm Pick Up" << endl;
            al_play_sample(allegro->stormSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            sandStormLevel++;
            updateScreen(allegro);
            stormCardsDeck[0]->showImage(allegro);
            usleep(DRAW_CARD_SLEEP);
    };

    rotate(stormCardsDeck.begin(), stormCardsDeck.begin() + 1, stormCardsDeck.end());
}

//! Initialize Methods
void Game::initializeBoard(ALL *allegro) {
#ifdef DEBUG_INIT
    cout << "Initialize Board" << endl;
    cout << "Set Tiles" << endl;
#endif
    //Set Tiles
    for (uint i = 0; i < tilesDeck.size(); i++) {
        tilesDeck[i] = new Tile(info.tiles[i], allegro);
    }
/*#ifdef DEBUG_INIT
    cout << "Set santds on the tiles" << endl;
#endif*/
    //Set the sands on the tiles
    for (int i = 1; i < 12; i++) {
        if (i != 6 && i != 10 && i != 2) {
            tilesDeck[2 * i]->setSand(1);
        }
    }
/*#ifdef DEBUG_INIT
    cout << "Ser characters over crash" << endl;
#endif*/
    //Set Character over crash site
    for (uint i = 0; i < tilesDeck.size(); i++) {
        if (tilesDeck[i]->gettileType() == CRASHSITE) {
            character->setPos(i % 5, i / 5);
            character2->setPos(i % 5, i / 5);
            tilesDeck[i]->setTurned(true);        // TURN CRASHSITE TO MAKE IT NO POSIBLE TO LOSE A MOVEMENT DIGGING IT.
            i = tilesDeck.size();
        }
    }
/*#ifdef DEBUG_INIT
    cout << "Reset Moves" << endl;
#endif*/
    character->resetNumberOfMoves();    // This is done because "initializeBoard" decreses the character number of moves by one
    character2->resetNumberOfMoves();
/*#ifdef DEBUG_INIT
    cout << "Board done!" << endl;
#endif*/
}
bool Game::initializeAllegro(ALL *allegro) {
    bool state = true;
#ifndef WINDOWED
    ALLEGRO_DISPLAY_MODE disp_data;
    al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
    al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    allegro->display = al_create_display(disp_data.width, disp_data.height);
    allegro->screenWidth = disp_data.width;
    allegro->screenHeight = disp_data.height;
#endif
#ifdef WINDOWED
    if (state) {
        state = false;
        if (allegro->display = al_create_display(SCREEN_X, SCREEN_Y)) {
            allegro->screenWidth = SCREEN_X;
            allegro->screenHeight = SCREEN_Y;
            //if (allegro->startMenuDisplay != NULL) { al_destroy_display(allegro->startMenuDisplay); } //Just in case
            al_register_event_source(allegro->events_queue, al_get_display_event_source(allegro->display));
            state = true;
        } else { cout << "Failed to create Display" << endl; }
    }
#endif
    //Load Images
    if (state) {
        state = false;
        al_set_target_backbuffer(allegro->display);
        if (allegro->fondo = al_load_bitmap_resized("Resources/PlayingBack.jpg", allegro)) {
            if (allegro->mouse = al_load_bitmap_resized("Resources/mouse.png", allegro)) {
                state = true;
            } else { cout << "Failed to load mouse.png" << endl; }
        } else { cout << "Failed to load scenario.png" << endl; }
    }

    //Audio
    al_stop_sample(&allegro->backgroundMusicID);        //Stop Current Song and play the new one
    allegro->backgroundMusic = al_load_sample("Resources/Sounds/desert.wav");
    if (allegro->backgroundMusic != NULL) {
        al_play_sample(allegro->backgroundMusic, allegro->volume / 2, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP,
                       &allegro->backgroundMusicID);
    } else { cerr << "Couldn't load desert.wav sample" << endl; }
    al_play_sample(allegro->startMusic, allegro->volume / 2, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
    //mouse
    if (allegro->mouse = al_load_bitmap_resized("Resources/mouse.png", allegro)) {
        if (allegro->cursor = al_create_mouse_cursor(allegro->mouse, 0, 0)) {
            al_set_mouse_cursor(allegro->display, allegro->cursor);
        }
    }
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(allegro->fondo, 0, 0, 0);
    al_flip_display();

    return state;
}
void Game::initializeStormCards(ALL *allegro) {
#ifdef DEBUG_INIT
    cout << "Initialize Storm Cards" << endl;
#endif
    // create storm cards Deck
    for (uint i = 0; i < stormCardsDeck.size(); i++) {
            stormCardsDeck[i] = new StormCard(info.storm[i], allegro);
    }
}
void Game::initializeCharacters(Container info, ALL *allegro) {
#ifdef DEBUG_INIT
    cout << "Initialize Characters" << endl;
#endif
    //Initailize both characters
    switch (info.myRol) {
        case ARCHEOLOGIST:
            character = new ArcheologistCharacter(0, 0, info, allegro);
            character->setName(info.name);
            break;
        case CLIMBER:
            character = new ClimberCharacter(0, 0, info, allegro);
            character->setName(info.name);
            break;
        case EXPLORER:
            character = new ExplorerCharacter(0, 0, info, allegro);
            character->setName(info.name);
            break;
        case METEOROLOGIST:
            character = new MeteorologistCharacter(0, 0, info, allegro);
            character->setName(info.name);
            break;
        case NAVIGATOR:
            character = new NavigatorCharacter(0, 0, info, allegro);
            character->setName(info.name);
            break;
        case WATER_CARRIER:
            character = new WaterCarrierCharacter(0, 0, info, allegro);
            character->setName(info.name);
            break;
    }
    switch (info.friendRol) {
        case ARCHEOLOGIST:
            character2 = new ArcheologistCharacter(0, 0, info, allegro);
            character2->setName(info.friendName);
            break;
        case CLIMBER:
            character2 = new ClimberCharacter(0, 0, info, allegro);
            character2->setName(info.friendName);
            break;
        case EXPLORER:
            character2 = new ExplorerCharacter(0, 0, info, allegro);
            character2->setName(info.friendName);
            break;
        case METEOROLOGIST:
            character2 = new MeteorologistCharacter(0, 0, info, allegro);
            character2->setName(info.friendName);
            break;
        case NAVIGATOR:
            character2 = new NavigatorCharacter(0, 0, info, allegro);
            character2->setName(info.friendName);
            break;
        case WATER_CARRIER:
            character2 = new WaterCarrierCharacter(0, 0, info, allegro);
            character2->setName(info.friendName);
            break;
    }
}
void Game::initializeButtons(ALL *allegro) {
#ifdef DEBUG_INIT
    cout << "Initialize Buttons" << endl;
#endif
    modesButtons[REMOVEBUTTON] = new Button(allegro->screenWidth * 0.68, allegro->screenHeight * 0.1037,
                                            "Resources/Buttons/Actions/noMouseRemoveSand.png",
                                            "Resources/Buttons/Actions/mouseRemoveSand.png", allegro);
    modesButtons[EXCAVATEBUTTON] = new Button(allegro->screenWidth * 0.68, allegro->screenHeight * 0.1343,
                                              "Resources/Buttons/Actions/noMouseExcavate.png",
                                              "Resources/Buttons/Actions/mouseExcavate.png", allegro);
    modesButtons[PICKUPBUTTON] = new Button(allegro->screenWidth * 0.68, allegro->screenHeight / 6,
                                            "Resources/Buttons/Actions/noMousePickUpPart.png",
                                            "Resources/Buttons/Actions/mousePickUpPart.png", allegro);
    modesButtons[OFFERWBUTTON] = new Button(allegro->screenWidth * 0.68, allegro->screenHeight * 0.1981,
                                            "Resources/Buttons/Actions/noMouseOfferWater.png",
                                            "Resources/Buttons/Actions/mouseOfferWater.png", allegro);
    modesButtons[OFFEREBUTTON] = new Button(allegro->screenWidth * 0.68, allegro->screenHeight * 0.2324,
                                            "Resources/Buttons/Actions/noMouseOfferEquipment.png",
                                            "Resources/Buttons/Actions/mouseOfferEquipment.png", allegro);
    modesButtons[REQUESTWBUTTON] = new Button(allegro->screenWidth * 0.68, allegro->screenHeight * 0.2639,
                                              "Resources/Buttons/Actions/noMouseRequestWater.png",
                                              "Resources/Buttons/Actions/mouseRequestWater.png", allegro);
    modesButtons[REQUESTEBUTTON] = new Button(allegro->screenWidth * 0.68, allegro->screenHeight * 0.2972,
                                              "Resources/Buttons/Actions/noMouseRequestEquipment.png",
                                              "Resources/Buttons/Actions/mouseRequestEquipment.png", allegro);
    endTurnButton = new Button(allegro->screenWidth - al_get_bitmap_width(
            al_load_bitmap_resized("Resources/Buttons/noMouseEndTurnButton.png", allegro)) * 5 / 4,
                               al_get_bitmap_height(
                                       al_load_bitmap_resized("Resources/Buttons/noMouseEndTurnButton.png", allegro)) *
                               3 / 2, "Resources/Buttons/noMouseEndTurnButton.png",
                               "Resources/Buttons/mouseEndTurnButton.png", allegro);
    muteButton = new Button(allegro->screenWidth / 4, al_get_bitmap_height(
            al_load_bitmap_resized("Resources/Buttons/noMouseEndTurnButton.png", allegro)) * 3 / 2,
                            "Resources/Buttons/muteButton.png", "Resources/Buttons/muteButton.png", allegro);
    noMuteButton = new Button(allegro->screenWidth / 4, al_get_bitmap_height(
            al_load_bitmap_resized("Resources/Buttons/noMouseEndTurnButton.png", allegro)) * 3 / 2,
                              "Resources/Buttons/noMuteButton.png", "Resources/Buttons/noMuteButton.png", allegro);
    // Special Buttons
    if (ALLEGRO_BITMAP *tempArrowImage = al_load_bitmap_resized("Resources/Buttons/Peep/nextPeepButton.png", allegro)) {
        previousButton = new Button(allegro->screenWidth / 2 - 300, allegro->screenHeight / 2,
                                    "Resources/Buttons/Peep/previousPeepButton.png",
                                    "Resources/Buttons/Peep/previousPeepButton.png", allegro);
        nextButton = new Button(allegro->screenWidth / 2 + 300 - al_get_bitmap_width(tempArrowImage),
                                allegro->screenHeight / 2, "Resources/Buttons/Peep/nextPeepButton.png",
                                "Resources/Buttons/Peep/nextPeepButton.png", allegro);
    } else { std::cout << "Failed to arrow image" << endl; }
    initializeWaterButtons(allegro);
    initializeEquipmentCardButton(allegro);
    if (MeteorologistCharacter *meteorologistCharacter = dynamic_cast<MeteorologistCharacter *>(character)) {
        initializePeepButtons(allegro);
    }
}
void Game::initializeWaterButtons(ALL *allegro) {
    // Water Buttons
    if (ALLEGRO_BITMAP *offsetImage = al_load_bitmap_resized("Resources/Buttons/Waters/1waterButtonImage.png",
                                                             allegro)) {
        watersButton[2] = new Button(allegro->screenWidth * 0.5 - al_get_bitmap_width(offsetImage) / 2,
                                     allegro->screenHeight * 0.5 - al_get_bitmap_height(offsetImage) / 2,
                                     "Resources/Buttons/Waters/3watersButtonImage.png",
                                     "Resources/Buttons/Waters/3watersButtonSelectedImage.png", allegro);
        watersButton[1] = new Button(watersButton[2]->getInitialPos().x,
                                     watersButton[2]->getInitialPos().y - 10 - al_get_bitmap_height(offsetImage),
                                     "Resources/Buttons/Waters/2watersButtonImage.png",
                                     "Resources/Buttons/Waters/2watersButtonSelectedImage.png", allegro);
        watersButton[0] = new Button(watersButton[2]->getInitialPos().x,
                                     watersButton[1]->getInitialPos().y - 10 - al_get_bitmap_height(offsetImage),
                                     "Resources/Buttons/Waters/1waterButtonImage.png",
                                     "Resources/Buttons/Waters/1waterButtonSelectedImage.png", allegro);
        watersButton[3] = new Button(watersButton[2]->getInitialPos().x,
                                     watersButton[2]->getInitialPos().y + 10 + al_get_bitmap_height(offsetImage),
                                     "Resources/Buttons/Waters/4watersButtonImage.png",
                                     "Resources/Buttons/Waters/4watersButtonSelectedImage.png", allegro);
        watersButton[4] = new Button(watersButton[2]->getInitialPos().x,
                                     watersButton[3]->getInitialPos().y + 10 + al_get_bitmap_height(offsetImage),
                                     "Resources/Buttons/Waters/5watersButtonImage.png",
                                     "Resources/Buttons/Waters/5watersButtonSelectedImage.png", allegro);
    } else { cout << "Couldn't load offset image" << endl; }
}
void Game::initializePeepButtons(ALL *allegro) {
    if (ALLEGRO_BITMAP *tempButtonImage = al_load_bitmap_resized("Resources/Buttons/Peep/putToBottomPeepImage.png",
                                                                 allegro)) {
        firstTimeUpdateScreenPeep = 0;
        selectPeepCard = new Button((allegro->screenWidth - al_get_bitmap_width(tempButtonImage)) / 2,
                                    allegro->screenHeight / 2 + 300, "Resources/Buttons/Peep/putToBottomPeepImage.png",
                                    "Resources/Buttons/Peep/putToBottomPeepImage.png", allegro);
    } else { std::cout << "Failed to load Peep Button Select" << endl; }
}
void Game::initializeEquipmentCardButton(ALL *allegro) {
    if (ALLEGRO_BITMAP *tempButtonImage = al_load_bitmap_resized("Resources/Buttons/Peep/SelectImage.png", allegro)) {
        firstTimeUpdateScreenChooseEquipment = 0;
        selectEquipmentCard = new Button((allegro->screenWidth - al_get_bitmap_width(tempButtonImage)) / 2,
                                         allegro->screenHeight / 2 + 300, "Resources/Buttons/Peep/SelectImage.png",
                                         "Resources/Buttons/Peep/mouseOverSelectImage.png", allegro);
    } else { std::cout << "Failed to load Peep Button Select" << endl; }
}
//! Key Methods
void Game::keyEscape() {
    // TODO: add more
    switch (modeEnum) {
        case DUNEBLASTERMODE:
            character->increaseDuneBlaster();
            break;
        case JETPACKMODE:
            character->increaseJetPack();
            break;
        case TERRASCOPEMODE:
            character->increaseTerrascope();
            break;
        case MOVEOTHER:
            if(NavigatorCharacter *navigatorCharacter = dynamic_cast<NavigatorCharacter *>(character)) {
                if (navigatorCharacter->usedPartnerMoves()) {
                    character2->resetNumberOfMoves();
                    navigatorCharacter->decreseMoves();
                }
            } else { cout << "Error: move other not being a Navigator" << endl; }
    }
    modeEnum = NORMAL;
}
//! Mouse Methods

void Game::checkMouse(ALL *allegro) {
    switch (modeEnum) {
        case PEEPMODE:
            nextButton->checkMouse(mouse.x, mouse.y, allegro->volume);
            previousButton->checkMouse(mouse.x, mouse.y, allegro->volume);
            selectPeepCard->checkMouse(mouse.x, mouse.y, allegro->volume);
            break;
        case JETPACKMODE:    //This should be inside tileCheckMouse directamente
            for (int i = 0; i < TILEDECKNUMBER; i++) {
                bool climber = false;
                if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character)) { climber = true; }
                if (tilesDeck[i]->canIMove(climber))
                    tilesDeck[i]->checkButton(mouse.x, mouse.y, allegro->volume);
            }
            break;
        case TERRASCOPEMODE:
            for (int i = 0; i < TILEDECKNUMBER; i++) {
                if (!tilesDeck[i]->isTurned())
                    tilesDeck[i]->checkButton(mouse.x, mouse.y, allegro->volume);
            }
            break;
        case OFFEREQUIPMENTMODE:
        case REQUESTEQUIPMENTMODE:
            nextButton->checkMouse(mouse.x, mouse.y, allegro->volume);
            previousButton->checkMouse(mouse.x, mouse.y, allegro->volume);
            selectEquipmentCard->checkMouse(mouse.x, mouse.y, allegro->volume);
            break;
        case OFFERWATERMODE:
        case REQUESTWATERMODE:
            for (int i = 0; i < min(character->getWaters(), MAXWATERS - character2->getWaters() - 1); i++) {
                watersButton[i]->checkMouse(mouse.x, mouse.y, allegro->volume);
            }
            break;
        default:
            endTurnButton->checkMouse(mouse.x, mouse.y, allegro->volume);
            character->checkButton(SECRETWATERRESERVE, mouse.x, mouse.y, allegro->volume);
            for (int i = 0; i < MODEBUTTONSNUMBER; i++) {
                modesButtons[i]->checkMouse(mouse.x, mouse.y, allegro->volume);
            }
            checkTilesButtons(allegro->volume);
            //allegro->volume Buttons
            if (allegro->volume == 0) { muteButton->checkMouse(mouse.x, mouse.y, allegro->volume); }
            else { noMuteButton->checkMouse(mouse.x, mouse.y, allegro->volume); }
            //Character special Buttons
            if (MeteorologistCharacter *meteorologistCharacter = dynamic_cast<MeteorologistCharacter *>(character)) {
                meteorologistCharacter->checkMouseOverButton(mouse.x, mouse.y, allegro->volume);
            } else if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character)) {    //I do a else if for the program to work faster
                climberCharacter->checkMouseOverButton(mouse.x, mouse.y, allegro->volume);
            } else if (WaterCarrierCharacter *waterCarrierCharacter = dynamic_cast<WaterCarrierCharacter *>(character)) {
                if (tilesDeck[character->getCardIndex()]->gettileType() == WATER &&
                    tilesDeck[character->getCardIndex()]->isTurned()) {
                    waterCarrierCharacter->checkMouseOverButton(mouse.x, mouse.y, allegro->volume);
                }
            } else if (NavigatorCharacter *navigatorCharacter = dynamic_cast<NavigatorCharacter *>(character)) {
                navigatorCharacter->checkMouseOverButton(mouse.x, mouse.y, modeEnum == MOVEOTHER, allegro->volume);
            }
    }
}
void Game::clickEquipmentButtons(ALL *allegro) {
    if (character->checkButton(DUNEBLAST, mouse.x, mouse.y, allegro->volume)) {
        cout << " Secret Dune Blaster Button Clicked" << endl;
        al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        if (character->haveDuneBlaster())
            performEquipmentsAction(DUNEBLAST, allegro);
    }
    if (character->checkButton(SECRETWATERRESERVE, mouse.x, mouse.y, allegro->volume)) {
        cout << " Secret Water Reserve Button Clicked" << endl;
        al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        if (character->haveWaterReserve())
            performEquipmentsAction(SECRETWATERRESERVE, allegro);
    }
    if (character->checkButton(JETPACK, mouse.x, mouse.y, allegro->volume)) {
        cout << " Secret JetPack Button Clicked" << endl;
        al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        if (character->haveJetPack())
            performEquipmentsAction(JETPACK, allegro);
    }
    if (character->checkButton(SOLARSHIELD, mouse.x, mouse.y, allegro->volume)) {
        cout << "Solar Shield Button Clicked" << endl;
        al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        if (character->haveSolarShield())
            performEquipmentsAction(SOLARSHIELD, allegro);
    }
    if (character->checkButton(TERRASCOPE, mouse.x, mouse.y, allegro->volume)) {
        cout << "Terrascope Button clicked" << endl;
        al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        if (character->haveTerrascope())
            performEquipmentsAction(TERRASCOPE, allegro);
    }
    if (character->checkButton(TIMETHROTTLE, mouse.x, mouse.y, allegro->volume)) {
        cout << "Time Throttle Button Clicked" << endl;
        al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        if (character->haveTimeThrottle())
            performEquipmentsAction(TIMETHROTTLE, allegro);
    }
}
//! Next and Previous Methods
void Game::nextEquipCard() {
    Character *tempCharacter;
    if (modeEnum == OFFEREQUIPMENTMODE) tempCharacter = character;
    else if (modeEnum == REQUESTEQUIPMENTMODE) tempCharacter = character2;
    switch (equipOffset) {
        case DUNEBLAST:
            if (tempCharacter->haveJetPack()) { equipOffset = JETPACK; }
            else if (tempCharacter->haveSolarShield()) { equipOffset = SOLARSHIELD; }
            else if (tempCharacter->haveTerrascope()) { equipOffset = TERRASCOPE; }
            else if (tempCharacter->haveWaterReserve()) { equipOffset = SECRETWATERRESERVE; }
            else if (tempCharacter->haveTimeThrottle()) { equipOffset = TIMETHROTTLE; }
            break;
        case JETPACK:
            if (tempCharacter->haveSolarShield()) { equipOffset = SOLARSHIELD; }
            else if (tempCharacter->haveTerrascope()) { equipOffset = TERRASCOPE; }
            else if (tempCharacter->haveWaterReserve()) { equipOffset = SECRETWATERRESERVE; }
            else if (tempCharacter->haveTimeThrottle()) { equipOffset = TIMETHROTTLE; }
            else if (tempCharacter->haveDuneBlaster()) { equipOffset = DUNEBLAST; }
            break;
        case SOLARSHIELD:
            if (tempCharacter->haveTerrascope()) { equipOffset = TERRASCOPE; }
            else if (tempCharacter->haveWaterReserve()) { equipOffset = SECRETWATERRESERVE; }
            else if (tempCharacter->haveTimeThrottle()) { equipOffset = TIMETHROTTLE; }
            else if (tempCharacter->haveDuneBlaster()) { equipOffset = DUNEBLAST; }
            else if (tempCharacter->haveJetPack()) { equipOffset = JETPACK; }
            break;
        case TERRASCOPE:
            if (tempCharacter->haveWaterReserve()) { equipOffset = SECRETWATERRESERVE; }
            else if (tempCharacter->haveTimeThrottle()) { equipOffset = TIMETHROTTLE; }
            else if (tempCharacter->haveDuneBlaster()) { equipOffset = DUNEBLAST; }
            else if (tempCharacter->haveJetPack()) { equipOffset = JETPACK; }
            else if (tempCharacter->haveSolarShield()) { equipOffset = SOLARSHIELD; }
            break;
        case SECRETWATERRESERVE:
            if (tempCharacter->haveTimeThrottle()) { equipOffset = TIMETHROTTLE; }
            else if (tempCharacter->haveDuneBlaster()) { equipOffset = DUNEBLAST; }
            else if (tempCharacter->haveJetPack()) { equipOffset = JETPACK; }
            else if (tempCharacter->haveSolarShield()) { equipOffset = SOLARSHIELD; }
            else if (tempCharacter->haveTerrascope()) { equipOffset = TERRASCOPE; }
            break;
        case TIMETHROTTLE:
            if (tempCharacter->haveDuneBlaster()) { equipOffset = DUNEBLAST; }
            else if (tempCharacter->haveJetPack()) { equipOffset = JETPACK; }
            else if (tempCharacter->haveSolarShield()) { equipOffset = SOLARSHIELD; }
            else if (tempCharacter->haveTerrascope()) { equipOffset = TERRASCOPE; }
            else if (tempCharacter->haveWaterReserve()) { equipOffset = SECRETWATERRESERVE; }
            break;
    }
}
void Game::previousEquipCard() {
    Character *tempCharacter;
    if (modeEnum == OFFEREQUIPMENTMODE) tempCharacter = character;
    else if (modeEnum == REQUESTEQUIPMENTMODE) tempCharacter = character2;
    switch (equipOffset) {
        case DUNEBLAST:
            if (tempCharacter->haveTimeThrottle()) { equipOffset = TIMETHROTTLE; }
            else if (tempCharacter->haveWaterReserve()) { equipOffset = SECRETWATERRESERVE; }
            else if (tempCharacter->haveTerrascope()) { equipOffset = TERRASCOPE; }
            else if (tempCharacter->haveSolarShield()) { equipOffset = SOLARSHIELD; }
            else if (tempCharacter->haveJetPack()) { equipOffset = JETPACK; }
            break;
        case JETPACK:
            if (tempCharacter->haveDuneBlaster()) { equipOffset = DUNEBLAST; }
            else if (tempCharacter->haveTimeThrottle()) { equipOffset = TIMETHROTTLE; }
            else if (tempCharacter->haveWaterReserve()) { equipOffset = SECRETWATERRESERVE; }
            else if (tempCharacter->haveTerrascope()) { equipOffset = TERRASCOPE; }
            else if (tempCharacter->haveSolarShield()) { equipOffset = SOLARSHIELD; }
            break;
        case SOLARSHIELD:
            if (tempCharacter->haveJetPack()) { equipOffset = JETPACK; }
            else if (tempCharacter->haveDuneBlaster()) { equipOffset = DUNEBLAST; }
            else if (tempCharacter->haveTimeThrottle()) { equipOffset = TIMETHROTTLE; }
            else if (tempCharacter->haveWaterReserve()) { equipOffset = SECRETWATERRESERVE; }
            else if (tempCharacter->haveTerrascope()) { equipOffset = TERRASCOPE; }
            break;
        case TERRASCOPE:
            if (tempCharacter->haveSolarShield()) { equipOffset = SOLARSHIELD; }
            else if (tempCharacter->haveJetPack()) { equipOffset = JETPACK; }
            else if (tempCharacter->haveDuneBlaster()) { equipOffset = DUNEBLAST; }
            else if (tempCharacter->haveTimeThrottle()) { equipOffset = TIMETHROTTLE; }
            else if (tempCharacter->haveWaterReserve()) { equipOffset = SECRETWATERRESERVE; }
            break;
        case SECRETWATERRESERVE:
            if (tempCharacter->haveTerrascope()) { equipOffset = TERRASCOPE; }
            else if (tempCharacter->haveSolarShield()) { equipOffset = SOLARSHIELD; }
            else if (tempCharacter->haveJetPack()) { equipOffset = JETPACK; }
            else if (tempCharacter->haveDuneBlaster()) { equipOffset = DUNEBLAST; }
            else if (tempCharacter->haveTimeThrottle()) { equipOffset = TIMETHROTTLE; }
            break;
        case TIMETHROTTLE:
            if (tempCharacter->haveWaterReserve()) { equipOffset = SECRETWATERRESERVE; }
            else if (tempCharacter->haveTerrascope()) { equipOffset = TERRASCOPE; }
            else if (tempCharacter->haveSolarShield()) { equipOffset = SOLARSHIELD; }
            else if (tempCharacter->haveJetPack()) { equipOffset = JETPACK; }
            else if (tempCharacter->haveDuneBlaster()) { equipOffset = DUNEBLAST; }
            break;
    }
}
void Game::nextPeepStormCard() {
    if (peepOffset < sandStormLevel - 1) { peepOffset++; }
    else { peepOffset = 0; }
}
void Game::previousPeepStormCard() {
    if (peepOffset != 0) { peepOffset--; }
    else { peepOffset = sandStormLevel - 1; }
}

Game::~Game() {
    delete character;
    delete character2;
    for (int i = 0; i < CANTOFSTORM; i++) { delete stormCardsDeck[i]; }
    for (int i = 0; i < TILEDECKNUMBER; i++) { delete tilesDeck[i]; }
    for (int i = 0; i < MODEBUTTONSNUMBER; i++) { delete modesButtons[i]; }
    for (int i = 0; i < MAXWATERS; i++) { delete watersButton[i]; }
    delete endTurnButton;
    delete nextButton;
    delete previousButton;
    delete selectEquipmentCard;
    delete engine;
    delete solarCrystal;
    delete propeller;
    delete navigationDeck;
    if (system("CLS")) system("clear");
}
