#include "Board.h"
#include "App.h"

#include <stdlib.h>
#include <stdint.h>

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

    while(App_updateWindow(app))
    {   
        App_takeKeyboardInput(app);

        App_clearWindow(app);
        App_drawBoard(app, board);
        App_swapWindowBuffers(app);

        Board_nextIteration(board);
        
        App_waitIfNeeded(app);
    }
    
   return 0;
}