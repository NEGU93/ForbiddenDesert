#ifndef HANDLER_H_INCLUDED
#define HANDLER_H_INCLUDED

#include "def.h"
#include "allegro.h"
#include "worms.h"

void refresh(ALL* allegro, worm * worms[MAX_WORMS]);

void handlerMove(ALL* allegro, worm *worm[MAX_WORMS]);
void handlerToogleLeft(ALL* allegro, worm* worm);

void handlerMoveRight(ALL* allegro, worm *worm);
void handlerStopRight(ALL* allegro, worm* worm);
void handlerToogleRight(ALL* allegro, worm* worm);

void handlerJumpLeft(ALL* allegro, worm* worm);
void handlerJumpRight(ALL* allegro, worm* worm);


#endif