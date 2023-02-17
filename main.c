#include "Board.h"
#include "App.h"
#include "Text.h"

#include <stdlib.h>
#include <stdint.h>

#include "kiss_sdl.h"

int main()
{

    struct Board* board = Board_cons(150, 150, 0, 1);
    Board_fillRandom(board, 0, 2);
    board->grid[0][0] = 0;
    board->grid[board->shape[0] - 1][board->shape[1] - 1] = 0;

    struct App* app = App_cons(500, 800, 100);
    app->cam->x = 20;
    app->cam->y = 20;
    app->cam->viewHeight = 130;
    app->cam->viewWidth = 130;



    App_init(app);
    (app->main_window).visible = 1;
    (app->main_window).bg = kiss_green;
    while(App_updateWindow(app))
    {   
        App_takeKeyboardInput(app);

        App_clearWindow(app);
        kiss_window_draw(&(app->main_window), app->renderer);
        kiss_button_draw(&(app->main_pauseButton), app->renderer);
        App_drawBoard(app, board);
        App_swapWindowBuffers(app);

        if(!app->pauseGame) Board_nextIteration(board);
        
        App_waitIfNeeded(app);
    }
    
   return 0;
}