#ifndef TEXT
#define TEXT

#include <SDL_ttf.h>


#include "App.h"


extern void Text_init(struct App* app);

extern void Text_quit();



extern void drawText(struct App* app, char *text, int x, int y, int r, int g, int b, int fontType,
                     int align, int maxWidth);

#endif