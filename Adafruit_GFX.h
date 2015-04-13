/*********************************************************************
This library is based on the work by Adafruit. It has been rewritten to
be compatable with ANSI C specifically for use on TI Stellaris boards.
*********************************************************************/

#ifndef _ADAFRUIT_GFX_H
#define _ADAFRUIT_GFX_H

#include <stdbool.h>
#include <stdint.h>

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C" {
#endif

#define swap(a, b) { int16_t t = a; a = b; b = t; }

typedef struct Adafruit_GFX {
	int16_t WIDTH;
	int16_t HEIGHT; // This is the 'raw' display w/h - never changes
	int16_t _width;
	int16_t _height; // Display w/h as modified by current rotation
	int16_t cursor_x;
	int16_t cursor_y;
	uint16_t textcolor;
	uint16_t textbgcolor;
	uint8_t textsize;
	uint8_t rotation;
	uint8_t *buffer;
	bool wrap; // If set, 'wrap' text at right edge of display
} tAdafruit_GFX;

//*****************************************************************************
//
// Prototypes.
//
//*****************************************************************************

extern void Adafruit_GFX_init(tAdafruit_GFX *psInst, int16_t w, int16_t h, uint8_t *buff); // Constructor

// This MUST be defined by the subclass:
extern void drawPixel(tAdafruit_GFX *psInst, int16_t x, int16_t y,
		uint16_t color);

// These MAY be overridden by the subclass to provide device-specific
// optimized code.  Otherwise 'generic' versions are used.
extern void drawLine(tAdafruit_GFX *psInst, int16_t x0, int16_t y0, int16_t x1,
		int16_t y1, uint16_t color);


extern void drawFastVLine(tAdafruit_GFX *psInst, int16_t x, int16_t y,
		int16_t h, uint16_t color);
extern void drawFastHLine(tAdafruit_GFX *psInst, int16_t x, int16_t y,
		int16_t w, uint16_t color);


extern void drawRect(tAdafruit_GFX *psInst, int16_t x, int16_t y, int16_t w,
		int16_t h, uint16_t color);
extern void fillRect(tAdafruit_GFX *psInst, int16_t x, int16_t y, int16_t w,
		int16_t h, uint16_t color);
extern void fillScreen(tAdafruit_GFX *psInst, uint16_t color);
//extern void invertDisplay(tAdafruit_GFX *psInst, bool i);

// These exist only with Adafruit_GFX (no subclass overrides)
extern void drawCircle(tAdafruit_GFX *psInst, int16_t x0, int16_t y0, int16_t r,
		uint16_t color);
extern void drawCircleHelper(tAdafruit_GFX *psInst, int16_t x0, int16_t y0,
		int16_t r, uint8_t cornername, uint16_t color);
extern void fillCircle(tAdafruit_GFX *psInst, int16_t x0, int16_t y0, int16_t r,
		uint16_t color);
extern void fillCircleHelper(tAdafruit_GFX *psInst, int16_t x0, int16_t y0,
		int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
extern void drawTriangle(tAdafruit_GFX *psInst, int16_t x0, int16_t y0,
		int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
extern void fillTriangle(tAdafruit_GFX *psInst, int16_t x0, int16_t y0,
		int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
extern void drawRoundRect(tAdafruit_GFX *psInst, int16_t x0, int16_t y0,
		int16_t w, int16_t h, int16_t radius, uint16_t color);
extern void fillRoundRect(tAdafruit_GFX *psInst, int16_t x0, int16_t y0,
		int16_t w, int16_t h, int16_t radius, uint16_t color);
extern void drawBitmap(tAdafruit_GFX *psInst, int16_t x, int16_t y,
		const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
/*void drawBitmap(tAdafruit_GFX *psInst,int16_t x, int16_t y, const uint8_t *bitmap, int16_t w,
 int16_t h, uint16_t color, uint16_t bg);*/
extern void drawXBitmap(tAdafruit_GFX *psInst, int16_t x, int16_t y,
		const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
extern void drawChar(tAdafruit_GFX *psInst, uint8_t c);
extern void setCursor(tAdafruit_GFX *psInst, int16_t x, int16_t y);
extern void setTextColor(tAdafruit_GFX *psInst, uint16_t c);
//void setTextColor(tAdafruit_GFX *psInst,uint16_t c, uint16_t bg);
extern void setTextSize(tAdafruit_GFX *psInst, uint8_t s);
extern void setTextWrap(tAdafruit_GFX *psInst, bool w);
extern void setRotation(tAdafruit_GFX *psInst, uint8_t r);

void Print(tAdafruit_GFX *psInst, const char *str);
extern uint8_t Write(tAdafruit_GFX *psInst, uint8_t);

extern int16_t height(tAdafruit_GFX *psInst);
extern int16_t width(tAdafruit_GFX *psInst);
extern uint8_t getRotation(tAdafruit_GFX *psInst);

#endif // _ADAFRUIT_GFX_H
