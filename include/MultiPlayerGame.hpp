//
// Created by barrachina on 12/21/18.
//

#ifndef FORBIDDENDESERT_MULTIPLAYERGAME_H
#define FORBIDDENDESERT_MULTIPLAYERGAME_H

#include "Game.hpp"

#define TIME_TERRASCOPE 3000

class MultiPlayerGame : public Game {
public:
    MultiPlayerGame(ALL *allegro, FSMI *gameNetwork);

    bool eventHandler(ALL *allegro) override;
    bool receiveHandler(ALL* allegro);
    void pickUpAction() override;

private:
    FSMI *gameNetwork;
    Pack gamePack;
    bool turn{};	// Used to know which turn is it... true = this computer turn. False = companion turn.

    //! Methods
    void chooseOption(ALL* allegro, Container temp);
    // Update Screen
    void updateScreen(ALL* allegro) override;
    void updateTilesButton(ALL* allegro) override;
    //End Of Game
    bool win(ALL *allegro) override;
    bool lose(ALL *allegro) override;
    void wannaPlay(ALL *allegro, char ID) override;
    //Keys Pressed
    void keyUp(ALL *allegro) override;
    void keyDown(ALL *allegro) override;
    void keyLeft(ALL *allegro) override;
    void keyRight(ALL *allegro) override;
    void keyE(ALL* allegro) override;
    void keyP(ALL *allegro) override;
    void keyR(ALL *allegro) override;
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
    void clickTilesButtons(ALL *allegro);
    void clickModesButtons(ALL* allegro);
    void checkTilesButtons(double volume);

    //Equipments methods
    void drawEquipment(EquipmentsEnum equipEnum);
    void drawStormCards(ALL *allegro);

    void performEquipmentsAction(EquipmentsEnum equipEnum, ALL *allegro) override;

};


#endif //FORBIDDENDESERT_MULTIPLAYERGAME_H
