#ifndef __GFX_INCLUDED__
#define __GFX_INCLUDED__
#include <iostream>
#include <fstream>
#include <cstring>
#include <tgmath.h>
#include "bit_ops.h"
#include "fonts/RobotoRegular.h"
#include "fonts/FreeSansBold9pt7b.h"


void set_pixel(uint8_t x, uint8_t y, bool set);
void draw_line(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, bool color);
void fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool color);
void draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool color);
void draw_progressbar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, int progress);
uint8_t draw_char(char c, const GFXfont *font, uint8_t x, uint8_t y, bool color);
uint8_t draw_char_invert(char c, const GFXfont *font, uint8_t x, uint8_t y);
uint8_t measure_char(char c, const GFXfont *font);
uint8_t measure_text(char* str, const GFXfont *font);
void draw_text(char* str, const GFXfont *font, uint8_t x, uint8_t y, bool color);
void draw_text_invert(char* str, const GFXfont *font, uint8_t x, uint8_t y);
void draw_text_center(char *str, const GFXfont *font, uint8_t x, uint8_t y, bool color);
void draw_text_invert_center(char *str, const GFXfont *font, uint8_t x, uint8_t y);
void reset_fbuffer();
void update_display();
void dump_header();
#endif