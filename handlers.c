#include "handlers.h"
#define RIGHT_LIMIT 1212-35
#define LEFT_LIMIT	675
#define GRAVITY 0.24
#define Vy 2.25* sqrt(3.0)
#define Vx 2.25
#define WORMSIZE 9

void walking(ALL* allegro, worm* worms[MAX_WORMS], int i);
void jumping(ALL *allegro, worm *worms[MAX_WORMS], int i);

void refresh(ALL* allegro, worm * worms[MAX_WORMS])  {
	int i;
	al_clear_to_color(al_map_rgb(0,0,0));
	al_draw_bitmap(allegro->fondo,0,0,0);
	/*
		Si el worms está mirando para la derecha, dibuja sin flip
	*/
	for(i = 0;i < MAX_WORMS; i++) {
		if(worms[i]->direccion==LEFT)
			al_draw_bitmap(worms[i]->currentImg,worms[i]->position.x,worms[i]->position.y,0);
		/*
			De lo contrario, dibuja con flip, que sería como si fuese una imagen espejada.
		*/
		else al_draw_bitmap(worms[i]->currentImg,worms[i]->position.x,worms[i]->position.y,ALLEGRO_FLIP_HORIZONTAL);
	}
	
	al_flip_display();
}

void handlerMove(ALL* allegro, worm *worms[MAX_WORMS]) {
	int i;
	/*
		Este "for" se hace para que haga cada que se haga con ambos worms
	*/
	for (i = 0;i <= 1;i++) {
		/*
			Si el worm está caminando realiza todo esta acción.
		*/
		if (worms[i]->estado == WALKING) {
			walking(allegro, worms, i);
		}
		/*
			Si está saltando, hace lo siguiente
		*/
		if (worms[i]->estado == JUMPING) {
			jumping(allegro, worms, i);
		}
		else worms[i]->numofjump = 0;
	}
}

void handlerToogleRight(ALL* allegro, worm* worm) {
	worm->direccion = RIGHT;
	worm->estado = STILL;
	worm->numofmove = 0;
	worm->currentImg = allegro->move[worm->numofmove];
}

void handlerToogleLeft(ALL* allegro, worm* worm) {
	worm->direccion = LEFT;
	worm->estado = STILL;
	worm->numofmove = 0;
	worm->currentImg = allegro->move[worm->numofmove];
}

/* Funciones internas al handlers, se encargan de cuando el worm está saltando o caminando*/

void jumping(ALL *allegro, worm *worms[MAX_WORMS], int i) {

	if (worms[i]->numofjump == 0) {
		al_set_timer_count(allegro->jumptimer[i], 0);
		worms[i]->inicial.x = worms[i]->position.x;
		al_play_sample(allegro->sample[1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	}
	//Etapas del salto

	if (worms[i]->numofjump <= 5) {			//Primer etapa, solo mueve la imagen pero no cambia la ubicación del gusanito
		worms[i]->currentImg = allegro->jump[5 + worms[i]->numofjump++];
	}
	if (worms[i]->numofjump > 5 && worms[i]->numofjump <= 10) {  //Segunda etapa
		worms[i]->currentImg = allegro->jump[14 - worms[i]->numofjump++];
	}
	//Tercera etapa, acá empieza a moverse hacia arriba
	if (worms[i]->currentImg == allegro->jump[5] && (worms[i]->currentImg == allegro->jump[5] && worms[i]->numofjump <= 40)) {
		//Movimiento en Y
		worms[i]->position.y = FLOOR - Vy *(al_get_timer_count(allegro->jumptimer[i])) + GRAVITY/2 * (al_get_timer_count(allegro->jumptimer[i])) * (al_get_timer_count(allegro->jumptimer[i]));
		//Movimiento en X
		if ((worms[i]->position.x > LEFT_LIMIT- WORMSIZE) && (worms[i]->position.x < RIGHT_LIMIT+ WORMSIZE)) {	//Se hace esto para que salte si hice toogle y estoy en el límite del estadio
			if (worms[i]->direccion == LEFT) {	
				if (worms[i]->position.x > LEFT_LIMIT) {		//Solo se mueve en x si aun está dentro del rango
					worms[i]->position.x = worms[i]->inicial.x - Vx * (al_get_timer_count(allegro->jumptimer[i]));
				}
			}
			else {
				if (worms[i]->position.x < RIGHT_LIMIT) {
					worms[i]->position.x = worms[i]->inicial.x + Vx * (al_get_timer_count(allegro->jumptimer[i]));
				}
			}
		}
		worms[i]->numofjump++;
		if (worms[i]->position.y > FLOOR) {
			worms[i]->position.y = FLOOR;
			worms[i]->numofjump = 41;
		}
	}

	//Etapas Finales, ya cayó el worm
	if (worms[i]->numofjump > 40 && worms[i]->numofjump <= 45)
	{
		worms[i]->currentImg = allegro->jump[worms[i]->numofjump - 35];
		worms[i]->position.y = FLOOR;
		worms[i]->numofjump++;
	}
	if (worms[i]->numofjump > 45 && worms[i]->numofjump < 50) {
		worms[i]->currentImg = allegro->jump[worms[i]->numofjump - 54];
		worms[i]->numofjump++;
	}
	if (worms[i]->numofjump == 50) {
		al_play_sample(allegro->sample[3], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		worms[i]->currentImg = allegro->move[0];
		worms[i]->estado = STILL;
	}
}

void walking(ALL* allegro, worm* worms[MAX_WORMS], int i) {
	if (al_get_timer_count(allegro->movingtimer[i]) > TOOGLE_TIME) {	//Controlo que el timer sea mayor a 100, para que recién se empieze a mover en el FRAME 5.

		//Solo se mueve si esta dentro de los limites del escenario
		if (((worms[i]->position.x > LEFT_LIMIT) || (worms[i]->direccion == RIGHT)) && ((worms[i]->position.x < RIGHT_LIMIT) || (worms[i]->direccion == LEFT))) {
			//Trabaja con las imagenes nada mas, no mueve realmente al gusano.
			if (worms[i]->currentImg != allegro->move[14]) {
				worms[i]->numofmove++;	//incremento el numero de movimiento.
				if (worms[i]->numofmove <= 14)	//las primeras 15 imagenes se muestran.
					worms[i]->currentImg = allegro->move[worms[i]->numofmove];
				else if (worms[i]->numofmove > 14 && worms[i]->numofmove <= 29)//las segundas 15.
					worms[i]->currentImg = allegro->move[worms[i]->numofmove - 15];
				else if (worms[i]->numofmove > 29 && worms[i]->numofmove <= 44)//Las terceras 15.
					worms[i]->currentImg = allegro->move[worms[i]->numofmove - 30];
			}
			else {
				worms[i]->currentImg = allegro->move[0];	//si llega a una de las partes del movimiento, cambia la posición del worm.
				worms[i]->numofmove++;
				//Mueve al gusano
				al_play_sample(allegro->sample[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

				if (worms[i]->direccion == LEFT) {
					worms[i]->position.x -= WORMSIZE;
				}
				else {
					worms[i]->position.x += WORMSIZE;
				}
			}
			if (worms[i]->numofmove == 44) {
				al_play_sample(allegro->sample[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				worms[i]->estado = STILL;
				worms[i]->numofmove = 0;
				worms[i]->currentImg = allegro->move[0];
				if (worms[i]->direccion == LEFT) {
					worms[i]->position.x -= WORMSIZE;
				}
				else {
					worms[i]->position.x += WORMSIZE;
				}
			}
		}
		else { //Si estoy en el limite no lo muevo
			worms[i]->currentImg = allegro->move[0];
			worms[i]->estado = STILL;
			worms[i]->numofmove = 0;
		}

	}
}