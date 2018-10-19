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

/* Includes */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "polo.h"

#if (!defined(_WIN32) && !defined(__APPLE__))
#include <GL/glx.h>
#endif

#ifdef _WIN32
//#include <GL/glext.h>
#endif

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif

#ifndef GL_BGRA
#define GL_BGRA GL_BGRA_EXT
#endif

#ifndef GL_BGR
#define GL_BGR GL_BGR_EXT
#endif



/* Definitions */
#define POLO_MOUSEBUTTON_NUM    3
#define POLO_MAX_IMAGES         1024

typedef void (*PoloSwapControl)(int value);

typedef struct
{
	GLuint texture;
	int textureWidth;
	int textureHeight;
	int width;
	int height;
} PoloImage;

typedef struct
{
	int isInitialized;
	void *userData;
	void (*drawCallback)(void *userData);
	void (*keyboardCallback)(void *userData, int key);
	void (*mouseMotionCallback)(void *userData, int x, int y);
	void (*mouseButtonCallback)(void *userData, int button, int state);
	void (*timerCallback)(void *userData, int id);
	Color penColor;
	Color fillColor1;
	Color fillColor2;
	PoloImage images[POLO_MAX_IMAGES];
	Color drawTint;
	float drawScale;
	void *font;
	float fontHeight;
	float fontBaseLine;
	int key;
	float mouseX;
	float mouseY;
	int mouseButtonState[POLO_MOUSEBUTTON_NUM];
} PoloState;

typedef struct
{
	unsigned char id[2];
	unsigned char size[4];
	unsigned char reserved[4];
	unsigned char pixelsOffset[4];
	unsigned char headerSize[4];
	unsigned char width[4];
	unsigned char height[4];
	unsigned char numberOfPlanes[2];
	unsigned char bitsPerPixel[2];
	unsigned char compression[4];
} PoloBMPHeader;



/* Static variables
   Note: static variables should always be avoided. They are used here
         because glut does not pass a user pointer to the callbacks. */
static PoloState poloState;



/* Private callbacks */
static void drawCallback()
{
	/* Configure OpenGL */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glTranslatef(-1.0, -1.0, 0.0);
	glScalef(2.0 / glutGet(GLUT_WINDOW_WIDTH),
	         2.0 / glutGet(GLUT_WINDOW_HEIGHT),
	         1.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	/* Call user callback */
	if (poloState.drawCallback)
		poloState.drawCallback(poloState.userData);
	
	updateScreen();
}

static void resizeCallback(int width, int height)
{
	glViewport(0, 0, width, height);
	
	clearScreen();
	updateScreen();
	clearScreen();
}

static void keyboardCallback(unsigned char key, int x, int y)
{
	poloState.key = key;
	
	if (poloState.keyboardCallback)
		poloState.keyboardCallback(poloState.userData,
		                           poloState.key);
}

static void specialCallback(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_F1:
			poloState.key = POLO_F1;
			break;
		case GLUT_KEY_F2:
			poloState.key = POLO_F2;
			break;
		case GLUT_KEY_F3:
			poloState.key = POLO_F3;
			break;
		case GLUT_KEY_F4:
			poloState.key = POLO_F4;
			break;
		case GLUT_KEY_F5:
			poloState.key = POLO_F5;
			break;
		case GLUT_KEY_F6:
			poloState.key = POLO_F6;
			break;
		case GLUT_KEY_F7:
			poloState.key = POLO_F7;
			break;
		case GLUT_KEY_F8:
			poloState.key = POLO_F8;
			break;
		case GLUT_KEY_F9:
			poloState.key = POLO_F9;
			break;
		case GLUT_KEY_F10:
			poloState.key = POLO_F10;
			break;
		case GLUT_KEY_F11:
			poloState.key = POLO_F11;
			break;
		case GLUT_KEY_F12:
			poloState.key = POLO_F12;
			break;
		case GLUT_KEY_LEFT:
			poloState.key = POLO_LEFT;
			break;
		case GLUT_KEY_RIGHT:
			poloState.key = POLO_RIGHT;
			break;
		case GLUT_KEY_UP:
			poloState.key = POLO_UP;
			break;
		case GLUT_KEY_DOWN:
			poloState.key = POLO_DOWN;
			break;
		case GLUT_KEY_PAGE_UP:
			poloState.key = POLO_PAGEUP;
			break;
		case GLUT_KEY_PAGE_DOWN:
			poloState.key = POLO_PAGEDOWN;
			break;
		case GLUT_KEY_HOME:
			poloState.key = POLO_HOME;
			break;
		case GLUT_KEY_END:
			poloState.key = POLO_END;
			break;
		default:
			return;
	}
	
	if (poloState.keyboardCallback)
		poloState.keyboardCallback(poloState.userData,
		                           poloState.key);
}

static void mouseMotionCallback(int x, int y)
{
	poloState.mouseX = x;
	poloState.mouseY = glutGet(GLUT_WINDOW_HEIGHT) - y;
	
	if (poloState.mouseMotionCallback)
		poloState.mouseMotionCallback(poloState.userData,
		                              poloState.mouseX,
		                              poloState.mouseY);
}

static void mouseButtonCallback(int button, int state, int x, int y)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			button = 0;
			break;
		case GLUT_RIGHT_BUTTON:
			button = 1;
			break;
		case GLUT_MIDDLE_BUTTON:
			button = 2;
			break;
		default:
			return;
	}
	
	if (state == GLUT_DOWN)
		poloState.mouseButtonState[button] = 1;
	else if (state == GLUT_UP)
		poloState.mouseButtonState[button] = 0;
	
	if (poloState.mouseButtonCallback)
		poloState.mouseButtonCallback(poloState.userData,
		                              button,
		                              poloState.mouseButtonState[button]);
	
	mouseMotionCallback(x, y);
}

static void timerCallback(int value)
{
	if (poloState.timerCallback)
		poloState.timerCallback(poloState.userData, value);
}



/* Initialization & exit */
void setPoloUserData(void *userData)
{
	poloState.userData = userData;
}

void initPolo(int width, int height, int fullscreen, const char *windowTitle)
{
	int argc = 1;
	int displayMode;
	const char *argv[2] = {"polo", NULL};
	
	if (poloState.isInitialized)
		return;
	
	/* Init state */
	setPenColor(POLO_WHITE);
	setFillColor(POLO_TRANSPARENT);
	setDrawTint(POLO_WHITE);
	setDrawScale(1.0);
	setTextFont(POLO_HELVETICA_18);
	
	/* Init glut */
	glutInit(&argc, (char **)argv);
	displayMode = GLUT_RGB | GLUT_DEPTH;
#ifndef __APPLE__
	displayMode |= GLUT_DOUBLE;
#endif /* __APPLE__ */
	glutInitWindowSize(width, height);
	glutInitDisplayMode(displayMode);
	glutCreateWindow(windowTitle);
	if (fullscreen)
		glutFullScreen();
	
	poloState.isInitialized = 1;
	
	/* Init glut run time (so it starts at 0 on all systems) */
	getTime();
	
	/* Set internal callbacks */
	glutDisplayFunc(drawCallback);
	glutReshapeFunc(resizeCallback);
	glutIdleFunc(drawCallback);
	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(specialCallback);
	glutMotionFunc(mouseMotionCallback);
	glutMouseFunc(mouseButtonCallback);
	glutPassiveMotionFunc(mouseMotionCallback);
	
	/* Configure freeglut */
#ifdef USE_FREEGLUT
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);	
#endif
	
	/* Enable vertical video sync */
#ifdef __APPLE__
	{
		const GLint sync = 1;
		CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &sync);
	}
#endif
	
#ifdef GLX_VERSION_1_4
	{
		PoloSwapControl swapControl;
		swapControl = (PoloSwapControl) glXGetProcAddress((const GLubyte *) "glXSwapIntervalSGI");
		if (swapControl)
			swapControl(1);
	}
#endif
	
#ifdef _WIN32
	{
		PoloSwapControl swapControl;
		swapControl = (PoloSwapControl) wglGetProcAddress((const GLubyte *) "wglSwapInterval");
		if (swapControl)
			swapControl(1);
	}
#endif
	
	/* Configure OpenGL */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_TEXTURE_2D);
	
	/* Clear front and back buffer */
	clearScreen();
	updateScreen();
	clearScreen();
}

void runPolo()
{
	glutMainLoop();
}

void exitPolo()
{
	if (!poloState.isInitialized)
		return;
	
#ifdef USE_FREEGLUT
	glutLeaveMainLoop();
	
	poloState.isInitialized = 0;
#else
	exit(0);
#endif
}



/* Drawing */

void setDrawCallback(void (*drawCallback)(void *userData))
{
	poloState.drawCallback = drawCallback;
}

static float saturate(float value)
{
	if (value < 0.0)
		return 0.0;
	if (value > 1.0)
		return 1.0;
	return value;
}

Color getColorFromRGBA(float red, float green, float blue, float alpha)
{
	float r = saturate(red);
	float g = saturate(green);
	float b = saturate(blue);
	float a = saturate(alpha);
	
	return ((((int)(r * 255.0)) << 24) +
	        (((int)(g * 255.0)) << 16) +
	        (((int)(b * 255.0)) << 8) +
	        (((int)(a * 255.0)) << 0));
}

Color getColorFromRGB(float red, float green, float blue)
{
	return getColorFromRGBA(red, green, blue, 1.0);
}

Color getColorFromHSVA(float hue, float saturation, float value, float alpha)
{
	float r, g, b;
	
	float h = hue - floor(hue);
	float s = saturate(saturation);
	float v = saturate(value);
	
	if (s == 0.0)
		r = g = b = v;
	else
	{
		float i, f, p, q, t;
		
		/* If hue == 1.0, then wrap it around the circle to 0.0 */
		if (h == 1.0)
			h = 0.0;
		
		/* Sector 0 to 5 */
		h *= 6.0;
		/* Integer part of h (0, 1, 2, 3, 4, 5 or 6) */
		i = floor(h);
		/* Factorial part of h (0 to 1) */
		f = h - i;
		
		p = v * (1.0f - s);
		q = v * (1.0f - s * f);
		t = v * (1.0f - s * (1.0f - f));
		
		switch ((int) h)
		{
			case 0:
				r = v;
				g = t;
				b = p;
				break;
			case 1:
				r = q;
				g = v;
				b = p;
				break;
			case 2:
				r = p;
				g = v;
				b = t;
				break;
			case 3:
				r = p;
				g = q;
				b = v;
				break;
			case 4:
				r = t;
				g = p;
				b = v;
				break;
			/* case 5 (or 6): */
			default:
				r = v;
				g = p;
				b = q;
				break;
		}
	}
	
	return getColorFromRGBA(r, g, b, alpha);
}

Color getColorFromHSV(float hue, float saturation, float value)
{
	return getColorFromHSVA(hue, saturation, value, 1.0);
}

void setPenColor(Color color)
{
	poloState.penColor = color;
}

void setFillColor(Color color)
{
	poloState.fillColor1 = color;
	poloState.fillColor2 = color;
}

void setFillGradient(Color color1, Color color2)
{
	poloState.fillColor1 = color1;
	poloState.fillColor2 = color2;
}

static void setPoloColor(Color color)
{
	if (!poloState.isInitialized)
		return;
	
	glColor4f(((color & 0xff000000) >> 24) / 255.0,
	          ((color & 0x00ff0000) >> 16) / 255.0,
	          ((color & 0x0000ff00) >> 8) / 255.0,
	          ((color & 0x000000ff) >> 0) / 255.0);
}

void drawPoint(float x, float y)
{
	if (!poloState.isInitialized)
		return;
	
	/* Translate coordinates to pixel center */
	glBegin(GL_POINTS);
	setPoloColor(poloState.penColor);
	glVertex2f(x + 0.5, y + 0.5);
	glEnd();
}

void drawLine(float x1, float y1, float x2, float y2)
{
	if (!poloState.isInitialized)
		return;
	
	x2 += (x2 == x1) ? 0 : ((x2 < x1) ? -1.0 : 1.0);
	y2 += (y2 == y1) ? 0 : ((y2 < y1) ? -1.0 : 1.0);
	
	/* Translate coordinates to pixel center */
	glBegin(GL_LINES);
	setPoloColor(poloState.penColor);
	glVertex2f(x1 + 0.5, y1 + 0.5);
	glVertex2f(x2 + 0.5, y2 + 0.5);
	glEnd();
}

void drawRect(float x, float y, float width, float height)
{
	if (!poloState.isInitialized)
		return;
	
	if ((width <= 0) || (height <= 0))
		return;
	
	glBegin(GL_QUADS);
	setPoloColor(poloState.fillColor2);
	glTexCoord2f(0, 0);
	glVertex2f(x, y);
	glTexCoord2f(1.0, 0);
	glVertex2f(x + width, y);
	setPoloColor(poloState.fillColor1);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(x + width, y + height);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(x, y + height);
	glEnd();
	
	/* Translate coordinates to pixel center */
	glBegin(GL_LINE_LOOP);
	setPoloColor(poloState.penColor);
	glVertex2f(x + 0.5, y + 0.5);
	glVertex2f(x + width - 0.5, y + 0.5);
	glVertex2f(x + width - 0.5, y + height - 0.5);
	glVertex2f(x + 0.5, y + height - 0.5);
	glEnd();
}

void drawRoundedRect(float x, float y, float width, float height, float edgeRadius)
{
	float maxEdgeRadius;
	int i;
	
	if (!poloState.isInitialized)
		return;
	
	if ((width <= 0) || (height <= 0))
		return;
	
	maxEdgeRadius = (width < height) ? width / 2.0 : height / 2.0;
	if (edgeRadius < 0)
		edgeRadius = 0;
	if (edgeRadius > maxEdgeRadius)
		edgeRadius = maxEdgeRadius;
	
	glBegin(GL_QUAD_STRIP);
	for(i = 0; i <= 180; i++)
	{
		float phase = i * 2.0 * M_PI / 360.0;
		float xr = edgeRadius * cos(phase);
		float yr = edgeRadius * sin(phase);
		float xp, yp1, yp2;
		
		if (i < 90)
			xp = edgeRadius - xr + 1.0;
		else
			xp = width - edgeRadius - xr - 1.0;
		yp1 = -yr + edgeRadius;
		yp2 = height + yr - edgeRadius;
		
		setPoloColor(poloState.fillColor2);
		glTexCoord2f(xp / width, yp1 / height);
		glVertex2f(x + xp, y + yp1);
		setPoloColor(poloState.fillColor1);
		glTexCoord2f(xp / width, yp2 / height);
		glVertex2f(x + xp, y + yp2);
	}
	glEnd();
	
	/* Translate coordinates to pixel center */
	glBegin(GL_LINE_LOOP);
	setPoloColor(poloState.penColor);
	for(i = 0; i < 360; i++)
	{
		float phase = i * 2.0 * M_PI / 360.0;
		float xr = edgeRadius * cos(phase);
		float yr = edgeRadius * sin(phase);
		float xp, yp;
		
		if ((i < 90) || (i >= 270))
			xp = xr + x + width - edgeRadius - 0.5;
		else
			xp = xr + x + edgeRadius + 0.5;
		if (i <= 180)
			yp = yr + y + height - edgeRadius - 0.5;
		else
			yp = yr + y + edgeRadius + 0.5;
		
		glVertex2f(xp, yp);
	}
	glEnd();
}

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
	if (!poloState.isInitialized)
		return;
	
	glBegin(GL_TRIANGLES);
	setPoloColor(poloState.fillColor2);
	glTexCoord2f(0, 0);
	glVertex2f(x1, y1);
	glTexCoord2f(1, 0);
	glVertex2f(x2, y2);
	setPoloColor(poloState.fillColor1);
	glTexCoord2f(1, 1);
	glVertex2f(x3, y3);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	setPoloColor(poloState.penColor);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glEnd();
}

void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	if (!poloState.isInitialized)
		return;
	
	glBegin(GL_QUADS);
	setPoloColor(poloState.fillColor2);
	glTexCoord2f(0, 0);
	glVertex2f(x1, y1);
	glTexCoord2f(1, 0);
	glVertex2f(x2, y2);
	setPoloColor(poloState.fillColor1);
	glTexCoord2f(1, 1);
	glVertex2f(x3, y3);
	glTexCoord2f(0, 1);
	glVertex2f(x4, y4);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	setPoloColor(poloState.penColor);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glVertex2f(x4, y4);
	glEnd();
}

void drawCircle(float x, float y, float radius)
{
	return drawRoundedRect(x - radius, y - radius, 2.0 * radius, 2.0 * radius, radius);
}

void clearScreen()
{
	if (!poloState.isInitialized)
		return;
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void updateScreen()
{
	if (!poloState.isInitialized)
		return;
	
	glutSwapBuffers();
}

float getScreenWidth()
{
	return glutGet(GLUT_WINDOW_WIDTH);
}

float getScreenHeight()
{
	return glutGet(GLUT_WINDOW_HEIGHT);
}

void setTextFont(enum PoloFont font)
{
	switch (font)
	{
		case POLO_COURIER_13:
			poloState.font = GLUT_BITMAP_8_BY_13;
			poloState.fontHeight = 14;
			poloState.fontBaseLine = 3;
			break;
		case POLO_COURIER_15:
			poloState.font = GLUT_BITMAP_9_BY_15;
			poloState.fontHeight = 16;
			poloState.fontBaseLine = 4;
			break;
		case POLO_TIMES_10:
			poloState.font = GLUT_BITMAP_TIMES_ROMAN_10;
			poloState.fontHeight = 14;
			poloState.fontBaseLine = 4;
			break;
		case POLO_TIMES_24:
			poloState.font = GLUT_BITMAP_TIMES_ROMAN_24;
			poloState.fontHeight = 29;
			poloState.fontBaseLine = 7;
			break;
		case POLO_HELVETICA_10:
			poloState.font = GLUT_BITMAP_HELVETICA_10;
			poloState.fontHeight = 14;
			poloState.fontBaseLine = 3;
			break;
		case POLO_HELVETICA_12:
			poloState.font = GLUT_BITMAP_HELVETICA_12;
			poloState.fontHeight = 16;
			poloState.fontBaseLine = 4;
			break;
		case POLO_HELVETICA_18:
			poloState.font = GLUT_BITMAP_HELVETICA_18;
			poloState.fontHeight = 23;
			poloState.fontBaseLine = 5;
			break;
	}
}

float getTextDrawWidth(const char *str)
{
	float maxLength = 0, currentLength = 0;
	
	if (!poloState.isInitialized)
		return 0;
	
	while(*str)
	{
		if(*str == '\n')
			currentLength = 0;
		else
			currentLength += glutBitmapWidth(poloState.font, *str);
		
		if(maxLength < currentLength)
			maxLength = currentLength;
		
		str++;
	}
	
	return maxLength;
}

float getTextDrawHeight(const char *str)
{
	int i = 1;

	if (!poloState.isInitialized)
		return 0;
	
	while (*str)
	{
		if (*str == '\n')
			i++;
		
		str++;
	}
	
	return i * poloState.fontHeight;
}

void drawText(float x, float y, const char *str)
{
	y += getTextDrawHeight(str);
	y += poloState.fontBaseLine;
	y -= poloState.fontHeight;
	
	setPoloColor(poloState.penColor);
	glRasterPos2f(x, y);
	while(*str)
	{
		if (*str == '\n')
		{
			y -= poloState.fontHeight;
			glRasterPos2f(x, y);
		}
		else
			glutBitmapCharacter(poloState.font, *str);
		
		str++;
	}
}

static unsigned int getLittleEndianValue(unsigned char *value, int bits)
{
	if (bits == 16)
		return (value[0] << 0) +
		       (value[1] << 8);
	else if (bits == 32)
		return (value[0] << 0) +
		       (value[1] << 8) +
		       (value[2] << 16) +
		       (value[3] << 24);
	
	return 0;
}

static int getNextPowerOf2(int value)
{
	value--;
	value |= value >> 1;
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;
	value++;
	
	return value;
}

static Image getFreeImage()
{
	Image i;
	
	for (i = 1; i < POLO_MAX_IMAGES; i++)
		if (!poloState.images[i].texture)
			return i;
	
	return 0;
}

Image loadImage(const char *path)
{
	PoloBMPHeader bmpHeader;
	int pixelsOffset, width, height, numberOfPlanes, bytesPerPixel, compression;
	int valid = 1;
	Image image = 0;
	
	if (!poloState.isInitialized)
		return 0;
	
	FILE *fp = fopen(path, "rb");
	if (!fp)
		return 0;
	
	if (fread(&bmpHeader, sizeof(bmpHeader), 1, fp) != 1)
		valid = 0;
	
	if ((bmpHeader.id[0] != 'B') || (bmpHeader.id[1] != 'M'))
		valid = 0;
	
	pixelsOffset = getLittleEndianValue(bmpHeader.pixelsOffset, 32);
	width = getLittleEndianValue(bmpHeader.width, 32);
	height = getLittleEndianValue(bmpHeader.height, 32);
	numberOfPlanes = getLittleEndianValue(bmpHeader.numberOfPlanes, 16);
	bytesPerPixel = getLittleEndianValue(bmpHeader.bitsPerPixel, 16) / 8;
	compression = getLittleEndianValue(bmpHeader.compression, 32);
	
	if ((width > 4096) || (height > 4096) || (height < -4096) ||
	    (numberOfPlanes != 1) ||
	    ((bytesPerPixel != 3) && (bytesPerPixel != 4)))
		valid = 0;
	
	if (valid)
	{
		/* OpenGL requires a textures of size 2 ^ m by 2 ^ n */
		int textureWidth = getNextPowerOf2(width);
		int textureHeight = getNextPowerOf2(abs(height));
		
		char *p = (char *)calloc(textureWidth * textureHeight, bytesPerPixel);
		int y;
		
		if (p)
		{
			int padding = (4 - (width * bytesPerPixel) & 0x3) & 0x3;
			
			fseek(fp, pixelsOffset, SEEK_SET);
			
			/* Read image line by line */
			if (height >= 0)
				for (y = 0; y < height; y++)
				{
					fread(&p[y * textureWidth * bytesPerPixel],
					      width * bytesPerPixel, 1, fp);
					fseek(fp, padding, SEEK_CUR);
				}
			else
				for (y = -height - 1; y >= 0; y--)
				{
					fread(&p[y * textureWidth * bytesPerPixel],
					      width * bytesPerPixel, 1, fp);
					fseek(fp, padding, SEEK_CUR);
				}
			
			image = getFreeImage();
			if (image)
			{
				PoloImage *poloImage = &poloState.images[image];
				
				glGenTextures(1, &(poloImage->texture));
				
				glBindTexture(GL_TEXTURE_2D, poloImage->texture);
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
				             textureWidth, textureHeight,
				             0, (bytesPerPixel == 4) ? GL_BGRA : GL_BGR,
				             GL_UNSIGNED_BYTE, p);
				
				glBindTexture(GL_TEXTURE_2D, 0);
				
				poloImage->textureWidth = textureWidth;
				poloImage->textureHeight = textureHeight;
				poloImage->width = width;
				poloImage->height = abs(height);
			}
			
			free(p);
		}
	}
	
	fclose(fp);
	
	return image;
}

float getImageWidth(Image image)
{
	if (image >= POLO_MAX_IMAGES)
		return 0;
	
	return poloState.images[image].width;
}

float getImageHeight(Image image)
{
	if (image >= POLO_MAX_IMAGES)
		return 0;
	
	return poloState.images[image].height;
}

void freeImage(Image image)
{
	if (!poloState.isInitialized)
		return;
	
	if (image >= POLO_MAX_IMAGES)
		return;
	
	glDeleteTextures(1, &poloState.images[image].texture);
	
	poloState.images[image].texture = 0;
	poloState.images[image].textureWidth = 0;
	poloState.images[image].textureHeight = 0;
	poloState.images[image].width = 0;
	poloState.images[image].height = 0;
}

void drawImage(float x, float y, Image image)
{
	PoloImage *theImage;
	float scale;
	float texCoordWidth, texCoordHeight;
	
	if (!poloState.isInitialized)
		return;
	
	if (image >= POLO_MAX_IMAGES)
		return;
	
	theImage = &poloState.images[image];
	scale = poloState.drawScale;
	
	if (!theImage->texture)
		return;
	
	texCoordWidth = ((float) theImage->width) / theImage->textureWidth;
	texCoordHeight = ((float) theImage->height) / theImage->textureHeight;
	
	glBindTexture(GL_TEXTURE_2D, theImage->texture);
	setPoloColor(poloState.drawTint);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(x, y);
	glTexCoord2f(texCoordWidth, 0);
	glVertex2f(x + theImage->width * scale, y);
	glTexCoord2f(texCoordWidth, texCoordHeight);
	glVertex2f(x + theImage->width * scale, y + theImage->height * scale);
	glTexCoord2f(0, texCoordHeight);
	glVertex2f(x, y + theImage->height * scale);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, 0);	
}

void setDrawTint(Color tint)
{
	poloState.drawTint = tint;
}

void setDrawScale(float scale)
{
	poloState.drawScale = scale;
}

void setTexture(Image image)
{
	PoloImage *poloImage;
	
	if (!poloState.isInitialized)
		return;
	
	if (image >= POLO_MAX_IMAGES)
		return;
	
	poloImage = &poloState.images[image];
	
	glBindTexture(GL_TEXTURE_2D, poloImage->texture);
}



/* Keyboard */

void setKeyboardCallback(void (*keyboardCallback)(void *userData, int key))
{
	poloState.keyboardCallback = keyboardCallback;
}

int getKey()
{
	return poloState.key;
}

void clearKey()
{
	poloState.key = 0;
}



/* Mouse */

void setMouseMotionCallback(void (*mouseMotionCallback)(void *userData, int x, int y))
{
	poloState.mouseMotionCallback = mouseMotionCallback;
}

void setMouseButtonCallback(void (*mouseButtonCallback)(void *userData, int button, int pressed))
{
	poloState.mouseButtonCallback = mouseButtonCallback;
}

float getMouseX()
{
	return poloState.mouseX;
}

float getMouseY()
{
	return poloState.mouseY;
}

int isMouseButtonPressed(int buttonIndex)
{
	if (buttonIndex >= POLO_MOUSEBUTTON_NUM)
		return 0;
	
	return poloState.mouseButtonState[buttonIndex];
}

void showMousePointer()
{
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void hideMousePointer()
{
	glutSetCursor(GLUT_CURSOR_NONE);
}



/* Time */

void setTimerCallback(void (*timerCallback)(void *userData, int id))
{
	poloState.timerCallback = timerCallback;
}

void runTimer(int id, int milliseconds)
{
	if (!poloState.isInitialized)
		return;
	
	glutTimerFunc(milliseconds, timerCallback, id);
}

float getTime()
{
	if (!poloState.isInitialized)
		return 0;
	
	return glutGet(GLUT_ELAPSED_TIME) * 0.001;
}
