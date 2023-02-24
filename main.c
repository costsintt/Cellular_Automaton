#include "Board.h"
#include "App.h"
#include "Text.h"

#include <stdlib.h>
#include <stdint.h>

#include "kiss_sdl.h"

#define BOARD_DEFAULT_WIDTH 150
#define BOARD_DEFAULT_HEIGHT 150

int main()
{
    struct Board* board = Board_cons(BOARD_DEFAULT_HEIGHT, BOARD_DEFAULT_WIDTH, 0, 1, 0, 1);
    struct Board* buffBoard; 
    Board_fillRandom(board, board->cellMinValue, board->cellMaxValue);

    struct App* app = App_cons(800, 1200, 10, 2);
    app->cam->viewHeightInCells = 100;
    app->cam->viewWidthInCells = 100;


    App_init(app);
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);


    while(App_updateWindow(app))
    {   
        App_takeKeyboardInput(app, board);

        App_clearWindow(app);
        kiss_window_draw(&app->main_window, app->renderer);
        App_drawBoard(app, board);
        kiss_window_draw(&app->buttonsWindow, app->renderer);
        graphWindow_draw(&app->graphWindow, app, 0, 1000, 0, 25000);
        kiss_button_draw(&app->pauseButton, app->renderer);
        kiss_button_draw(&app->loadButton, app->renderer);
        kiss_button_draw(&app->saveButton, app->renderer);
        
        App_swapWindowBuffers(app);

        if(app->copyBoard)
        {
            buffBoard = Board_copy(board);
            app->copyBoard = false;
        }
        else if(app->loadBoard)
        {
            Board_decons(&board);
            board = Board_copy(buffBoard);
            app->loadBoard = false;
        }
        else if(!app->pauseBoardIter)
        {
            app->datas->push(app->datas, Board_countInRange(board, app->lengthOfOneData, 0, 1));
            Board_nextIteration(board);
        }
        
        App_waitIfNeeded(app);
    }
    
   return 0;
}