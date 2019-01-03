//
// Created by barrachina on 12/21/18.
//

#include <include/SinglePlayerGame.hpp>

#include "SinglePlayerGame.hpp"

#define TERRASCOPE_PEEP_TIME 600000

/**
 * Single Player Constructor
 * */
SinglePlayerGame::SinglePlayerGame(ALL *allegro, Container *info_in) {
    info = *info_in;
    //allegro->volume = 1;
    if (initializeAllegro(allegro)) {
        //Characters
        initializeCharacters(info, allegro);
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

        mouse.x = static_cast<int>(allegro->screenWidth / 2.0);
        mouse.y = static_cast<int>(allegro->screenHeight / 2.0);
        redraw = true;
        gameOver = false;
        playAgain = false;

        modeEnum = NORMAL;
    } else { cout << "Couldn't initialize allegro" << endl; }
}
void SinglePlayerGame::updateScreen(ALL *allegro) {
#ifdef DEBUG_UPDATE_SCREEN
    cout << "Update Screen" << endl;
#endif
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
#ifdef DEBUG_UPDATE_SCREEN
            cout << "Default case" << endl;
#endif
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(allegro->fondo, 0, 0, 0);
            //Update Tiles
#ifdef DEBUG_UPDATE_SCREEN
            cout << "Update Tiles" << endl;
#endif
            pos tempPos;
            for (uint i = 0; i < tilesDeck.size(); i++) {
                tempPos = getSpot(i % 5, i / 5, allegro, tilesDeck[i]->getTileBackImage());
                tilesDeck[i]->setCoord(tempPos);
            }
            for (auto &i : tilesDeck) {
                i->updateTile(allegro);
            }
            //End of Turn Button
#ifdef DEBUG_UPDATE_SCREEN
            cout << "Update End of Turn Button" << endl;
#endif
            endTurnButton->updateButton(allegro);
            if (allegro->volume == 0) { muteButton->updateButton(allegro); }
            else { noMuteButton->updateButton(allegro); }
            //Tiles Buttons
#ifdef DEBUG_UPDATE_SCREEN
            cout << "Update Tiles Button" << endl;
#endif
            updateTilesButton(allegro); // Don't let the player think he can still move
            //Update Characters
#ifdef DEBUG_UPDATE_SCREEN
            cout << "Update Characters" << endl;
#endif
            character->updateChar(allegro, 1, endTurnButton->getMiddleOfX(), endTurnButton->getMiddleOfY()); //Character must be after Tiles
            character2->updateChar(allegro, 2);

            //Update Parts
#ifdef DEBUG_UPDATE_SCREEN
            cout << "Update Parts" << endl;
#endif
            propeller->updatePart(allegro);
            solarCrystal->updatePart(allegro);
            navigationDeck->updatePart(allegro);
            engine->updatePart(allegro);
            int i = 0;
            if (engine->getPickedState()) {
                al_draw_bitmap(allegro->partsImages[i], allegro->screenWidth / 4, allegro->screenHeight / 5 + i *
                                                                                                              al_get_bitmap_height(allegro->partsImages[i]), 0);
            } else {
                al_draw_bitmap(allegro->partsImages[i + 4], allegro->screenWidth / 4,
                               allegro->screenHeight / 5 + i * al_get_bitmap_height(allegro->partsImages[i]), 0);
            }
            i++;
            if (navigationDeck->getPickedState()) {
                al_draw_bitmap(allegro->partsImages[i], allegro->screenWidth / 4, allegro->screenHeight / 5 + i *
                                                                                                              al_get_bitmap_height(allegro->partsImages[i]), 0);
            } else {
                al_draw_bitmap(allegro->partsImages[i + 4], allegro->screenWidth / 4,
                               allegro->screenHeight / 5 + i * al_get_bitmap_height(allegro->partsImages[i]), 0);
            }
            i++;
            if (propeller->getPickedState()) {
                al_draw_bitmap(allegro->partsImages[i], allegro->screenWidth / 4, allegro->screenHeight / 5 + i *
                                                                                                              al_get_bitmap_height(allegro->partsImages[i]), 0);
            } else {
                al_draw_bitmap(allegro->partsImages[i + 4], allegro->screenWidth / 4,
                               allegro->screenHeight / 5 + i * al_get_bitmap_height(allegro->partsImages[i]), 0);
            }
            i++;
            if (solarCrystal->getPickedState()) {
                al_draw_bitmap(allegro->partsImages[i], allegro->screenWidth / 4, allegro->screenHeight / 5 + i *
                                                                                                              al_get_bitmap_height(allegro->partsImages[i]), 0);
            } else {
                al_draw_bitmap(allegro->partsImages[i + 4], allegro->screenWidth / 4,
                               allegro->screenHeight / 5 + i * al_get_bitmap_height(allegro->partsImages[i]), 0);
            }
            //Buttons
            for (int i = 0; i < MODEBUTTONSNUMBER; i++) {
                modesButtons[i]->updateButton(allegro);
            }
            if (MeteorologistCharacter *meteorologistCharacter = dynamic_cast<MeteorologistCharacter *>(character)) {
                meteorologistCharacter->updateMeteorologistButton(allegro);    //Only display the button for the one who can use it... XD
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
#ifdef DEBUG_UPDATE_SCREEN
            cout << "Default case done" << endl;
#endif
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
#ifdef DEBUG_UPDATE_SCREEN
    cout << "Update Done" << endl;
#endif
}
void SinglePlayerGame::updateTilesButton(ALL *allegro) {
#ifdef DEBUG_UPDATE_SCREEN
    cout << "Update Tiles Button" << endl;
#endif
    //Equipments Mode
    if (modeEnum == JETPACKMODE) {
        for (int i = 0; i < TILEDECKNUMBER; i++) {
            if (tilesDeck[i]->canIMove(false)) {
                tilesDeck[i]->updateButton(allegro);
            }
        }
    } else if (modeEnum == TERRASCOPEMODE) {
        for (int i = 0; i < TILEDECKNUMBER; i++)
            if (tilesDeck[i]->gettileType() != STORM && !tilesDeck[i]->isTurned()) {
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
            if ((tilesDeck[tempIndex]->haveSand() || !tilesDeck[tempIndex]->isTurned())) { tilesDeck[tempIndex]->updateButton(allegro); }
            if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character)) { climber = true; }
        }
        if (tilesDeck[tempIndex]->canIMove(climber)) {
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
//! End of game methods
bool SinglePlayerGame::win(ALL *allegro) {
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

                wannaPlay(allegro, WE_WON);
                return true;
            }
        }
    }
    return false;
}
void SinglePlayerGame::wannaPlay(ALL *allegro, char ID) {
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
        } else if (ID == WE_WON) {
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
                    run = false;
                } else if (gameOverButton->checkMouse(ev.mouse.x, ev.mouse.y, allegro->volume)) {
                    gameOver = true;
                    playAgain = false;
                    run = false;
                }
            }

        }
        delete playAgainButton;
        delete gameOverButton;
        al_destroy_event_queue(tempQueue);
    } else { cout << "Could not initialize tempQueue" << endl; }
}
bool SinglePlayerGame::lose(ALL *allegro) {
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

    wannaPlay(allegro, WE_LOST);
    return true;
}
//! Key methods
void SinglePlayerGame::keyUp(ALL *allegro) {
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
                                }
                            }
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
            }
        }
    } else {    // MoveOther Mode
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
                                        character->moveUp(); // TODO: this is not working yet, the other computer never knows if climber has its take with me active
                                    }
                                }
                            } else { cout << "Error: no moves left even though I was supposed to have" << endl; }
                        }
                    }
                }
            }
        } else { cout << "Error: Move Other mode without being a navigator character" << endl; }
    }
}
void SinglePlayerGame::keyDown(ALL *allegro) {
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
                                }
                            }
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
                    }
                }
            } else if (modeEnum == DUNEBLASTERMODE) {
                while (tilesDeck[character->getCardIndex() + 1]->haveSand()) {
                    tilesDeck[character->getCardIndex() + 1]->removeSand();
                    SandMarkersLeft++;
                    cout << "Sand Marker ++" << endl;
                }
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
                            } else { cout << "Error: no moves left even though I was supposed to have" << endl; }
                        }
                    }
                }
            }
        } else { cout << "Error: Move Other mode without being a navigator character" << endl; }
    }
}
void SinglePlayerGame::keyRight(ALL *allegro) {
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
                            // If take with me...
                            if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character)) {
                                if (climberCharacter->getTakeWithMe() && character2->getCardIndex() + 5 == character->getCardIndex()) {
                                    character2->moveRight();
                                }
                            }
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
                            } else { cout << "Error: no moves left even though I was supposed to have" << endl; }
                        }
                    }
                }
            }
        } else { cout << "Error: Move Other mode without being a navigator character" << endl; }
    }
}
void SinglePlayerGame::keyLeft(ALL *allegro) {
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
                                }
                            }
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
                    }
                }
            } else if (modeEnum == DUNEBLASTERMODE) {
                while (tilesDeck[character->getCardIndex() - 5]->haveSand()) {
                    tilesDeck[character->getCardIndex() - 5]->removeSand();
                    SandMarkersLeft++;
                    cout << "Sand Marker ++" << endl;
                }
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
                            }
                        }
                    }
                }
            }
        } else { cout << "Error: Move Other mode without being a navigator character" << endl; }
    }
}
void SinglePlayerGame::keyE(ALL *allegro) {
    if (modeEnum == NORMAL) {
        if (!(tilesDeck[character->getCardIndex()]->isTurned())) {
            if (!tilesDeck[character->getCardIndex()]->haveSand()) {
                if (character->dig()) {
                    al_play_sample(allegro->excavateSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                    tilesDeck[character->getCardIndex()]->turnTile();
                    performTileAction(tilesDeck[character->getCardIndex()]->gettileType(), character->getCardIndex(),
                                      allegro);
                }
            }
        }
    }
}
void SinglePlayerGame::keyP(ALL *allegro) {
    if (modeEnum == NORMAL) {
        pickUpAction();
    }
}
void SinglePlayerGame::keyR(ALL *allegro) {
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
            }
        }
    }
}

//! Click Methods
void SinglePlayerGame::click(ALL *allegro) {
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
#ifdef DEBUG_GAME
            cout << "Stopping background music" << endl;
#endif
        }
    }
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
void SinglePlayerGame::clickOnPeepMode(ALL *allegro) {
    /* Click while in peep mode */
    if (MeteorologistCharacter *meteorologistCharacter = dynamic_cast<MeteorologistCharacter *>(character)) {
        if (nextButton->checkMouse(mouse.x, mouse.y, allegro->volume)) { nextPeepStormCard(); }
        if (previousButton->checkMouse(mouse.x, mouse.y, allegro->volume)) { previousPeepStormCard(); }
        if (selectPeepCard->checkMouse(mouse.x, mouse.y, allegro->volume)) {
            for (int i = 0; i < peepOffset; i++) {
                swapStormCards(peepOffset - i - 1, peepOffset - i);
            } //Move the card to the front by swaping with each card
            std::rotate(stormCardsDeck.begin(), stormCardsDeck.begin() + 1,
                        stormCardsDeck.end());        //Rotate to take it to the bottom.
            //meteorologistCharacter->peep();
            modeEnum = NORMAL;
            resetPeepOffset();
        }
    } else { cout << "Enter in 'click on peep mode' without being an meteorologist" << endl; }
}
void SinglePlayerGame::clickOnJetPackMode(ALL *allegro) {
    for (int i = 0; i < TILEDECKNUMBER; i++) {
        bool climber = false;
        //The climber's ability doesn't affect movement by Jetpack.
        //if (ClimberCharacter* climberCharacter = dynamic_cast<ClimberCharacter*>(character)) { climber = true; }
        if (tilesDeck[i]->canIMove(false)) {
            if (tilesDeck[i]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                character->setPos(i % 5, i / 5);
                character->increaseMoves();
                al_play_sample(allegro->teleportSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                modeEnum = NORMAL;
            }
        }
    }
}
void SinglePlayerGame::clickOnTerrascopeMode(ALL *allegro) {
    for (int i = 0; i < TILEDECKNUMBER; i++) {
        if (tilesDeck[i]->checkButton(mouse.x, mouse.y, allegro->volume) && !tilesDeck[i]->isTurned()) {
            int backup = tilesDeck[i]->getSand();
            tilesDeck[i]->turnTerrascopeTile();
            tilesDeck[i]->updateTile(allegro);
            al_flip_display();
            tilesDeck[i]->setSand(backup);
            tilesDeck[i]->setTurned(false);
            modeEnum = NORMAL;
            usleep(TERRASCOPE_PEEP_TIME);
        }
    }
}
void SinglePlayerGame::clickOnRequestWaterMode(ALL *allegro) {
    for (int i = 0; i < min(character->getWaters(), MAXWATERS - character2->getWaters() - 1); i++) {
        if (watersButton[i]->checkMouse(mouse.x, mouse.y, allegro->volume)) {  // && character->getWaters() > (i + 1)) {
            modeEnum = NORMAL;
            updateScreen(allegro);
            for (int c = 0; c < i + 1; c++) {
                character->drinkWater();
                character2->addWater();
            }
        }
    }
}
void SinglePlayerGame::clickOnOfferEquipmentMode(ALL *allegro) {
    if (nextButton->checkMouse(mouse.x, mouse.y, allegro->volume)) { nextEquipCard(); }
    if (previousButton->checkMouse(mouse.x, mouse.y, allegro->volume)) { previousEquipCard(); }
    if (selectEquipmentCard->checkMouse(mouse.x, mouse.y, allegro->volume)) {
        modeEnum = NORMAL;
        updateScreen(allegro);
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
    }
}
void SinglePlayerGame::clickOnRequestEquipmentMode(ALL *allegro) {
    if (nextButton->checkMouse(mouse.x, mouse.y, allegro->volume)) { nextEquipCard(); }
    if (previousButton->checkMouse(mouse.x, mouse.y, allegro->volume)) { previousEquipCard(); }
    if (selectEquipmentCard->checkMouse(mouse.x, mouse.y, allegro->volume)) {
        modeEnum = NORMAL;
        updateScreen(allegro);
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
void SinglePlayerGame::clickOnOfferWaterMode(ALL *allegro) {
    for (int i = 0; i < min(character->getWaters(), MAXWATERS - character2->getWaters() - 1); i++) {
        if (watersButton[i]->checkMouse(mouse.x, mouse.y, allegro->volume)) {  // && character->getWaters() > (i + 1)) {
            modeEnum = NORMAL;
            updateScreen(allegro);
            for (int c = 0; c < i + 1; c++) {
                character->drinkWater();
                character2->addWater();
            }

        }
    }
}
void SinglePlayerGame::clickOnDefaultMode(ALL *allegro) {
    clickModesButtons(allegro);
    clickEquipmentButtons(allegro);
    clickTilesButtons(allegro);
    /*if (tilesDeck[character->getCardIndex()]->checkButton(mouse.x, mouse.y, allegro->volume)) {
		keyR(allegro);
		modeEnum = NORMAL;
	}*/
    if (endTurnButton->checkMouse(mouse.x, mouse.y, allegro->volume)) {
        //If there was a selected character, then return true, else return false
        //!!!!! TODO: make a swap of characters to see if it works OK
        al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        drawStormCards(allegro);
        Character *tempChar;                // pointer to character to make a swap
        character->resetNumberOfMoves();    // This has to go after draw cards.
                                            // Because if you are a meteorologist you may be able to draw fewer cards.
        character2->resetNumberOfMoves();

        tempChar = character2;
        character2 = character;
        character = tempChar;
        //turn = !turn;
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
        if (tilesDeck[character->getCardIndex()]->gettileType() == WATER && tilesDeck[character->getCardIndex()]->isTurned()) {
            if (waterCarrierCharacter->checkMouseOverButton(mouse.x, mouse.y, allegro->volume)) {
                al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                waterCarrierCharacter->getWatersFromWell();
                al_play_sample(allegro->waterSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
        }
    } else if (NavigatorCharacter *navigatorCharacter = dynamic_cast<NavigatorCharacter *>(character)) {
        if (navigatorCharacter->checkMouseOverButton(mouse.x, mouse.y, modeEnum == MOVEOTHER, allegro->volume)) {
            cout << "Click on special navigator button" << endl;
            al_play_sample(allegro->clickOnButtonSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            if (modeEnum == MOVEOTHER) {
                modeEnum = NORMAL;
                if(navigatorCharacter->usedPartnerMoves()) {
                    character2->resetNumberOfMoves();
                    navigatorCharacter->decreseMoves();
                }
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
void SinglePlayerGame::clickTilesButtons(ALL *allegro) {
    if (modeEnum != MOVEOTHER) {
        // Click on the tile I'm on
        if ((tilesDeck[character->getCardIndex()]->haveSand() || !tilesDeck[character->getCardIndex()]->isTurned())) {
            if (tilesDeck[character->getCardIndex()]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                if (modeEnum == DUNEBLASTERMODE) {
                    while (tilesDeck[character->getCardIndex()]->haveSand()) {
                        tilesDeck[character->getCardIndex()]->removeSand();
                        SandMarkersLeft++;
                        cout << "Sand Marker ++" << endl;
                    }
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
                    }
                } else {
                    if (!(tilesDeck[character->getCardIndex()]->isTurned()))
                        if (character->dig()) {
                            al_play_sample(allegro->excavateSound, allegro->volume, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                            tilesDeck[character->getCardIndex()]->turnTile();
                            performTileAction(tilesDeck[character->getCardIndex()]->gettileType(),
                                              character->getCardIndex(), allegro);
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
                        }
                    } else if (modeEnum == REMOVESAND) {
                        if (tilesDeck[character->getCardIndex() - 6]->haveSand()) {
                            if (character->removeSand()) { //this time there is no need to check if its an archeologist because its an explorer
                                tilesDeck[character->getCardIndex() - 6]->removeSand();
                                SandMarkersLeft++;
                                cout << "Sand Marker ++" << endl;
                            }
                        }
                    } else if (modeEnum == DUNEBLASTERMODE) {
                        while (tilesDeck[character->getCardIndex() - 6]->haveSand()) {
                            tilesDeck[character->getCardIndex() - 6]->removeSand();
                            SandMarkersLeft++;
                            cout << "Sand Marker ++" << endl;
                        }
                    }
                    modeEnum = NORMAL;
                }
            }
            if (character->getCardIndex() > 4 && character->getCardIndex() % 5 != 4) {
                if (tilesDeck[character->getCardIndex() - 4]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                    if (modeEnum == NORMAL) {
                        if (tilesDeck[character->getCardIndex() - 4]->canIMove(climber)) {
                            explorerCharacter->moveDownLeftDiagonally();
                        }
                    } else if (modeEnum == REMOVESAND) {
                        if (tilesDeck[character->getCardIndex() - 4]->haveSand()) {
                            if (character->removeSand()) {
                                tilesDeck[character->getCardIndex() - 4]->removeSand();
                                SandMarkersLeft++;
                                cout << "Sand Marker ++" << endl;
                            }
                        }

                    } else if (modeEnum == DUNEBLASTERMODE) {
                        while (tilesDeck[character->getCardIndex() - 4]->haveSand()) {
                            tilesDeck[character->getCardIndex() - 4]->removeSand();
                            SandMarkersLeft++;
                            cout << "Sand Marker ++" << endl;
                        }
                    }
                    modeEnum = NORMAL;
                }
            }
            if (character->getCardIndex() < TILEDECKNUMBER - 4 && character->getCardIndex() % 5 != 0) {
                if (tilesDeck[character->getCardIndex() + 4]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                    if (modeEnum == NORMAL) {
                        if (tilesDeck[character->getCardIndex() + 4]->canIMove(climber)) {
                            explorerCharacter->moveUpRightDiagonally();
                        }
                    } else if (modeEnum == REMOVESAND) {
                        if (tilesDeck[character->getCardIndex() + 4]->haveSand()) {
                            if (character->removeSand()) {
                                tilesDeck[character->getCardIndex() + 4]->removeSand();
                                SandMarkersLeft++;
                                cout << "Sand Marker ++" << endl;
                            }
                        }
                    } else if (modeEnum == DUNEBLASTERMODE) {
                        while (tilesDeck[character->getCardIndex() + 4]->haveSand()) {
                            tilesDeck[character->getCardIndex() + 4]->removeSand();
                            SandMarkersLeft++;
                            cout << "Sand Marker ++" << endl;
                        }
                    }
                    modeEnum = NORMAL;
                }
            }
            if (character->getCardIndex() < TILEDECKNUMBER - 6 && character->getCardIndex() % 5 != 4) {
                if (tilesDeck[character->getCardIndex() + 6]->checkButton(mouse.x, mouse.y, allegro->volume)) {
                    if (modeEnum == NORMAL) {
                        if (tilesDeck[character->getCardIndex() + 6]->canIMove(climber)) {
                            explorerCharacter->moveDownRightDiagonally();
                        }
                    } else if (modeEnum == REMOVESAND) {
                        if (tilesDeck[character->getCardIndex() + 6]->haveSand()) {
                            if (character->removeSand()) {
                                tilesDeck[character->getCardIndex() + 6]->removeSand();
                                SandMarkersLeft++;
                                cout << "Sand Marker ++" << endl;;
                            }
                        }
                    } else if (modeEnum == DUNEBLASTERMODE) {
                        while (tilesDeck[character->getCardIndex() + 6]->haveSand()) {
                            tilesDeck[character->getCardIndex() + 6]->removeSand();
                            SandMarkersLeft++;
                            cout << "Sand Marker ++" << endl;
                        }
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
void SinglePlayerGame::clickModesButtons(ALL *allegro) {
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
void SinglePlayerGame::checkTilesButtons(double volume) {
    int tempIndex;
    bool climber = false;
    if (modeEnum == MOVEOTHER) {
        tempIndex = character2->getCardIndex();
        if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character2)) { climber = true; }
    } else {
        tempIndex = character->getCardIndex();
        if ((tilesDeck[tempIndex]->haveSand() || !tilesDeck[tempIndex]->isTurned())) {
            tilesDeck[tempIndex]->checkButton(mouse.x, mouse.y, volume);
        }
        if (ClimberCharacter *climberCharacter = dynamic_cast<ClimberCharacter *>(character)) { climber = true; }
    }
    if (tilesDeck[tempIndex]->canIMove(climber)) {
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
//! Draw Methods
void SinglePlayerGame::drawEquipment(EquipmentsEnum equipEnum) {
/* Gets the equipment and adds it to the player */
    switch (equipEnum) {
        case DUNEBLAST:
            character->increaseDuneBlaster();
            break;
        case JETPACK:
            character->increaseJetPack();
            break;
        case SOLARSHIELD:
            character->increaseSolarShield();
            break;
        case TERRASCOPE:
            character->increaseTerrascope();
            break;
        case SECRETWATERRESERVE:
            character->increaseWaterReserve();
            break;
        case TIMETHROTTLE:
            character->increaseTimeThrottle();
            break;
    }
    if (equipIndex == 11) { equipIndex = 0; }
    else { equipIndex++; }
}
void SinglePlayerGame::drawStormCards(ALL *allegro) {
//The sandStormLevel does not indicate the number of stormCards to be drawn
    int discount = 0;        //Sometimes, if meteorologist is playing, it may be a discount on how many cards to draw
    if (MeteorologistCharacter *meteorologistCharacter = dynamic_cast<MeteorologistCharacter *>(character)) {
        discount = character->getNumberOfMoves();
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
void SinglePlayerGame::performEquipmentsAction(EquipmentsEnum equipEnum, ALL *allegro) {
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
            break;
        case TIMETHROTTLE:
            character->decreaseTimeThrottle();
            character->increaseMoves();
            character->increaseMoves();
            break;
        default:
            break;
    }
}
bool SinglePlayerGame::eventHandler(ALL *allegro) {
    bool stillPlaying = true;
    ALLEGRO_EVENT ev;
    al_wait_for_event(allegro->events_queue, &ev);

    if (ev.type == ALLEGRO_EVENT_TIMER) { redraw = true; }
    else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        stillPlaying = false;
    } else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
        mouse.x = ev.mouse.x;
        mouse.y = ev.mouse.y;
        checkMouse(allegro);
    } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        if (ev.mouse.button == SECONDARY_BUTTON) { keyEscape(); }
        else click(allegro);
    } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
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
                if (auto *explorerCharacter = dynamic_cast<ArcheologistCharacter *>(character)) {
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
    //this 'if' checks if the other player I'm moving has already moved 3 spaces... if so... get out of the move other mode
    if (modeEnum == MOVEOTHER) {
        if (auto *navigatorCharacter = dynamic_cast<NavigatorCharacter *>(character)) {
            if (!navigatorCharacter->havePartnersMovesLeft()) {
                modeEnum = NORMAL;
                character2->resetNumberOfMoves();
                navigatorCharacter->decreseMoves();
            }
        } else { cout << "Error: was in moveother mode without being a Navigator" << endl; }
    }
    //checks if win or lose
    if (redraw && al_is_event_queue_empty(allegro->events_queue)) {
        if (lose(allegro)) { stillPlaying = false; }
        else if (win(allegro)) { stillPlaying = false; }
        updateScreen(allegro);
    }
    return stillPlaying;
}
void SinglePlayerGame::pickUpAction() {
/*	Picks up the part in question. The function use the character acording to whose turn is it and then checks if its posible to pick up
	the part. If it is, picks up the correct part
	*/
    Character *tempChar;            //pointer to character. it will point to character 1 or 2 acording to whose turn is it.
    tempChar = character;
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





