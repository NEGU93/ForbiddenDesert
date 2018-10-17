#ifndef PART_H_INCLUDED
#define PART_H_INCLUDED

#include "def.hpp"
#include "allegro.hpp"

class Part {
public:
	Part();
	Part(const char *partImageName);
	~Part();
	void updatePart(ALL* allegro);
	//Find Item
	void findRow(int newRow, ALL* allegro);
	void findColum(int newColumn, ALL* allegro);
	void pickUpPart();
	//Getters
	bool getFoundState() { return found; }
	bool getPickedState() { return picked; }
	int getRow() { return row; }
	int getColumn() { return column; }
	uint getCardIndex() { return 5 * column + row; }
	//Setters
	void setCardIndex(int index, ALL* allegro);
private:
	pos updateCoords(ALL* allegro);
	pos partCoords;
	bool found;
	bool picked;
	int row;		//This will state the location of the part... if -1 means it has not been found yet.
	int column;
	ALLEGRO_BITMAP *partImage;
	ALLEGRO_SAMPLE *pickUpSound;
};

#endif