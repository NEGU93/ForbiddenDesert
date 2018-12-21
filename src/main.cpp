#pragma warning(disable: 4996)
#include "def.hpp"
#include "allegro.hpp"

#include "ChoosePlayer.hpp"
#include "Game.hpp"

/* ------------------- TODO -----------------------
5. Para poder declarar variables arm� constructores por defecto vacios... estar�a bueno borrarlos y ponerle argumentos por defecto (diapositiva 10-Administraci�n de memoria, pagina 8)
*/

/*BUG REPORT
1.Al utilizar los vinoculares, se da vuelta la carta a espiar en la pantalla del jugador que lo utiliza, pero no se muestra al segundo jugador.
Luego de espiar esa carta, para volver a colocarla boca abajo, se pone boca abajo la �ltima carta dada vuelta, pero para AMBOS jugadores.
Dado que esa carta nunca se muestra para el segundo jugador, a este se le da vuelta la �ltima carta explorada, generando una desinconizaci�n
entre las cartas de los jugadores. Esto se soluciona haciedno que la carta espiada con vinoculares se muestre a ambos jugadores, o cuidandose de 
no voltear la �ltima carta jugada en el jugador quyo turno no est� en proceso.

2.Tuve un problema con offer water, pero es porque no se hacer bien la l�gica de la condici�n de que se puede hacer adyacente.

3.Al utilizar el tunel, el programa no se fija si los turnos llegaron a cero, permitiendo el jugador tener un numero negativo de turnos restantes
viajando de un lado hacia el otro del tunel repetidas veces. (Creo que arreglado, hice que se fijara si puede hacer setPos utilizando decreseMoves que devuelva bool)
*/

int main() {
    //Allegro initialization
    ALL allegroData = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    ALL *allegro = &allegroData;
    if (!init_allegro(allegro)) {
        cout << "Failed to initialize allegro. Closing Forbidden Desert." << endl;
    }
    FSMI *gameNetwork = nullptr;
    bool stillPlaying;
    bool chooseMode = true;
    bool singlePlayer;

    auto *startMenu = new StartMenu(allegro);
    while (chooseMode) {
        chooseMode = startMenu->eventHandler(allegro);
    }
    singlePlayer = startMenu->get_player_mode();

    if (singlePlayer) {
        cout << "Single Player Mode" << endl;
        char *rol1;
        char *rol2;
        stillPlaying = true;
        ChoosePlayer *choosePlayer;
        choosePlayer = new ChoosePlayer(allegro, rol1, rol2);
        while (stillPlaying) {
            stillPlaying = choosePlayer->eventHandler(allegro);
        }
        cout << "Roles chosen: " << rol1 << rol2 << endl;
    }
    else {
        cout << "Multi Player Mode" << endl;
        do { //Connection
            if (gameNetwork != NULL)
                delete gameNetwork;
            gameNetwork = new FSMI;
            gameNetwork->start(allegro);
            al_destroy_display(allegro->startMenuDisplay);
        } while (gameNetwork->net.abort);
    }
	//Game
	return 0;
	Game *game = nullptr;
	do {
		if (game != nullptr) {
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

	if(!singlePlayer) {
        delete gameNetwork;
    }
	delete game;
	destroyAll(allegro);

	return 0;
}