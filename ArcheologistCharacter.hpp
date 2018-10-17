#ifndef ARCHEOLOGISTCHARACTER_H_
#define ARCHEOLOGISTCHARACTER_H_

#include "Character.hpp"

class ArcheologistCharacter : public Character{
public:
	ArcheologistCharacter(int rowSet, int columnSet, Container inf, ALL *allegro);
    ~ArcheologistCharacter();

private:
	ALLEGRO_BITMAP* watersImage[MAXOFWATERS];
	ALLEGRO_BITMAP* archeologistImage;
};

#endif