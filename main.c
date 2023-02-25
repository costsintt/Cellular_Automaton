#include "Board.h"
#include "App.h"
#include "Text.h"
#include "gridManipul.h"

#include <stdlib.h>
#include <stdint.h>

#include "kiss_sdl.h"

#define BOARD_DEFAULT_WIDTH 150
#define BOARD_DEFAULT_HEIGHT 150

int main()
{
    struct Board* board = Board_cons(25, 25, 0, 1, 0, 1);
    struct Board* buffBoard; 
    board->grid[10][8].type = 1;
    board->grid[10][9].type = 1;
    board->grid[10][10].type = 1;
    board->grid[9][8].type = 1;
    board->grid[8][9].type = 1;
    board->grid[15][15].type = 2;
    board->grid[1][1].type = 2;
    board->grid[2][2].type = 3;
    board->grid[23][23].type = 1;

    struct App* app = App_cons(800, 1200, 50, 2);
    app->cam->viewHeightInCells = 25;
    app->cam->viewWidthInCells = 25;


    App_init(app);
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);
    app->pauseBoardIter = true;

    while(App_updateWindow(app))
    {   
        App_takeKeyboardInput(app, board);
        App_clearWindow(app);
        kiss_window_draw(&app->main_window, app->renderer);
        App_drawBoard(app->main_window, app, board);
        kiss_window_draw(&app->buttonsWindow, app->renderer);
        graphWindow_draw(&app->graphWindow, app, 0, 1000, 0, board->shape[0] * board->shape[1]);
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
            app->datas->push(app->datas,
                             Board_countInRange(board, app->lengthOfOneData,
                                                board->cellMinValue, board->cellMaxValue));
            board->nextIteration(board);
        }
        
        App_waitIfNeeded(app);
    }
    
   return 0;
}