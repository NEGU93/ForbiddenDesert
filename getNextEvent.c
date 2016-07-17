#include "getNextEvent.h"

actualevent getNextEvent(ALL* allegro,worm* worms[MAX_WORMS],bool keys[6],bool redraw, bool state) {
	ALLEGRO_EVENT rawEvent;
	
	if(al_get_next_event(allegro->events_queue, &rawEvent)) {//rawEvent son eventos que no generan los handlers, funcion? 								
															 //filtra los eventos y se quedan con los que le importan.
		if(rawEvent.type == ALLEGRO_EVENT_TIMER) {
			redraw=true;
			//			WORM QUE SE MUEVE CON FLECHITAS		//
			if(keys[KEY_RIGHT]) {		//Entra a este if si la tecla permanece apretada
				if(worms[0]->estado == STILL) {		//Si el worm está quieto, lo pone en estado de caminando y envia un evento de START_MOVING;
					worms[0]->estado = WALKING;
					return CONTINUE_MOVING;
				}
			}
			else {											//Si no está apretada la tecla RIGHT.
				if (worms[0]->direccion == RIGHT) {
					if (worms[0]->estado == WALKING) {
						return CONTINUE_MOVING;
					}
					else if (worms[0]->estado == STILL) {
						al_set_timer_count(allegro->movingtimer[0], 0);
					}
				}
			}
			
			if(keys[KEY_LEFT]) {
				if(worms[0]->estado == STILL) {
					if (al_get_timer_count(allegro->movingtimer[0]) > 100) {
						worms[0]->estado = WALKING;
					}
					return CONTINUE_MOVING;
				}
			}
			else {						//La tecla left no está apretada
				if (worms[0]->direccion == LEFT) {
					if (worms[0]->estado == WALKING) {
						return CONTINUE_MOVING;
					}
					else if (worms[0]->estado == STILL) {
						al_set_timer_count(allegro->movingtimer[0], 0);
					}
				}
			}
			//			WORM QUE SE MUEVE CON A-D-W			//
			if (keys[KEY_D]) {		//Entra a este if si la tecla permanece apretada
				if (worms[1]->estado == STILL) {		//Si el worm está quieto, lo pone en estado de caminando y envia un evento de START_MOVING;
					worms[1]->estado = WALKING;
					return CONTINUE_MOVING;
				}
			}
			else {											//Si no está apretada la tecla RIGHT.
				if (worms[1]->direccion == RIGHT) {
					if (worms[1]->estado == WALKING) {
						return CONTINUE_MOVING;
					}
					else if (worms[1]->estado == STILL) {
						al_set_timer_count(allegro->movingtimer[1], 0);
					}
				}
			}

			if (keys[KEY_A]) {
				if (worms[1]->estado == STILL) {
					if (al_get_timer_count(allegro->movingtimer[1]) > 100) {
						worms[1]->estado = WALKING;
					}
					return CONTINUE_MOVING;
				}
			}
			else {						//La tecla left no está apretada
				if (worms[1]->direccion == LEFT) {
					if (worms[1]->estado == WALKING) {
						return CONTINUE_MOVING;
					}
					else if (worms[1]->estado == STILL) {
						al_set_timer_count(allegro->movingtimer[1], 0);
					}
				}
			}

			return CONTINUE_MOVING;
		}	
	
		if(rawEvent.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch(rawEvent.keyboard.keycode)	{
				////////////////////////////////////////////////////////////////////////////////////////////////////////
				/////////////                  WORM QUE SE MUEVE CON FLECHAS						////////////////////
				////////////////////////////////////////////////////////////////////////////////////////////////////////

				case ALLEGRO_KEY_RIGHT:
					if (worms[0]->estado != JUMPING) {
						al_start_timer(allegro->movingtimer[0]);	//Inicializa un timer.
						worms[0]->direccion = RIGHT;
						//worms[0]->estado = WALKING;
						keys[KEY_RIGHT] = true;	//Pongo estas variables para que si mantengo apretado una tecla, el worm se siga moviendo
					}
					break;

				case ALLEGRO_KEY_LEFT:
					if (worms[0]->estado != JUMPING) {
						al_start_timer(allegro->movingtimer[0]);	//Inicializa un timer.
						worms[0]->direccion = LEFT;
						//worms[0]->estado = WALKING;
						keys[KEY_LEFT] = true;
					}
					break;

				case ALLEGRO_KEY_UP:
						al_start_timer(allegro->jumptimer[0]);
						worms[0]->estado = JUMPING;
						keys[KEY_UP] = true;
					break;

				
				////////////////////////////////////////////////////////////////////////////////////////////////////////
				/////////////                  WORM QUE SE MUEVE CON W-A-D							////////////////////
				////////////////////////////////////////////////////////////////////////////////////////////////////////
				case ALLEGRO_KEY_D:
					if (worms[1]->estado != JUMPING) {
						al_start_timer(allegro->movingtimer[1]);	//Inicializa un timer.
						worms[1]->direccion = RIGHT;
						//worms[0]->estado = WALKING;
						keys[KEY_D] = true;	//Pongo estas variables para que si mantengo apretado una tecla, el worm se siga moviendo
					}
					break;

				case ALLEGRO_KEY_A:
					if (worms[1]->estado != JUMPING) {
						al_start_timer(allegro->movingtimer[1]);	//Inicializa un timer.
						worms[1]->direccion = LEFT;
						//worms[0]->estado = WALKING;
						keys[KEY_A] = true;
					}
					break;

				case ALLEGRO_KEY_W:
						al_start_timer(allegro->jumptimer[1]);
						worms[1]->estado = JUMPING;
						keys[KEY_W] = true;
					break;

				default:
					al_play_sample(allegro->sample[4], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					break;			
			}
		}
	
		if(rawEvent.type == ALLEGRO_EVENT_KEY_UP) {
			switch(rawEvent.keyboard.keycode)
			{
				////////////////////////////////////////////////////////////////////////////////////////////////////////
				/////////////                  WORM QUE SE MUEVE CON FLECHAS						////////////////////
				////////////////////////////////////////////////////////////////////////////////////////////////////////

				case ALLEGRO_KEY_RIGHT:
					keys[KEY_RIGHT] = false;
					if (worms[0]->estado == JUMPING) {	//Me aseguro que no este en medio de un salto para ni interrumpir el salto.
						return CONTINUE_MOVING;
					}
					al_stop_timer(allegro->movingtimer[0]);
					if (al_get_timer_count(allegro->movingtimer[0]) <= TOOGLE_TIME)	{
						al_set_timer_count(allegro->movingtimer[0], 0);//Reseteo el movingtimer.
						return TOOGLE_RIGHT;
					}
					break;

				case ALLEGRO_KEY_LEFT:
					keys[KEY_LEFT] = false;
					if (worms[0]->estado == JUMPING) {
						return CONTINUE_MOVING;
					}
					al_stop_timer(allegro->movingtimer[0]);
					if (al_get_timer_count(allegro->movingtimer[0]) <= TOOGLE_TIME)	{
						al_set_timer_count(allegro->movingtimer[0], 0);//Reseteo el movingtimer.
						return TOOGLE_LEFT;
					}
					break;

				case ALLEGRO_KEY_UP:
					keys[KEY_UP] = false;
					return CONTINUE_MOVING;
					break;

				////////////////////////////////////////////////////////////////////////////////////////////////////////
				/////////////                  WORM QUE SE MUEVE CON A-D-W							////////////////////
				////////////////////////////////////////////////////////////////////////////////////////////////////////

				case ALLEGRO_KEY_D:
					keys[KEY_D] = false;
					if (worms[1]->estado == JUMPING) {	//Me aseguro que no este en medio de un salto para ni interrumpir el salto.
						return CONTINUE_MOVING;
					}
					al_stop_timer(allegro->movingtimer[1]);
					if (al_get_timer_count(allegro->movingtimer[1]) <= TOOGLE_TIME) {
						al_set_timer_count(allegro->movingtimer[1], 0);//Reseteo el movingtimer.
						return TOOGLE_D;
					}
					break;

				case ALLEGRO_KEY_A:
					keys[KEY_A] = false;
					if (worms[1]->estado == JUMPING) {
						return CONTINUE_MOVING;
					}
					al_stop_timer(allegro->movingtimer[1]);
					if (al_get_timer_count(allegro->movingtimer[1]) <= TOOGLE_TIME) {
						al_set_timer_count(allegro->movingtimer[1], 0);//Reseteo el movingtimer.
						return TOOGLE_A;
					}
					break;

				case ALLEGRO_KEY_W:
					keys[KEY_W] = false;
					return CONTINUE_MOVING;
					break;

				default:
					break;
			}
		}

		if(rawEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			return EXIT;
	}
	return NO_EVENT;
}


