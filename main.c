#include "Board.h"
#include "App.h"
#include "Text.h"
#include "gridManipul.h"

#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "kiss_sdl.h"

#define BOARD_DEFAULT_WIDTH 150
#define BOARD_DEFAULT_HEIGHT 150


int main()
{
    srand(time(NULL));
    size_t boardHeight = 150;
    size_t boardWidth = 150;

    uint8_t cellMinValue = 0;
    uint8_t cellMaxValue = 2;
    uint8_t dataArrayLength = 3;

    uint32_t screenHeight = 800;
    uint32_t screenWidth = 1200;
    uint32_t msecondsInTick = 1;
    

    struct Board* board = Board_cons(boardHeight, boardWidth, cellMinValue, cellMaxValue, 0, 0);
    struct Board* buffBoard;


    Board_fillRandom(board, 10,
                     (struct Cell){1,0,0,0},
                     (struct Cell){0,0,0,0}, (struct Cell){0,0,0,0}, (struct Cell){0,0,0,0},
                     (struct Cell){0,0,0,0}, (struct Cell){0,0,0,0}, (struct Cell){0,0,0,0},
                     (struct Cell){0,0,0,0}, (struct Cell){0,0,0,0}, (struct Cell){0,0,0,0}
                    );
    board->grid[75][75] = (struct Cell){2,0,100,0};
    // board->grid[15][15] = (struct Cell){1,0,10,0};
    // board->grid[20][20] = (struct Cell){2,0,200,0};
    // board->grid[21][21] = (struct Cell){1,0,10,0};
    // board->grid[18][18] = (struct Cell){1,0,10,0};
    // board->grid[25][25] = (struct Cell){1,0,10,0};

    struct App* app = App_cons(screenHeight, screenWidth, msecondsInTick, dataArrayLength,
                               board->shape[0], board->shape[1], 0, 0, "appName3000");


    while(App_updateWindow(app))
    {   
        App_takeKeyboardInput(app, board);
        App_clearWindow(app);
        App_drawBoard(&app->main_window, app, board);
        kiss_window_draw(&app->buttonsWindow, app->renderer);
        graphWindow_draw(&app->graphWindow, app, 0, 4000, 0, board->shape[0] * board->shape[1]);
        kiss_button_draw(&app->pauseButton, app->renderer);
        kiss_button_draw(&app->loadButton, app->renderer);
        kiss_button_draw(&app->saveButton, app->renderer);
        kiss_button_draw(&app->nextIterButton, app->renderer);
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
        else if(!app->pauseBoardIter || app->goToNextIter)
        {
            app->datas->push(app->datas,
                             Board_countInRange(board, app->lengthOfOneData,
                                                board->cellMinValue, board->cellMaxValue));
            board->nextIteration(board);
            app->goToNextIter = false;
        }
        
        App_waitIfNeeded(app);
    }
    
   return 0;
}