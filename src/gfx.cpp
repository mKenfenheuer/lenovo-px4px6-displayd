#include "gfx.h"

using namespace std;

char fbuffer[1030] = {0x00};

void set_pixel(uint8_t x, uint8_t y, bool set)
{
    if (x >= 127 && y >= 63)
        return;

    uint8_t bit = y % 8;
    uint8_t row = (y - bit) / 8;
    uint16_t byte = row * 128 + x;

    if (x == 127 && y == 63)
    {
        bit = 8;
        row = 4;
        byte = 1029;
    }

    bool state = get_bit(fbuffer[byte], bit);

    if (state && !set)
    {
        fbuffer[byte] = clear_bit(fbuffer[byte], bit);
    }
    else if (!state && set)
    {
        fbuffer[byte] = set_bit(fbuffer[byte], bit);
    }
}

void flip_pixel(uint8_t x, uint8_t y)
{
    if (x >= 127 && y >= 63)
        return;

    uint8_t bit = y % 8;
    uint8_t row = (y - bit) / 8;
    uint16_t byte = row * 128 + x;

    if (x == 127 && y == 63)
    {
        bit = 8;
        row = 4;
        byte = 1029;
    }

    fbuffer[byte] = flip_bit(fbuffer[byte], bit);
}

void draw_line(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, bool color)
{
    int diff_y = end_y - start_y;
    int diff_x = end_x - start_x;
    float steepness = diff_y / (float)diff_x;
    int dir_x = diff_x < 0 ? -1 : 1;
    int dir_y = diff_y < 0 ? -1 : 1;
    if (abs(diff_x) > abs(diff_y))
    {
        for (int x = 0; x <= abs(diff_x); x++)
        {
            float y = start_y + (steepness * x * dir_x);
            set_pixel(start_x + x, round(y), color);
        }
    }
    else
    {
        for (int y = 0; y <= abs(diff_y); y++)
        {
            float x = start_x + (y / steepness * dir_y);
            set_pixel(round(x), start_y + y, color);
        }
    }
}
void fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool color)
{
    for (int xc = x; xc <= x + width; xc++)
        for (int yc = y; yc <= y + height; yc++)
            set_pixel(xc, yc, color);
}
void draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool color)
{
    width -= 1;
    height -= 1;

    draw_line(x, y, x + width, y, color);
    draw_line(x, y, x, y + height, color);
    draw_line(x, y + height, x + width, y + height, color);
    draw_line(x + width, y, x + width, y + height, color);
}
void draw_progressbar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, int progress)
{
    draw_progressbar(x, y, width, height, (double)progress);
}
void draw_progressbar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, double progress)
{
    if (progress > 100)
        progress = 100;
    if (progress < 0)
        progress = 0;
    if (height < 5)
        height = 5;
    if (width < 5)
        width = 5;

    draw_rect(x, y, width, height, true);

    int progress_width = round((width - 5) * progress / 100.0f);

    fill_rect(x + 2, y + 2, progress_width, height - 5, true);

    char text_progress[5];
    sprintf((char *)&text_progress, "%.2lf%%", progress);
    draw_text_invert_center((char *)&text_progress, &Roboto_Regular6pt7b, x + width / 2, y + height / 2 + 3);
}
uint8_t measure_char(char c, const GFXfont *font)
{
    if (c < font->first || c > font->last)
        return 0;

    GFXglyph glyph = font->glyph[c - font->first];

    return glyph.xAdvance;
}
uint8_t measure_text(char *str, const GFXfont *font)
{
    uint8_t xx = 0;
    size_t len = strlen(str);
    for (size_t pos = 0; pos < len; pos++)
    {
        xx += measure_char(str[pos], font);
    }

    return xx;
}
uint8_t draw_char(char c, const GFXfont *font, uint8_t x, uint8_t y, bool color)
{
    if (c < font->first || c > font->last)
        return 0;

    GFXglyph glyph = font->glyph[c - font->first];

    uint16_t bo = glyph.bitmapOffset;
    uint8_t w = glyph.width, h = glyph.height;
    uint8_t xo = glyph.xOffset, yo = glyph.yOffset;
    uint8_t xx, yy, bits = 0, bit = 0;
    int16_t xo16 = 0, yo16 = 0;

    for (yy = 0; yy < glyph.height; yy++)
    {
        for (xx = 0; xx < glyph.width; xx++)
        {
            if (!(bit++ & 7))
            {
                bits = font->bitmap[bo++];
            }
            if (bits & 0x80)
            {
                set_pixel(x + xo + xx, y + yo + yy, true);
            }
            bits <<= 1;
        }
    }

    return glyph.xAdvance;
}
uint8_t draw_char_invert(char c, const GFXfont *font, uint8_t x, uint8_t y)
{
    if (c < font->first || c > font->last)
        return 0;

    GFXglyph glyph = font->glyph[c - font->first];

    uint16_t bo = glyph.bitmapOffset;
    uint8_t w = glyph.width, h = glyph.height;
    uint8_t xo = glyph.xOffset, yo = glyph.yOffset;
    uint8_t xx, yy, bits = 0, bit = 0;
    int16_t xo16 = 0, yo16 = 0;

    for (yy = 0; yy < glyph.height; yy++)
    {
        for (xx = 0; xx < glyph.width; xx++)
        {
            if (!(bit++ & 7))
            {
                bits = font->bitmap[bo++];
            }
            if (bits & 0x80)
            {
                flip_pixel(x + xo + xx, y + yo + yy);
            }
            bits <<= 1;
        }
    }

    return glyph.xAdvance;
}
void draw_text(char *str, const GFXfont *font, uint8_t x, uint8_t y, bool color)
{
    uint8_t xx = 0;
    size_t len = strlen(str);
    for (size_t pos = 0; pos < len; pos++)
    {
        xx += draw_char(str[pos], font, xx + x, y, true);
    }
}
void draw_text_invert(char *str, const GFXfont *font, uint8_t x, uint8_t y)
{
    uint8_t xx = 0;
    size_t len = strlen(str);
    for (size_t pos = 0; pos < len; pos++)
    {
        xx += draw_char_invert(str[pos], font, xx + x, y);
    }
}
void draw_text_center(char *str, const GFXfont *font, uint8_t x, uint8_t y, bool color)
{
    uint8_t xlen = measure_text(str, font);
    uint8_t xx = 0;
    size_t len = strlen(str);
    for (size_t pos = 0; pos < len; pos++)
    {
        xx += draw_char(str[pos], font, xx + x - (xlen / 2), y, true);
    }
}
void draw_text_invert_center(char *str, const GFXfont *font, uint8_t x, uint8_t y)
{
    uint8_t xlen = measure_text(str, font);
    uint8_t xx = 0;
    size_t len = strlen(str);
    for (size_t pos = 0; pos < len; pos++)
    {
        xx += draw_char_invert(str[pos], font, xx + x - (xlen / 2), y);
    }
}
void reset_fbuffer()
{
    memset(fbuffer, 0, sizeof fbuffer);
}
void update_display()
{
    ofstream display_fb;
    display_fb.open("/proc/ums8485md/fb", std::ios::binary);
    display_fb.write(fbuffer, sizeof(fbuffer));
    display_fb.clear();

    ofstream display_update;
    display_update.open("/proc/ums8485md/display", std::ios::binary);
    char display_cmd[1] = {0x01};
    display_update.write(display_cmd, sizeof(display_cmd));
    display_update.close();
}

void dump_header()
{
    printf("char ilogo[1030] = {");
    for (int i = 0; i < sizeof(fbuffer); i++)
    {
        if (i != sizeof(fbuffer) - 1)
        {
            printf("0x%02x,", fbuffer[i]);
        }
        else
        {
            printf("0x%02x", fbuffer[i]);
        }
    }
    printf("};\n");
}
