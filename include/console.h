#ifndef __INCLUDE_CONSOLE_H__
#define __INCLUDE_CONSOLE_H__
#include <types.h>
typedef enum real_color
{
    RC_BLACK = 0,
    RC_BLUE,
    RC_GREEN,
    RC_CYAN,
    RC_RED,
    RC_MAGENTA,
    RC_BROWN,
    RC_LIGHT_GREY,
    RC_DARK_GREY,
    RC_LIGHT_BLUE,
    RC_LIGHT_GREEN,
    RC_LIGHT_CYAN,
    RC_LIGHT_RED,
    RC_LIGHT_MAGENTA,
    RC_LIGHT_BROWN, //yellow
    RC_WHITE
} real_color_t;

void ce_clear();

void ce_write_cchar(char c, real_color_t back, real_color_t fore);
void ce_write_cstr(char *str, real_color_t back, real_color_t fore);
void ce_write_cnum(uint32_t num, real_color_t back, real_color_t fore);

void ce_write_char(char c);
void ce_write_str(char *str);
void ce_write_num(uint32_t num);

static uint16_t *video_mem = (uint16_t *)0xC00B8000;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static inline uint16_t get_cchar(char c, real_color_t back, real_color_t fore);
static void move_cursor();
#endif
