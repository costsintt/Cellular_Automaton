#ifndef APP
#define APP

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <SDL.h>

#include "kiss_sdl.h"
#include "Camera.h"
#include "Board.h"
#include "eLists.h"

enum DIRECTIONS {DIR_UPWARD, DIR_DOWNWARD, DIR_RIGHTWARD, DIR_LEFTWARD};

struct App
{
    int screenHeight;
    int screenWidth;

    struct Camera* cam;

    unsigned tickDuration;
    unsigned ticksPassedToTheLatestUpdate; //derived

    struct sList* datas; //derived
    uint8_t lengthOfOneData;
    
    char* name;

    //below visual shits
    SDL_Window* sdlWindow; //derived
    SDL_Renderer* renderer; //derived
    const uint8_t* keyboard; //derived

    kiss_array objects;
    kiss_window main_window;
    kiss_window test_window;
    kiss_window buttonsWindow;
    kiss_window graphWindow;
    kiss_button pauseButton;
    kiss_button loadButton;
    kiss_button saveButton;
    kiss_button nextIterButton;

    int draw; //make it bool
    bool pauseBoardIter;
    bool copyBoard;
    bool loadBoard;
    bool goToNextIter;
};

void graphWindow_draw(kiss_window* window, struct App* app,
                      uint64_t fromX, uint64_t toX, uint64_t fromY, uint64_t toY);

struct App* App_cons(size_t screenHeight, size_t screenWidth, unsigned tickDuration,
                     uint8_t lengthOfOneData, size_t camViewHeightInCells,
                     size_t camViewWidthInCells, size_t camXInCells, size_t camYInCells,
                     char* appName);


void App_decons(struct App** app);


void App_initDrawingThings(struct App* self);


bool App_updateWindow(struct App* self);


void App_drawBoard(kiss_window* window, struct App* self, struct Board* board);


void App_updateCamera(struct App* self, struct Board* board);


void App_clearWindow(struct App* self);


void App_swapWindowBuffers(struct App* self);


void App_waitIfNeeded(struct App* self);


bool App_willCameraClip(struct App* self, struct Board* board,
                        size_t vel, uint8_t dir);


#endif