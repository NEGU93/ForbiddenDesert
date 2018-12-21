#include "Packages.hpp"
#include <iostream>
#include <include/Packages.hpp>


//Constructor
Pack::Pack(PolonetConn connection){
	buffer = NULL;
	for (int i = 0; i < BUFFERSIZE; i++)
		recbuffer[i] = 0;	//Inicializo buffer en 0
	size = 0;
	this->connection = connection;	//Guardo conexion determinada en el objeto
}
//DESTRUCTOR
Pack::~Pack(){
	if(buffer!=NULL)
		delete[] buffer;	//Elimino memoria alocada con new
	else
		return;
}
//Envio paquetes
void Pack::sendPack(){
	if (isConnected(connection))
		sendData(connection, buffer, size);	//En caso de haber una conexion vigente, envio paquete completo de datos
	else
		std::cout << "Warning: Attempt of sending packs through a non-stablished connection" << endl;
}
Container::Container(){//Constructor, inicializo Slots, ID, arreglo de tiles y nombre del usuario en 0
	Slot1 = 0;
	Slot2 = 0;
	Slot3 = 0;
	Slot4 = 0;
	Slot5 = 0;
	Slot6 = 0;
	Slot7 = 0;
	ID = 0;
	for (int i = 0; i < CANTOFTILES; i++) tiles[i] = DEFAULT;
	for (int i = 0; i < CANTOFSTORM; i++) storm[i] = SUNBEATSDOWN;
	for (int i = 0; i < CANTOFEQUIPMENT; i++) equipment[i] = DUNEBLAST;
	for (int i = 0; i < SIZEOFNAME; i++) name[i] = 0;
}	
Container Pack::receivePack(){
	Container ret;
	unsigned char ID;
	receiveData(connection, recbuffer, sizeof(recbuffer));
	ID = recbuffer[0];
	switch(ID)
	{
	case  NAME:
		ret.ID = recbuffer[0];
		break;
	case  NAME_IS:
		ret.ID = recbuffer[0];
		for (int i = 0; i < 20; i++) ret.name[i] = 0;
		memcpy(&ret.name[0], &recbuffer[2], recbuffer[1]);
		ret.name[recbuffer[1]] = '\0';
		break;
	case  ACK:
		ret.ID = recbuffer[0];
		break;
	case  I_START:
		ret.ID = recbuffer[0];
		break;
	case  YOU_START:
		ret.ID = recbuffer[0];
		break;
	case  MOVE:
		ret.ID = recbuffer[0];
		ret.Slot1 = recbuffer[1];
		ret.Slot2 = recbuffer[2];
		break;
	case  REMOVE:
		ret.ID = recbuffer[0];
		ret.Slot1 = recbuffer[1];
		ret.Slot2 = recbuffer[2];
		break;
	case  OFFER_EQUIPMENT:
		ret.ID = recbuffer[0];
		ret.Slot1 = recbuffer[1];
		break;
	case  OFFER_WATER:
		ret.ID = recbuffer[0];
		ret.Slot1 = recbuffer[1];
		break;
	case  REQUEST_EQUIPMENT:
		ret.ID = recbuffer[0];
		ret.Slot1 = recbuffer[1];
		break;
	case  REQUEST_WATER:
		ret.ID = recbuffer[0];
		ret.Slot1 = recbuffer[1];
		break;
	case  REQUEST_MOVE:
		ret.ID = recbuffer[0];
		ret.Slot1 = recbuffer[1];
		ret.Slot2 = recbuffer[2];
		break;
	case  PLAY_EQUIPMENT:
		ret.ID = recbuffer[0];
		ret.Slot1 = recbuffer[1];
		ret.Slot2 = recbuffer[2];
		ret.Slot3 = recbuffer[3];
		break;
	case  PEEP:
		ret.ID = recbuffer[0];
		ret.Slot1 = recbuffer[1];
		break;
	case  EXCAVATE:
		ret.ID = recbuffer[0];
		break;
	case  PICKUP:
		ret.ID = recbuffer[0];
		break;
	case  TAKE_WATER:
		ret.ID = recbuffer[0];
		ret.Slot1 = recbuffer[1];
		break;
	case  DECREASE_WITHDRAWAL:
		ret.ID = recbuffer[0];
		break;
	case  PASS:
		ret.ID = recbuffer[0];
		break;
	case  AGREE:
		ret.ID = recbuffer[0];
		break;
	case  DISAGREE:
		ret.ID = recbuffer[0];
		break;
	case  ERROR_:
		ret.ID = recbuffer[0];
		break;
	case  GAME_OVER:
		ret.ID = recbuffer[0];
		break;
	case PLAY_AGAIN:
		ret.ID = recbuffer[0];
	case  WE_WON:
		ret.ID = recbuffer[0];
		break;
	case  WE_LOST:
		ret.ID = recbuffer[0];
		break;
	case  QUIT:
		ret.ID = recbuffer[0];
		break;
	case  DRAW_STORM_CARDS:
		ret.ID = recbuffer[0];
		ret.Slot1 = recbuffer[1];
		ret.Slot2 = recbuffer[2];
		ret.Slot3 = recbuffer[3];
		ret.Slot4 = recbuffer[4];
		ret.Slot5 = recbuffer[5];
		ret.Slot6 = recbuffer[6];
		break;
	case  START_INFO:
		ret.ID = recbuffer[0];
		ret.Slot1 = recbuffer[1];
		ret.Slot2 = recbuffer[2];
		ret.myRol = (roles)recbuffer[2];
		ret.friendRol = (roles)recbuffer[1];
		for (int i = 0; i < CANTOFTILES; i++)
			ret.tiles[i] = (TilesEnum)recbuffer[i + 3];
		for (int i = 0; i < CANTOFSTORM; i++)
			ret.storm[i] = (StormCardsEnum)recbuffer[i + 3 + CANTOFTILES];
		for (int i = 0; i < CANTOFEQUIPMENT; i++)
			ret.equipment[i] = (EquipmentsEnum)recbuffer[i + 3 + CANTOFTILES + CANTOFSTORM];
		break;
	default:
		ret.ID = NOEVENT;
	}
	//cleanBuffer();
	return ret;
}
void Pack::cleanBuffer(void){
	for (int i = 0; i < BUFFERSIZE; i++)
		recbuffer[i] = 0;
}
// Packs methods
void Pack::ack(){
	if (buffer != NULL)
		delete[] buffer;
	size = sizeof(char);
	buffer = new char[size];
	buffer[0] = ACK;
}
void Pack::agree(){
	if (buffer != NULL)
		delete[] buffer;
	size = sizeof(char);
	buffer = new char[size];
	buffer[0] = AGREE;
}
void Pack::disagree(){
	if (buffer != NULL)
		delete[] buffer;
	size = sizeof(char);
	buffer = new char[size];
	buffer[0] = DISAGREE;
}
void Pack::name(){
	if (buffer != NULL)
		delete[] buffer;
	size = sizeof(char);
	buffer = new char[size];
	buffer[0] = NAME;
}
void Pack::nameis(char* name){
	if (buffer != NULL)
		delete[] buffer;
	size = 2 * sizeof(char) + strlen(name);
	buffer = new char[size];
	buffer[0] = NAME_IS;
	buffer[1] = strlen(name);
	memcpy(&buffer[2], name, strlen(name));
}
roles Pack::rolesort(){		// genera los roles de manera aleatoria
	char rol = rand() % 6 + 0x30;
	return (roles)rol;
}

void Pack::waitForAck(ALL *allegro) {
	char wait;
	al_start_timer(allegro->timeout);
	do {
		cout << "Waiting for ack" << endl;
		wait = receivePack().ID;
		cleanBuffer();
	} while (wait != ACK && al_get_timer_count(allegro->timeout) == TIMEOUT);
	al_stop_timer(allegro->timeout);
	al_set_timer_count(allegro->timeout, 0);
}

void Container::startinfo(roles rol1, roles rol2) {
	char* buffer;
	char size = 3 * sizeof(char) + CANTOFEQUIPMENT + CANTOFSTORM + CANTOFTILES;
	buffer = new char[size];
	buffer[0] = START_INFO;
	buffer[1]= START_INFO;
	//Choose Player
	myRol = (roles) rol1;
	friendRol = (roles) rol2;
	int i = 3;
	buffer[i++] = STORM;
	buffer[i++] = CRASHSITE;
	buffer[i++] = LAUNCHPAD;
	buffer[i++] = DEFAULT;
	int c = 1;
	while (c <= 3) {
		buffer[i++] = TUNNEL;
		c++;
	}
	c = 1;
	while (c <= 2) {
		buffer[i++] = WATER;
		c++;
	}
	buffer[i++] = ENGINECOLUMN;
	buffer[i++] = ENGINEROW;
	buffer[i++] = SOLARCRYSCOLUMN;
	buffer[i++] = SOLARCRYSROW;
	buffer[i++] = NAVIGATIONCOLUMN;
	buffer[i++] = NAVIGATIONROW;
	buffer[i++] = PROPELLERCOLUMN;
	buffer[i++] = PROPELLERROW;
	buffer[i++] = GEAR1;
	buffer[i++] = GEAR2;
	buffer[i++] = GEAR3;
	buffer[i++] = GEAR4;
	buffer[i++] = GEAR5;
	buffer[i++] = GEAR6;
	buffer[i++] = GEAR7;
	buffer[i] = GEAR8;

	std:: random_shuffle(&buffer[3],&buffer[i],myRand);		//Mezclo todos los tiles
	int search;
	for(search = 0; buffer[search] != STORM; search++);		// Busco la sand storm para reubicarla
	std::swap<char>(buffer[search], buffer[15]);			// Reubico la sand storm en el medio, (15 del buffer: la mitad de CANTOFTILES + 3)

	for (int a = 0; a < CANTOFTILES; a++) {
		tiles[a] = (TilesEnum)buffer[a + 3];				// Guardo la informacion del tablero para que la retenga luego de enviarla
	}
	//SandStormCards
	c = 1;
	i++;
	while (c < 3) { //Create 2 of each
		buffer[i++] = RIGHT1;
		buffer[i++] = RIGHT2;
		buffer[i++] = RIGHT3;
		buffer[i++] = LEFT1;
		buffer[i++] = LEFT2;
		buffer[i++] = LEFT3;
		buffer[i++] = UP1;
		buffer[i++] = UP2;
		buffer[i++] = UP3;
		buffer[i++] = DOWN1;
		buffer[i++] = DOWN2;
		buffer[i++] = DOWN3;
		c++;
	}
	c = 1;
	while (c < 4) {
		buffer[i++] = STORMUP;
		c++;
	}
	c = 1;
	while (c < 5) {
		buffer[i++] = SUNBEATSDOWN;
		c++;
	}

	std::random_shuffle(&buffer[CANTOFTILES + 3], &buffer[i], myRand);	//Mezclo todos los tiles... (no son los storm cards?)
	//Harcodie para poder debuggear el SolarShield
	for (int a = 0; a < CANTOFSTORM; a++) {
		storm[a] = (StormCardsEnum)buffer[a + CANTOFTILES + 3];	//Guardo la informacion del tablero para que la retenga luego de enviarla
	}
	//Equipment Cards
	c = 1;
	while (c < 4) {
		buffer[i++] = DUNEBLAST;
		buffer[i++] = JETPACK;
		c++;
	}
	c=1;
	while (c < 3) {
		buffer[i++] = SOLARSHIELD;
		buffer[i++] = TERRASCOPE;
		c++;
	}
	buffer[i++] = SECRETWATERRESERVE;
	buffer[i] = TIMETHROTTLE;

	std::random_shuffle(&buffer[CANTOFTILES + CANTOFSTORM + 3], &buffer[i], myRand);	//Mezclo todos los tiles
	buffer[CANTOFTILES + CANTOFSTORM + 3] = TIMETHROTTLE;//FOR DEBUGGING
	for (int a = 0; a < CANTOFEQUIPMENT; a++) {
		equipment[a] = (EquipmentsEnum)buffer[a + CANTOFTILES + CANTOFSTORM + 3];	//Guardo la informacion del tablero para que la retenga luego de enviarla
	}
}
Container Pack::startinfo(ALL* allegro){
	Container info;

	delete[] buffer;
	size = 3 * sizeof(char) + CANTOFEQUIPMENT + CANTOFSTORM + CANTOFTILES;
	buffer = new char[size];
	buffer[0] = START_INFO;
	buffer[1]=rolesort();
	//Choose Player
	bool stillPlaying = true;
	ChoosePlayer *choosePlayer;
	choosePlayer = new ChoosePlayer(allegro /*, &buffer[1], &buffer[2]*/ );	// TODO: change this or multiplayer broken
	while (stillPlaying) {
		stillPlaying = choosePlayer->eventHandler(allegro);
	}
	//while ((buffer[2] = rolesort()) == buffer[1]);
	//info.myRol = (roles)buffer[1];			// Guardo mi rol
	//info.friendRol = (roles)buffer[2];		// Guardo rol de mi amigo
	info.myRol = (roles) choosePlayer->get_p1_role();
	info.friendRol = (roles) choosePlayer->get_p2_role();
	int i = 3;
	buffer[i++] = STORM;
	buffer[i++] = CRASHSITE;
	buffer[i++] = LAUNCHPAD;
	buffer[i++] = DEFAULT;
	int c = 1;
	while (c <= 3) {
		buffer[i++] = TUNNEL;
		c++;
	}
	c = 1;
	while (c <= 2) {
		buffer[i++] = WATER;
		c++;
	}
	buffer[i++] = ENGINECOLUMN;
	buffer[i++] = ENGINEROW;
	buffer[i++] = SOLARCRYSCOLUMN;
	buffer[i++] = SOLARCRYSROW;
	buffer[i++] = NAVIGATIONCOLUMN;
	buffer[i++] = NAVIGATIONROW;
	buffer[i++] = PROPELLERCOLUMN;
	buffer[i++] = PROPELLERROW;
	buffer[i++] = GEAR1;
	buffer[i++] = GEAR2;
	buffer[i++] = GEAR3;
	buffer[i++] = GEAR4;
	buffer[i++] = GEAR5;
	buffer[i++] = GEAR6;
	buffer[i++] = GEAR7;
	buffer[i] = GEAR8;
	
	std:: random_shuffle(&buffer[3],&buffer[i],myRand);		//Mezclo todos los tiles
	int search;
	for(search = 0; buffer[search] != STORM; search++);		// Busco la sand storm para reubicarla
	std::swap<char>(buffer[search], buffer[15]);			// Reubico la sand storm en el medio, (15 del buffer: la mitad de CANTOFTILES + 3)
	
	for (int a = 0; a < CANTOFTILES; a++) {
		info.tiles[a] = (TilesEnum)buffer[a + 3];			//Guardo la informacion del tablero para que la retenga luego de enviarla
	}
	//SandStormCards
	c = 1;
	i++;
	while (c < 3) { //Create 2 of each
		buffer[i++] = RIGHT1;
		buffer[i++] = RIGHT2;
		buffer[i++] = RIGHT3;
		buffer[i++] = LEFT1;
		buffer[i++] = LEFT2;
		buffer[i++] = LEFT3;
		buffer[i++] = UP1;
		buffer[i++] = UP2;
		buffer[i++] = UP3;
		buffer[i++] = DOWN1;
		buffer[i++] = DOWN2;
		buffer[i++] = DOWN3;
		c++;
	}
	c = 1;
	while (c < 4) {
		buffer[i++] = STORMUP;
		c++;
	}
	c = 1;
	while (c < 5) {
		buffer[i++] = SUNBEATSDOWN;
		c++;
	}

	std::random_shuffle(&buffer[CANTOFTILES + 3], &buffer[i], myRand);	//Mezclo todos los tiles... (no son los storm cards?)
	//Harcodie para poder debuggear el SolarShield
	for (int a = 0; a < CANTOFSTORM; a++) {
		info.storm[a] = (StormCardsEnum)buffer[a + CANTOFTILES + 3];	//Guardo la informacion del tablero para que la retenga luego de enviarla
	}
	//Equipment Cards
	c = 1;
	while (c < 4) {
		buffer[i++] = DUNEBLAST;
		buffer[i++] = JETPACK;
		c++;
	}
	c=1;
	while (c < 3) {
		buffer[i++] = SOLARSHIELD;
		buffer[i++] = TERRASCOPE;
		c++;
	}
	buffer[i++] = SECRETWATERRESERVE;
	buffer[i] = TIMETHROTTLE;

	std::random_shuffle(&buffer[CANTOFTILES + CANTOFSTORM + 3], &buffer[i], myRand);	//Mezclo todos los tiles
	buffer[CANTOFTILES + CANTOFSTORM + 3] = TIMETHROTTLE;//FOR DEBUGGING
	for (int a = 0; a < CANTOFEQUIPMENT; a++) {
		info.equipment[a] = (EquipmentsEnum)buffer[a + CANTOFTILES + CANTOFSTORM + 3];	//Guardo la informacion del tablero para que la retenga luego de enviarla
	}
	//Destroy allegro's things that were initialize in ChoosePlayer
	al_unregister_event_source(allegro->events_queue, al_get_display_event_source(allegro->display));
	al_destroy_display(allegro->display);
	delete choosePlayer;
	return info;
}

void Pack::youstart(){
	if (buffer != NULL)
		delete[] buffer;
	size = sizeof(char);
	buffer = new char[size];
	buffer[0] = YOU_START;
}
void Pack::istart(){
	if (buffer != NULL)
		delete[] buffer;
	size = sizeof(char);
	buffer = new char[size];
	buffer[0] = I_START;
}
void Pack::excavate(){
	if (buffer != NULL)
		delete[] buffer;
	size = sizeof(char);
	buffer = new char[size];
	buffer[0] = EXCAVATE;
}
void Pack::pickup(){
	if (buffer != NULL)
		delete[] buffer;
	size = sizeof(char);
	buffer = new char[size];
	buffer[0] = PICKUP;
}
void Pack::move(char row, char col){
	if (buffer != NULL)
		delete[] buffer;
	size = 3 * sizeof(char);
	buffer = new char[size];
	buffer[0] = MOVE;
	buffer[1] = row;
	buffer[2] = col;
}
void Pack::remove(char row, char col){
	if (buffer != NULL)
		delete[] buffer;
	size = 3 * sizeof(char);
	buffer = new char[size];
	buffer[0] = REMOVE;
	buffer[1] = row;
	buffer[2] = col;
}
void Pack::offerequipment(char equipment){
	if (buffer != NULL)
		delete[] buffer;
	size = 2 * sizeof(char);
	buffer = new char[size];
	buffer[0] = OFFER_EQUIPMENT;
	buffer[1] = equipment;
}
void Pack::offerwater(char water){
	if (buffer != NULL)
		delete[] buffer;
	size = 2 * sizeof(char);
	buffer = new char[size];
	buffer[0] = OFFER_WATER;
	buffer[1] = water;
}
void Pack::requestequipment(char equipment){
	if (buffer != NULL)
		delete[] buffer;
	size = 2 * sizeof(char);
	buffer = new char[size];
	buffer[0] = REQUEST_EQUIPMENT;
	buffer[1] = equipment;
}
void Pack::requestwater(char water){
	if (buffer != NULL)
		delete[] buffer;
	size = 2 * sizeof(char);
	buffer = new char[size];
	buffer[0] = REQUEST_WATER;
	buffer[1] = water;
}
void Pack::requestmove(char row, char col){
	if (buffer != NULL)
		delete[] buffer;
	size = 3 * sizeof(char);
	buffer = new char[size];
	buffer[0] = REQUEST_MOVE;
	buffer[1] = row;
	buffer[2] = col;
}
void Pack::takewater(){
	if (buffer != NULL)
		delete[] buffer;
	size = sizeof(char);
	buffer = new char[size];
	buffer[0] = TAKE_WATER;
}
void Pack::decreasewithdrawal(){
	if (buffer != NULL)
		delete[] buffer;
	size = sizeof(char);
	buffer = new char[sizeof(char)];
	buffer[0] = DECREASE_WITHDRAWAL;
}
void Pack::playequipment(char equipment, char row, char col){
	if (buffer != NULL)
		delete[] buffer;
	size = sizeof(equipment) + sizeof(row) + sizeof(col) + sizeof(char);
	buffer = new char[size];
	buffer[0] = PLAY_EQUIPMENT;
	buffer[1] = equipment;
	buffer[2] = row;
	buffer[3] = col;
}
void Pack::pass(){
	if (buffer != NULL)
		delete[] buffer;
	size = sizeof(char);
	buffer = new char[sizeof(char)];
	buffer[0] = PASS;
}
void Pack::wewon(){
	if (buffer != NULL)
		delete[] buffer;
	size = sizeof(char);
	buffer = new char[sizeof(char)];
	buffer[0] = WE_WON;
}
void Pack::welost(){
	if (buffer != NULL)
		delete[] buffer;
	size = sizeof(char);
	buffer = new char[sizeof(char)];
	buffer[0] = WE_LOST;
}
void Pack::gameover() {
	if (buffer != NULL)
		delete[] buffer;
	size = sizeof(char);
	buffer = new char[sizeof(char)];
	buffer[0] = (char)GAME_OVER;
}
void Pack::playagain() {
	if (buffer != NULL)
		delete[] buffer;
	size = sizeof(char);
	buffer = new char[sizeof(char)];
	buffer[0] = (char)PLAY_AGAIN;
}
void Pack::quit(){
	if (buffer != NULL)
		delete[] buffer;
	size = sizeof(char);
	buffer = new char[size];
	buffer[0] = (char)QUIT;
}
void Pack::error(){
	if (buffer != NULL)
		delete[] buffer;
	size = sizeof(char);
	buffer = new char[size];
	buffer[0] = (char)ERROR_;
}
void Pack::peep(char index){
	if (buffer != NULL)
		delete[] buffer;
	size = 2 * sizeof(char);
	buffer = new char[size];
	buffer[0] = PEEP;
	buffer[1] = index;
}
void Pack::drawstrcard(char first, char second, char third, char forth, char fifth, char sixth){
	if (buffer != NULL)
		delete[] buffer;
	size = sizeof(char) + sizeof(first) + sizeof(second) + sizeof(third);
	if (forth != 0)size++;
	if (fifth != 0)size++;
	if (sixth != 0)size++;
	buffer = new char[size];
	buffer[0] = DRAW_STORM_CARDS;
	buffer[1] = first;
	buffer[2] = second;
	switch (size)
	{
	case 4:
		buffer[3] = third;
		break;
	case 5:
		buffer[3] = third;
		buffer[4] = forth;
		break;
	case 6:
		buffer[3] = third;
		buffer[4] = forth;
		buffer[5] = fifth;
		break;
	case 7:
		buffer[3] = third;
		buffer[4] = forth;
		buffer[5] = fifth;
		buffer[6] = sixth;
		break;
	}
}


void Container:: addName(char* name){
	strncpy(this->name, name, SIZEOFNAME);
}
void Container:: addFriendName(char* friendName){
	strncpy(this->friendName, friendName, SIZEOFNAME);
}
void Container:: addInfo(Container info){
	myRol = info.myRol;
	friendRol = info.friendRol;
	for (int i = 0; i < CANTOFTILES; i++) 
		tiles[i] = info.tiles[i];
	for (int i = 0; i < CANTOFSTORM; i++)
		storm[i] = info.storm[i];
	for (int i = 0; i < CANTOFEQUIPMENT; i++)
		equipment[i] = info.equipment[i];
}

int myRand(int i){
	//funcion que genera numero aleatorio, se usa para ordenar los tiles, como callback
	return rand() % i;
}