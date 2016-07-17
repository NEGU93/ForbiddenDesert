#include "StartMenu.h"

StartMenu::StartMenu(ALL* allegro, FSMI *gameConnection)	
{
	al_set_target_backbuffer(allegro->startMenuDisplay);
	al_clear_to_color(al_map_rgb(0,0,0));
	if(background = al_load_bitmap("Resources/startMenuImage.jpg")){
		al_draw_bitmap(background,0,0,0);
		al_draw_filled_rectangle(0, HEIGHT - 15, WIDTH, HEIGHT, al_map_rgb(0,0,0));
		al_draw_text(allegro->font, al_map_rgb(WHITE), 10, HEIGHT / 3 - 30, 0, "Please, enter IP");
		al_draw_filled_rectangle(5, HEIGHT / 3, WIDTH * 0.7, HEIGHT / 3 + 40, al_map_rgb(0,0,0));
		al_draw_text(allegro->font, al_map_rgb(WHITE), 10, HEIGHT / 2 - 30 ,0, "Your name");
		al_draw_filled_rectangle(5, HEIGHT / 2, WIDTH * 0.7, HEIGHT / 2 + 40, al_map_rgb(0,0,0));
		
		al_flip_display();

		gameConnection->start(allegro);
	}
	else { cout << "Failed to load startMenuImage.jpg" << endl; }
}
//StartMenu::~StartMenu(){
//	al_destroy_bitmap(background);
//}
