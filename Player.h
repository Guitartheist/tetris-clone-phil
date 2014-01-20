#include <SDL/SDL.h>
#include "Grid.h"
#include "Piece.h"
#include "Controller.h"

#ifndef PLAYER_H
#define PLAYER_H

typedef struct Player
{
    int score;
    int lines;
    int pieces;//number of pieces dropped so far
    Piece active;
    Piece held;
    Grid grid;
    Uint8 swapped; //set to 1 when hold is used for the first time
    Uint8 swappable; //set to 0 when hold is used, 1 when hold piece is locked into the grid

    // fast drop (is user holding moveDown key?)
    int fastDrop;

    // fast slide (is user holding down a lateral motion key?)
    //-1 moves left, 1 moves right
    //-2 moves right when left is released
    //2 moves left when right is released
    int fastSlide;

    //used to calibrate fall speed
    int ticks;
    //used to calibrate slide duration
    int slideTime;

    //used in time tracking game modes
    int startTime;
    int totalTime;

    Controller controller;
} Player;

void drawGame(Player,SDL_Surface*);
void initPlayer(Player*);

#endif // PLAYER_H