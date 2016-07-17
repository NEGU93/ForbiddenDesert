#ifndef ALLEGRO_H_INCLUDED
#define ALLEGRO_H_INCLUDED

#define ALLEGRO_STATICLINK 
#include<allegro5\allegro.h>
#include<allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>

#include <cmath>

#define SCREEN_X 1440	
#define SCREEN_Y 900

#define MAXRESOLUTION_X 1920
#define MAXRESOLUTION_Y 1080

typedef struct {
	int screenWidth;
	int screenHeight;
	double volume;
	//DISPLAYS
	ALLEGRO_DISPLAY *display;
	ALLEGRO_DISPLAY *startMenuDisplay;
	//EVENT QUEUE
	ALLEGRO_EVENT_QUEUE *events_queue;
	//TIMERS
	ALLEGRO_TIMER * timer;
	ALLEGRO_TIMER *timeout;
	//Samples (TODO: posría ponerse en una segunda esctructura)
	ALLEGRO_SAMPLE *backgroundMusic;
	ALLEGRO_SAMPLE *clickOnButtonSound;
	ALLEGRO_SAMPLE *sunBeatsDownSound;
	ALLEGRO_SAMPLE *waterSound;
	ALLEGRO_SAMPLE *stormSound;
	ALLEGRO_SAMPLE *excavateSound;
	ALLEGRO_SAMPLE* curiositySound;
	ALLEGRO_SAMPLE* teleportSound;
	ALLEGRO_SAMPLE* startMusic;
	ALLEGRO_SAMPLE* rejectWaterGirlSound;
	ALLEGRO_SAMPLE* rejectEquipmentGirlSound;
	ALLEGRO_SAMPLE* rejectWaterBoySound;
	ALLEGRO_SAMPLE* rejectEquipmentBoySound;
	ALLEGRO_SAMPLE_ID backgroundMusicID;
	//FONTS
	ALLEGRO_FONT *font;
	ALLEGRO_FONT *gameFont;
	ALLEGRO_FONT *statusFont;
	//BITMAPS
	ALLEGRO_BITMAP* fondo;
	ALLEGRO_BITMAP* mouse;
	ALLEGRO_BITMAP* menuBackgroundImage;
	ALLEGRO_BITMAP* backgroundStartMenuImage;
	ALLEGRO_BITMAP* stormMeter[15];
	ALLEGRO_BITMAP* haveEquipments[6];
	ALLEGRO_BITMAP* noEquipments[6];
	ALLEGRO_BITMAP* equipmentCards[6];
	ALLEGRO_BITMAP* partsImages[8];
	ALLEGRO_BITMAP* rectangleImage[6];
	//CURSOR
	ALLEGRO_MOUSE_CURSOR* cursor;
	
} ALL;

bool init_allegro(ALL *allegro);
void destroyAll(ALL *allegro);
char *getcharAllegro(ALL *allegro, int x, int y);
void printAllegro(ALL* allegro, char *msg);
void drawStartMenu(ALL *allegro);
ALLEGRO_BITMAP *al_load_bitmap_resized(const char *filename, ALL* allegro); //TODO: que pase solo los ints que necesito y no toda la struct

#endif

