#include "Board.h"
#include "App.h"

#include <stdlib.h>

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

    unsigned char* keyboard = SDL_GetKeyboardState(NULL);

    while(App_updateWindow(app))
    {
        SDL_SetRenderDrawColor(app->renderer, 0, 50, 10, 0);
        SDL_RenderClear(app->renderer);
        SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);

        unsigned camVel = 1;
        if(keyboard[SDL_SCANCODE_LSHIFT]) camVel = 8;
        if(keyboard[SDL_SCANCODE_W] && app->cam->y >= camVel) app->cam->y -= camVel;
        if(keyboard[SDL_SCANCODE_S]) app->cam->y += camVel;
        if(keyboard[SDL_SCANCODE_A] && app->cam->x >= camVel) app->cam->x -= camVel;
        if(keyboard[SDL_SCANCODE_D]) app->cam->x += camVel;
        

        App_drawBoard(app, board);
        Board_nextIteration(board);
        SDL_RenderPresent(app->renderer);

        unsigned ticksPassed = SDL_GetTicks64() - app->ticksPassedToTheLatestUpdate;
        SDL_Delay(ticksPassed < app->tickDuration ? app->tickDuration - ticksPassed : 0);
    }
   return 0;
}