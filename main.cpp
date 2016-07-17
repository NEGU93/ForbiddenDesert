#pragma warning(disable: 4996)
#include "def.h"
#include "allegro.h"

#include "ChoosePlayer.h"
#include "Game.h"

/* ------------------- TODO -----------------------
5. Para poder declarar variables armé constructores por defecto vacios... estaría bueno borrarlos y ponerle argumentos por defecto (diapositiva 10-Administración de memoria, pagina 8)
*/

/*BUG REPORT
1.Al utilizar los vinoculares, se da vuelta la carta a espiar en la pantalla del jugador que lo utiliza, pero no se muestra al segundo jugador.
Luego de espiar esa carta, para volver a colocarla boca abajo, se pone boca abajo la última carta dada vuelta, pero para AMBOS jugadores.
Dado que esa carta nunca se muestra para el segundo jugador, a este se le da vuelta la última carta explorada, generando una desinconización
entre las cartas de los jugadores. Esto se soluciona haciedno que la carta espiada con vinoculares se muestre a ambos jugadores, o cuidandose de 
no voltear la última carta jugada en el jugador quyo turno no está en proceso.

2.Tuve un problema con offer water, pero es porque no se hacer bien la lógica de la condición de que se puede hacer adyacente.

3.Al utilizar el tunel, el programa no se fija si los turnos llegaron a cero, permitiendo el jugador tener un numero negativo de turnos restantes
viajando de un lado hacia el otro del tunel repetidas veces. (Creo que arreglado, hice que se fijara si puede hacer setPos utilizando decreseMoves que devuelva bool)
*/

void main(void) {
	//Allegro initialization
	ALL allegroData = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	ALL* allegro = &allegroData;
	if (!init_allegro(allegro)) {
		cout << "Failed to initialize allegro. Closing Forbidden Desert." << endl;
	}
	bool stillPlaying = true;
	FSMI *gameNetwork = NULL;
	//Connection
	do {
		if (gameNetwork != NULL)
			delete gameNetwork;
		gameNetwork = new FSMI;
		gameNetwork->start(allegro);
		al_destroy_display(allegro->startMenuDisplay);
	} while (gameNetwork->net.abort);
	//Game
	Game *game = NULL;
	do {
		if (game != NULL) {
			delete game;
			gameNetwork->restart(allegro);
		}
		game = new Game(allegro, gameNetwork);
		stillPlaying = true;
		while (stillPlaying) {
			stillPlaying = game->eventHandler(allegro);
		}
		//ESTO LO HICE PARA QUE NO APAREZCAN DOS DISPLAY UNA VEZ QUE QUIERO JUGAR DE NUEVO
		al_destroy_display(allegro->display);
	} while (game->getPlayAgain() && !game->getGameOver());

	delete gameNetwork;
	delete game;
	destroyAll(allegro);
}