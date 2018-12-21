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
    ALL allegroData = {0, 0, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
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
    al_destroy_display(allegro->startMenuDisplay);
    singlePlayer = startMenu->get_player_mode();
    delete startMenu;

    Container *info = new Container();
    if (singlePlayer) {
        cout << "Single Player Mode" << endl;
        stillPlaying = true;
        ChoosePlayer *choosePlayer;
        choosePlayer = new ChoosePlayer(allegro);
        while (stillPlaying) {
            stillPlaying = choosePlayer->eventHandler(allegro);
        }
        char rol1 = static_cast<char>(choosePlayer->get_p1_role());
        char rol2 = static_cast<char>(choosePlayer->get_p2_role());
        cout << "Roles chosen: " << rol1 << " & " << rol2 << endl;
        info->startinfo((roles) rol1, (roles) rol2);

        //Destroy allegro's things that were initialize in ChoosePlayer
        al_unregister_event_source(allegro->events_queue, al_get_display_event_source(allegro->display));
        al_destroy_display(allegro->display);
        delete choosePlayer;
    }
    else {
        cout << "Multi Player Mode" << endl;
        do { //Connection
            if (gameNetwork != nullptr)
                delete gameNetwork;
            gameNetwork = new FSMI;
            gameNetwork->start(allegro);
            al_destroy_display(allegro->startMenuDisplay);
        } while (gameNetwork->net.abort);
    }
	//Game
	cout << "Players chosen. Ready to start game" << endl;
	Game *game = nullptr;
	do {
		if (game != nullptr) {  // New game?
			delete game;
			gameNetwork->restart(allegro);
		}
		if (singlePlayer) {
		    game = new Game(allegro, info);
		}
		else {
            game = new Game(allegro, gameNetwork);
		}

		stillPlaying = true;
		while (stillPlaying) {
			stillPlaying = game->eventHandler(allegro);
		}
		al_destroy_display(allegro->display); // If not I'll end up with 2 displays
	} while (game->getPlayAgain() && !game->getGameOver());

	if(!singlePlayer) {
        delete gameNetwork;
    }
	delete game;
	destroyAll(allegro);

	return 0;
}