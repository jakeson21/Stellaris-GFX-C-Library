/*********************************************************************
This library is based on the work by Adafruit. It has been rewritten to
be compatable with ANSI C specifically for use on TI Stellaris boards.
*********************************************************************/

/*
 This is the core graphics library for all our displays, providing a common
 set of graphics primitives (points, lines, circles, etc.).  It needs to be
 paired with a hardware-specific library for each display device we carry
 (to handle the lower-level functions).

 Adafruit invests time and resources providing this open source code, please
 support Adafruit & open-source hardware by purchasing products from Adafruit!
 
 Copyright (c) 2013 Adafruit Industries.  All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 - Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 - Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include <Adafruit_GFX.h>
#include "glcdfont.c"
/*
 #ifdef __AVR__
 #include <avr/pgmspace.h>
 #else
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
 #endif
 */

void Adafruit_GFX_init(tAdafruit_GFX *psInst, int16_t w, int16_t h, uint8_t *buff)
{
	psInst->WIDTH = w; // physical dimension - set, then do not change
	psInst->HEIGHT = h; // physical dimension - set, then do not change
	psInst->_width = w; // this reflects orientation of display
	psInst->_height = h; // this reflects orientation of display
	psInst->rotation = 0;
	psInst->cursor_y = 0;
	psInst->cursor_x = 0;
	psInst->textsize = 1;
	psInst->textcolor = 0xFFFF;
	psInst->textbgcolor = 0xFFFF;
	psInst->wrap = true;
	psInst->buffer = buff;

}

// Draw a circle outline
void drawCircle(tAdafruit_GFX *psInst, int16_t x0, int16_t y0, int16_t r,
		uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	drawPixel(psInst, x0, y0 + r, color);
	drawPixel(psInst, x0, y0 - r, color);
	drawPixel(psInst, x0 + r, y0, color);
	drawPixel(psInst, x0 - r, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		drawPixel(psInst,x0 + x, y0 + y, color);
		drawPixel(psInst,x0 - x, y0 + y, color);
		drawPixel(psInst,x0 + x, y0 - y, color);
		drawPixel(psInst,x0 - x, y0 - y, color);
		drawPixel(psInst,x0 + y, y0 + x, color);
		drawPixel(psInst,x0 - y, y0 + x, color);
		drawPixel(psInst,x0 + y, y0 - x, color);
		drawPixel(psInst,x0 - y, y0 - x, color);
	}
}

void drawCircleHelper(tAdafruit_GFX *psInst, int16_t x0, int16_t y0, int16_t r,
		uint8_t cornername, uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (cornername & 0x4) {
			drawPixel(psInst,x0 + x, y0 + y, color);
			drawPixel(psInst,x0 + y, y0 + x, color);
		}
		if (cornername & 0x2) {
			drawPixel(psInst,x0 + x, y0 - y, color);
			drawPixel(psInst,x0 + y, y0 - x, color);
		}
		if (cornername & 0x8) {
			drawPixel(psInst,x0 - y, y0 + x, color);
			drawPixel(psInst,x0 - x, y0 + y, color);
		}
		if (cornername & 0x1) {
			drawPixel(psInst,x0 - y, y0 - x, color);
			drawPixel(psInst,x0 - x, y0 - y, color);
		}
	}
}

void fillCircle(tAdafruit_GFX *psInst, int16_t x0, int16_t y0, int16_t r,
		uint16_t color) {
	drawFastVLine(psInst,x0, y0 - r, 2 * r + 1, color);
	fillCircleHelper(psInst,x0, y0, r, 3, 0, color);
}

// Used to do circles and roundrects
void fillCircleHelper(tAdafruit_GFX *psInst, int16_t x0, int16_t y0, int16_t r,
		uint8_t cornername, int16_t delta, uint16_t color) {

	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x1) {
			drawFastVLine(psInst,x0 + x, y0 - y, 2 * y + 1 + delta, color);
			drawFastVLine(psInst,x0 + y, y0 - x, 2 * x + 1 + delta, color);
		}
		if (cornername & 0x2) {
			drawFastVLine(psInst,x0 - x, y0 - y, 2 * y + 1 + delta, color);
			drawFastVLine(psInst,x0 - y, y0 - x, 2 * x + 1 + delta, color);
		}
	}
}

// Bresenham's algorithm - thx wikpedia
void drawLine(tAdafruit_GFX *psInst, int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		uint16_t color) {
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for (; x0 <= x1; x0++) {
		if (steep) {
			drawPixel(psInst,y0, x0, color);
		} else {
			drawPixel(psInst,x0, y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

// Draw a rectangle
void drawRect(tAdafruit_GFX *psInst, int16_t x, int16_t y, int16_t w, int16_t h,
		uint16_t color) {
	drawFastHLine(psInst, x, y, w, color);
	drawFastHLine(psInst, x, y + h - 1, w, color);
	drawFastVLine(psInst, x, y, h, color);
	drawFastVLine(psInst, x + w - 1, y, h, color);
}

/*
void drawFastVLine(tAdafruit_GFX *psInst, int16_t x, int16_t y, int16_t h,
		uint16_t color) {
	// Update in subclasses if desired!
	drawLine(psInst,x, y, x, y + h - 1, color);
}

void drawFastHLine(tAdafruit_GFX *psInst, int16_t x, int16_t y, int16_t w,
		uint16_t color) {
	// Update in subclasses if desired!
	drawLine(psInst,x, y, x + w - 1, y, color);
}
*/

void fillRect(tAdafruit_GFX *psInst, int16_t x, int16_t y, int16_t w, int16_t h,
		uint16_t color) {
	// Update in subclasses if desired!
	int16_t i;
	for (i = x; i < x + w; i++) {
		drawFastVLine(psInst, i, y, h, color);
	}
}

void fillScreen(tAdafruit_GFX *psInst, uint16_t color) {
	fillRect(psInst,0, 0, psInst->_width, psInst->_height, color);
}

// Draw a rounded rectangle
void drawRoundRect(tAdafruit_GFX *psInst, int16_t x, int16_t y, int16_t w, int16_t h,
		int16_t r, uint16_t color) {
	// smarter version
	drawFastHLine(psInst,x + r, y, w - 2 * r, color); // Top
	drawFastHLine(psInst,x + r, y + h - 1, w - 2 * r, color); // Bottom
	drawFastVLine(psInst,x, y + r, h - 2 * r, color); // Left
	drawFastVLine(psInst,x + w - 1, y + r, h - 2 * r, color); // Right
	// draw four corners
	drawCircleHelper(psInst,x + r, y + r, r, 1, color);
	drawCircleHelper(psInst,x + w - r - 1, y + r, r, 2, color);
	drawCircleHelper(psInst,x + w - r - 1, y + h - r - 1, r, 4, color);
	drawCircleHelper(psInst,x + r, y + h - r - 1, r, 8, color);
}

// Fill a rounded rectangle
void fillRoundRect(tAdafruit_GFX *psInst, int16_t x, int16_t y, int16_t w, int16_t h,
		int16_t r, uint16_t color) {
	// smarter version
	fillRect(psInst,x + r, y, w - 2 * r, h, color);

	// draw four corners
	fillCircleHelper(psInst,x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
	fillCircleHelper(psInst,x + r, y + r, r, 2, h - 2 * r - 1, color);
}

// Draw a triangle
void drawTriangle(tAdafruit_GFX *psInst, int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		int16_t x2, int16_t y2, uint16_t color) {
	drawLine(psInst,x0, y0, x1, y1, color);
	drawLine(psInst,x1, y1, x2, y2, color);
	drawLine(psInst,x2, y2, x0, y0, color);
}

// Fill a triangle
void fillTriangle(tAdafruit_GFX *psInst, int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		int16_t x2, int16_t y2, uint16_t color) {

	int16_t a, b, y, last;

	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1) {
		swap(y0, y1);
		swap(x0, x1);
	}
	if (y1 > y2) {
		swap(y2, y1);
		swap(x2, x1);
	}
	if (y0 > y1) {
		swap(y0, y1);
		swap(x0, x1);
	}

	if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
		a = b = x0;
		if (x1 < a)
			a = x1;
		else if (x1 > b)
			b = x1;
		if (x2 < a)
			a = x2;
		else if (x2 > b)
			b = x2;
		drawFastHLine(psInst,a, y0, b - a + 1, color);
		return;
	}

	int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
			dx12 = x2 - x1, dy12 = y2 - y1;
	int32_t sa = 0, sb = 0;

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if (y1 == y2)
		last = y1;   // Include y1 scanline
	else
		last = y1 - 1; // Skip it

	for (y = y0; y <= last; y++) {
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		/* longhand:
		 a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
		 b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		 */
		if (a > b)
			swap(a, b);
		drawFastHLine(psInst,a, y, b - a + 1, color);
	}

	// For lower part of triangle, find scanline crossings for segments
	// 0-2 and 1-2.  This loop is skipped if y1=y2.
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for (; y <= y2; y++) {
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		/* longhand:
		 a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		 b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		 */
		if (a > b)
			swap(a, b);
		drawFastHLine(psInst,a, y, b - a + 1, color);
	}
}

void drawBitmap(tAdafruit_GFX *psInst, int16_t x, int16_t y, const uint8_t *bitmap,
		int16_t w, int16_t h, uint16_t color) {

	int16_t i, j, byteWidth = (w + 7) / 8;

	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++) {
			if (bitmap[j * byteWidth + i / 8] & (128 >> (i & 7))) {
				drawPixel(psInst,x + i, y + j, color);
			}
		}
	}
}
/*
// Draw a 1-bit color bitmap at the specified x, y position from the
// provided bitmap buffer (must be PROGMEM memory) using color as the
// foreground color and bg as the background color.
void drawBitmap(tAdafruit_GFX *psInst, int16_t x, int16_t y, const uint8_t *bitmap,
		int16_t w, int16_t h, uint16_t color, uint16_t bg) {

	int16_t i, j, byteWidth = (w + 7) / 8;

	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++) {
			if (pgm_read_byte(bitmap + j * byteWidth + i / 8)
					& (128 >> (i & 7))) {
				drawPixel(psInst,x + i, y + j, color);
			} else {
				drawPixel(psInst,x + i, y + j, bg);
			}
		}
	}
}*/

//Draw XBitMap Files (*.xbm), exported from GIMP,
//Usage: Export from GIMP to *.xbm, rename *.xbm to *.c and open in editor.
//C Array can be directly used with this function
void drawXBitmap(tAdafruit_GFX *psInst, int16_t x, int16_t y, const uint8_t *bitmap,
		int16_t w, int16_t h, uint16_t color) {

	int16_t i, j, byteWidth = (w + 7) / 8;

	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++) {
			if (bitmap[j * byteWidth + i / 8] & (1 << (i % 8))) {
				drawPixel(psInst,x + i, y + j, color);
			}
		}
	}
}

uint8_t Write(tAdafruit_GFX *psInst, uint8_t c) {
	if (c == '\n') {
		psInst->cursor_y += psInst->textsize * 8;
		psInst->cursor_x = 0;
	} else if (c == '\r') {
		// skip em
	} else {
		drawChar(psInst, c);
		psInst->cursor_x += psInst->textsize * 6;
		if (psInst->wrap && (psInst->cursor_x > (psInst->_width - psInst->textsize * 6))) {
			psInst->cursor_y += psInst->textsize * 8;
			psInst->cursor_x = 0;
		}
	}
	return 1;
}


void Print(tAdafruit_GFX *psInst, const char *str)
{
  while (*str){
    Write(psInst, *str++);
  }
}

// Draw a character
void drawChar(tAdafruit_GFX *psInst, uint8_t c) {
	int16_t x = psInst->cursor_x;
	int16_t y = psInst->cursor_y;
	uint16_t color = psInst->textcolor;
	uint16_t bg = psInst->textbgcolor;
	uint8_t size = psInst->textsize;

	int8_t i;
	int8_t j;
	if ((x >= psInst->_width) || // Clip right
			(y >= psInst->_height) || // Clip bottom
			((x + 6 * size - 1) < 0) || // Clip left
			((y + 8 * size - 1) < 0))   // Clip top
		return;

	for (i = 0; i < 6; i++) {
		uint8_t line;
		if (i == 5)
			line = 0x0;
		else
			line = font[(c * 5) + i];
		for (j = 0; j < 8; j++) {
			if (line & 0x1) {
				if (size == 1) // default size
					drawPixel(psInst, x + i, y + j, color);
				else {  // big size
					fillRect(psInst, x + (i * size), y + (j * size), size, size, color);
				}
			} else if (bg != color) {
				if (size == 1) // default size
					drawPixel(psInst, x + i, y + j, bg);
				else {  // big size
					fillRect(psInst, x + i * size, y + j * size, size, size, bg);
				}
			}
			line >>= 1;
		}
	}
}

void setCursor(tAdafruit_GFX *psInst, int16_t x, int16_t y) {
	psInst->cursor_x = x;
	psInst->cursor_y = y;
}

void setTextSize(tAdafruit_GFX *psInst, uint8_t s) {
	psInst->textsize = (s > 0) ? s : 1;
}

void setTextColor(tAdafruit_GFX *psInst, uint16_t c) {
	// For 'transparent' background, we'll set the bg
	// to the same as fg instead of using a flag
	psInst->textcolor = psInst->textbgcolor = c;
}
/*
void setTextColor(tAdafruit_GFX *psInst, uint16_t c, uint16_t b) {
	psInst->textcolor = c;
	psInst->textbgcolor = b;
}
*/
void setTextWrap(tAdafruit_GFX *psInst, bool w) {
	psInst->wrap = w;
}

uint8_t getRotation(tAdafruit_GFX *psInst) {
	return psInst->rotation;
}

void setRotation(tAdafruit_GFX *psInst, uint8_t x) {
	psInst->rotation = (x & 3);
	switch (psInst->rotation) {
	case 0:
	case 2:
		psInst->_width = psInst->WIDTH;
		psInst->_height = psInst->HEIGHT;
		break;
	case 1:
	case 3:
		psInst->_width = psInst->HEIGHT;
		psInst->_height = psInst->WIDTH;
		break;
	}
}

// Return the size of the display (per current rotation)
int16_t width(tAdafruit_GFX *psInst) {
	return psInst->_width;
}

int16_t height(tAdafruit_GFX *psInst) {
	return psInst->_height;
}

/*
void invertDisplay(tAdafruit_GFX *psInst, bool i) {
	// Do nothing, must be subclassed if supported
}
*/
