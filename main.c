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
    struct Board* board = Board_cons(BOARD_DEFAULT_HEIGHT, BOARD_DEFAULT_WIDTH, 0, 1);
    Board_fillRandom(board, 0, 2);

    struct App* app = App_cons(500, 700, 100);
    app->cam->viewHeightInCells = 100;
    app->cam->viewWidthInCells = 100;



    App_init(app);
    app->main_window.base.visible = 1;
    app->buttonsWindow.base.visible = 1;
    app->main_window.bg = kiss_green;
    
    while(App_updateWindow(app))
    {   
        App_takeKeyboardInput(app, board);

        App_clearWindow(app);
        kiss_window_draw(&app->main_window, app->renderer);
        kiss_window_draw(&app->buttonsWindow, app->renderer);
        kiss_button_draw(&app->main_pauseButton, app->renderer);
        App_drawBoard(app, board);
        App_swapWindowBuffers(app);

        if(!app->pauseBoardIter) Board_nextIteration(board);
        
        App_waitIfNeeded(app);
    }
    
   return 0;
}