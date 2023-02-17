#ifndef APP
#define APP

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <SDL.h>

#include "kiss_sdl.h"
#include "Camera.h"
#include "Board.h"


#define APP_CAMERA_DEFAULT_X 0
#define APP_CAMERA_DEFAULT_Y 0
#define APP_CAMERA_DEFAULT_VIEWHEIGHT 25
#define APP_CAMERA_DEFAULT_VIEWWIDTH 40
#define APP_WINDOW_NAME "lololo"
#define APP_WINDOW_DEFAULT_X 150
#define APP_WINDOW_DEFAULT_Y 150


struct App
{
    size_t screenHeight;
    size_t screenWidth;

    float cellHeight; //derived
    float cellWidth; //derived

    struct Camera* cam;

    unsigned tickDuration;
    unsigned ticksPassedToTheLatestUpdate; //derived

    SDL_Window* sdlWindow;
    SDL_Renderer* renderer;
    const uint8_t* keyboard;

    kiss_array objects;
    kiss_window main_window;
    kiss_button main_pauseButton;

    int draw;
    bool pauseGame;
};


struct App* App_cons(size_t screenHeight, size_t screenWidth, unsigned tickDuration);


void App_decons(struct App** app);


void App_init(struct App* self);


bool App_updateWindow(struct App* self);


void App_drawBoard(struct App* self, struct Board* board);


void App_takeKeyboardInput(struct App* self);


void App_clearWindow(struct App* self);


void App_swapWindowBuffers(struct App* self);


void App_waitIfNeeded(struct App* self);




#endif