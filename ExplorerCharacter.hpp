#ifndef EXPLORERCHARACTER_H_
#define EXPLORERCHARACTER_H_

#include "Character.hpp"

class ExplorerCharacter : public Character{
public:
	ExplorerCharacter(int rowSet, int columnSet, Container info, ALL* allegro);
	~ExplorerCharacter();
	//Special movements.
	void moveUpRightDiagonally();
	void moveUpLeftDiagonally();
	void moveDownRightDiagonally();
	void moveDownLeftDiagonally();
	void useDuneBlasterDiagonally();

private:
	ALLEGRO_BITMAP* watersImage[MAXOFWATERS];
	ALLEGRO_BITMAP* explorerImage;
};

#endif