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
#define APP_CAMERA_DEFAULT_VIEWHEIGHTINCELLS 5
#define APP_CAMERA_DEFAULT_VIEWWIDTHINCELLS 5
#define APP_WINDOW_NAME "lololo"

struct App
{
    int screenHeight;
    int screenWidth;

    // float cellHeightInPix; //derived 
    // float cellWidthInPix; //derived
    //depricated; now they evaluated in draw function every frame.

    struct Camera* cam;

    unsigned tickDuration;
    unsigned ticksPassedToTheLatestUpdate; //derived

    SDL_Window* sdlWindow;
    SDL_Renderer* renderer;
    const uint8_t* keyboard;

    kiss_array objects;
    kiss_window main_window;
    kiss_window buttonsWindow;
    kiss_window graphWindow;
    kiss_button pauseButton;
    kiss_button loadButton;
    kiss_button saveButton;

    int draw; //make it bool
    bool pauseBoardIter;
    bool copyBoard;
    bool loadBoard;
};

void graphWindow_draw(kiss_window* window, struct App* app);

struct App* App_cons(size_t screenHeight, size_t screenWidth, unsigned tickDuration);


void App_decons(struct App** app);


void App_init(struct App* self);


bool App_updateWindow(struct App* self);


void App_drawBoard(struct App* self, struct Board* board);


void App_takeKeyboardInput(struct App* self, struct Board* board);


void App_clearWindow(struct App* self);


void App_swapWindowBuffers(struct App* self);


void App_waitIfNeeded(struct App* self);




#endif