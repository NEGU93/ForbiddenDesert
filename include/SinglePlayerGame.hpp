//
// Created by barrachina on 12/21/18.
//

#ifndef FORBIDDENDESERT_SINGLEPLAYERGAME_H
#define FORBIDDENDESERT_SINGLEPLAYERGAME_H

#include "Game.hpp"

class SinglePlayerGame : public Game {
public:
    SinglePlayerGame(ALL *allegro, Container *info);
    bool eventHandler(ALL *allegro) override;
private:
    //! Methods
    void pickUpAction() override;
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
    void click(ALL* allegro) override;
    void clickOnPeepMode(ALL* allegro) override;
    void clickOnJetPackMode(ALL* allegro) override;
    void clickOnTerrascopeMode(ALL* allegro) override;
    void clickOnRequestWaterMode(ALL*  allegro) override;
    void clickOnOfferEquipmentMode(ALL* allegro) override;
    void clickOnRequestEquipmentMode(ALL* allegro) override;
    void clickOnOfferWaterMode(ALL* allegro) override;
    void clickOnDefaultMode(ALL* allegro) override;     // TODO: here to change player
    void clickTilesButtons(ALL *allegro) override;
    void clickModesButtons(ALL* allegro) override;
    void checkTilesButtons(double volume) override;

    //Equipments methods
    void drawEquipment(EquipmentsEnum equipEnum) override;
    void drawStormCards(ALL *allegro) override;

    void performEquipmentsAction(EquipmentsEnum equipEnum, ALL *allegro) override;
};


#endif //FORBIDDENDESERT_SINGLEPLAYERGAME_H
