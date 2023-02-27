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
    size_t boardHeight = 25;
    size_t boardWidth = 25;

    uint8_t cellMinValue = 0;
    uint8_t cellMaxValue = 2;
    uint8_t dataArrayLength = 3;

    uint32_t screenHeight = 800;
    uint32_t screenWidth = 1200;
    uint32_t msecondsInTick = 50;
    

    struct Board* board = Board_cons(boardHeight, boardWidth, cellMinValue, cellMaxValue, 0, 0);
    struct Board* buffBoard;


    // Board_fillRandom(board, 12,
    //                  (struct Cell){1,0,0,0}, (struct Cell){2,0,0,0}, (struct Cell){1,0,0,0},
    //                  (struct Cell){0,0,0,0}, (struct Cell){0,0,0,0}, (struct Cell){0,0,0,0},
    //                  (struct Cell){0,0,0,0}, (struct Cell){0,0,0,0}, (struct Cell){0,0,0,0},
    //                  (struct Cell){0,0,0,0}, (struct Cell){0,0,0,0}, (struct Cell){0,0,0,0}
    //                 );
    board->grid[0][0] = (struct Cell){1,0,10,0};

    struct App* app = App_cons(screenHeight, screenWidth, msecondsInTick, dataArrayLength,
                               board->shape[0], board->shape[1], 0, 0);


    while(App_updateWindow(app))
    {   
        App_takeKeyboardInput(app, board);
        App_clearWindow(app);
        kiss_window_draw(&app->main_window, app->renderer);
        App_drawBoard(&app->test_window, app, board);
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