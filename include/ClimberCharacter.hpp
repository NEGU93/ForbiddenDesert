#ifndef CLIMBERCHARACTER_H_
#define CLIMBERCHARACTER_H_

#include "Character.hpp"
#include "Button.hpp"

class ClimberCharacter : public Character {
public:
	ClimberCharacter(int rowSet, int columnSet, Container info, ALL* allegro);
	~ClimberCharacter();
	void updateClimber(ALL* allegro);
	void clickOnButton(int x, int y, double volume);
	void checkMouseOverButton(int x, int y, double volume) { takeWithMeButton->checkMouse(x, y, volume); }
	//Getter
	bool getTakeWithMe() { return takeWithMe; }
private:
	void toogleTakeWithMe() { takeWithMe = !takeWithMe; }
	ALLEGRO_BITMAP* watersImage[MAXOFWATERS];
	ALLEGRO_BITMAP* climberImage;
	Button *takeWithMeButton;
	ALLEGRO_BITMAP* selectedImage;
	ALLEGRO_BITMAP* unableToTakeCharacterWithMe;	//TODO: Esto aun no funciona... habr�a que hacer un updateChar que le pase una variable extra y le diga si puede o no (game sabe si hay dos pjs en el mismo lugar)
	bool takeWithMe;
};

#endif