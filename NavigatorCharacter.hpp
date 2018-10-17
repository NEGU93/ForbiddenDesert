#ifndef NAVIGATORCHARACTER_H_
#define NAVIGATORCHARACTER_H_

#include "Character.hpp"

class NavigatorCharacter : public Character{
public:
	NavigatorCharacter(int rowSet, int columnSet, Container info, ALL *allegro);
	~NavigatorCharacter();

	bool checkMouseOverButton(int x, int y, bool moveState, double volume);
	void updateNavigator(ALL* allegro, bool moveState);

	bool havePartnersMovesLeft() { return (partnerMovesLeft > 0); }
	bool decresePartnerMoves();

	bool haveMoves() { return numberOfMoves > 0; }
	void decreseMoves() { numberOfMoves--; }

	void resetPartnerMoves();
private:
	ALLEGRO_BITMAP* watersImage[MAXOFWATERS];
	ALLEGRO_BITMAP* navigatorImage;
	Button *moveOtherPlayerButton;
	Button *doneButton;
	int partnerMovesLeft;
};

#endif