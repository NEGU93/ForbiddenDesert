#include "Game.hpp"
#include <cstdlib>
#include <array>
//Used to shuffle
#include <random>
#include <chrono>

#define TIME_TERRASCOPE 3000

Game::Game(ALL *allegro, FSMI *gameNetwork) {
    this->gameNetwork = gameNetwork;
    //allegro->volume = 1;
    if (initializeAllegro(allegro)) {
        //Conection
        Container temp = this->gameNetwork->getInfo();
        if (gameNetwork->getInfo().myTurn == I_START)
            turn = true;
        else turn = false;
        gamePack.connection = gameNetwork->net.connection;    //Guardo conexion en el paquete actual
        //Characters
        initializeCharacters(temp, allegro);
        //Buttons
        initializeButtons(allegro);
        //Cards and Decks
        initializeBoard(allegro);
        initializeStormCards(allegro);
        //Parts
        engine = new Part("Resources/Parts/engineImage.png");
        solarCrystal = new Part("Resources/Parts/solarCrystalImage.png");
        propeller = new Part("Resources/Parts/propellerImage.png");
        navigationDeck = new Part("Resources/Parts/navegationDeckImage.png");
        //Indexes
        equipIndex = 0;            //TODO: eliminar esto en un futuro
        peepOffset = 0;
        //Data
        SandMarkersLeft = 40;
        sandStormLevel = 3; //TODO: CUAL ERA EL NUMERO REAL? (era 3 jaja igual no cuesta NADA dejar a el jugador elegir la dificultad y queda mas copado)

        mouse.x = allegro->screenWidth / 2.0;
        mouse.y = allegro->screenHeight / 2.0;
        redraw = true;
        gameOver = false;
        playAgain = false;

        modeEnum = NORMAL;
    } else { cout << "Couldn't initialize allegro" << endl; }
}
/*-------------------------------------------------------------------------------------------------------------------
* eventHandler:																										*
*			Core of the class, waits for events from keyboard, mouse, etc. Uses allegro events and acts acordingly.	*
* Practically the only function available (public) of the class.													*
*	Recomended Use:																									*
//Game																												*
stillPlaying = true;																								*
Game game = Game(allegro, &gameNetwork);																			*
while (stillPlaying) {																								*
	stillPlaying = game.eventHandler(allegro);																		*
}																													*
--------------------------------------------------------------------------------------------------------------------*/
bool Game::eventHandler(ALL *allegro) {
    bool stillPlaying = true;
    ALLEGRO_EVENT ev;
    al_wait_for_event(allegro->events_queue, &ev);

    if (ev.type == ALLEGRO_EVENT_TIMER) { redraw = true; }
    else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        gamePack.quit();
        gamePack.sendPack();
        gamePack.waitForAck(allegro);
        stillPlaying = false;
    }
    else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
        mouse.x = ev.mouse.x;
        mouse.y = ev.mouse.y;
        checkMouse(allegro);
    }
    else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        if (ev.mouse.button == SECONDARY_BUTTON) { keyEscape(); }
        else click(allegro);
    }
    else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
        if (turn) { //Check if is my turn tu play... else... do nothing... just wait ^^
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_ESCAPE: //In case you want to cancel any mode you've played before.
                    keyEscape();
                    modeEnum = NORMAL;
                    break;
                case ALLEGRO_KEY_UP:
                    keyUp(allegro);
                    if (modeEnum != MOVEOTHER) { modeEnum = NORMAL; }
                    break;
                case ALLEGRO_KEY_DOWN:
                    keyDown(allegro);
                    if (modeEnum != MOVEOTHER) { modeEnum = NORMAL; }
                    break;
                case ALLEGRO_KEY_LEFT:
                    keyLeft(allegro);
                    if (modeEnum == REMOVESAND) { modeEnum = NORMAL; }
                    break;
                case ALLEGRO_KEY_RIGHT:
                    keyRight(allegro);
                    if (modeEnum == REMOVESAND) { modeEnum = NORMAL; }
                    break;
                case ALLEGRO_KEY_E:
                    keyE(allegro);
                    modeEnum = NORMAL;
                    break;
                case ALLEGRO_KEY_P:
                    keyP(allegro);
                    modeEnum = NORMAL;
                    break;
                case ALLEGRO_KEY_R:
                    keyR(allegro);
                    if (ArcheologistCharacter *explorerCharacter = dynamic_cast<ArcheologistCharacter *>(character)) {
                        if (modeEnum == REMOVESAND) {
                            if (tilesDeck[character->getCardIndex()]->haveSand()) {
                                tilesDeck[character->getCardIndex()]->removeSand();
                            }
                        }
                    }
                    if (modeEnum == NORMAL) { modeEnum = REMOVESAND; }
                    else { modeEnum = NORMAL; }
                    break;
                case ALLEGRO_KEY_W:
                    if (modeEnum == NORMAL &&
                        character2->getWaters() < MAXWATERS) {    //TODO: cambiar cuando haga el water carrier
                        modeEnum = OFFERWATERMODE;
                    } else { modeEnum = NORMAL; }
                    break;
                case ALLEGRO_KEY_O:
                    if (modeEnum == NORMAL) {
                        if (character->getCardIndex() == character2->getCardIndex()) {
                            if (character->haveDuneBlaster() || character->haveJetPack() ||
                                character->haveSolarShield() || character->haveTerrascope() ||
                                character->haveTimeThrottle()) {
                                modeEnum = OFFEREQUIPMENTMODE;
                                firstTimeUpdateScreenChooseEquipment = 0;
                            }
                        }
                    } else { modeEnum = NORMAL; }
                    break;
                default:
                    break;
            }
        }
    }
    //this 'if' checks if the other player I'm moving has already moved 3 spaces... if so... get out of the move other mode
    if (modeEnum == MOVEOTHER) {
        if (NavigatorCharacter *navigatorCharacter = dynamic_cast<NavigatorCharacter *>(character)) {
            if (!navigatorCharacter->havePartnersMovesLeft()) {
                modeEnum = NORMAL;
                character2->resetNumberOfMoves();
                navigatorCharacter->decreseMoves();
            }
        } else { cout << "Error: was in moveother mode without being a Navigator" << endl; }
    }
    //checks if win or lose
    if (redraw && al_is_event_queue_empty(allegro->events_queue)) {
        if (turn) {
            if (lose(allegro)) { stillPlaying = false; }
            else if (win(allegro)) { stillPlaying = false; }
        } else { stillPlaying = receiveHandler(allegro); }        //Handler of polonet
        updateScreen(allegro);
    }
    if (!turn) { stillPlaying = receiveHandler(allegro); } //Redundance make it more difficult to lose a command.
    return stillPlaying;
}
/*reciveHandler takes care of the connecion part. While in recibe handler it will check if there was an package sent by polonet and act acordingly.
*		Similar to eventHandler. But takes care of polonet insted of allegro. Between both functions they take care of all the events in the game class.
*/
bool Game::receiveHandler(ALL *allegro) {
    int backup;
    Container temp;
    do {
        gamePack.cleanBuffer();
        temp = gamePack.receivePack();
        //gamePack.cleanBuffer();
        switch (temp.ID) {
            case MOVE://ANDA BIEN
                //cout << "move" << endl;
                character2->setPos(temp.Slot1, temp.Slot2);
                gamePack.ack();
                gamePack.sendPack();
                return true;
            case REMOVE://ANDA BIEN
                cout << "remove" << endl;
                if (character2->removeSand()) {
                    if (tilesDeck[5 * temp.Slot2 + temp.Slot1]->haveSand()) {
                        tilesDeck[5 * temp.Slot2 + temp.Slot1]->removeSand();
                        SandMarkersLeft++;
                        cout << "Sand Marker ++" << endl;
                        if (ArcheologistCharacter *explorerCharacter = dynamic_cast<ArcheologistCharacter *>(character2)) {
                            if (tilesDeck[5 * temp.Slot2 + temp.Slot1]->haveSand()) {
                                tilesDeck[5 * temp.Slot2 + temp.Slot1]->removeSand();
                                SandMarkersLeft++;
                                cout << "Sand Marker ++" << endl;
                            }
                        }
                        gamePack.ack();
                        gamePack.sendPack();
                        temp.ID = NOEVENT;
                    } else { cout << "My partner told me to remove sand where it was no sand" << endl; }
                    return true;
                } else {
                    cout << "Big mistake! tell me to remove sand when char 2 had no moves left!" << endl;
                    return false;
                }
            case OFFER_EQUIPMENT://ANDA BIEN
                cout << "offer equipment" << endl;
                chooseOption(allegro, temp);
                gamePack.cleanBuffer();
                return true;
            case OFFER_WATER://ANDA BIEN
                cout << "offer water" << endl;
                chooseOption(allegro, temp);
                gamePack.cleanBuffer();
                return true;
            case REQUEST_EQUIPMENT:
                cout << "request equipment" << endl;
                chooseOption(allegro, temp);
                gamePack.cleanBuffer();
                return true;
            case REQUEST_WATER:
                cout << "request water" << endl;
                chooseOption(allegro, temp);
                gamePack.cleanBuffer();
                return true;
            case REQUEST_MOVE:
                cout << "Request Move" << endl;
                //character->resetNumberOfMoves();		//To make sure it set the position
                character->setPos(temp.Slot1, temp.Slot2);
                gamePack.ack();
                gamePack.sendPack();
                return true;
            case PLAY_EQUIPMENT:
                switch (temp.Slot1) {
                    case DUNEBLAST:
                        character2->decreaseDuneBlaster(); //TODO: te deja hacerlo aunque no haya sands? daría bronca errarle al tile
                        while (tilesDeck[5 * temp.Slot3 + temp.Slot2]->haveSand()) {
                            tilesDeck[5 * temp.Slot3 + temp.Slot2]->removeSand();
                            SandMarkersLeft++;
                            cout << "Sand Marker ++" << endl;;
                        }
                        gamePack.ack();
                        gamePack.sendPack();
                        break;
                    case JETPACK:
                        character2->decreaseJetPack();
                        character2->setPos(temp.Slot2, temp.Slot3);
                        character2->increaseMoves();
                        gamePack.ack();
                        gamePack.sendPack();
                        break;
                    case SOLARSHIELD:
                        character2->decreaseSolarShield();
                        character2->shield = true;
                        gamePack.ack();
                        gamePack.sendPack();
                        break;
                    case TERRASCOPE:
                        character2->decreaseTerrascope();
                        backup = tilesDeck[character2->getCardIndex()]->getSand();
                        tilesDeck[character2->getCardIndex()]->turnTerrascopeTile();
                        //tilesDeck[character2->getCardIndex()]->updateTile(allegro); //TODO: why this? I update Below
                        al_flip_display();
                        gamePack.ack();
                        gamePack.sendPack();
                        usleep(TIME_TERRASCOPE);
                        tilesDeck[character2->getCardIndex()]->setSand(backup);
                        tilesDeck[character2->getCardIndex()]->setTurned(false);
                        break;
                    case SECRETWATERRESERVE:
                        cout << "Received SecretWaterReserve" << endl;
                        character2->decreaseWaterReserve();
                        character2->addWater();
                        character2->addWater();
                        if (character->getRow() == character2->getRow() &&
                            character->getColumn() == character2->getColumn()) {
                            character->addWater();
                            character->addWater();
                        }
                        gamePack.ack();
                        gamePack.sendPack();
                        break;
                    case TIMETHROTTLE:
                        character2->decreaseTimeThrottle();
                        character2->increaseMoves();
                        character2->increaseMoves();
                        break;
                }
                return true;
            case PEEP:
                gamePack.ack();
                gamePack.sendPack();
                for (int i = 0; i < temp.Slot1; i++) { swapStormCards(temp.Slot1 - i - 1, temp.Slot1 - i); }
                rotate(stormCardsDeck.begin(), stormCardsDeck.begin() + 1, stormCardsDeck.end());
                if (MeteorologistCharacter *meteorologistCharacter = dynamic_cast<MeteorologistCharacter *>(character2)) { meteorologistCharacter->peep(); }
                else { cout << "My partner send me a peep without being a Meteorologist" << endl; }
                return true;
            case EXCAVATE:
                if (character2->dig()) {
                    tilesDeck[character2->getCardIndex()]->turnTile();
                    performTileAction(tilesDeck[character2->getCardIndex()]->gettileType(), character2->getCardIndex(),
                                      allegro);
                    gamePack.ack();
                    gamePack.sendPack();
                    return true;
                } else {
                    cout << "Big Mistake! they told me to dig while player 2 had no movements left!" << endl;
                    return false;
                }
            case PICKUP:
                pickUpAction();
                gamePack.ack();
                gamePack.sendPack();
                return true;
            case TAKE_WATER:
                if (WaterCarrierCharacter *waterCarrierCharacter = dynamic_cast<WaterCarrierCharacter *>(character2)) {
                    waterCarrierCharacter->getWatersFromWell();
                    gamePack.ack();
                    gamePack.sendPack();
                } else { cout << "I've received TakeWater whithout be character2 a waterCarrier" << endl; }
                return true;
            case PASS:
                gamePack.ack();
                gamePack.sendPack();
                drawStormCards(allegro);
                turn = true; //must go after drawStormCards
                character2->resetNumberOfMoves(); //must go after drawStormCards
                character->resetNumberOfMoves();
                character->shield = false;
                return true;
            case GAME_OVER:
                playAgain = false;
                gameOver = true;
                return false;
            case PLAY_AGAIN:
                playAgain = true;
                gameOver = false;
                return false;
            case WE_WON:
                wannaPlay(allegro, WE_WON);
                return true;
            case WE_LOST:
                wannaPlay(allegro, WE_LOST);
                return true;
            case QUIT:
                cout << "Quit" << endl;
                playAgain = false;
                gameOver = true;
                gamePack.ack();
                gamePack.sendPack();
                return false;
        }
    } while (temp.ID != NOEVENT);

    return true;
}

bool Game::win(ALL *allegro) {
    /*Used to check if the players won*/
    if (engine->getPickedState() && solarCrystal->getPickedState() && propeller->getPickedState() &&
        navigationDeck->getPickedState()) {
        character->playGettingCloseSound();
        if (tilesDeck[character->getCardIndex()]->gettileType() == LAUNCHPAD) {
            if (tilesDeck[character2->getCardIndex()]->gettileType() == LAUNCHPAD) {
                al_draw_bitmap(allegro->menuBackgroundImage, 0, 0, 0);
                al_draw_text(allegro->gameFont, al_map_rgb(WHITE), allegro->screenWidth / 2,
                             allegro->screenHeight * 0.4, ALLEGRO_ALIGN_CENTRE, "WE WON");
                al_flip_display();
                gamePack.wewon();
                gamePack.sendPack();
                char wait;
                do {
                    wait = gamePack.receivePack().ID;
                    gamePack.cleanBuffer();
                } while (wait != PLAY_AGAIN && wait != GAME_OVER);

                if (wait == PLAY_AGAIN) {
                    wannaPlay(allegro, WE_WON);
                    //playAgain = true;
                } else if (wait == GAME_OVER) {
                    playAgain = false;
                    gameOver = true;
                }
                return true;
            }
        }
    }
    return false;
}
bool Game::lose(ALL *allegro) {
    /* Lose: used to check if the players lose*/
    if (sandStormLevel < 15) {
        if (SandMarkersLeft > 0) {
            if (character->haveWater()) {
                if (character2->haveWater()) {
                    return false;
                } else {
                    al_draw_text(allegro->gameFont, al_map_rgb(WHITE), allegro->screenWidth / 2,
                                 allegro->screenHeight * 0.9, ALLEGRO_ALIGN_CENTRE,
                                 "Your partner has died from thirst");
                    cout << "Your partner has died from thirst" << endl;
                }
            } else {
                al_draw_text(allegro->gameFont, al_map_rgb(WHITE), allegro->screenWidth / 2,
                             allegro->screenHeight * 0.9, ALLEGRO_ALIGN_CENTRE, "You have died from thirst");
                cout << "You have died from thirst" << endl;
            }
        } else {
            al_draw_text(allegro->gameFont, al_map_rgb(WHITE), allegro->screenWidth / 2, allegro->screenHeight * 0.9,
                         ALLEGRO_ALIGN_CENTRE, "You habe been burried by the sand");
            cout << "You have been burried by the sand" << endl;
        }
    } else {
        al_draw_text(allegro->gameFont, al_map_rgb(WHITE), allegro->screenWidth / 2, allegro->screenHeight * 0.9,
                     ALLEGRO_ALIGN_CENTRE, "You have been killed by the storm");
        cout << "You have been killed by the storm" << endl;
    }
    //If i'm here, it's because I lose.
    al_draw_bitmap(allegro->menuBackgroundImage, 0, 0, 0);
    al_draw_text(allegro->gameFont, al_map_rgb(WHITE), allegro->screenWidth / 2, allegro->screenHeight * 0.4,
                 ALLEGRO_ALIGN_CENTRE, "WE LOST");
    al_flip_display();
    gamePack.welost();
    gamePack.sendPack();
    char wait;
    do {
        wait = gamePack.receivePack().ID;
        gamePack.cleanBuffer();
    } while (wait != PLAY_AGAIN && wait != GAME_OVER);

    if (wait == PLAY_AGAIN) {
        wannaPlay(allegro, WE_LOST);
    } else if (wait == GAME_OVER) {
        gamePack.gameover();
        gamePack.sendPack();
        gamePack.waitForAck(allegro);
        playAgain = false;
        gameOver = true;
    }
    return true;
}
void Game::wannaPlay(ALL *allegro, char ID) {
    ALLEGRO_EVENT ev;
    ALLEGRO_EVENT_QUEUE *tempQueue;
    if (tempQueue = al_create_event_queue()) {
        al_register_event_source(tempQueue, al_get_mouse_event_source());
        Button *playAgainButton, *gameOverButton;
        playAgainButton = new Button(allegro->screenWidth * 0.25, allegro->screenHeight * 0.65,
                                     "Resources/Buttons/playAgainButton.png",
                                     "Resources/Buttons/playAgainSelectedButton.png", allegro);
        gameOverButton = new Button(allegro->screenWidth * 0.65, allegro->screenHeight * 0.65,
                                    "Resources/Buttons/gameOverButton.png",
                                    "Resources/Buttons/gameOverSelectedButton.png", allegro);
        bool run = true;
        al_draw_bitmap(allegro->menuBackgroundImage, 0, 0, 0);
        if (ID == WE_LOST) {
            al_draw_text(allegro->gameFont, al_map_rgb(WHITE), allegro->screenWidth / 2, allegro->screenHeight * 0.4,
                         ALLEGRO_ALIGN_CENTRE, "WE LOST");
        }
        else if (ID == WE_WON) {
            al_draw_text(allegro->gameFont, al_map_rgb(WHITE), allegro->screenWidth / 2, allegro->screenHeight * 0.4,
                         ALLEGRO_ALIGN_CENTRE, "WE WON");
        }
        while (run) {
            playAgainButton->updateButton(allegro);
            gameOverButton->updateButton(allegro);
            al_flip_display();
            al_wait_for_event(tempQueue, &ev);
            if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
                playAgainButton->checkMouse(ev.mouse.x, ev.mouse.y, allegro->volume);
                gameOverButton->checkMouse(ev.mouse.x, ev.mouse.y, allegro->volume);
            } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                if (playAgainButton->checkMouse(ev.mouse.x, ev.mouse.y, allegro->volume)) {
                    playAgain = true;
                    gameOver = false;
                    gamePack.playagain();
                    gamePack.sendPack();
                    run = false;
                } else if (gameOverButton->checkMouse(ev.mouse.x, ev.mouse.y, allegro->volume)) {
                    gameOver = true;
                    playAgain = false;
                    gamePack.gameover();
                    gamePack.sendPack();
                    run = false;
                }
            }

        }
        delete playAgainButton;
        delete gameOverButton;
        al_destroy_event_queue(tempQueue);
    } else { cout << "Could not initialize tempQueue" << endl; }
}

void Game::pickUpAction() {
    /*	Picks up the part in question. The function use the character acording to whose turn is it and then checks if its posible to pick up
	the part. If it is, picks up the correct part
	*/
    Character *tempChar;            //pointer to character. it will point to character 1 or 2 acording to whose turn is it.
    if (turn) { tempChar = character; }
    else { tempChar = character2; }
    // This code makes the player be able to pick up multiple items at once if they are at the same place
    if (tempChar->getRow() == engine->getRow() && tempChar->getColumn() == engine->getColumn()) {
        if (engine->getFoundState()) {            //Only able to pick it up if I found it
            if (tilesDeck[tempChar->getCardIndex()]->isTurned()) {
                if (tilesDeck[tempChar->getCardIndex()]->canIMove(false)) {
                    if (tempChar->pickUpPart()) { engine->pickUpPart(); }
                }
            }
        }
    }
    if (tempChar->getRow() == solarCrystal->getRow() && tempChar->getColumn() == solarCrystal->getColumn()) {
        if (solarCrystal->getFoundState()) {
            if (tilesDeck[tempChar->getCardIndex()]->isTurned()) {
                if (tilesDeck[tempChar->getCardIndex()]->canIMove(false)) {
                    if (tempChar->pickUpPart()) { solarCrystal->pickUpPart(); }
                }
            }
        }
    }
    if (tempChar->getRow() == propeller->getRow() && tempChar->getColumn() == propeller->getColumn()) {
        if (propeller->getFoundState()) {
            if (tilesDeck[tempChar->getCardIndex()]->isTurned()) {
                if (tilesDeck[tempChar->getCardIndex()]->canIMove(false)) {
                    if (tempChar->pickUpPart()) { propeller->pickUpPart(); }
                }
            }
        }
    }
    if (navigationDeck->getRow() == tempChar->getRow() && tempChar->getColumn() == navigationDeck->getColumn()) {
        if (navigationDeck->getFoundState()) {
            if (tilesDeck[tempChar->getCardIndex()]->isTurned()) {
                if (tilesDeck[tempChar->getCardIndex()]->canIMove(false)) {
                    if (tempChar->pickUpPart()) { navigationDeck->pickUpPart(); }
                }
            }
        }
    }
}
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
void Game::updateScreen(ALL *allegro) {
    /* Updates the screen */
    redraw = false;
    switch (modeEnum) {
        case PEEPMODE:
            if (firstTimeUpdateScreenPeep == 0) {
                al_draw_bitmap(allegro->menuBackgroundImage, 0, 0, 0);
                firstTimeUpdateScreenPeep++;
            }
            stormCardsDeck[peepOffset]->peepShowImage(allegro);
            nextButton->updateButton(allegro);
            previousButton->updateButton(allegro);
            selectPeepCard->updateButton(allegro);
            break;
        case OFFEREQUIPMENTMODE:
            if (firstTimeUpdateScreenChooseEquipment == 0) {
                al_draw_bitmap(allegro->menuBackgroundImage, 0, 0, 0);
                firstTimeUpdateScreenChooseEquipment++;
                if (character->haveDuneBlaster())
                    equipOffset = DUNEBLAST;
                else if (character->haveJetPack())
                    equipOffset = JETPACK;
                else if (character->haveSolarShield())
                    equipOffset = SOLARSHIELD;
                else if (character->haveTerrascope())
                    equipOffset = TERRASCOPE;
                else if (character->haveTimeThrottle())
                    equipOffset = TIMETHROTTLE;
            }
            al_draw_bitmap(allegro->equipmentCards[equipOffset - 1],
                           (allegro->screenWidth - al_get_bitmap_width(allegro->equipmentCards[equipOffset - 1])) / 2,
                           (allegro->screenHeight - al_get_bitmap_height(allegro->equipmentCards[equipOffset - 1])) / 2,
                           0);
            nextButton->updateButton(allegro);
            previousButton->updateButton(allegro);
            selectEquipmentCard->updateButton(allegro);
            break;
        case REQUESTEQUIPMENTMODE:
            if (firstTimeUpdateScreenChooseEquipment == 0) {
                al_draw_bitmap(allegro->menuBackgroundImage, 0, 0, 0);
                firstTimeUpdateScreenChooseEquipment++;
                if (character2->haveDuneBlaster())
                    equipOffset = DUNEBLAST;
                else if (character2->haveJetPack())
                    equipOffset = JETPACK;
                else if (character2->haveSolarShield())
                    equipOffset = SOLARSHIELD;
                else if (character2->haveTerrascope())
                    equipOffset = TERRASCOPE;
                else if (character2->haveTimeThrottle())
                    equipOffset = TIMETHROTTLE;
            }
            al_draw_bitmap(allegro->equipmentCards[equipOffset - 1],
                           (allegro->screenWidth - al_get_bitmap_width(allegro->equipmentCards[equipOffset - 1])) / 2,
                           (allegro->screenHeight - al_get_bitmap_height(allegro->equipmentCards[equipOffset - 1])) / 2,
                           0);
            nextButton->updateButton(allegro);
            previousButton->updateButton(allegro);
            selectEquipmentCard->updateButton(allegro);
            break;
        default:
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(allegro->fondo, 0, 0, 0);
            //Update Tiles
            pos tempPos;
            for (uint i = 0; i < tilesDeck.size(); i++) {
                tempPos = getSpot(i % 5, i / 5, allegro, tilesDeck[i]->getTileBackImage());
                tilesDeck[i]->setCoord(tempPos);
            }
            for (uint i = 0; i < tilesDeck.size(); i++) {
                tilesDeck[i]->updateTile(allegro);
            }
            //End of Turn Button
            endTurnButton->updateButton(allegro);
            if (allegro->volume == 0) { muteButton->updateButton(allegro); }
            else { noMuteButton->updateButton(allegro); }
            //Tiles Buttons
            if (turn) { updateTilesButton(allegro); } //Don't let the player think he can still move
            //Update Characters
            if (turn) {
                character->updateChar(allegro, 1, endTurnButton->getMiddleOfX(),
                                      endTurnButton->getMiddleOfY()); //Character must be after Tiles
                character2->updateChar(allegro, 2);
            } else {
                character->updateChar(allegro, 1);
                character2->updateChar(allegro, 2, endTurnButton->getMiddleOfX(), endTurnButton->getMiddleOfY());
            }
            //Update Parts
            propeller->updatePart(allegro);
            solarCrystal->updatePart(allegro);
            navigationDeck->updatePart(allegro);
            engine->updatePart(allegro);
            int i = 0;
            if (engine->getPickedState()) {
                al_draw_bitmap(allegro->partsImages[i], allegro->screenWidth / 4, allegro->screenHeight / 5 + i *
                                                                                                              al_get_bitmap_height(
                                                                                                                      allegro->partsImages[i]),
                               0);
            }
            else {
                al_draw_bitmap(allegro->partsImages[i + 4], allegro->screenWidth / 4,
                               allegro->screenHeight / 5 + i * al_get_bitmap_height(allegro->partsImages[i]), 0);
            }
            i++;
            if (navigationDeck->getPickedState()) {
                al_draw_bitmap(allegro->partsImages[i], allegro->screenWidth / 4, allegro->screenHeight / 5 + i *
                                                                                                              al_get_bitmap_height(
                                                                                                                      allegro->partsImages[i]),
                               0);
            }
            else {
                al_draw_bitmap(allegro->partsImages[i + 4], allegro->screenWidth / 4,
                               allegro->screenHeight / 5 + i * al_get_bitmap_height(allegro->partsImages[i]), 0);
            }
            i++;
            if (propeller->getPickedState()) {
                al_draw_bitmap(allegro->partsImages[i], allegro->screenWidth / 4, allegro->screenHeight / 5 + i *
                                                                                                              al_get_bitmap_height(
                                                                                                                      allegro->partsImages[i]),
                               0);
            }
            else {
                al_draw_bitmap(allegro->partsImages[i + 4], allegro->screenWidth / 4,
                               allegro->screenHeight / 5 + i * al_get_bitmap_height(allegro->partsImages[i]), 0);
            }
            i++;
            if (solarCrystal->getPickedState()) {
                al_draw_bitmap(allegro->partsImages[i], allegro->screenWidth / 4, allegro->screenHeight / 5 + i *
                                                                                                              al_get_bitmap_height(
                                                                                                                      allegro->partsImages[i]),
                               0);
            }
            else {
                al_draw_bitmap(allegro->partsImages[i + 4], allegro->screenWidth / 4,
                               allegro->screenHeight / 5 + i * al_get_bitmap_height(allegro->partsImages[i]), 0);
            }
            //Buttons
            for (int i = 0; i < MODEBUTTONSNUMBER; i++) {
                modesButtons[i]->updateButton(allegro);
            }
            if (MeteorologistCharacter *meteorologistCharacter = dynamic_cast<MeteorologistCharacter *>(character)) {
                meteorologistCharacter->updateMeteorologistButton(
                        allegro);    //Only display the button for the one who can use it... XD
            } else if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character)) {
                climberCharacter->updateClimber(allegro);
            } else if (WaterCarrierCharacter *waterCarrierCharacter = dynamic_cast<WaterCarrierCharacter *>(character)) {
                bool imOnTurnedWell = (tilesDeck[character->getCardIndex()]->gettileType() == WATER &&
                                       tilesDeck[character->getCardIndex()]->isTurned());
                waterCarrierCharacter->updateGetWatersFromWell(imOnTurnedWell, allegro);
            } else if (NavigatorCharacter *navigatorCharacter = dynamic_cast<NavigatorCharacter *>(character)) {
                navigatorCharacter->updateNavigator(allegro, modeEnum == MOVEOTHER);
            }
            //Sand Storm Level Meter
            if (sandStormLevel < 15) {
                al_draw_bitmap(allegro->stormMeter[sandStormLevel], allegro->screenWidth * 12 / 17,
                               allegro->screenHeight - al_get_bitmap_height(allegro->stormMeter[sandStormLevel]), 0);
            }
            //Sand Markers Left (TEXT)
            if (SandMarkersLeft < 0) { SandMarkersLeft = 0; }
            al_draw_textf(allegro->gameFont, al_map_rgb(BLACK), allegro->screenWidth * 30 / 34,
                          allegro->screenHeight * 25 / 32, ALLEGRO_ALIGN_LEFT, "%d", SandMarkersLeft);
    }
    if (modeEnum == OFFERWATERMODE || modeEnum == REQUESTWATERMODE) {
        al_draw_bitmap(allegro->menuBackgroundImage, 0, 0, 0);
        al_draw_text(allegro->font, al_map_rgb(WHITE), allegro->screenWidth * 0.5,
                     watersButton[0]->getInitialPos().y - 55, ALLEGRO_ALIGN_CENTRE, "How many waters?");
        if (modeEnum == OFFERWATERMODE) {
            for (int i = 0; i < min(character->getWaters(), MAXWATERS - character2->getWaters() - 1); i++)
                watersButton[i]->updateButton(allegro);//update all the waters Button
        } else if (modeEnum == REQUESTWATERMODE) {
            for (int i = 0; i < min((MAXWATERS - character->getWaters() - 1), character2->getWaters()); i++)
                watersButton[i]->updateButton(allegro);//update all the waters Button
        }
    }
    al_flip_display();
}
void Game::updateTilesButton(ALL *allegro) {
    //Equipments Mode
    if (modeEnum == JETPACKMODE) {
        for (int i = 0; i < TILEDECKNUMBER; i++) {
            if (tilesDeck[i]->canIMove(false)) {
                tilesDeck[i]->updateButton(allegro);
            }
        }
    } else if (modeEnum == TERRASCOPEMODE) {
        for (int i = 0; i < TILEDECKNUMBER; i++)
            if (tilesDeck[i]->gettileType() != STORM) {
                tilesDeck[i]->updateButton(allegro);
            }
    } else {
        int tempIndex;
        bool climber = false;
        //Normal Movements
        if (modeEnum == MOVEOTHER) {
            tempIndex = character2->getCardIndex();
            if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character2)) { climber = true; }
        } else {
            tempIndex = character->getCardIndex();
            if (turn && (tilesDeck[tempIndex]->haveSand() ||
                         !tilesDeck[tempIndex]->isTurned())) { tilesDeck[tempIndex]->updateButton(allegro); }
            if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character)) { climber = true; }
        }
        if (turn && tilesDeck[tempIndex]->canIMove(climber)) {
            if (modeEnum == NORMAL || modeEnum == MOVEOTHER) {
                if (tempIndex > 0 && tempIndex % 5 != 0) {
                    if (tilesDeck[tempIndex - 1]->canIMove(climber)) {
                        tilesDeck[tempIndex - 1]->updateButton(allegro);
                    }
                }
                if (tempIndex < TILEDECKNUMBER - 1 && tempIndex % 5 != 4) {
                    if (tilesDeck[tempIndex + 1]->canIMove(climber)) {
                        tilesDeck[tempIndex + 1]->updateButton(allegro);
                    }
                }
                if (tempIndex > 4) {
                    if (tilesDeck[tempIndex - 5]->canIMove(climber)) {
                        tilesDeck[tempIndex - 5]->updateButton(allegro);
                    }
                }
                if (tempIndex < TILEDECKNUMBER - 5) {
                    if (tilesDeck[tempIndex + 5]->canIMove(climber)) {
                        tilesDeck[tempIndex + 5]->updateButton(allegro);
                    }
                }
                if (modeEnum == NORMAL) {
                    if (ExplorerCharacter *explorerCharacter = dynamic_cast<ExplorerCharacter *>(character)) {
                        if (tempIndex > 5 && tempIndex % 5 != 0) {
                            if (tilesDeck[tempIndex - 6]->canIMove(climber)) {
                                tilesDeck[tempIndex - 6]->updateButton(allegro);
                            }
                        }
                        if (tempIndex > 4 && tempIndex % 5 != 4) {
                            if (tilesDeck[tempIndex - 4]->canIMove(climber)) {
                                tilesDeck[tempIndex - 4]->updateButton(allegro);
                            }
                        }
                        if (tempIndex < TILEDECKNUMBER - 4 && tempIndex % 5 != 0) {
                            if (tilesDeck[tempIndex + 4]->canIMove(climber)) {
                                tilesDeck[tempIndex + 4]->updateButton(allegro);
                            }
                        }
                        if (tempIndex < TILEDECKNUMBER - 6 && tempIndex % 5 != 4) {
                            if (tilesDeck[tempIndex + 6]->canIMove(climber)) {
                                tilesDeck[tempIndex + 6]->updateButton(allegro);
                            }
                        }
                    }
                } else if (modeEnum == MOVEOTHER) {
                    if (ExplorerCharacter *explorerCharacter = dynamic_cast<ExplorerCharacter *>(character2)) {
                        if (tempIndex > 5 && tempIndex % 5 != 0) {
                            if (tilesDeck[tempIndex - 6]->canIMove(climber)) {
                                tilesDeck[tempIndex - 6]->updateButton(allegro);
                            }
                        }
                        if (tempIndex > 4 && tempIndex % 5 != 4) {
                            if (tilesDeck[tempIndex - 4]->canIMove(climber)) {
                                tilesDeck[tempIndex - 4]->updateButton(allegro);
                            }
                        }
                        if (tempIndex < TILEDECKNUMBER - 4 && tempIndex % 5 != 0) {
                            if (tilesDeck[tempIndex + 4]->canIMove(climber)) {
                                tilesDeck[tempIndex + 4]->updateButton(allegro);
                            }
                        }
                        if (tempIndex < TILEDECKNUMBER - 6 && tempIndex % 5 != 4) {
                            if (tilesDeck[tempIndex + 6]->canIMove(climber)) {
                                tilesDeck[tempIndex + 6]->updateButton(allegro);
                            }
                        }
                    }
                }
            }
            if (modeEnum == REMOVESAND || modeEnum == DUNEBLASTERMODE) {
                if (tempIndex > 0 && tempIndex % 5 != 0) {
                    if (tilesDeck[tempIndex - 1]->haveSand()) {
                        tilesDeck[tempIndex - 1]->updateButton(allegro);
                    }
                }
                if (tempIndex < TILEDECKNUMBER - 1 && tempIndex % 5 != 4) {
                    if (tilesDeck[tempIndex + 1]->haveSand()) {
                        tilesDeck[tempIndex + 1]->updateButton(allegro);
                    }
                }
                if (tempIndex > 4) {
                    if (tilesDeck[tempIndex - 5]->haveSand()) {
                        tilesDeck[tempIndex - 5]->updateButton(allegro);
                    }
                }
                if (tempIndex < TILEDECKNUMBER - 5) {
                    if (tilesDeck[tempIndex + 5]->haveSand()) {
                        tilesDeck[tempIndex + 5]->updateButton(allegro);
                    }
                }
                if (ExplorerCharacter *explorerCharacter = dynamic_cast<ExplorerCharacter *>(character)) {
                    if (tempIndex > 5 && tempIndex % 5 != 0) {
                        if (tilesDeck[tempIndex - 6]->haveSand()) {
                            tilesDeck[tempIndex - 6]->updateButton(allegro);
                        }
                    }
                    if (tempIndex > 4 && tempIndex % 5 != 4) {
                        if (tilesDeck[tempIndex - 4]->haveSand()) {
                            tilesDeck[tempIndex - 4]->updateButton(allegro);
                        }
                    }
                    if (tempIndex < TILEDECKNUMBER - 4 && tempIndex % 5 != 0) {
                        if (tilesDeck[tempIndex + 4]->haveSand()) {
                            tilesDeck[tempIndex + 4]->updateButton(allegro);
                        }
                    }
                    if (tempIndex < TILEDECKNUMBER - 6 && tempIndex % 5 != 4) {
                        if (tilesDeck[tempIndex + 6]->haveSand()) {
                            tilesDeck[tempIndex + 6]->updateButton(allegro);
                        }
                    }
                }
            }
            if (tilesDeck[tempIndex]->gettileType() == TUNNEL && tilesDeck[tempIndex]->isTurned()) {
                for (uint i = 0; i < tilesDeck.size(); i++) {
                    if (tilesDeck[i]->gettileType() == TUNNEL && i != tempIndex && tilesDeck[i]->isTurned()) {
                        if (tilesDeck[i]->canIMove(climber)) { tilesDeck[i]->updateButton(allegro); }
                    }
                }
            }
        }
    }
}
//Decks Swap/Shuffle Methods
void Game::shuffleTiles() {
    /* This function automatically shuffles the tile Deck*/
    uint seed = chrono::system_clock::now().time_since_epoch().count();    //Magic, its to make the suffle work, something about using a counter to set the value.
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
//Perform Actions
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
            drawEquipment(gameNetwork->getInfo().equipment[equipIndex]);
            break;
        case GEAR2:
            drawEquipment(gameNetwork->getInfo().equipment[equipIndex]);
            break;
        case GEAR3:
            drawEquipment(gameNetwork->getInfo().equipment[equipIndex]);
            break;
        case GEAR4:
            drawEquipment(gameNetwork->getInfo().equipment[equipIndex]);
            break;
        case GEAR5:
            drawEquipment(gameNetwork->getInfo().equipment[equipIndex]);
            break;
        case GEAR6:
            drawEquipment(gameNetwork->getInfo().equipment[equipIndex]);
            break;
        case GEAR7:
            drawEquipment(gameNetwork->getInfo().equipment[equipIndex]);
            break;
        case GEAR8:
            drawEquipment(gameNetwork->getInfo().equipment[equipIndex]);
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
            drawEquipment(gameNetwork->getInfo().equipment[equipIndex]);
            break;
    }
}
void Game::performEquipmentsAction(EquipmentsEnum equipEnum, ALL *allegro) {
    /* Similar to performTileAction. gets the enum of the equipment and acts accordingly */
    bool run = true;
    switch (equipEnum) {
        case DUNEBLAST:
            character->decreaseDuneBlaster();
            modeEnum = DUNEBLASTERMODE;
            break;
        case JETPACK:
            character->decreaseJetPack();
            modeEnum = JETPACKMODE;
            break;
        case SOLARSHIELD:
            character->decreaseSolarShield();
            character->shield = true;
            gamePack.playequipment(SOLARSHIELD, character->getRow(), character->getColumn());
            gamePack.sendPack();
            gamePack.waitForAck(allegro);
            break;
        case TERRASCOPE:
            character->decreaseTerrascope();
            al_play_sample(allegro->curiositySound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            modeEnum = TERRASCOPEMODE;
            break;
        case SECRETWATERRESERVE:
            //2 waters for each player
            character->decreaseWaterReserve();
            character->addWater();
            character->addWater();
            al_play_sample(allegro->waterSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            if (character->getRow() == character2->getRow() && character->getColumn() == character2->getColumn()) {
                character2->addWater();
                character2->addWater();
            }
            gamePack.playequipment(SECRETWATERRESERVE, character->getRow(), character->getColumn());
            gamePack.sendPack();
            gamePack.waitForAck(allegro);
            break;
        case TIMETHROTTLE:
            character->decreaseTimeThrottle();
            character->increaseMoves();
            character->increaseMoves();
            gamePack.playequipment(TIMETHROTTLE, character->getRow(), character->getColumn());
            gamePack.sendPack();
            gamePack.waitForAck(allegro);
            break;
        default:
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
            usleep(600);
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
            usleep(600);
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
            usleep(600);
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
            usleep(600);
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
            usleep(600);
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
            usleep(600);
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
            usleep(600);
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
            usleep(600);
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
            usleep(600);
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
            usleep(600);
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
            usleep(600);
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
            usleep(600);
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
            usleep(600);
            break;
        case STORMUP:
            cout << "Storm Pick Up" << endl;
            al_play_sample(allegro->stormSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            sandStormLevel++;
            updateScreen(allegro);
            stormCardsDeck[0]->showImage(allegro);
            usleep(600);
    };

    rotate(stormCardsDeck.begin(), stormCardsDeck.begin() + 1, stormCardsDeck.end());
}
//Draw Methods
void Game::drawEquipment(EquipmentsEnum equipEnum) {
    /* Gets the equipment and adds it to the player */
    switch (equipEnum) {
        case DUNEBLAST:
            if (turn) { character->increaseDuneBlaster(); }
            else { character2->increaseDuneBlaster(); }
            break;
        case JETPACK:
            if (turn) { character->increaseJetPack(); }
            else { character2->increaseJetPack(); }
            break;
        case SOLARSHIELD:
            if (turn) { character->increaseSolarShield(); }
            else { character2->increaseSolarShield(); }
            break;
        case TERRASCOPE:
            if (turn) { character->increaseTerrascope(); }
            else { character2->increaseTerrascope(); }
            break;
        case SECRETWATERRESERVE:
            if (turn) { character->increaseWaterReserve(); }
            else { character2->increaseWaterReserve(); }
            break;
        case TIMETHROTTLE:
            if (turn) { character->increaseTimeThrottle(); }
            else { character2->increaseTimeThrottle(); }
            break;
    }
    if (equipIndex == 11) { equipIndex = 0; }
    else { equipIndex++; }
}
void Game::drawStormCards(ALL *allegro) {
    /* This function runs after the end of a turn...
	It uses the sandStormLevel and draw the correct number of storm cards running its action with performStormCardAction*/
//The sandStormLevel does not indicate the number of stormCards to be drawn
    int discount = 0;        //Sometimes, if meteorologist is playing, it may be a discount on how many cards to draw
    if (turn) {
        if (MeteorologistCharacter *meteorologistCharacter = dynamic_cast<MeteorologistCharacter *>(character)) {
            discount = character->getNumberOfMoves();
        }
    } else if (MeteorologistCharacter *meteorologistCharacter = dynamic_cast<MeteorologistCharacter *>(character2)) {
        discount = character2->getNumberOfMoves();
    }
    character->resetNumberOfMoves();
    character2->resetNumberOfMoves();
    if (sandStormLevel < 6) {
        if (discount < 3) {
            for (int i = 0; i < 3 - discount; i++) {
                cout << "Pick Up One Storm Card" << endl;
                this->performStormCardAction(allegro, stormCardsDeck[0]->getStormCardType());
            }
        }
    } else if (sandStormLevel >= 6 && sandStormLevel < 10) {
        if (discount != 4) {
            for (int i = 0; i < 4 - discount; i++) {
                cout << "Pick Up One Storm Card" << endl;
                this->performStormCardAction(allegro, stormCardsDeck[0]->getStormCardType());
            }
        }
    } else if (sandStormLevel >= 10 && sandStormLevel < 13) {
        for (int i = 0; i < 5 - discount; i++) {
            cout << "Pick Up One Storm Card" << endl;
            this->performStormCardAction(allegro, stormCardsDeck[0]->getStormCardType());
        }
    } else if (sandStormLevel >= 13 && sandStormLevel < 15) {
        for (int i = 0; i < 6 - discount; i++) {
            cout << "Pick Up One Storm Card" << endl;
            this->performStormCardAction(allegro, stormCardsDeck[0]->getStormCardType());
        }
    }
}
//Initialize Methods
void Game::initializeBoard(ALL *allegro) {
    //Set Tiles
    for (uint i = 0; i < tilesDeck.size(); i++) {
        tilesDeck[i] = new Tile(gameNetwork->getInfo().tiles[i], allegro);
    }
    //Set the sands on the tiles
    for (int i = 1; i < 12; i++) {
        if (i != 6 && i != 10 && i != 2) {
            tilesDeck[2 * i]->setSand(1);
        }
    }
    //Set Character over crash site
    for (uint i = 0; i < tilesDeck.size(); i++) {
        if (tilesDeck[i]->gettileType() == CRASHSITE) {
            character->setPos(i % 5, i / 5);
            character2->setPos(i % 5, i / 5);
            tilesDeck[i]->setTurned(true);        //TURN CRASHSITE TO MAKE IT NO POSIBLE TO LOSE A MOVEMENT DIGGING IT.
            i = tilesDeck.size();
        }
    }

    character->resetNumberOfMoves();    //This is done because "initializeBoard" decreses the character number of moves by one
    character2->resetNumberOfMoves();
}
bool Game::initializeAllegro(ALL *allegro) {
    bool state = true;
    /*TODO: lo comento porque es dificil para debuguear... luego agregarlo. tira mas facha*/
    ALLEGRO_DISPLAY_MODE disp_data;
    al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
    al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    allegro->display = al_create_display(disp_data.width, disp_data.height);
    allegro->screenWidth = disp_data.width;
    allegro->screenHeight = disp_data.height;
    //if (state) {
    //	state = false;
    //	if (allegro->display = al_create_display(SCREEN_X, SCREEN_Y)) {
    //		allegro->screenWidth = SCREEN_X;
    //		allegro->screenHeight = SCREEN_Y;
    //		//if (allegro->startMenuDisplay != NULL) { al_destroy_display(allegro->startMenuDisplay); } //Just in case
    //		al_register_event_source(allegro->events_queue, al_get_display_event_source(allegro->display));
    //		state = true;
    //	}
    //	else { cout << "Failed to create Display" << endl; }
    //}
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
    }
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
    //create storm cards Deck
    int i = 0;
    for (uint i = 0; i < stormCardsDeck.size(); i++) {
        stormCardsDeck[i] = new StormCard(gameNetwork->getInfo().storm[i], allegro);
    }
}
void Game::initializeCharacters(Container info, ALL *allegro) {
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
    //allegro->volume Buttons
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
//Key Methods
void Game::keyUp(ALL *allegro) {
    /* Used when the kuy up was pressed, it does what it needs to be done after it happens */
    if (modeEnum != MOVEOTHER) {
        if (character->getCardIndex() > 0) {
            if (modeEnum == NORMAL) {
                bool climber = false;
                if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character)) { climber = true; }
                if (tilesDeck[character->getCardIndex() - 1]->canIMove(climber)) { //If I'm not on top... move up
                    if (tilesDeck[character->getCardIndex()]->canIMove(climber)) {
                        if (character->moveUp()) {
                            if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character)) {
                                if (climberCharacter->getTakeWithMe() &&
                                    character2->getCardIndex() - 1 == character->getCardIndex()) {
                                    character2->moveUp();
                                    gamePack.requestmove(character2->getRow(), character2->getColumn());
                                    gamePack.sendPack();
                                    gamePack.waitForAck(allegro);
                                    usleep(100);
                                    gamePack.move(character->getRow(), character->getColumn());
                                    gamePack.sendPack();
                                    gamePack.waitForAck(allegro);
                                }
                            }
                            //Connection
                            gamePack.move(character->getRow(), character->getColumn());
                            gamePack.sendPack();
                            gamePack.waitForAck(allegro);
                        }
                    }
                }
            } else if (modeEnum == REMOVESAND) {
                if (tilesDeck[character->getCardIndex() - 1]->haveSand()) {
                    if (character->removeSand()) {
                        tilesDeck[character->getCardIndex() - 1]->removeSand();
                        if (ArcheologistCharacter *explorerCharacter = dynamic_cast<ArcheologistCharacter *>(character)) {
                            if (tilesDeck[character->getCardIndex() - 1]->haveSand()) {
                                tilesDeck[character->getCardIndex() - 1]->removeSand();
                                SandMarkersLeft++;
                                cout << "Sand Marker ++" << endl;
                            }
                        }
                        gamePack.remove(character->getRow() - 1, character->getColumn());
                        gamePack.sendPack();
                        gamePack.waitForAck(allegro);
                        SandMarkersLeft++;
                        cout << "Sand Marker ++" << endl;
                    }
                }
            } else if (modeEnum == DUNEBLASTERMODE) {
                while (tilesDeck[character->getCardIndex() - 1]->haveSand()) {
                    tilesDeck[character->getCardIndex() - 1]->removeSand();
                    SandMarkersLeft++;
                    cout << "Sand Marker ++" << endl;
                }
                gamePack.playequipment(DUNEBLAST, character->getRow() - 1, character->getColumn());
                gamePack.sendPack();
                gamePack.waitForAck(allegro);
            }
        }
    } else {
        if (NavigatorCharacter *navigatorCharacter = dynamic_cast<NavigatorCharacter *>(character)) {
            if (character2->getCardIndex() > 0) {
                if (navigatorCharacter->havePartnersMovesLeft()) {
                    bool climber = false;
                    if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character2)) { climber = true; }
                    if (tilesDeck[character2->getCardIndex() - 1]->canIMove(climber)) { //If I'm not on top... move up
                        if (tilesDeck[character2->getCardIndex()]->canIMove(climber)) {
                            if (character2->moveUp() && navigatorCharacter->decresePartnerMoves()) {
                                if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character2)) {
                                    if (climberCharacter->getTakeWithMe() &&
                                        character->getCardIndex() - 1 == character2->getCardIndex()) {
                                        character->moveUp(); //TODO: this is not working yet, the other computer never knows if climber has its take with me active
                                    }
                                }
                                gamePack.requestmove(character2->getRow(), character2->getColumn());
                                gamePack.sendPack();
                                gamePack.waitForAck(allegro);
                            } else { cout << "Error: no moves left even though I was supposed to have" << endl; }
                        }
                    }
                }
            }
        } else { cout << "Error: Move Other mode without being a navigator character" << endl; }
    }
}
void Game::keyDown(ALL *allegro) {
    if (modeEnum != MOVEOTHER) {
        if (character->getCardIndex() < tilesDeck.size() - 1) {
            if (modeEnum == NORMAL) {
                bool climber = false;
                if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character)) { climber = true; }
                if (tilesDeck[character->getCardIndex() + 1]->canIMove(climber)) {
                    if (tilesDeck[character->getCardIndex()]->canIMove(climber)) {
                        if (character->moveDown()) {
                            if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character)) {
                                if (climberCharacter->getTakeWithMe() &&
                                    character2->getCardIndex() + 1 == character->getCardIndex()) {
                                    character2->moveDown();
                                    gamePack.requestmove(character2->getRow(), character2->getColumn());
                                    gamePack.sendPack();
                                    gamePack.waitForAck(allegro);
                                    usleep(100);
                                    gamePack.move(character->getRow(), character->getColumn());
                                    gamePack.sendPack();
                                    gamePack.waitForAck(allegro);
                                }
                            }
                            gamePack.move(character->getRow(), character->getColumn());
                            gamePack.sendPack();
                            gamePack.waitForAck(allegro);
                        }
                    }
                }
            } else if (modeEnum == REMOVESAND) {
                if (tilesDeck[character->getCardIndex() + 1]->haveSand()) {
                    if (character->removeSand()) {
                        tilesDeck[character->getCardIndex() + 1]->removeSand();
                        if (ArcheologistCharacter *explorerCharacter = dynamic_cast<ArcheologistCharacter *>(character)) {
                            if (tilesDeck[character->getCardIndex() + 1]->haveSand()) {
                                tilesDeck[character->getCardIndex() + 1]->removeSand();
                                SandMarkersLeft++;
                                cout << "Sand Marker ++" << endl;
                            }
                        }
                        SandMarkersLeft++;
                        cout << "Sand Marker ++" << endl;
                        gamePack.remove(character->getRow() + 1, character->getColumn());
                        gamePack.sendPack();
                        gamePack.waitForAck(allegro);
                    }
                }
            } else if (modeEnum == DUNEBLASTERMODE) {
                while (tilesDeck[character->getCardIndex() + 1]->haveSand()) {
                    tilesDeck[character->getCardIndex() + 1]->removeSand();
                    SandMarkersLeft++;
                    cout << "Sand Marker ++" << endl;
                }
                gamePack.playequipment(DUNEBLAST, character->getRow() + 1, character->getColumn());
                gamePack.sendPack();
                gamePack.waitForAck(allegro);
            }
        }
    } else {
        if (NavigatorCharacter *navigatorCharacter = dynamic_cast<NavigatorCharacter *>(character)) {
            if (character2->getCardIndex() < tilesDeck.size() - 1) {
                if (navigatorCharacter->havePartnersMovesLeft()) {
                    bool climber = false;
                    if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character2)) { climber = true; }
                    if (tilesDeck[character2->getCardIndex() + 1]->canIMove(climber)) {
                        if (tilesDeck[character2->getCardIndex()]->canIMove(climber)) {
                            if (character2->moveDown() && navigatorCharacter->decresePartnerMoves()) {
                                if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character2)) {
                                    if (climberCharacter->getTakeWithMe() &&
                                        character->getCardIndex() + 1 == character2->getCardIndex()) {
                                        character->moveDown();
                                    }
                                }
                                gamePack.requestmove(character2->getRow(), character2->getColumn());
                                gamePack.sendPack();
                                gamePack.waitForAck(allegro);
                            } else { cout << "Error: no moves left even though I was supposed to have" << endl; }
                        }
                    }
                }
            }
        } else { cout << "Error: Move Other mode without being a navigator character" << endl; }
    }
}
void Game::keyRight(ALL *allegro) {
    if (modeEnum == OFFEREQUIPMENTMODE || modeEnum == REQUESTEQUIPMENTMODE) { nextEquipCard(); }
    else if (modeEnum == PEEPMODE) { nextPeepStormCard(); }

    if (modeEnum != MOVEOTHER) {
        if (character->getCardIndex() < tilesDeck.size() - 5) {
            if (modeEnum == NORMAL) {
                bool climber = false;
                if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character)) { climber = true; }
                if (tilesDeck[character->getCardIndex() + 5]->canIMove(climber)) {
                    if (tilesDeck[character->getCardIndex()]->canIMove(climber)) {
                        if (character->moveRight()) {
                            if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character)) {
                                if (climberCharacter->getTakeWithMe() &&
                                    character2->getCardIndex() + 5 == character->getCardIndex()) {
                                    character2->moveRight();
                                    gamePack.requestmove(character2->getRow(), character2->getColumn());
                                    gamePack.sendPack();
                                    gamePack.waitForAck(allegro);
                                    usleep(100);
                                    gamePack.move(character->getRow(), character->getColumn());
                                    gamePack.sendPack();
                                    gamePack.waitForAck(allegro);
                                }
                            }
                            gamePack.move(character->getRow(), character->getColumn());
                            gamePack.sendPack();
                            gamePack.waitForAck(allegro);
                        }
                    }
                }
            } else if (modeEnum == REMOVESAND) {
                if (tilesDeck[character->getCardIndex() + 5]->haveSand()) {
                    if (character->removeSand()) {
                        tilesDeck[character->getCardIndex() + 5]->removeSand();
                        if (ArcheologistCharacter *explorerCharacter = dynamic_cast<ArcheologistCharacter *>(character)) {
                            if (tilesDeck[character->getCardIndex() + 5]->haveSand()) {
                                tilesDeck[character->getCardIndex() + 5]->removeSand();
                                SandMarkersLeft++;
                                cout << "Sand Marker ++" << endl;
                            }
                        }
                        gamePack.remove(character->getRow(), character->getColumn() + 1);
                        gamePack.sendPack();
                        gamePack.waitForAck(allegro);
                        SandMarkersLeft++;
                        cout << "Sand Marker ++" << endl;
                    }
                }
            } else if (modeEnum == DUNEBLASTERMODE) {
                while (tilesDeck[character->getCardIndex() + 5]->haveSand()) {
                    tilesDeck[character->getCardIndex() + 5]->removeSand();
                    SandMarkersLeft++;
                    cout << "Sand Marker ++" << endl;
                }
                gamePack.playequipment(DUNEBLAST, character->getRow(), character->getColumn() + 1);
                gamePack.sendPack();
                gamePack.waitForAck(allegro);
            }
        }
    } else {
        if (NavigatorCharacter *navigatorCharacter = dynamic_cast<NavigatorCharacter *>(character)) {
            if (character2->getCardIndex() < tilesDeck.size() - 5) {
                if (navigatorCharacter->havePartnersMovesLeft()) {
                    bool climber = false;
                    if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character2)) { climber = true; }
                    if (tilesDeck[character2->getCardIndex() + 5]->canIMove(climber)) { //If I'm not on top... move up
                        if (tilesDeck[character2->getCardIndex()]->canIMove(climber)) {
                            if (character2->moveRight() && navigatorCharacter->decresePartnerMoves()) {
                                if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character2)) {
                                    if (climberCharacter->getTakeWithMe() &&
                                        character->getCardIndex() + 5 == character2->getCardIndex()) {
                                        character->moveRight();
                                    }
                                }
                                gamePack.requestmove(character2->getRow(), character2->getColumn());
                                gamePack.sendPack();
                                gamePack.waitForAck(allegro);
                            } else { cout << "Error: no moves left even though I was supposed to have" << endl; }
                        }
                    }
                }
            }
        } else { cout << "Error: Move Other mode without being a navigator character" << endl; }
    }
}
void Game::keyLeft(ALL *allegro) {
    if (modeEnum == OFFEREQUIPMENTMODE || modeEnum == REQUESTEQUIPMENTMODE) { previousEquipCard(); }
    else if (modeEnum == PEEPMODE) { previousPeepStormCard(); }

    if (modeEnum != MOVEOTHER) {
        if (character->getCardIndex() > 4) {
            if (modeEnum == NORMAL) {
                bool climber = false;
                if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character)) { climber = true; }
                if (tilesDeck[character->getCardIndex() - 5]->canIMove(climber)) {
                    if (tilesDeck[character->getCardIndex()]->canIMove(climber)) {
                        if (character->moveLeft()) {
                            if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character)) {
                                if (climberCharacter->getTakeWithMe() &&
                                    character2->getCardIndex() - 5 == character->getCardIndex()) {
                                    character2->moveLeft();
                                    gamePack.requestmove(character2->getRow(), character2->getColumn());
                                    gamePack.sendPack();
                                    gamePack.waitForAck(allegro);
                                    usleep(100);
                                    gamePack.move(character->getRow(), character->getColumn());
                                    gamePack.sendPack();
                                    gamePack.waitForAck(allegro);
                                }
                            }
                            gamePack.move(character->getRow(), character->getColumn());
                            gamePack.sendPack();
                            gamePack.waitForAck(allegro);
                        }
                    }
                }
            } else if (modeEnum == REMOVESAND) {
                if (tilesDeck[character->getCardIndex() - 5]->haveSand()) {
                    if (character->removeSand()) {
                        tilesDeck[character->getCardIndex() - 5]->removeSand();
                        if (ArcheologistCharacter *explorerCharacter = dynamic_cast<ArcheologistCharacter *>(character)) {
                            if (tilesDeck[character->getCardIndex() - 5]->haveSand()) {
                                tilesDeck[character->getCardIndex() - 5]->removeSand();
                                SandMarkersLeft++;
                                cout << "Sand Marker ++" << endl;
                            }
                        }
                        SandMarkersLeft++;
                        cout << "Sand Marker ++" << endl;
                        gamePack.remove(character->getRow(), character->getColumn() - 1);
                        gamePack.sendPack();
                        gamePack.waitForAck(allegro);
                    }
                }
            } else if (modeEnum == DUNEBLASTERMODE) {
                while (tilesDeck[character->getCardIndex() - 5]->haveSand()) {
                    tilesDeck[character->getCardIndex() - 5]->removeSand();
                    SandMarkersLeft++;
                    cout << "Sand Marker ++" << endl;
                }
                gamePack.playequipment(DUNEBLAST, character->getRow(), character->getColumn() - 1);
                gamePack.sendPack();
                gamePack.waitForAck(allegro);
            }
        }
    } else {
        if (NavigatorCharacter *navigatorCharacter = dynamic_cast<NavigatorCharacter *>(character)) {
            if (character2->getCardIndex() > 4) {
                if (navigatorCharacter->havePartnersMovesLeft()) {
                    bool climber = false;
                    if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character2)) { climber = true; }
                    if (tilesDeck[character2->getCardIndex() - 5]->canIMove(climber)) {
                        if (tilesDeck[character2->getCardIndex()]->canIMove(climber)) {
                            if (character2->moveLeft() && navigatorCharacter->decresePartnerMoves()) {
                                if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character2)) {
                                    if (climberCharacter->getTakeWithMe() &&
                                        character->getCardIndex() - 5 == character2->getCardIndex()) {
                                        character->moveLeft();
                                    }
                                }
                                gamePack.requestmove(character2->getRow(), character2->getColumn());
                                gamePack.sendPack();
                                gamePack.waitForAck(allegro);
                            }
                        }
                    }
                }
            }
        } else { cout << "Error: Move Other mode without being a navigator character" << endl; }
    }
}
void Game::keyE(ALL *allegro) {
    if (modeEnum == NORMAL) {
        if (!(tilesDeck[character->getCardIndex()]->isTurned())) {
            if (!tilesDeck[character->getCardIndex()]->haveSand()) {
                if (character->dig()) {
                    al_play_sample(allegro->excavateSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                    tilesDeck[character->getCardIndex()]->turnTile();
                    performTileAction(tilesDeck[character->getCardIndex()]->gettileType(), character->getCardIndex(),
                                      allegro);
                    gamePack.excavate();
                    gamePack.sendPack();
                    gamePack.waitForAck(allegro);
                }
            }
        }
    }
}
void Game::keyP(ALL *allegro) {
    if (modeEnum == NORMAL) {
        pickUpAction();
        gamePack.pickup();
        gamePack.sendPack();
        gamePack.waitForAck(allegro);
    }
}
void Game::keyR(ALL *allegro) {
    if (modeEnum == REMOVESAND) {
        if (tilesDeck[character->getCardIndex()]->haveSand()) {
            if (character->removeSand()) {
                if (ArcheologistCharacter *explorerCharacter = dynamic_cast<ArcheologistCharacter *>(character)) {
                    if (tilesDeck[character->getCardIndex()]->haveSand()) {
                        tilesDeck[character->getCardIndex()]->removeSand();
                        SandMarkersLeft++;
                        cout << "Sand Marker ++" << endl;
                    }
                }
                tilesDeck[character->getCardIndex()]->removeSand();
                SandMarkersLeft++;
                cout << "Sand Marker ++" << endl;
                gamePack.remove(character->getRow(), character->getColumn());
                gamePack.sendPack();
                gamePack.waitForAck(allegro);
            }
        }
    }
}
void Game::keyEscape() {
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
    }
}
void Game::chooseOption(ALL *allegro, Container temp) {
    ALLEGRO_EVENT ev;
    ALLEGRO_EVENT_QUEUE *tempQueue;
    Button *disagreeButton, *agreeButton;
    if (tempQueue = al_create_event_queue()) {
        al_register_event_source(tempQueue, al_get_mouse_event_source());
        if (ALLEGRO_BITMAP *offsetImage = al_load_bitmap_resized("Resources/Buttons/agreeButton.png", allegro)) {
            disagreeButton = new Button((int) allegro->screenWidth * 0.55, (int) allegro->screenHeight * 0.52,
                                        "Resources/Buttons/disagreeButton.png",
                                        "Resources/Buttons/disagreeButtonSelected.png", allegro);
            agreeButton = new Button((int) allegro->screenWidth * 0.45 - al_get_bitmap_width(offsetImage),
                                     (int) allegro->screenHeight * 0.52, "Resources/Buttons/agreeButton.png",
                                     "Resources/Buttons/agreeButtonSelected.png", allegro);
            al_destroy_bitmap(offsetImage);
            al_draw_bitmap(allegro->menuBackgroundImage, 0, 0, 0);
            switch (temp.ID) {
                case OFFER_WATER:
                case REQUEST_WATER:
                    if (temp.ID == OFFER_WATER)
                        al_draw_textf(allegro->font, al_map_rgb(WHITE), 0.5 * allegro->screenWidth,
                                      0.45 * allegro->screenHeight, ALLEGRO_ALIGN_CENTRE, "Offer %d waters",
                                      temp.Slot1);
                    else
                        al_draw_textf(allegro->font, al_map_rgb(WHITE), 0.5 * allegro->screenWidth,
                                      0.45 * allegro->screenHeight, ALLEGRO_ALIGN_CENTRE, "Request %d waters",
                                      temp.Slot1);
                    break;
                case OFFER_EQUIPMENT:
                case REQUEST_EQUIPMENT:
                    switch (temp.Slot1) {
                        case DUNEBLAST:
                            if (temp.ID == OFFER_EQUIPMENT)
                                al_draw_text(allegro->font, al_map_rgb(WHITE), 0.5 * allegro->screenWidth,
                                             0.45 * allegro->screenHeight, ALLEGRO_ALIGN_CENTRE, "Offer a duneblast");
                            else
                                al_draw_text(allegro->font, al_map_rgb(WHITE), 0.5 * allegro->screenWidth,
                                             0.45 * allegro->screenHeight, ALLEGRO_ALIGN_CENTRE, "Request a duneblast");
                            break;
                        case JETPACK:
                            if (temp.ID == OFFER_EQUIPMENT)
                                al_draw_text(allegro->font, al_map_rgb(WHITE), 0.5 * allegro->screenWidth,
                                             0.45 * allegro->screenHeight, ALLEGRO_ALIGN_CENTRE, "Offer a jetpack");
                            else
                                al_draw_text(allegro->font, al_map_rgb(WHITE), 0.5 * allegro->screenWidth,
                                             0.45 * allegro->screenHeight, ALLEGRO_ALIGN_CENTRE, "Request a jetpack");
                            break;
                        case SOLARSHIELD:
                            if (temp.ID == OFFER_EQUIPMENT)
                                al_draw_text(allegro->font, al_map_rgb(WHITE), 0.5 * allegro->screenWidth,
                                             0.45 * allegro->screenHeight, ALLEGRO_ALIGN_CENTRE, "Offer a solarshield");
                            else
                                al_draw_text(allegro->font, al_map_rgb(WHITE), 0.5 * allegro->screenWidth,
                                             0.45 * allegro->screenHeight, ALLEGRO_ALIGN_CENTRE,
                                             "Request a solarshield");
                            break;
                        case TERRASCOPE:
                            if (temp.ID == OFFER_EQUIPMENT)
                                al_draw_text(allegro->font, al_map_rgb(WHITE), 0.5 * allegro->screenWidth,
                                             0.45 * allegro->screenHeight, ALLEGRO_ALIGN_CENTRE, "Offer a terrascope");
                            else
                                al_draw_text(allegro->font, al_map_rgb(WHITE), 0.5 * allegro->screenWidth,
                                             0.45 * allegro->screenHeight, ALLEGRO_ALIGN_CENTRE,
                                             "Request a terrascope");
                            break;
                        case SECRETWATERRESERVE:
                            if (temp.ID == OFFER_EQUIPMENT)
                                al_draw_text(allegro->font, al_map_rgb(WHITE), 0.5 * allegro->screenWidth,
                                             0.45 * allegro->screenHeight, ALLEGRO_ALIGN_CENTRE,
                                             "Offer a secret water reserve");
                            else
                                al_draw_text(allegro->font, al_map_rgb(WHITE), 0.5 * allegro->screenWidth,
                                             0.45 * allegro->screenHeight, ALLEGRO_ALIGN_CENTRE,
                                             "Request a secret water reserve");
                            break;
                        case TIMETHROTTLE:
                            if (temp.ID == OFFER_EQUIPMENT)
                                al_draw_text(allegro->font, al_map_rgb(WHITE), 0.5 * allegro->screenWidth,
                                             0.45 * allegro->screenHeight, ALLEGRO_ALIGN_CENTRE,
                                             "Offer a time throttle");
                            else
                                al_draw_text(allegro->font, al_map_rgb(WHITE), 0.5 * allegro->screenWidth,
                                             0.45 * allegro->screenHeight, ALLEGRO_ALIGN_CENTRE,
                                             "Request a time throttle");
                            break;
                    }
                    break;
            }

            bool run = true;
            while (run) {
                agreeButton->updateButton(allegro);
                disagreeButton->updateButton(allegro);
                al_flip_display();
                al_wait_for_event(tempQueue, &ev);
                if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
                    mouse.x = ev.mouse.x;
                    mouse.y = ev.mouse.y;
                    agreeButton->checkMouse(mouse.x, mouse.y, allegro->volume);
                    disagreeButton->checkMouse(mouse.x, mouse.y, allegro->volume);
                }
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                    if (agreeButton->checkMouse(mouse.x, mouse.y, allegro->volume)) {
                        modeEnum = NORMAL;
                        gamePack.agree();
                        gamePack.sendPack();
                        run = false;
                        if (temp.ID == OFFER_WATER) {
                            for (int i = 1; i <= temp.Slot1; i++) {
                                character2->drinkWater();
                                character->addWater();
                            }
                        } else if (temp.ID == REQUEST_WATER) {
                            for (int i = 1; i <= temp.Slot1; i++) {
                                character->drinkWater();
                                character2->addWater();
                            }
                        } else if (temp.ID == OFFER_EQUIPMENT || temp.ID == REQUEST_EQUIPMENT) {
                            switch (temp.Slot1) {
                                case DUNEBLAST:
                                    if (temp.ID == OFFER_EQUIPMENT) {
                                        character->increaseDuneBlaster();
                                        character2->decreaseDuneBlaster();
                                    } else {
                                        character2->increaseDuneBlaster();
                                        character->decreaseDuneBlaster();
                                    }
                                    break;
                                case JETPACK:
                                    if (temp.ID == OFFER_EQUIPMENT) {
                                        character->increaseJetPack();
                                        character2->decreaseJetPack();
                                    } else {
                                        character2->increaseJetPack();
                                        character->decreaseJetPack();
                                    }
                                    break;
                                case SOLARSHIELD:
                                    if (temp.ID == OFFER_EQUIPMENT) {
                                        character->increaseSolarShield();
                                        character2->decreaseSolarShield();
                                    } else {
                                        character2->increaseSolarShield();
                                        character->decreaseSolarShield();
                                    }
                                    break;
                                case TERRASCOPE:
                                    if (temp.ID == OFFER_EQUIPMENT) {
                                        character->increaseTerrascope();
                                        character2->decreaseTerrascope();
                                    } else {
                                        character2->increaseTerrascope();
                                        character->decreaseTerrascope();
                                    }
                                    break;
                                case SECRETWATERRESERVE:
                                    if (temp.ID == OFFER_EQUIPMENT) {
                                        character->increaseWaterReserve();
                                        character2->decreaseWaterReserve();
                                    } else {
                                        character2->increaseWaterReserve();
                                        character->decreaseWaterReserve();
                                    }
                                    break;
                                case TIMETHROTTLE:
                                    if (temp.ID == OFFER_EQUIPMENT) {
                                        character->increaseTimeThrottle();
                                        character2->decreaseTimeThrottle();
                                    } else {
                                        character2->increaseTimeThrottle();
                                        character->decreaseTimeThrottle();
                                    }
                                    break;
                            }
                        }
                    } else if (disagreeButton->checkMouse(mouse.x, mouse.y, allegro->volume)) {
                        modeEnum = NORMAL;
                        if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character)) {
                            al_play_sample(allegro->rejectWaterBoySound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE,
                                           NULL);    //TODO: HAY FORMA DE SABER SI ES WATER O EQUIP?
                        }
                        if (ArcheologistCharacter *archeologistWater = dynamic_cast<ArcheologistCharacter *>(character)) {
                            al_play_sample(allegro->rejectWaterGirlSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE,
                                           NULL);
                        }
                        if (WaterCarrierCharacter *waterCarrierCharacter = dynamic_cast<WaterCarrierCharacter *>(character)) {
                            al_play_sample(allegro->rejectWaterGirlSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE,
                                           NULL);
                        }
                        if (NavigatorCharacter *navigatorCharacter = dynamic_cast<NavigatorCharacter *>(character)) {
                            al_play_sample(allegro->rejectWaterGirlSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE,
                                           NULL);
                        }
                        if (ExplorerCharacter *explorerCharacter = dynamic_cast<ExplorerCharacter *>(character)) {
                            al_play_sample(allegro->rejectWaterBoySound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE,
                                           NULL);
                        }
                        if (MeteorologistCharacter *meteorologistCharacter = dynamic_cast<MeteorologistCharacter *>(character)) {
                            al_play_sample(allegro->rejectWaterBoySound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE,
                                           NULL);
                        }
                        gamePack.disagree();
                        gamePack.sendPack();
                        run = false;
                    }
                }
            }
            //al_destroy_event_queue(tempQueue);
        } else { cout << "Couldn't load offsetImage in chooseOption" << endl; }
        al_destroy_event_queue(tempQueue);
        delete agreeButton;
        delete disagreeButton;
    } else { cout << "Could't initialize tempQueue" << endl; }
}
//Mouse Methods
void Game::click(ALL *allegro) {
    /* Runs the click depending in which state i'm in */
    //Volume Buttons
    if (allegro->volume == 0) {
        if (muteButton->checkMouse(mouse.x, mouse.y, allegro->volume)) {
            allegro->volume = 1;
            al_play_sample(allegro->backgroundMusic, allegro->volume / 2, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP,
                           &allegro->backgroundMusicID);
        }
    } else {
        if (noMuteButton->checkMouse(mouse.x, mouse.y, allegro->volume)) {
            allegro->volume = 0;
            al_stop_sample(&allegro->backgroundMusicID);
        }
    }
    if (turn) {
        switch (modeEnum) {
            case PEEPMODE:
                clickOnPeepMode(allegro);
                break;
            case JETPACKMODE:
                clickOnJetPackMode(allegro);
                break;
            case TERRASCOPEMODE:
                clickOnTerrascopeMode(allegro);
                break;
            case OFFEREQUIPMENTMODE:
                clickOnOfferEquipmentMode(allegro);
                break;
            case REQUESTEQUIPMENTMODE:
                clickOnRequestEquipmentMode(allegro);
                break;
            case OFFERWATERMODE:
                clickOnOfferWaterMode(allegro);
                break;
            case REQUESTWATERMODE:
                clickOnRequestWaterMode(allegro);
                break;
            default:
                clickOnDefaultMode(allegro);
        }
    }
}
void Game::clickOnPeepMode(ALL *allegro) {
    /* Click while in peep mode */
    if (MeteorologistCharacter *meteorologistCharacter = dynamic_cast<MeteorologistCharacter *>(character)) {
        if (nextButton->checkMouse(mouse.x, mouse.y, allegro->volume)) { nextPeepStormCard(); }
        if (previousButton->checkMouse(mouse.x, mouse.y, allegro->volume)) { previousPeepStormCard(); }
        if (selectPeepCard->checkMouse(mouse.x, mouse.y, allegro->volume)) {
            for (int i = 0; i < peepOffset; i++) {
                swapStormCards(peepOffset - i - 1, peepOffset - i);
            } //Move the card to the front by saping with each card
            std::rotate(stormCardsDeck.begin(), stormCardsDeck.begin() + 1,
                        stormCardsDeck.end());        //Rotate to take it to the bottom.
            //meteorologistCharacter->peep();
            gamePack.peep(peepOffset);
            gamePack.sendPack();
            gamePack.waitForAck(allegro);
            modeEnum = NORMAL;
            resetPeepOffset();
        }
    } else { cout << "Enter in 'click on peep mode' without being an meteorologist" << endl; }
}
void Game::clickOnJetPackMode(ALL *allegro) {
    for (int i = 0; i < TILEDECKNUMBER; i++) {
        bool climber = false;
        //The climber's ability doesn't affect movement by Jetpack.
        //if (ClimberCharacter* climberCharacter = dynamic_cast<ClimberCharacter*>(character)) { climber = true; }
        if (tilesDeck[i]->canIMove(false)) {
            if (tilesDeck[i]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                character->setPos(i % 5, i / 5);
                character->increaseMoves();
                al_play_sample(allegro->teleportSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                //Polonet
                gamePack.playequipment(JETPACK, character->getRow(), character->getColumn());
                gamePack.sendPack();
                gamePack.waitForAck(allegro);
                modeEnum = NORMAL;
            }
        }
    }
}
void Game::clickOnTerrascopeMode(ALL *allegro) {
    for (int i = 0; i < TILEDECKNUMBER; i++) {
        if (tilesDeck[i]->checkButton(mouse.x, mouse.y, allegro->volume)) {
            int backup = tilesDeck[i]->getSand();
            tilesDeck[i]->turnTerrascopeTile();
            tilesDeck[i]->updateTile(allegro);
            al_flip_display();
            gamePack.playequipment(TERRASCOPE, i % 5, i / 5);
            gamePack.sendPack();
            usleep(3500);
            tilesDeck[i]->setSand(backup);
            tilesDeck[i]->setTurned(false);
            //Polonet
            gamePack.waitForAck(allegro);
            modeEnum = NORMAL;
        }
    }
}
void Game::clickOnOfferWaterMode(ALL *allegro) {
    for (int i = 0; i < min(character->getWaters(), MAXWATERS - character2->getWaters() - 1); i++) {
        if (watersButton[i]->checkMouse(mouse.x, mouse.y, allegro->volume)) {  // && character->getWaters() > (i + 1)) {
            modeEnum = NORMAL;
            updateScreen(allegro);
            cout << "Send Offer Water" << endl;
            gamePack.offerwater(i + 1);// "+1" Because the array starts with 0.
            gamePack.sendPack();
            char wait;
            do {
                wait = gamePack.receivePack().ID;
                gamePack.cleanBuffer();
            } while (wait != AGREE && wait != DISAGREE);
            if (wait == AGREE) {
                for (int c = 0; c < i + 1; c++) {
                    character->drinkWater();
                    character2->addWater();
                }
            } else { //if disagree, play the not thirsty sound
                if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character2)) {
                    al_play_sample(allegro->rejectWaterBoySound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE,
                                   NULL);    //TODO: HAY FORMA DE SABER SI ES WATER O EQUIP?
                }
                if (ArcheologistCharacter *archeologistWater = dynamic_cast<ArcheologistCharacter *>(character2)) {
                    al_play_sample(allegro->rejectWaterGirlSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
                if (WaterCarrierCharacter *waterCarrierCharacter = dynamic_cast<WaterCarrierCharacter *>(character2)) {
                    al_play_sample(allegro->rejectWaterGirlSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
                if (NavigatorCharacter *navigatorCharacter = dynamic_cast<NavigatorCharacter *>(character2)) {
                    al_play_sample(allegro->rejectWaterGirlSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
                if (ExplorerCharacter *explorerCharacter = dynamic_cast<ExplorerCharacter *>(character2)) {
                    al_play_sample(allegro->rejectWaterBoySound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
                if (MeteorologistCharacter *meteorologistCharacter = dynamic_cast<MeteorologistCharacter *>(character2)) {
                    al_play_sample(allegro->rejectWaterBoySound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }
        }
    }
}
void Game::clickOnRequestWaterMode(ALL *allegro) {
    for (int i = 0; i < MAXWATERS; i++) {
        if (watersButton[i]->checkMouse(mouse.x, mouse.y, allegro->volume) && character2->getWaters() > (i + 1)) {
            modeEnum = NORMAL;
            updateScreen(allegro);
            cout << "Send Request Water" << endl;
            gamePack.requestwater(i + 1);// "+1" Because the array starts with 0.
            gamePack.sendPack();
            //Polonet
            char wait;
            //al_start_timer(allegro->timeout);
            do {
                wait = gamePack.receivePack().ID;
                gamePack.cleanBuffer();
            } while (wait != AGREE && wait != DISAGREE);
            if (wait == AGREE) {
                for (int c = 0; c < i + 1; c++) {
                    character2->drinkWater();
                    character->addWater();
                }
            }
        }
    }
}
void Game::clickOnOfferEquipmentMode(ALL *allegro) {
    if (nextButton->checkMouse(mouse.x, mouse.y, allegro->volume)) { nextEquipCard(); }
    if (previousButton->checkMouse(mouse.x, mouse.y, allegro->volume)) { previousEquipCard(); }
    if (selectEquipmentCard->checkMouse(mouse.x, mouse.y, allegro->volume)) {
        modeEnum = NORMAL;
        updateScreen(allegro);
        gamePack.offerequipment(equipOffset);
        gamePack.sendPack();
        char wait;
        do {
            wait = gamePack.receivePack().ID;
            gamePack.cleanBuffer();
        } while (wait != AGREE && wait != DISAGREE);
        if (wait == AGREE) {
            switch (equipOffset) {
                case DUNEBLAST:
                    character->decreaseDuneBlaster();
                    character2->increaseDuneBlaster();
                    break;
                case JETPACK:
                    character->decreaseJetPack();
                    character2->increaseJetPack();
                    break;
                case SOLARSHIELD:
                    character->decreaseSolarShield();
                    character2->increaseSolarShield();
                    break;
                case TERRASCOPE:
                    character->decreaseTerrascope();
                    character2->increaseTerrascope();
                    break;
                case SECRETWATERRESERVE:
                    character->decreaseWaterReserve();
                    character2->increaseWaterReserve();
                    break;
                case TIMETHROTTLE:
                    character->decreaseTimeThrottle();
                    character2->increaseTimeThrottle();
                    break;
            }
        } else {
            if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character2)) {
                al_play_sample(allegro->rejectEquipmentBoySound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE,
                               NULL);    //TODO: HAY FORMA DE SABER SI ES WATER O EQUIP?
            }
            if (ArcheologistCharacter *archeologistWater = dynamic_cast<ArcheologistCharacter *>(character2)) {
                al_play_sample(allegro->rejectEquipmentGirlSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            if (WaterCarrierCharacter *waterCarrierCharacter = dynamic_cast<WaterCarrierCharacter *>(character2)) {
                al_play_sample(allegro->rejectEquipmentGirlSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            if (NavigatorCharacter *navigatorCharacter = dynamic_cast<NavigatorCharacter *>(character2)) {
                al_play_sample(allegro->rejectEquipmentGirlSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            if (ExplorerCharacter *explorerCharacter = dynamic_cast<ExplorerCharacter *>(character2)) {
                al_play_sample(allegro->rejectEquipmentBoySound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            if (MeteorologistCharacter *meteorologistCharacter = dynamic_cast<MeteorologistCharacter *>(character2)) {
                al_play_sample(allegro->rejectEquipmentBoySound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
        }
    }
}
void Game::clickOnRequestEquipmentMode(ALL *allegro) {
    if (nextButton->checkMouse(mouse.x, mouse.y, allegro->volume)) { nextEquipCard(); }
    if (previousButton->checkMouse(mouse.x, mouse.y, allegro->volume)) { previousEquipCard(); }
    if (selectEquipmentCard->checkMouse(mouse.x, mouse.y, allegro->volume)) {
        modeEnum = NORMAL;
        updateScreen(allegro);
        gamePack.requestequipment(equipOffset);
        gamePack.sendPack();
        char wait;
        do {
            wait = gamePack.receivePack().ID;
            gamePack.cleanBuffer();
        } while (wait != AGREE && wait != DISAGREE);
        if (wait == AGREE) {
            switch (equipOffset) {
                case DUNEBLAST:
                    character2->decreaseDuneBlaster();
                    character->increaseDuneBlaster();
                    break;
                case JETPACK:
                    character2->decreaseJetPack();
                    character->increaseJetPack();
                    break;
                case SOLARSHIELD:
                    character2->decreaseSolarShield();
                    character->increaseSolarShield();
                    break;
                case TERRASCOPE:
                    character2->decreaseTerrascope();
                    character->increaseTerrascope();
                    break;
                case SECRETWATERRESERVE:
                    character2->decreaseWaterReserve();
                    character->increaseWaterReserve();
                    break;
                case TIMETHROTTLE:
                    character2->decreaseTimeThrottle();
                    character->increaseTimeThrottle();
                    break;
            }
        }
    }
}
void Game::clickOnDefaultMode(ALL *allegro) {
    clickModesButtons(allegro);
    clickEquipmentButtons(allegro);
    clickTilesButtons(allegro);
    /*if (tilesDeck[character->getCardIndex()]->checkButton(mouse.x, mouse.y, allegro->volume)) {
		keyR(allegro);
		modeEnum = NORMAL;
	}*/
    if (endTurnButton->checkMouse(mouse.x, mouse.y,
                                  allegro->volume)) {            //If there whas a selected character, then return true, else return false
        al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        gamePack.pass();
        gamePack.sendPack();
        gamePack.waitForAck(allegro);
        drawStormCards(allegro);
        character->resetNumberOfMoves();        //This has to go after draw cards. Because if you are a meteorologist you may be able to draw fewer cards.
        character2->resetNumberOfMoves();
        turn = !turn;
        character2->shield = false;
    }
    if (MeteorologistCharacter *meteorologistCharacter = dynamic_cast<MeteorologistCharacter *>(character)) {
        if (meteorologistCharacter->checkMouseOverButton(mouse.x, mouse.y, allegro->volume)) {
            al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            cout << "PEEP" << endl;
            if (meteorologistCharacter->peep()) {
                modeEnum = PEEPMODE;
                peepOffset = 0;
                firstTimeUpdateScreenPeep = 0;
            }
        }
    } else if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character)) {
        climberCharacter->clickOnButton(mouse.x, mouse.y, allegro->volume);
    } else if (WaterCarrierCharacter *waterCarrierCharacter = dynamic_cast<WaterCarrierCharacter *>(character)) {
        if (tilesDeck[character->getCardIndex()]->gettileType() == WATER &&
            tilesDeck[character->getCardIndex()]->isTurned()) {
            if (waterCarrierCharacter->checkMouseOverButton(mouse.x, mouse.y, allegro->volume)) {
                al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                waterCarrierCharacter->getWatersFromWell();
                al_play_sample(allegro->waterSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                gamePack.takewater();
                gamePack.sendPack();
                gamePack.waitForAck(allegro);
            }
        }
    } else if (NavigatorCharacter *navigatorCharacter = dynamic_cast<NavigatorCharacter *>(character)) {
        if (navigatorCharacter->checkMouseOverButton(mouse.x, mouse.y, modeEnum == MOVEOTHER, allegro->volume)) {
            cout << "Click on special navigator button" << endl;
            al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            if (modeEnum == MOVEOTHER) {
                modeEnum = NORMAL;
                character2->resetNumberOfMoves();
                navigatorCharacter->decreseMoves();
            } else {
                if (navigatorCharacter->haveMoves()) {
                    modeEnum = MOVEOTHER;
                    navigatorCharacter->resetPartnerMoves();
                    character2->resetNumberOfMoves();    //I reset them to be able to move him
                }
            }
        }
    }
}
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
void Game::clickModesButtons(ALL *allegro) {
    if (modesButtons[REMOVEBUTTON]->checkMouse(mouse.x, mouse.y, allegro->volume)) {
        cout << "Remove Sand Button Clicked" << endl;
        al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        if (modeEnum == REMOVESAND) {
            if (tilesDeck[character->getCardIndex()]->haveSand()) {
                if (character->removeSand()) {
                    tilesDeck[character->getCardIndex()]->removeSand();
                    if (ArcheologistCharacter *explorerCharacter = dynamic_cast<ArcheologistCharacter *>(character)) {
                        if (tilesDeck[character->getCardIndex()]->haveSand()) {
                            tilesDeck[character->getCardIndex()]->removeSand();
                            SandMarkersLeft++;
                            cout << "Sand Marker ++" << endl;
                        }
                    }
                    SandMarkersLeft++;
                    cout << "Sand Marker ++" << endl;
                    gamePack.remove(character->getRow(), character->getColumn());
                    gamePack.sendPack();
                    gamePack.waitForAck(allegro);
                }
            }
            modeEnum = NORMAL;
        } else { modeEnum = REMOVESAND; }
    } else if (modesButtons[EXCAVATEBUTTON]->checkMouse(mouse.x, mouse.y,
                                                        allegro->volume)) {    //I use else if because it will take less time for the computer to check the buttons
        cout << "Excavate Button Clicked" << endl;
        al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        keyE(allegro);
        modeEnum = NORMAL;
    } else if (modesButtons[PICKUPBUTTON]->checkMouse(mouse.x, mouse.y, allegro->volume)) {
        cout << "Pick Up Part Button Clicked" << endl;
        al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        keyP(allegro);
        modeEnum = NORMAL;
    } else if (modesButtons[OFFERWBUTTON]->checkMouse(mouse.x, mouse.y, allegro->volume)) {
        cout << "Offer Water Button Clicked" << endl;
        al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        if (WaterCarrierCharacter *waterCarrierCharacter = dynamic_cast<WaterCarrierCharacter *>(character)) {
            if (character->getRow() == character2->getRow()) {
                if (character2->getColumn() >= character->getColumn() - 1 &&
                    character2->getColumn() <= character->getColumn() + 1) {
                    if (min(character->getWaters(), MAXWATERS - character2->getWaters() - 1) !=
                        0) { modeEnum = OFFERWATERMODE; }
                }
            } else if (character->getColumn() == character2->getColumn()) {
                if (character2->getColumn() == character->getColumn() - 1 ||
                    character2->getColumn() == character->getColumn() + 1) {
                    if (min(character->getWaters(), MAXWATERS - character2->getWaters() - 1) !=
                        0) { modeEnum = OFFERWATERMODE; }
                }
            }
        } else if (character->getRow() == character2->getRow() && character->getColumn() == character2->getColumn()) {
            if (min(character->getWaters(), MAXWATERS - character2->getWaters() - 1) !=
                0) { modeEnum = OFFERWATERMODE; }
        }
    } else if (modesButtons[OFFEREBUTTON]->checkMouse(mouse.x, mouse.y, allegro->volume)) {
        cout << "Offer Equipment Button Clicked" << endl;
        al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        //if the player does not have any equipment...
        if (character->getCardIndex() == character2->getCardIndex()) {
            if (character->haveDuneBlaster() || character->haveJetPack() || character->haveSolarShield() ||
                character->haveTerrascope() || character->haveTimeThrottle()) {
                modeEnum = OFFEREQUIPMENTMODE;
                firstTimeUpdateScreenChooseEquipment = 0;
            }
        }
    } else if (modesButtons[REQUESTWBUTTON]->checkMouse(mouse.x, mouse.y, allegro->volume)) {
        al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        if (WaterCarrierCharacter *waterCarrierCharacter = dynamic_cast<WaterCarrierCharacter *>(character2)) {
            if (character->getRow() == character2->getRow()) {
                if (character->getColumn() >= character2->getColumn() - 1 &&
                    character->getColumn() <= character2->getColumn() + 1) {
                    if (min(character2->getWaters(), MAXWATERS - character->getWaters() - 1) !=
                        0) { modeEnum = REQUESTWATERMODE; }
                }
            } else if (character->getColumn() == character2->getColumn()) {
                if (character2->getColumn() == character->getColumn() - 1 ||
                    character2->getColumn() == character->getColumn() + 1) {
                    if (min(character2->getWaters(), MAXWATERS - character->getWaters() - 1) !=
                        0) { modeEnum = REQUESTWATERMODE; }
                }
            }
        } else if (character->getColumn() == character2->getColumn() && character->getRow() == character2->getRow()) {
            if (min(character2->getWaters(), MAXWATERS - character->getWaters() - 1) !=
                0) { modeEnum = REQUESTWATERMODE; }
        }
    } else if (modesButtons[REQUESTEBUTTON]->checkMouse(mouse.x, mouse.y, allegro->volume)) {
        al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        if (character->getCardIndex() == character2->getCardIndex()) {
            if (character2->haveDuneBlaster() || character2->haveJetPack() || character2->haveSolarShield() ||
                character2->haveTerrascope() || character2->haveTimeThrottle()) {
                modeEnum = REQUESTEQUIPMENTMODE;
                firstTimeUpdateScreenChooseEquipment = 0;
            }
        }
    }
}
void Game::checkTilesButtons(double volume) { //TODO: cuando el jetPack actualiza todos los tiles tendría que ser acá.
    int tempIndex;
    bool climber = false;
    if (modeEnum == MOVEOTHER) {
        tempIndex = character2->getCardIndex();
        if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character2)) { climber = true; }
    } else {
        tempIndex = character->getCardIndex();
        if (turn && (tilesDeck[tempIndex]->haveSand() || !tilesDeck[tempIndex]->isTurned())) {
            tilesDeck[tempIndex]->checkButton(mouse.x, mouse.y, volume);
        }
        if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character)) { climber = true; }
    }
    if (turn && tilesDeck[tempIndex]->canIMove(climber)) {
        if (modeEnum == NORMAL || modeEnum == MOVEOTHER) {
            if (tempIndex > 0) {
                if (tilesDeck[tempIndex - 1]->canIMove(climber) && tempIndex % 5 != 0) {
                    tilesDeck[tempIndex - 1]->checkButton(mouse.x, mouse.y, volume);
                }
            }
            if (tempIndex < TILEDECKNUMBER - 1) {
                if (tilesDeck[tempIndex + 1]->canIMove(climber) && tempIndex % 5 != 4) {
                    tilesDeck[tempIndex + 1]->checkButton(mouse.x, mouse.y, volume);
                }
            }
            if (tempIndex > 4) {
                if (tilesDeck[tempIndex - 5]->canIMove(climber)) {
                    tilesDeck[tempIndex - 5]->checkButton(mouse.x, mouse.y, volume);
                }
            }
            if (tempIndex < TILEDECKNUMBER - 5) {
                if (tilesDeck[tempIndex + 5]->canIMove(climber)) {
                    tilesDeck[tempIndex + 5]->checkButton(mouse.x, mouse.y, volume);
                }
            }
            //Tunnel Tile
            if (tilesDeck[tempIndex]->gettileType() == TUNNEL && tilesDeck[tempIndex]->isTurned()) {
                for (uint i = 0; i < tilesDeck.size(); i++) {
                    if (tilesDeck[i]->gettileType() == TUNNEL && i != tempIndex && tilesDeck[i]->isTurned()) {
                        if (tilesDeck[i]->canIMove(climber)) { tilesDeck[i]->checkButton(mouse.x, mouse.y, volume); }
                    }
                }
            }
            // Explorer Move Diagonally
            if (modeEnum == NORMAL) {
                if (ExplorerCharacter *explorerCharacter = dynamic_cast<ExplorerCharacter *>(character)) { //TODO: fix this for character2
                    if (tempIndex > 5 && tempIndex % 5 != 0) {
                        if (tilesDeck[tempIndex - 6]->canIMove(climber)) {
                            tilesDeck[tempIndex - 6]->checkButton(mouse.x, mouse.y, volume);
                        }
                    }
                    if (tempIndex > 5 && tempIndex % 5 != 4) {
                        if (tilesDeck[tempIndex - 4]->canIMove(climber)) {
                            tilesDeck[tempIndex - 4]->checkButton(mouse.x, mouse.y, volume);
                        }
                    }
                    if (tempIndex < TILEDECKNUMBER - 4 && tempIndex % 5 != 0) {
                        if (tilesDeck[tempIndex + 4]->canIMove(climber)) {
                            tilesDeck[tempIndex + 4]->checkButton(mouse.x, mouse.y, volume);
                        }
                    }
                    if (tempIndex < TILEDECKNUMBER - 6 && tempIndex % 5 != 4) {
                        if (tilesDeck[tempIndex + 6]->canIMove(climber)) {
                            tilesDeck[tempIndex + 6]->checkButton(mouse.x, mouse.y, volume);
                        }
                    }
                }
            } else if (modeEnum == MOVEOTHER) {
                if (ExplorerCharacter *explorerCharacter = dynamic_cast<ExplorerCharacter *>(character2)) {
                    if (tempIndex > 5 && tempIndex % 5 != 0) {
                        if (tilesDeck[tempIndex - 6]->canIMove(climber)) {
                            tilesDeck[tempIndex - 6]->checkButton(mouse.x, mouse.y, volume);
                        }
                    }
                    if (tempIndex > 5 && tempIndex % 5 != 4) {
                        if (tilesDeck[tempIndex - 4]->canIMove(climber)) {
                            tilesDeck[tempIndex - 4]->checkButton(mouse.x, mouse.y, volume);
                        }
                    }
                    if (tempIndex < TILEDECKNUMBER - 4 && tempIndex % 5 != 0) {
                        if (tilesDeck[tempIndex + 4]->canIMove(climber)) {
                            tilesDeck[tempIndex + 4]->checkButton(mouse.x, mouse.y, volume);
                        }
                    }
                    if (tempIndex < TILEDECKNUMBER - 6 && tempIndex % 5 != 4) {
                        if (tilesDeck[tempIndex + 6]->canIMove(climber)) {
                            tilesDeck[tempIndex + 6]->checkButton(mouse.x, mouse.y, volume);
                        }
                    }
                }
            }
        }
        if (modeEnum == REMOVESAND || modeEnum ==
                                      DUNEBLASTERMODE) { //I pass always true because in this mode I only don't want not to be marked if its the storm
            if (tempIndex > 0) {
                if (tilesDeck[tempIndex - 1]->haveSand() && tempIndex % 5 != 0) {
                    tilesDeck[tempIndex - 1]->checkButton(mouse.x, mouse.y, volume);
                }
            }
            if (tempIndex < TILEDECKNUMBER - 1) {
                if (tilesDeck[tempIndex + 1]->haveSand() && tempIndex % 5 != 4) {
                    tilesDeck[tempIndex + 1]->checkButton(mouse.x, mouse.y, volume);
                }
            }
            if (tempIndex > 4) {
                if (tilesDeck[tempIndex - 5]->haveSand()) {
                    tilesDeck[tempIndex - 5]->checkButton(mouse.x, mouse.y, volume);
                }
            }
            if (tempIndex < TILEDECKNUMBER - 5) {
                if (tilesDeck[tempIndex + 5]->haveSand()) {
                    tilesDeck[tempIndex + 5]->checkButton(mouse.x, mouse.y, volume);
                }
            }
            // Explorer Move Diagonally
            if (ExplorerCharacter *explorerCharacter = dynamic_cast<ExplorerCharacter *>(character)) {
                if (tempIndex > 5 && tempIndex % 5 != 0) {
                    if (tilesDeck[tempIndex - 6]->haveSand()) {
                        tilesDeck[tempIndex - 6]->checkButton(mouse.x, mouse.y, volume);
                    }
                }
                if (tempIndex > 5 && tempIndex % 5 != 4) {
                    if (tilesDeck[tempIndex - 4]->haveSand()) {
                        tilesDeck[tempIndex - 4]->checkButton(mouse.x, mouse.y, volume);
                    }
                }
                if (tempIndex < TILEDECKNUMBER - 4 && tempIndex % 5 != 0) {
                    if (tilesDeck[tempIndex + 4]->haveSand()) {
                        tilesDeck[tempIndex + 4]->checkButton(mouse.x, mouse.y, volume);
                    }
                }
                if (tempIndex < TILEDECKNUMBER - 6 && tempIndex % 5 != 4) {
                    if (tilesDeck[tempIndex + 6]->haveSand()) {
                        tilesDeck[tempIndex + 6]->checkButton(mouse.x, mouse.y, volume);
                    }
                }
            }
        }
    }
}
void Game::clickTilesButtons(ALL *allegro) {
    if (modeEnum != MOVEOTHER) {
        // Click on the tile I'm on
        if (turn &&
            (tilesDeck[character->getCardIndex()]->haveSand() || !tilesDeck[character->getCardIndex()]->isTurned())) {
            if (tilesDeck[character->getCardIndex()]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                if (modeEnum == DUNEBLASTERMODE) {
                    while (tilesDeck[character->getCardIndex()]->haveSand()) {
                        tilesDeck[character->getCardIndex()]->removeSand();
                        SandMarkersLeft++;
                        cout << "Sand Marker ++" << endl;
                    }
                    gamePack.playequipment(DUNEBLAST, character->getRow(), character->getColumn());
                    gamePack.sendPack();
                    gamePack.waitForAck(allegro);
                } else if (tilesDeck[character->getCardIndex()]->haveSand()) {
                    if (character->removeSand()) {
                        tilesDeck[character->getCardIndex()]->removeSand();
                        SandMarkersLeft++;
                        cout << "Sand Marker ++" << endl;
                        if (ArcheologistCharacter *archeologistCharacter = dynamic_cast<ArcheologistCharacter *>(character)) {
                            if (tilesDeck[character->getCardIndex()]->haveSand()) {
                                tilesDeck[character->getCardIndex()]->removeSand();
                                SandMarkersLeft++;
                                cout << "Sand Marker ++" << endl;
                            }
                        }
                        gamePack.remove(character->getRow(), character->getColumn());
                        gamePack.sendPack();
                        gamePack.waitForAck(allegro);
                    }
                } else {
                    if (!(tilesDeck[character->getCardIndex()]->isTurned()))
                        if (character->dig()) {
                            al_play_sample(allegro->excavateSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                            tilesDeck[character->getCardIndex()]->turnTile();
                            performTileAction(tilesDeck[character->getCardIndex()]->gettileType(),
                                              character->getCardIndex(), allegro);
                            gamePack.excavate();
                            gamePack.sendPack();
                            gamePack.waitForAck(allegro);
                        }
                }
            }
        }
        //Click on Adyacent Tiles
        if (character->getCardIndex() > 0) {
            if (tilesDeck[character->getCardIndex() - 1]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                keyUp(allegro);
                modeEnum = NORMAL;
            }
        }
        if (character->getCardIndex() < TILEDECKNUMBER - 1) {
            if (tilesDeck[character->getCardIndex() + 1]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                keyDown(allegro);
                modeEnum = NORMAL;
            }
        }
        if (character->getCardIndex() > 4) {
            if (tilesDeck[character->getCardIndex() - 5]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                keyLeft(allegro);
                modeEnum = NORMAL;
            }
        }
        if (character->getCardIndex() < TILEDECKNUMBER - 5) {
            if (tilesDeck[character->getCardIndex() + 5]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                keyRight(allegro);
                modeEnum = NORMAL;
            }
        }
        //Tunnel Tile
        bool climber = false;
        if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character)) { climber = true; }
        if (tilesDeck[character->getCardIndex()]->gettileType() == TUNNEL &&
            tilesDeck[character->getCardIndex()]->isTurned()) {
            for (uint i = 0; i < tilesDeck.size(); i++) {
                if (tilesDeck[i]->gettileType() == TUNNEL && i != character->getCardIndex() &&
                    tilesDeck[i]->isTurned()) { //TUNNEL not the same I'm in, it's discovered
                    if (tilesDeck[i]->canIMove(climber)) {
                        if (tilesDeck[i]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                            character->setPos(i % 5, i / 5);
                            gamePack.move(character->getRow(), character->getColumn());
                            gamePack.sendPack();
                            gamePack.waitForAck(allegro);
                        }
                    }
                }
            }
        }
        //If I'm Explorer, click on diagonally adyacent tiles
        if (ExplorerCharacter *explorerCharacter = dynamic_cast<ExplorerCharacter *>(character)) {
            bool climber = false;
            if (ClimberCharacter *explorerCharacter = dynamic_cast<ClimberCharacter *>(character)) {
                climber = true;
                cout << "Error: character is both explorer and climber" << endl;
            }
            if (character->getCardIndex() > 5 && character->getCardIndex() % 5 != 0) {
                if (tilesDeck[character->getCardIndex() - 6]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                    if (modeEnum == NORMAL) {
                        if (tilesDeck[character->getCardIndex() - 6]->canIMove(
                                climber)) {    //Here climber will always be false
                            explorerCharacter->moveUpLeftDiagonally();
                            gamePack.move(character->getRow(), character->getColumn());
                            gamePack.sendPack();
                            gamePack.waitForAck(allegro);
                        }
                    } else if (modeEnum == REMOVESAND) {
                        if (tilesDeck[character->getCardIndex() - 6]->haveSand()) {
                            if (character->removeSand()) { //this time there is no need to check if its an archeologist because its an explorer
                                tilesDeck[character->getCardIndex() - 6]->removeSand();
                                SandMarkersLeft++;
                                cout << "Sand Marker ++" << endl;
                                gamePack.remove(character->getRow() - 1, character->getColumn() - 1);
                                gamePack.sendPack();
                                gamePack.waitForAck(allegro);
                            }
                        }
                    } else if (modeEnum == DUNEBLASTERMODE) {
                        while (tilesDeck[character->getCardIndex() - 6]->haveSand()) {
                            tilesDeck[character->getCardIndex() - 6]->removeSand();
                            SandMarkersLeft++;
                            cout << "Sand Marker ++" << endl;
                        }
                        gamePack.playequipment(DUNEBLAST, character->getRow() - 1, character->getColumn() - 1);
                        gamePack.sendPack();
                        gamePack.waitForAck(allegro);
                    }
                    modeEnum = NORMAL;
                }
            }
            if (character->getCardIndex() > 4 && character->getCardIndex() % 5 != 4) {
                if (tilesDeck[character->getCardIndex() - 4]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                    if (modeEnum == NORMAL) {
                        if (tilesDeck[character->getCardIndex() - 4]->canIMove(climber)) {
                            explorerCharacter->moveDownLeftDiagonally();
                            gamePack.move(character->getRow(), character->getColumn());
                            gamePack.sendPack();
                            //Polonet
                            gamePack.waitForAck(allegro);
                        }
                    } else if (modeEnum == REMOVESAND) {
                        if (tilesDeck[character->getCardIndex() - 4]->haveSand()) {
                            if (character->removeSand()) {
                                tilesDeck[character->getCardIndex() - 4]->removeSand();
                                SandMarkersLeft++;
                                cout << "Sand Marker ++" << endl;
                                gamePack.remove(character->getRow(), character->getColumn() + 1);
                                gamePack.sendPack();
                                gamePack.waitForAck(allegro);
                            }
                        }

                    } else if (modeEnum == DUNEBLASTERMODE) {
                        while (tilesDeck[character->getCardIndex() - 4]->haveSand()) {
                            tilesDeck[character->getCardIndex() - 4]->removeSand();
                            SandMarkersLeft++;
                            cout << "Sand Marker ++" << endl;
                        }
                        gamePack.playequipment(DUNEBLAST, character->getRow() + 1, character->getColumn() - 1);
                        gamePack.sendPack();
                        gamePack.waitForAck(allegro);
                    }
                    modeEnum = NORMAL;
                }
            }
            if (character->getCardIndex() < TILEDECKNUMBER - 4 && character->getCardIndex() % 5 != 0) {
                if (tilesDeck[character->getCardIndex() + 4]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                    if (modeEnum == NORMAL) {
                        if (tilesDeck[character->getCardIndex() + 4]->canIMove(climber)) {
                            explorerCharacter->moveUpRightDiagonally();
                            gamePack.move(character->getRow(), character->getColumn());
                            gamePack.sendPack();
                            gamePack.waitForAck(allegro);
                        }
                    } else if (modeEnum == REMOVESAND) {
                        if (tilesDeck[character->getCardIndex() + 4]->haveSand()) {
                            if (character->removeSand()) {
                                tilesDeck[character->getCardIndex() + 4]->removeSand();
                                SandMarkersLeft++;
                                cout << "Sand Marker ++" << endl;
                                gamePack.remove(character->getRow(), character->getColumn() + 1);
                                gamePack.sendPack();
                                gamePack.waitForAck(allegro);
                            }
                        }
                    } else if (modeEnum == DUNEBLASTERMODE) {
                        while (tilesDeck[character->getCardIndex() + 4]->haveSand()) {
                            tilesDeck[character->getCardIndex() + 4]->removeSand();
                            SandMarkersLeft++;
                            cout << "Sand Marker ++" << endl;
                        }
                        gamePack.playequipment(DUNEBLAST, character->getRow() - 1, character->getColumn() + 1);
                        gamePack.sendPack();
                        gamePack.waitForAck(allegro);
                    }
                    modeEnum = NORMAL;
                }
            }
            if (character->getCardIndex() < TILEDECKNUMBER - 6 && character->getCardIndex() % 5 != 4) {
                if (tilesDeck[character->getCardIndex() + 6]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                    if (modeEnum == NORMAL) {
                        if (tilesDeck[character->getCardIndex() + 6]->canIMove(climber)) {
                            explorerCharacter->moveDownRightDiagonally();
                            gamePack.move(character->getRow(), character->getColumn());
                            gamePack.sendPack();
                            gamePack.waitForAck(allegro);
                        }
                    } else if (modeEnum == REMOVESAND) {
                        if (tilesDeck[character->getCardIndex() + 6]->haveSand()) {
                            if (character->removeSand()) {
                                tilesDeck[character->getCardIndex() + 6]->removeSand();
                                SandMarkersLeft++;
                                cout << "Sand Marker ++" << endl;
                                gamePack.remove(character->getRow(), character->getColumn() + 1);
                                gamePack.sendPack();
                                gamePack.waitForAck(allegro);
                            }
                        }
                    } else if (modeEnum == DUNEBLASTERMODE) {
                        while (tilesDeck[character->getCardIndex() + 6]->haveSand()) {
                            tilesDeck[character->getCardIndex() + 6]->removeSand();
                            SandMarkersLeft++;
                            cout << "Sand Marker ++" << endl;
                        }
                        gamePack.playequipment(DUNEBLAST, character->getRow() + 1, character->getColumn() + 1);
                        gamePack.sendPack();
                        gamePack.waitForAck(allegro);
                    }
                    modeEnum = NORMAL;
                }
            }
        }
    } else {
        //Click on Adyacent Tiles
        if (character2->getCardIndex() > 0) {
            if (tilesDeck[character2->getCardIndex() - 1]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                keyUp(allegro);
            }
        }
        if (character2->getCardIndex() < TILEDECKNUMBER - 1) {
            if (tilesDeck[character2->getCardIndex() + 1]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                keyDown(allegro);
            }
        }
        if (character2->getCardIndex() > 4) {
            if (tilesDeck[character2->getCardIndex() - 5]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                keyLeft(allegro);
            }
        }
        if (character2->getCardIndex() < TILEDECKNUMBER - 5) {
            if (tilesDeck[character2->getCardIndex() + 5]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                keyRight(allegro);
            }
        }
        //Tunnel Tile
        bool climber = false;
        if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character2)) { climber = true; }
        if (tilesDeck[character2->getCardIndex()]->gettileType() == TUNNEL &&
            tilesDeck[character2->getCardIndex()]->isTurned()) {
            for (uint i = 0; i < tilesDeck.size(); i++) {
                if (tilesDeck[i]->gettileType() == TUNNEL && i != character2->getCardIndex() &&
                    tilesDeck[i]->isTurned()) {
                    if (tilesDeck[i]->canIMove(climber)) {
                        if (tilesDeck[i]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                            character2->setPos(i % 5, i / 5);
                            gamePack.requestmove(character2->getRow(), character2->getColumn());
                            gamePack.sendPack();
                            gamePack.waitForAck(allegro);
                        }
                    }
                }
            }
        }
        //If I'm Explorer, click on diagonally adyacent tiles
        if (ExplorerCharacter *explorerCharacter = dynamic_cast<ExplorerCharacter *>(character2)) {
            if (NavigatorCharacter *navigatorCharacter = dynamic_cast<NavigatorCharacter *>(character)) {
                bool climber = false;
                if (ClimberCharacter *explorerCharacter = dynamic_cast<ClimberCharacter *>(character2)) {
                    climber = true;
                    cout << "Error: character is both explorer and climber" << endl;
                }
                if (character2->getCardIndex() > 5 && character2->getCardIndex() % 5 != 0) {
                    if (tilesDeck[character2->getCardIndex() - 6]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                        if (navigatorCharacter->havePartnersMovesLeft()) {
                            if (tilesDeck[character2->getCardIndex() - 6]->canIMove(
                                    climber)) {    //Here climber will always be false
                                explorerCharacter->moveUpLeftDiagonally();
                                navigatorCharacter->decresePartnerMoves();
                                //Conection
                            }
                        }
                    }
                }
                if (character2->getCardIndex() > 4 && character2->getCardIndex() % 5 != 4) {
                    if (tilesDeck[character2->getCardIndex() - 4]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                        if (navigatorCharacter->havePartnersMovesLeft()) {
                            if (tilesDeck[character2->getCardIndex() - 4]->canIMove(climber)) {
                                explorerCharacter->moveDownLeftDiagonally();
                                navigatorCharacter->decresePartnerMoves();
                            }
                        }
                    }
                }
                if (character2->getCardIndex() < TILEDECKNUMBER - 4 && character2->getCardIndex() % 5 != 0) {
                    if (tilesDeck[character2->getCardIndex() + 4]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                        if (navigatorCharacter->havePartnersMovesLeft()) {
                            if (tilesDeck[character2->getCardIndex() + 4]->canIMove(climber)) {
                                explorerCharacter->moveUpRightDiagonally();
                                navigatorCharacter->decresePartnerMoves();
                            }
                        }
                    }
                }
                if (character2->getCardIndex() < TILEDECKNUMBER - 6 && character->getCardIndex() % 5 != 4) {
                    if (tilesDeck[character2->getCardIndex() + 6]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                        if (navigatorCharacter->havePartnersMovesLeft()) {
                            if (tilesDeck[character2->getCardIndex() + 6]->canIMove(climber)) {
                                explorerCharacter->moveDownRightDiagonally();
                                navigatorCharacter->decresePartnerMoves();
                            }
                        }
                    }
                }
            }
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
//Next and Previous Methods
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