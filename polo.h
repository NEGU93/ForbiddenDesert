/**
 * libpolo
 * Lightweight graphics library for educational environments
 * (C) 2011 by the libpolo team.
 *     Marc S. Ressl (mressl@itba.edu.ar)
 *     Jorge Prendes (jprendes@itba.edu.ar)
 * Released under the GPL
 *
 * Requires the glut library.
 */

/*
 * Basics:
 * - Call initPolo() to initialize the graphics window.
 * - libpolo continuously calls a user function to redraw the screen. 
 *   You can define this function with setDrawCallback().
 * - Call runPolo() to run graphics and get your user function called.
 * - To close the graphics window, call exitPolo().
 *
 * Drawing:
 * - The coordinate origin (0, 0) is at the lower left.
 * - Color components (RGBA, HSVA) are in the range 0.0 to 1.0.
 * - Alpha is the level of opacity (0.0 is transparent, 1.0 is opaque).
 * - Drawing primitives are rendered using a border color (use setPenColor())
 *   and fill color (setFillColor() or setFillGradient()).
 *
 * Using images:
 * - Images must be in uncompressed BMP format, either 24-bit RGB or 32-bit RGBA.
 * - Make sure you called initPolo before loading images.
 * - Use loadImage() to load a BMP file from disk. You will get an Image reference.
 *   If the image could not be loaded you get the value 0.
 * - Use drawImage() to draw an image to screen.
 * - Use drawSetTint() to set the tint parameter. This lets you tint your image
     with a color and alpha. To just paint, use tint POLO_WHITE.
 * - Use freeImage() after you finished using your image.
 * - Call setTexture() to fill drawing primitives with a texture.
 *   Make sure the width and height of your image is a power of 2 when using
 *   textures.
 *
 * Keyboard and mouse:
 * - Use getKey(), getMouseX(), getMouseY() and getMouseButtonState()
 *   to query the keyboard and mouse. Use clearKey() to clear a read key.
 * - Keyboard codes are in UNICODE. Special keys are defined in PoloKeys.
 *
 * Time:
 * - Use getTime() to get the number of seconds since the program started.
 *   This value is accurate up to several milliseconds.
 *
 */

#ifndef _LIBPOLO_H
#define _LIBPOLO_H

/* #define USE_FREEGLUT */

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#ifdef USE_FREEGLUT
#include <GLUT/freeglut.h>
#endif
#else
#include <GL/glut.h>
#ifdef USE_FREEGLUT 
#include <GL/freeglut.h>
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Definitions
#define Color unsigned int
#define Image unsigned int

enum PoloColors
{
	POLO_TRANSPARENT    = 0x00000000,
	POLO_WHITE          = 0xffffffff,
	POLO_BLACK          = 0x000000ff,
	
	POLO_CANTALOUPE     = 0xffcc66ff,
	POLO_HONEYDEW       = 0xccff66ff,
	POLO_SPINDRIFT      = 0x66ffccff,
	POLO_SKY            = 0x66ccffff,
	POLO_LAVENDER       = 0xcc66ffff,
	POLO_CARNATION      = 0xff6fcfff,
	POLO_LICORICE       = 0x000000ff,
	POLO_SNOW           = 0xffffffff,
	POLO_SALMON         = 0xff6666ff,
	POLO_BANANA         = 0xffff66ff,
	POLO_FLORA          = 0x66ff66ff,
	POLO_ICE            = 0x66ffffff,
	POLO_ORCHID         = 0x6666ffff,
	POLO_BUBBLEGUM      = 0xff66ffff,
	POLO_LEAD           = 0x191919ff,
	POLO_MERCURY        = 0xe6e6e6ff,
	POLO_TANGERINE      = 0xff8000ff,
	POLO_LIME           = 0x80ff00ff,
	POLO_SEAFOAM        = 0x00ff80ff,
	POLO_AQUA           = 0x0080ffff,
	POLO_GRAPE          = 0x8000ffff,
	POLO_STRAWBERRY     = 0xff0080ff,
	POLO_TUNGSTEN       = 0x333333ff,
	POLO_SILVER         = 0xccccccff,
	POLO_MARASCHINO     = 0xff0000ff,
	POLO_LEMON          = 0xffff00ff,
	POLO_SPRING         = 0x00ff00ff,
	POLO_TURQUOISE      = 0x00ffffff,
	POLO_BLUEBERRY      = 0x0000ffff,
	POLO_MAGENTA        = 0xff00ffff,
	POLO_IRON           = 0x424242ff,
	POLO_MAGNESIUM      = 0xb3b3b3ff,
	POLO_MOCHA          = 0x804000ff,
	POLO_FERN           = 0x408000ff,
	POLO_MOSS           = 0x008040ff,
	POLO_OCEAN          = 0x004080ff,
	POLO_EGGPLANT       = 0x400080ff,
	POLO_MAROON         = 0x800040ff,
	POLO_STEEL          = 0x666666ff,
	POLO_ALUMINUM       = 0x999999ff,
	POLO_CAYENNE        = 0x800000ff,
	POLO_ASPARAGUS      = 0x808000ff,
	POLO_CLOVER         = 0x008000ff,
	POLO_TEAL           = 0x008080ff,
	POLO_MIDNIGHT       = 0x000080ff,
	POLO_PLUM           = 0x800080ff,
	POLO_TIN            = 0x7f7f7fff,
	POLO_NICKEL         = 0x808080ff,
};

enum PoloKey
{
	POLO_BACKSPACE = 0x08,
	POLO_TAB = 0x09,
	POLO_ENTER = 0x0d,
	POLO_ESC = 0x1b,
	POLO_F1 = 0xe000,
	POLO_F2,
	POLO_F3,
	POLO_F4,
	POLO_F5,
	POLO_F6,
	POLO_F7,
	POLO_F8,
	POLO_F9,
	POLO_F10,
	POLO_F11,
	POLO_F12,
	POLO_UP,
	POLO_DOWN,
	POLO_LEFT,
	POLO_RIGHT,
	POLO_PAGEUP,
	POLO_PAGEDOWN,
	POLO_HOME,
	POLO_END,
};

enum PoloFont
{
	POLO_COURIER_13,
	POLO_COURIER_15,
	POLO_TIMES_10,
	POLO_TIMES_24,
	POLO_HELVETICA_10,
	POLO_HELVETICA_12,
	POLO_HELVETICA_18,
};

// Initialization & exit
void setPoloUserData(void *userData);
void initPolo(int width, int height, int fullscreen, const char *windowTitle);
void runPolo();
void exitPolo();

// Drawing
void setDrawCallback(void (*drawCallback)(void *userData));

Color getColorFromRGBA(float red, float green, float blue, float alpha);
Color getColorFromRGB(float red, float green, float blue);
Color getColorFromHSVA(float hue, float saturation, float value, float alpha);
Color getColorFromHSV(float hue, float saturation, float value);

void setPenColor(Color color);
void setFillColor(Color color);
void setFillGradient(Color color1, Color color2);

void drawPoint(float x, float y);
void drawLine(float x1, float y1, float x2, float y2);
void drawRect(float x, float y, float width, float height);
void drawRoundedRect(float x, float y, float width, float height, float edgeRadius);
void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
void drawCircle(float x, float y, float radius);

void clearScreen();
void updateScreen();
float getScreenWidth();
float getScreenHeight();

void setTextFont(enum PoloFont font);
float getTextDrawWidth(const char *str);
float getTextDrawHeight(const char *str);
void drawText(float x, float y, const char *str);

Image loadImage(const char *path);
float getImageWidth(Image image);
float getImageHeight(Image image);
void drawImage(float x, float y, Image image);
void setDrawTint(Color tint);
void setDrawScale(float scale);
void setTexture(Image image);
void freeImage(Image image);

// Keyboard
void setKeyboardCallback(void (*keyboardCallback)(void *userData, int key));
int getKey();
void clearKey();

// Mouse
void setMouseMotionCallback(void (*mouseMotionCallback)(void *userData, int x, int y));
void setMouseButtonCallback(void (*mouseButtonCallback)(void *userData, int button, int pressed));
float getMouseX();
float getMouseY();
int isMouseButtonPressed(int buttonIndex);
void showMousePointer();
void hideMousePointer();

// Time
void setTimerCallback(void (*timerCallback)(void *userData, int id));
void runTimer(int id, int milliseconds);
float getTime();

#ifdef __cplusplus
}
#endif

#endif

