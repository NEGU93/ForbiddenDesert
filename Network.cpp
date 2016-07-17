#include "Network.h"

initNetwork::initNetwork() {
	seedStart();
	port = PORT;
	randGenerator();
	abort = false;
}
void initNetwork::initializeNet(ALL *allegro) {
	al_set_target_backbuffer(allegro->startMenuDisplay);
	getIP(allegro);
	connection = openConnection(IP, port);
	printAllegro(allegro, "Trying Connection as Client");
	al_set_timer_count(allegro->timeout, 0);
	al_start_timer(allegro->timeout);

	for (; randNum && (!(isConnected(connection))); randNum--) {
		connection = openConnection(IP, port);
		Sleep(0.99);
	}
	for (int i = 20; isPending(connection) && i; i--) {
		Sleep(200);
	}
	if (!(isConnected(connection))) {
		printAllegro(allegro, "Trying Connection as Server");
		startListening(port);
		while (!(connection = getAvailableConnection()) && al_get_timer_count(allegro->timeout)<TIME_OUT)
			Sleep(500);
		while (isPending(connection))
			Sleep(1);
		typeconn = SERVER;
		printAllegro(allegro, "Connection Sucessful!");
	}
	else {
		printAllegro(allegro, "Connection Sucessful!");
		typeconn = CLIENT;
	}
	if (al_get_timer_count(allegro->timeout) >= TIME_OUT) {
		printAllegro(allegro, "TIMEOUT- Connection aborted");
		abort = true;
	}
}
void  initNetwork::seedStart() {
	srand(time(NULL));
}
void initNetwork::randGenerator() {
	randNum = rand() % 9801 + 200;
}
void initNetwork::getIP(ALL* allegro) {
	printAllegro(allegro, "Please, enter the IP");
	char* a;
	a = getcharAllegro(allegro, 8, HEIGHT * allegro->screenHeight / MAXRESOLUTION_Y / 3);
	memcpy(IP, a, strlen(a) + 1);
}
initNetwork::~initNetwork() {
	closeConnection(connection);
}