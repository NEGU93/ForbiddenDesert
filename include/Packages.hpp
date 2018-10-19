#ifndef PACKAGE_H_
#define PACKAGE_H_

#include <iostream>
#include "polonet.h"
//#include <windows.h>
#include <algorithm>
#include "Tile.hpp"
#include "StormCard.hpp"
#include "ChoosePlayer.hpp"

#define BUFFERSIZE 255
#define CANTOFTILES 25
#define CANTOFSTORM 31
#define CANTOFEQUIPMENT 12
#define NAME 0x10
#define NAME_IS 0x11
#define ACK 0x01
#define I_START 0x21
#define YOU_START 0x20
#define MOVE 0x31
#define REMOVE 0x32
#define OFFER_EQUIPMENT 0x35
#define OFFER_WATER 0x36
#define REQUEST_EQUIPMENT 0x37
#define REQUEST_WATER 0x38
#define REQUEST_MOVE 0x39
#define PLAY_EQUIPMENT 0x3D
#define PEEP 0x3C
#define EXCAVATE 0x33
#define PICKUP 0x34
#define TAKE_WATER 0x3A
#define DECREASE_WITHDRAWAL 0x3B
#define PASS 0x3E
#define AGREE 0x02
#define DISAGREE 0x03
#define ERROR_ 0xFF
#define PLAY_AGAIN 0x43
#define GAME_OVER 0x42
#define WE_WON 0x40
#define WE_LOST 0x41
#define QUIT 0xfe
#define DRAW_STORM_CARDS 0x3F
#define START_INFO 0x12
#define NOEVENT 0x00

#define TIMEOUT 5 //5 seconds for timeout.

typedef enum {ARCHEOLOGIST=0x30, CLIMBER, EXPLORER, METEOROLOGIST, NAVIGATOR, WATER_CARRIER} roles;
enum EquipmentsEnum{DUNEBLAST=0x01, JETPACK, SOLARSHIELD, TERRASCOPE, SECRETWATERRESERVE,TIMETHROTTLE}; 

int myRand(int i);

class Container;
class Pack
{
public:

	void sendPack();//
	Container receivePack();
	Pack(PolonetConn connection=0);
	~Pack();
	void cleanBuffer();
	void waitForAck(ALL *allegro);
	//Genero todos los paquetes de datos
	void ack();//
	void agree();//
	void disagree();//
	void name();//
	void nameis(char* name);
	Container startinfo(ALL* allegro);
	void youstart();//
	void istart();//
	void move(char row, char column);//
	void remove(char row, char column);//
	void excavate();//
	void pickup();//
	void offerequipment(char equipment);//
	void offerwater(char water);//
	void requestequipment(char equipment);
	void requestwater(char water);
	void requestmove(char row, char column);//
	void takewater();
	void decreasewithdrawal();
	void peep(char index); 
	void playequipment(char equipment, char row, char col);
	void pass();
	void drawstrcard(char first, char second, char third=0, char forth=0, char fifth=0, char sixth=0); //
	void wewon();
	void welost();
	void gameover();
	void playagain();
	void quit();
	void error();
	PolonetConn connection;

protected:
	char* buffer;
	char recbuffer[BUFFERSIZE];
	char size;
	roles rolesort();	//Rol del usuario, tambien aleatorio

};


class Container
{
public:
	Container();
	//~Container();	No hay destructor
	
	void addName(char* name);
	void addFriendName(char* name);
	void addInfo(Container info);
	unsigned char ID;	//ID de cada uno
	char Slot1;
	char Slot2;
	char Slot3;
	char Slot4;
	char Slot5;
	char Slot6;
	char Slot7;
	char name[SIZEOFNAME];	//Nombre
	char friendName[SIZEOFNAME];
	TilesEnum tiles[CANTOFTILES];	//Arreglo de las cartas
	StormCardsEnum storm[CANTOFSTORM];
	EquipmentsEnum equipment[CANTOFEQUIPMENT];
//	EquipmentEnum equipment[6];
	roles myRol;
	roles friendRol;
	char myTurn;
};

#endif // PACKAGE_H_
