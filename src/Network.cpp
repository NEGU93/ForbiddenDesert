#include "Network.hpp"

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
#ifdef __linux__
        usleep(1000);
#elif _WIN32
        Sleep(1);
#endif
	}
	for (int i = 20; isPending(connection) && i; i--) {
#ifdef __linux__
        usleep(200000);
#elif _WIN32
        Sleep(200);
#endif
	}
	if (!(isConnected(connection))) {
		printAllegro(allegro, "Trying Connection as Server");
		startListening(port);
		while (!(connection = getAvailableConnection()) && al_get_timer_count(allegro->timeout)<TIME_OUT)
#ifdef __linux__
            usleep(500000);
#elif _WIN32
        Sleep(500);
#endif
		while (isPending(connection)) {
#ifdef __linux__
            usleep(1000);
#elif _WIN32
            Sleep(1);
#endif
        }
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