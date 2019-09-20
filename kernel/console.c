#include <console.h>
#include <common.h>
static void move_cursor()
{
    uint16_t cursor_t = cursor_y * 80 + cursor_x;
    outb(0x3d4, 14);
    outb(0x3d5, cursor_t >> 8);
    outb(0x3d4, 15);
    outb(0x3d5, cursor_t & 0xff);
}

static inline uint16_t get_cchar(char c, real_color_t back, real_color_t fore)
{
    return ((uint16_t)c & 0xff) | (((uint8_t)back << 4) | ((uint8_t)fore & 0xf)) << 8;
}
static void scroll()
{
    uint16_t blank = get_cchar(' ', RC_BLACK, RC_WHITE);
    if (cursor_y >= 25)
    {
        for (int i = 0; i < 24 * 80; ++i)
        {
            video_mem[i] = video_mem[i + 80];
        }
        for (int i = 24 * 80; i < 25 * 80; ++i)
        {
            video_mem[i] = blank;
        }
        cursor_y = 24;
    }
}

void ce_clear()
{
    uint16_t blank = get_cchar(' ', RC_BLACK, RC_WHITE);
    for (int i = 0; i < 80 * 25; ++i)
    {
        video_mem[i] = blank;
    }
    cursor_x = cursor_y = 0;
    move_cursor();
}

void ce_write_cchar(char c, real_color_t back, real_color_t fore)
{
    uint32_t color = get_cchar(0, back, fore);
    //backspace
    if (c == 0x08 && cursor_x)
    {
        cursor_x--;
    } //tab
    else if (c == 0x09)
    {
        cursor_x = (cursor_x + 8) & ~(8 - 1);
    }
    else if (c == '\r')
    {
        cursor_x = 0;
    }
    else if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    else if (c == '\t')
    {
        cursor_x += 8;
    }
    else if (c >= ' ')
    {
        video_mem[cursor_y * 80 + cursor_x] = c | color;
        cursor_x++;
    }

    if (cursor_x >= 80)
    {
        cursor_x = 0;
        cursor_y++;
    }
    scroll();
    move_cursor();
}
void ce_write_cstr(char *str, real_color_t back, real_color_t fore)
{
    while (*str)
    {
        ce_write_cchar(*str++, back, fore);
    }
}
void ce_write_cnum(uint32_t num, real_color_t back, real_color_t fore)
{
    uint8_t data[16];
    int top = 0;
    while (num > 0)
    {
        data[top++] = num % 10;
        num /= 10;
    }
    top -= 1;
    while (top >= 0)
    {
        ce_write_cchar(data[top--] + '0', back, fore);
    }
}

void ce_write_char(char c)
{
    ce_write_cchar(c, RC_BLACK, RC_WHITE);
}
void ce_write_str(char *str)
{
    ce_write_cstr(str, RC_BLACK, RC_WHITE);
}
void ce_write_num(uint32_t num)
{
    ce_write_cnum(num, RC_BLACK, RC_WHITE);
}
