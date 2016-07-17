
#ifndef FSMINIT
#define FSMINIT

#include <iostream>
using namespace std;
#include "allegro.h"
#include "Packages.h"
#include "Network.h"

#include "def.h"

typedef enum {TURN,ASK4NAME, IDLE,W4NAME,READINGNAME,W4STRING,SENDNAME,W4ACK,STARTINFO,W4STARTINFO,W4ACK2,SENDACK,W4TURN,W4ACK3,END} eventtype;

class FSMI
{
public:
	FSMI();
	void start(ALL *allegro);
	void restart(ALL *allegro);
	void checkdata();
	Container getInfo();
	void print(Container ex);
	initNetwork net;
protected:
	Container data;			//Se usa de buffer para mandar y recibir informacion, por medio de los slots
	Container Informacion;	// Container para tener los datos guardados del usuario, solo se usan name, tiles, roles y turno. No slots.
	Pack pck;	//Paquete de datos
	char nameready;
	char state;
	char prevstate;
	char exit;	//Para controlar el flujo de la fsm (1 o 0)
	char turn;
	void changestate(char newstate);	//Variable para la maquina de estados inicial
	void turnsort();	// Funcion para determinar el turno inicial, aleatorio
};

#endif