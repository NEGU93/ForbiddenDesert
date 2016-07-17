#include "allegro.h"
#include "def.h"


#define FPS 30
#define SAMPLES 3	//Number of audios that can me played at the same time
#define FONTSIZE 60

#define OK 0
#define READY 1
#define EXIT -1
//Dejar estos aca!

//////////////////////////////////////////////////////////////////////////////////////////////
//				INICIALIZO ALLEGRO Y TODOS LOS COMPLEMENTOS QUE SE USAN						//
//////////////////////////////////////////////////////////////////////////////////////////////
bool init_allegro(ALL *allegro) {
	bool state = false;
	allegro->volume = 1.0;
	//Initialize Allegro
	if (al_init()) {
		if (al_init_primitives_addon()) {
			if (al_init_image_addon()) {
				if (al_install_mouse()) {
					if (al_install_keyboard()) {
						//CODE TO FULLSCREEN, CHANGE IN CASE OF MULFUNCTION
						/*TODO: lo comento porque es dificil para debuguear... luego agregarlo. tira mas facha*/
						/*ALLEGRO_DISPLAY_MODE   disp_data;
						al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
						al_set_new_display_flags(ALLEGRO_FULLSCREEN);
						allegro->display = al_create_display(disp_data.width, disp_data.height);
						allegro->screenWidth = disp_data.width;
						allegro->screenHeight = disp_data.height;*/
						//allegro->display = al_create_display(SCREEN_X, SCREEN_Y);
						allegro->screenWidth = SCREEN_X;
						allegro->screenHeight = SCREEN_Y;

						allegro->startMenuDisplay = al_create_display(WIDTH * allegro->screenWidth / MAXRESOLUTION_X, HEIGHT * allegro->screenHeight / MAXRESOLUTION_Y);
						al_set_window_title(allegro->startMenuDisplay, "Start Menu");

						if (allegro->startMenuDisplay != NULL) {
							state = true;
						}
						else { cout << "Failed to initialize startMenuDisplay" << endl; al_uninstall_keyboard(); }
					}
					else { cout << "Failed to initialize keyboard" << endl; al_uninstall_mouse(); }
				}
				else { cout << "Failed to initialize mouse" << endl; al_shutdown_image_addon(); }
			}
			else { cout << "Failed to initialize image addon" << endl; al_shutdown_primitives_addon(); }
		}
		else { cout << "Failed to initialize primitives addon" << endl; }
	}
	else { cout << "Failed to initialize allegro" << endl; }
	//Timer for polonet
	if (state) {
		state = false;
		if (allegro->timeout = al_create_timer(1)) {
			al_set_timer_count(allegro->timeout, 0);
			state = true;
		}
		else { cout << "Failed to initialize timeout" << endl; }
	}
	//Initialize events
	if (state) {
		state = false;
		if (allegro->timer = al_create_timer(1.0 / FPS)) {
			if (allegro->events_queue = al_create_event_queue()) {
				al_register_event_source(allegro->events_queue, al_get_display_event_source(allegro->startMenuDisplay));
				al_register_event_source(allegro->events_queue, al_get_timer_event_source(allegro->timer));
				al_register_event_source(allegro->events_queue, al_get_keyboard_event_source());
				al_register_event_source(allegro->events_queue, al_get_mouse_event_source());
				state = true;
			}
		}
		else { cout << "Failed to initialize timer" << endl; }
	}
	//Initialize stormMeter image
	if (state) {
		state = false;
		if (allegro->stormMeter[0] = al_load_bitmap_resized("Resources/StormMeter/stormMeterImage.png", allegro)) {
			if (allegro->stormMeter[1] = al_load_bitmap_resized("Resources/StormMeter/stormMeter1Image.png", allegro)) {
				if (allegro->stormMeter[2] = al_load_bitmap_resized("Resources/StormMeter/stormMeter2Image.png", allegro)) {
					if (allegro->stormMeter[3] = al_load_bitmap_resized("Resources/StormMeter/stormMeter3Image.png", allegro)) {
						if (allegro->stormMeter[4] = al_load_bitmap_resized("Resources/StormMeter/stormMeter4Image.png", allegro)) {
							if (allegro->stormMeter[5] = al_load_bitmap_resized("Resources/StormMeter/stormMeter5Image.png", allegro)) {
								if (allegro->stormMeter[6] = al_load_bitmap_resized("Resources/StormMeter/stormMeter6Image.png", allegro)) {
									if (allegro->stormMeter[7] = al_load_bitmap_resized("Resources/StormMeter/stormMeter7Image.png", allegro)) {
										if (allegro->stormMeter[8] = al_load_bitmap_resized("Resources/StormMeter/stormMeter8Image.png", allegro)) {
											if (allegro->stormMeter[9] = al_load_bitmap_resized("Resources/StormMeter/stormMeter9Image.png", allegro)) {
												if (allegro->stormMeter[10] = al_load_bitmap_resized("Resources/StormMeter/stormMeter10Image.png", allegro)) {
													if (allegro->stormMeter[11] = al_load_bitmap_resized("Resources/StormMeter/stormMeter11Image.png", allegro)) {
														if (allegro->stormMeter[12] = al_load_bitmap_resized("Resources/StormMeter/stormMeter12Image.png", allegro)) {
															if (allegro->stormMeter[13] = al_load_bitmap_resized("Resources/StormMeter/stormMeter13Image.png", allegro)) {
																if (allegro->stormMeter[14] = al_load_bitmap_resized("Resources/StormMeter/stormMeter14Image.png", allegro)) {
																	state = true;
																}
																else { cout << "Unable to load stormMeter14Image" << endl; }
															}
															else { cout << "Unable to load stormMeter13Image" << endl; }
														}
														else { cout << "Unable to load stormMeter12Image" << endl; }
													}
													else { cout << "Unable to load stormMeter11Image" << endl; }
												}
												else { cout << "Unable to load stormMeter10Image" << endl; }
											}
											else { cout << "Unable to load stormMeter9Image" << endl; }
										}
										else { cout << "Unable to load stormMeter8Image" << endl; }
									}
									else { cout << "Unable to load stormMeter7Image" << endl; }
								}
								else { cout << "Unable to load stormMeter6Image" << endl; }
							}
							else { cout << "Unable to load stormMeter5Image" << endl; }
						}
						else { cout << "Unable to load stormMeter4Image" << endl; }
					}
					else { cout << "Unable to load stormMeter3Image" << endl; }
				}
				else { cout << "Unable to load stormMeter2Image" << endl; }
			}
			else { cout << "Unable to load stormMeter1Image" << endl; }
		}
		else { cout << "Unable to load stormMeterImage" << endl; }
	}
	if (state) {
		state = false;
		if (allegro->menuBackgroundImage = al_load_bitmap_resized("Resources/BackMenu.png", allegro)) { state = true; }
	}
	if (state) {
		state = false;
		if (allegro->backgroundStartMenuImage = al_load_bitmap_resized("Resources/startMenuImage.png", allegro)) { state = true; }
	}
	//Initialize Equipment
	if (state) {
		state = false;
		if (allegro->haveEquipments[0] = al_load_bitmap_resized("Resources/Equipment/haveDuneBlasterImage.png", allegro)) {
			if (allegro->haveEquipments[1] = al_load_bitmap_resized("Resources/Equipment/haveJetPackImage.png", allegro)) {
				if (allegro->haveEquipments[2] = al_load_bitmap_resized("Resources/Equipment/haveSecretWaterReserveImage.png", allegro)) {
					if (allegro->haveEquipments[3] = al_load_bitmap_resized("Resources/Equipment/haveSolarShieldImage.png", allegro)) {
						if (allegro->haveEquipments[4] = al_load_bitmap_resized("Resources/Equipment/haveTerrascopeImage.png", allegro)) {
							if (allegro->haveEquipments[5] = al_load_bitmap_resized("Resources/Equipment/haveTimeThrottleImage.png", allegro)) {
								if (allegro->noEquipments[0] = al_load_bitmap_resized("Resources/Equipment/noDuneBlasterImage.png", allegro)) {
									if (allegro->noEquipments[1] = al_load_bitmap_resized("Resources/Equipment/noJetPackImage.png", allegro)) {
										if (allegro->noEquipments[2] = al_load_bitmap_resized("Resources/Equipment/noSecretWaterReserveImage.png", allegro)) {
											if (allegro->noEquipments[3] = al_load_bitmap_resized("Resources/Equipment/noSolarShieldImage.png", allegro)) {
												if (allegro->noEquipments[4] = al_load_bitmap_resized("Resources/Equipment/noTerrascopeImage.png", allegro)) {
													if (allegro->noEquipments[5] = al_load_bitmap_resized("Resources/Equipment/noTimeThrottleImage.png", allegro)) {
														if (allegro->equipmentCards[0] = al_load_bitmap_resized("Resources/Equipment/duneBlasterImage.png", allegro)) {
															if (allegro->equipmentCards[1] = al_load_bitmap_resized("Resources/Equipment/jetPackImage.png", allegro)) {
																if (allegro->equipmentCards[2] = al_load_bitmap_resized("Resources/Equipment/solarShieldImage.png", allegro)) {
																	if (allegro->equipmentCards[3] = al_load_bitmap_resized("Resources/Equipment/terrascopeImage.png", allegro)) {
																		if (allegro->equipmentCards[4] = al_load_bitmap_resized("Resources/Equipment/secretWaterReserveImage.png", allegro)) {
																			if (allegro->equipmentCards[5] = al_load_bitmap_resized("Resources/Equipment/timeThrottleImage.png", allegro)) {
																				state = true;
																			}
																			else { cout << "Unable to load timeThrottleImage.png" << endl; }
																		}
																		else { cout << "Unable to load secretWaterReserveImage.png" << endl; }
																	}
																	else { cout << "Unable to load terrascopeImage.png" << endl; }
																}
																else { cout << "Unable to load solarShieldImage.png" << endl; }
															}
															else { cout << "Unable to load jetPackImage.png" << endl; }
														}
														else { cout << "Unable to load duneBlasterImage.png" << endl; }
													}
													else { cout << "Unable to load noTimeThrottleImage.png" << endl; }
												}
												else { cout << "Unable to load noTerrascopeImage.png" << endl; }
											}
											else { cout << "Unable to load noSolarShieldImage.png" << endl; }
										}
										else { cout << "Unable to load noSecretWaterReserveImage.png" << endl; }
									}
									else { cout << "Unable to load noJetPackImage.png" << endl; }
								}
								else { cout << "Unable to load noDuneBlasterImage.png" << endl; }
							}
							else { cout << "Unable to load haveTimeThrottleImage.png" << endl; }
						}
						else { cout << "Unable to load haveTerrascopeImage.png" << endl; }
					}
					else { cout << "Unable to load haveSolarShieldImage.png" << endl; }
				}
				else { cout << "Unable to load haveSecretWaterReserveImage.png" << endl; }
			}
			else { cout << "Unable to load haveJetPackImage.png" << endl; }
		}
		else { cout << "Unable to load haveDuneBlasterImage.png" << endl; }
	}
	//Load Part Images
	if (state) {
		state = false;
		if (allegro->partsImages[0] = al_load_bitmap_resized("Resources/Parts/engineImage.png", allegro)) {
			if (allegro->partsImages[1] = al_load_bitmap_resized("Resources/Parts/navegationDeckImage.png", allegro)) {
				if (allegro->partsImages[2] = al_load_bitmap_resized("Resources/Parts/propellerImage.png", allegro)) {
					if (allegro->partsImages[3] = al_load_bitmap_resized("Resources/Parts/solarCrystalImage.png", allegro)) {
						if (allegro->partsImages[4] = al_load_bitmap_resized("Resources/Parts/noEngineImage.png", allegro)) {
							if (allegro->partsImages[5] = al_load_bitmap_resized("Resources/Parts/noNavegationDeckImage.png", allegro)) {
								if (allegro->partsImages[6] = al_load_bitmap_resized("Resources/Parts/noPropellerImage.png", allegro)) {
									if (allegro->partsImages[7] = al_load_bitmap_resized("Resources/Parts/noSolarCrystalImage.png", allegro)) {
										state = true;
									}
									else { cout << "Failed to load No Solar Crystal Image" << endl; }
								}
								else { cout << "Failed to load no Propeller Image" << endl; }
							}
							else { cout << "Failed to load no NavigationDeckImage" << endl; }
						}
						else { cout << "Failed to load no EngineImage" << endl; }
					}
					else { cout << "Failed to load no Solar Crystal Image" << endl; }
				}
				else { cout << "Failed to load Propeller Image" << endl; }
			}
			else { cout << "Failed to load NavigationDeckImage" << endl; }
		}
		else { cout << "Failed to load EngineImage" << endl; }
	}
	//Rectangle
	if (state) {
		state = false;
		if (allegro->rectangleImage[0] = al_load_bitmap_resized("Resources/Buttons/AdventurerSpecialsDescription/archeologistRectangle.png", allegro)) {
			if (allegro->rectangleImage[1] = al_load_bitmap_resized("Resources/Buttons/AdventurerSpecialsDescription/climberRectangle.png", allegro)) {
				if (allegro->rectangleImage[2] = al_load_bitmap_resized("Resources/Buttons/AdventurerSpecialsDescription/explorerRectangle.png", allegro)) {
					if (allegro->rectangleImage[3] = al_load_bitmap_resized("Resources/Buttons/AdventurerSpecialsDescription/navigatorRectangle.png", allegro)) {
						if (allegro->rectangleImage[4] = al_load_bitmap_resized("Resources/Buttons/AdventurerSpecialsDescription/waterCarrierRectangle.png", allegro)) {
							if (allegro->rectangleImage[5] = al_load_bitmap_resized("Resources/Buttons/AdventurerSpecialsDescription/meteorologistRectangle.png", allegro)) {
								state = true;
							}
							else { cout << "Failed to load Meteorologist Rectangle" << endl; }
						}
						else { cout << "Failed to load Water Carrier Rectangle" << endl; }
					}
					else { cout << "Failed to load Navigator Rectangle" << endl; }
				}
				else { cout << "Failed to load Explorer Rectangle" << endl; }
			}
			else { cout << "Failed to load Climber Rectangle" << endl; }
		}
		else { cout << "Failed to load Archeologist Rectangle" << endl; }
	}
	//Load Audio
	if (state) {
		state = false;
		if (al_install_audio()) {
			if (al_init_acodec_addon()) {
				if (al_reserve_samples(SAMPLES)) {
					if (allegro->backgroundMusic = al_load_sample("Resources/Sounds/MenuBackMusic.wav")) {
						if (allegro->clickOnButtonSound = al_load_sample("Resources/Sounds/ClickOnButton.wav")) {
							if (allegro->sunBeatsDownSound = al_load_sample("Resources/Sounds/SunBeatsDown.wav")) {
								if (allegro->waterSound = al_load_sample("Resources/Sounds/WATER.wav")) {
									if (allegro->stormSound = al_load_sample("Resources/Sounds/storm.wav")) {
										if (allegro->excavateSound = al_load_sample("Resources/Sounds/excavate.wav")) {
											if (allegro->curiositySound = al_load_sample("Resources/Sounds/curiosity.wav")) {
												if (allegro->teleportSound = al_load_sample("Resources/Sounds/teleport.wav")) {
													if (allegro->startMusic = al_load_sample("Resources/Sounds/blood.wav")) {
														if (allegro->rejectEquipmentBoySound = al_load_sample("Resources/Sounds/RejectEquipment(B).wav")) {
															if (allegro->rejectEquipmentGirlSound = al_load_sample("Resources/Sounds/RejectEquipment(G).wav")) {
																if (allegro->rejectWaterBoySound = al_load_sample("Resources/Sounds/RejectWater(B).wav")) {
																	if (allegro->rejectWaterGirlSound = al_load_sample("Resources/Sounds/RejectWater(G).wav")) {
																		state = true;
																	}
																}
																else { cout << "Failed to load Reject Water Sound" << endl; }
															}
															else { cout << "Failed to load Reject Equipment Sound" << endl; }
														}
														else { cout << "Failed to load Reject Equipment Sound" << endl; }
													}
													else { cout << "Failed to load blood sound" << endl; }
												}
												else { cout << "Failed to load teleport Sound" << endl; }
											}
											else { cout << "Failed to load curiosity Sound" << endl; }
										}
										else { cout << "Failed to load Excavate Sound" << endl; }
									}
									else { cout << "Failed to load Storm Sound sample" << endl; }
								}
								else { cout << "Failed to load sample water sound" << endl; }
							}
							else { cout << "Failed to load sample sun beats down" << endl; }
						}
						else { cout << "Failed to load samples" << endl; }
					}
				}
				else { cout << "Failed to reserve samples" << endl; }
			}
			else { cout << "Failed to init audio addon" << endl; }
		}
		else { cout << "Failed to install audio" << endl; }
	}
	//Initialize fonts
	if (state) {
		state = false;
		al_init_font_addon();
		if (al_init_ttf_addon()) {
			if (allegro->font = al_load_ttf_font("Resources/Papyrus.ttf", 25, 0)) {
				if (allegro->statusFont = al_load_ttf_font("Resources/consola.ttf", 12, 0)) {
					if (allegro->gameFont = al_load_ttf_font("Resources/Papyrus.ttf", FONTSIZE * allegro->screenHeight / MAXRESOLUTION_Y, 0)) {
						state = true;
					}
					else { cout << "Failed to initialize gameFont" << endl; }
				}
				else { cout << "Failed to initialize consola.ttf" << endl; }
			}
			else { cout << "Failed to initialize Papyrus.ttf" << endl; }
		}
		else { cout << "Failed to initialize ttf addon" << endl; }
	}

	al_flip_display();
	al_start_timer(allegro->timer);

	return state;
}

void destroyAll(ALL *allegro) {
	al_destroy_event_queue(allegro->events_queue);
	//Timers
	al_destroy_timer(allegro->timer);
	al_destroy_timer(allegro->timeout);
	//Sample
	al_destroy_sample(allegro->clickOnButtonSound);
	al_destroy_sample(allegro->sunBeatsDownSound);
	al_destroy_sample(allegro->waterSound);
	al_destroy_sample(allegro->excavateSound);
	al_destroy_sample(allegro->curiositySound);
	al_destroy_sample(allegro->teleportSound);
	al_destroy_sample(allegro->startMusic);
	al_destroy_sample(allegro->rejectWaterGirlSound);
	al_destroy_sample(allegro->rejectEquipmentBoySound);
	al_destroy_sample(allegro->rejectWaterBoySound);
	al_destroy_sample(allegro->rejectEquipmentGirlSound);
	al_destroy_sample(allegro->backgroundMusic);
	//Fonts
	al_destroy_font(allegro->font);
	al_destroy_font(allegro->statusFont);
	al_destroy_font(allegro->gameFont);
	//Bitmaps
	al_destroy_bitmap(allegro->menuBackgroundImage);
	al_destroy_bitmap(allegro->backgroundStartMenuImage);
	al_destroy_bitmap(allegro->fondo);
	al_destroy_bitmap(allegro->mouse);
	for (int i = 0; i < 15; i++)
		al_destroy_bitmap(allegro->stormMeter[i]);
	for (int i = 0; i < 6; i++) {
		al_destroy_bitmap(allegro->haveEquipments[i]);
		al_destroy_bitmap(allegro->noEquipments[i]);
		al_destroy_bitmap(allegro->equipmentCards[i]);
	}
	for (int i = 0; i < 8; i++)
		al_destroy_bitmap(allegro->partsImages[i]);
	for (int i = 0; i < 6; i++)
		al_destroy_bitmap(allegro->rectangleImage[i]);
	//Cursor
	al_destroy_mouse_cursor(allegro->cursor);
	//Addons
	al_uninstall_keyboard();
	al_uninstall_mouse();
	al_shutdown_image_addon();
	al_shutdown_ttf_addon();
	al_shutdown_primitives_addon();
	al_uninstall_audio();

}

char* getcharAllegro(ALL* allegro, int x, int y)
{
	ALLEGRO_USTR* str = al_ustr_new("");            //creo ustring para escribir en allegro
	int pos = (int)al_ustr_size(str);
	char* mystring;

	char quit = OK;
	while (quit == OK) {
		al_draw_filled_rectangle(x, y, x + WIDTH * allegro->screenWidth / MAXRESOLUTION_X / 2 - 8, y + 40, al_map_rgb(0, 0, 0));
		al_draw_ustr(allegro->font, al_map_rgb(WHITE), x, y, ALLEGRO_ALIGN_LEFT, str);  //area donde escribo
		al_flip_display();  //muestro lo que escribo

		ALLEGRO_EVENT e;
		al_wait_for_event(allegro->events_queue, &e);
		switch (e.type) {
		case ALLEGRO_EVENT_KEY_CHAR:
			if (e.keyboard.unichar >= 32) {
				pos += al_ustr_append_chr(str, e.keyboard.unichar);
			}
			else if (e.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
				if (al_ustr_prev(str, &pos))
					al_ustr_truncate(str, pos);
			}
			if (e.keyboard.unichar == 13) {
				quit = READY;
			}
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			quit = EXIT;
			break;
		}
	}
	if (quit == READY)
		mystring = (char *)al_cstr(str);     //TIRA WARNING POR QUE LA FUNCION TRABAJA CON CONST    
	if (quit == EXIT)
		mystring = NULL;

	return mystring;
}
void printAllegro(ALL *allegro, char *msg) {
	al_set_target_backbuffer(allegro->startMenuDisplay);
	al_draw_filled_rectangle(0, HEIGHT * allegro->screenHeight / MAXRESOLUTION_Y - 15, WIDTH * allegro->screenWidth / MAXRESOLUTION_X, HEIGHT * allegro->screenHeight / MAXRESOLUTION_Y, al_map_rgb(0, 0, 0));
	al_draw_textf(allegro->statusFont, al_map_rgb(WHITE), WIDTH * allegro->screenWidth / MAXRESOLUTION_X / 2, HEIGHT * allegro->screenHeight / MAXRESOLUTION_Y - 12, ALLEGRO_ALIGN_CENTRE, "%s", msg);
	al_flip_display();
}

ALLEGRO_BITMAP *al_load_bitmap_resized(const char *filename, ALL* allegro) {
	ALLEGRO_BITMAP *resized_bmp, *loaded_bmp, *prev_target;
	//La idea es que esta funcion hace lo mismo que al_load_bitmap pero ya resizeado...

	// 1. load the bitmap at the original size				 
	loaded_bmp = al_load_bitmap(filename);
	if (!loaded_bmp) return NULL;

	// 2. create a temporary bitmap of size we want
	resized_bmp = al_create_bitmap(al_get_bitmap_width(loaded_bmp) * allegro->screenWidth / MAXRESOLUTION_X, al_get_bitmap_height(loaded_bmp) * allegro->screenHeight / MAXRESOLUTION_Y);
	if (!resized_bmp) {
		al_destroy_bitmap(loaded_bmp);
		return NULL;
	}

	// 3. set the target bitmap to the resized bmp
	prev_target = al_get_target_bitmap();
	al_set_target_bitmap(resized_bmp);

	// 4. copy the loaded bitmap to the resized bmp
	al_draw_scaled_bitmap(loaded_bmp,
		0, 0,                                // source origin
		al_get_bitmap_width(loaded_bmp),     // source width
		al_get_bitmap_height(loaded_bmp),    // source height
		0, 0,                                // target origin
		al_get_bitmap_width(loaded_bmp) * allegro->screenWidth / MAXRESOLUTION_X,		// target dimensions
		al_get_bitmap_height(loaded_bmp) * allegro->screenHeight / MAXRESOLUTION_Y,
		0                                    // flags
		);

	// 5. restore the previous target and clean up
	al_set_target_bitmap(prev_target);
	al_destroy_bitmap(loaded_bmp);

	return resized_bmp;
}

void drawStartMenu(ALL *allegro) {
	al_set_target_backbuffer(allegro->startMenuDisplay);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_bitmap(allegro->backgroundStartMenuImage, 0, 0, 0);
	al_draw_filled_rectangle(0, HEIGHT * allegro->screenHeight / MAXRESOLUTION_Y - 15, WIDTH * allegro->screenWidth / MAXRESOLUTION_X, HEIGHT * allegro->screenHeight / MAXRESOLUTION_Y, al_map_rgb(0, 0, 0));
	al_draw_text(allegro->font, al_map_rgb(WHITE), 10, HEIGHT * allegro->screenHeight / MAXRESOLUTION_Y / 3 - 30, 0, "Please, enter IP:");
	al_draw_filled_rectangle(5, HEIGHT * allegro->screenHeight / MAXRESOLUTION_Y / 3, WIDTH * allegro->screenWidth / MAXRESOLUTION_X * 0.7, HEIGHT * allegro->screenHeight / MAXRESOLUTION_Y / 3 + 40, al_map_rgb(0, 0, 0));
	al_draw_text(allegro->font, al_map_rgb(WHITE), 10, HEIGHT * allegro->screenHeight / MAXRESOLUTION_Y / 2 - 30, 0, "Your name:");
	al_draw_filled_rectangle(5, HEIGHT * allegro->screenHeight / MAXRESOLUTION_Y / 2, WIDTH * allegro->screenWidth / MAXRESOLUTION_X * 0.7, HEIGHT * allegro->screenHeight / MAXRESOLUTION_Y / 2 + 40, al_map_rgb(0, 0, 0));
}