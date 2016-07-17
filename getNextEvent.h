#ifndef GETNEXTEVENT_H_INCLUDED
#define GETNEXTEVETN_H_INCLUDED

#include "allegro.h"
#include "worms.h"
#include "def.h"

typedef enum{KEY_LEFT,KEY_RIGHT,KEY_UP,KEY_A,KEY_D,KEY_W};

actualevent getNextEvent(ALL* allegro,worm* worms[MAX_WORMS],bool keys[6],bool redraw,bool state);


#endif // !GETNEXTEVENT_H_INCLUDED
