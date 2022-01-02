#ifndef __CONSOLE_SETTINGS_H__
#define __CONSOLE_SETTINGS_H__
#include <windows.h>
void set_color(UINT uFore,UINT uBack);
void set_title(LPCSTR lpTitle);
void full_screen();
void set_pos(int x, int y);
void set_posC(COORD a);
void hide_scroll_bar();
//void window_minimize(); //Œ¥∆Ù”√
void hide_console_cursor(); 
void set_font_size(int x,int y);
#endif
