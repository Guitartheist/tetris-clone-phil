#include "Block.h"
#include "Piece.h"

//Random number between 0 and maximum
int roll(int maximum)
{
    return (rand()%maximum);
}

PieceList *allPieces = 0;
PieceList *nextPiece = 0;
int numberOfPieces = 0;
int nextPiecePosition = 0;

int getPiece(int piece)
{
    PieceList *findPiece;
    int i;
    if (piece+1>=numberOfPieces)
        rollPieceSet();
    findPiece = allPieces;
    for (i=0;i<piece;i++)
        findPiece=findPiece->next;
    return findPiece->val;
}

void rollPieceSet()
{
    int i;
    Uint8 pieceSet[7];

    pieceSet[0]=roll(7);
    pieceSet[1]=roll(7);

    while (pieceSet[1]==pieceSet[0])
        pieceSet[1]=roll(7);

    pieceSet[2]=roll(7);

    while (pieceSet[2]==pieceSet[0]||pieceSet[2]==pieceSet[1])
        pieceSet[2]=roll(7);

    pieceSet[3]=roll(7);

    while (pieceSet[3]==pieceSet[0]||pieceSet[3]==pieceSet[1]||pieceSet[3]==pieceSet[2])
        pieceSet[3]=roll(7);

    pieceSet[4]=roll(7);

    while (pieceSet[4]==pieceSet[0]||pieceSet[4]==pieceSet[1]||pieceSet[4]==pieceSet[2]||pieceSet[4]==pieceSet[3])
        pieceSet[4]=roll(7);

    pieceSet[5]=roll(7);

    while (pieceSet[5]==pieceSet[0]||pieceSet[5]==pieceSet[1]||pieceSet[5]==pieceSet[2]||pieceSet[5]==pieceSet[3]||pieceSet[5]==pieceSet[4])
        pieceSet[5]=roll(7);

    pieceSet[6]=roll(7);

    while (pieceSet[6]==pieceSet[0]||pieceSet[6]==pieceSet[1]||pieceSet[6]==pieceSet[2]||pieceSet[6]==pieceSet[3]||pieceSet[6]==pieceSet[4]||pieceSet[6]==pieceSet[5])
        pieceSet[6]=roll(7);

    if (allPieces==0)
    {
        allPieces=(PieceList*)malloc(sizeof(PieceList));
        allPieces->val=pieceSet[0];
        allPieces->next=0;
        numberOfPieces++;

        nextPiece=(PieceList*)malloc(sizeof(PieceList));
        allPieces->next=nextPiece;
        nextPiece->val=pieceSet[1];
        numberOfPieces++;

        for (i=2; i<7; i++)
        {
            nextPiece->next=(PieceList*)malloc(sizeof(PieceList));
            nextPiece=nextPiece->next;
            nextPiece->val=pieceSet[i];
            nextPiece->next=0;
            numberOfPieces++;
        }
    }

    else
    {
        for (i=0; i<7; i++)
        {
            nextPiece->next=(PieceList*)malloc(sizeof(PieceList));
            nextPiece=nextPiece->next;
            nextPiece->val=pieceSet[i];
            nextPiece->next=0;
            numberOfPieces++;
        }
    }
}

void spawnPiece(Piece* piece, enum PieceShape type)
{
    piece->x=(GRIDXSIZE/2)*BLOCKSIZE;
    piece->y=BLOCKSIZE;
    piece->direction=UP;
    piece->type=type;
}

void createPiece(Piece* piece, enum PieceShape type, int x, int y)
{
    piece->x=x;
    piece->y=y;
    piece->direction=UP;
    piece->type=type;
}

void rotatePieceRight(Piece* piece, Grid* grid)
{
    int i;
    Piece collisionTest;
    Uint8 coords[8];
    collisionTest.x=piece->x;
    collisionTest.y=piece->y;
    collisionTest.type=piece->type;
    switch (piece->direction)
    {
    case UP:
        collisionTest.direction=RIGHT;
        break;
    case RIGHT:
        collisionTest.direction=DOWN;
        break;
    case DOWN:
        collisionTest.direction=LEFT;
        break;
    case LEFT:
        collisionTest.direction=UP;
        break;
    }
    getPieceCollision(&collisionTest,coords);
    for (i=0; i<7; i+=2)
    {
        if (coords[i]>=GRIDXSIZE)
            return;
        if (coords[i+1]>=GRIDYSIZE)
            return;
        if (coords[i]<0||coords[i+1]<0)
            return;
        if (grid->grid[coords[i]][coords[i+1]].position.x>=0)
            return;
    }
    switch (piece->direction)
    {
    case UP:
        piece->direction=RIGHT;
        break;
    case RIGHT:
        piece->direction=DOWN;
        break;
    case DOWN:
        piece->direction=LEFT;
        break;
    case LEFT:
        piece->direction=UP;
        break;
    }
}

void rotatePieceLeft(Piece* piece, Grid* grid)
{
    int i;
    Piece collisionTest;
    Uint8 coords[8];
    collisionTest.x=piece->x;
    collisionTest.y=piece->y;
    collisionTest.type=piece->type;
    switch (piece->direction)
    {
    case UP:
        collisionTest.direction=LEFT;
        break;
    case RIGHT:
        collisionTest.direction=UP;
        break;
    case DOWN:
        collisionTest.direction=RIGHT;
        break;
    case LEFT:
        collisionTest.direction=DOWN;
        break;
    }
    getPieceCollision(&collisionTest,coords);
    for (i=0; i<7; i+=2)
    {
        if (coords[i]>=GRIDXSIZE)
            return;
        if (coords[i+1]>=GRIDYSIZE)
            return;
        if (coords[i]<0||coords[i+1]<0)
            return;
        if (grid->grid[coords[i]][coords[i+1]].position.x>=0)
            return;
    }
    switch (piece->direction)
    {
    case UP:
        piece->direction=LEFT;
        break;
    case RIGHT:
        piece->direction=UP;
        break;
    case DOWN:
        piece->direction=RIGHT;
        break;
    case LEFT:
        piece->direction=DOWN;
        break;
    }
}

void movePieceRight(Piece* piece, Grid* grid)
{
    int i;
    Piece collisionTest;
    Uint8 coords[8];
    collisionTest.x=piece->x+BLOCKSIZE;
    collisionTest.y=piece->y;
    collisionTest.type=piece->type;
    collisionTest.direction=piece->direction;
    getPieceCollision(&collisionTest,coords);
    for (i=0; i<7; i+=2)
    {
        if (coords[i]>=GRIDXSIZE)
            return;
        if (coords[i+1]>=GRIDYSIZE)
            return;
        if (coords[i]<0||coords[i+1]<0)
            return;
        if (grid->grid[coords[i]][coords[i+1]].position.x>=0)
            return;
    }
    piece->x=collisionTest.x;
}

void movePieceLeft(Piece* piece, Grid* grid)
{
    int i;
    Piece collisionTest;
    Uint8 coords[8];
    collisionTest.x=piece->x-BLOCKSIZE;
    collisionTest.y=piece->y;
    collisionTest.type=piece->type;
    collisionTest.direction=piece->direction;
    getPieceCollision(&collisionTest,coords);
    for (i=0; i<7; i+=2)
    {
        if (coords[i]>=GRIDXSIZE)
            return;
        if (coords[i+1]>=GRIDYSIZE)
            return;
        if (coords[i]<0||coords[i+1]<0)
            return;
        if (grid->grid[coords[i]][coords[i+1]].position.x>=0)
            return;
    }
    piece->x=collisionTest.x;
}

int movePieceDown(Piece* piece, Grid* grid)
{
    int i;
    Piece collisionTest;
    Uint8 coords[8];
    collisionTest.x=piece->x;
    collisionTest.y=piece->y+BLOCKSIZE;
    collisionTest.type=piece->type;
    collisionTest.direction=piece->direction;
    getPieceCollision(&collisionTest,coords);
    for (i=0; i<7; i+=2)
    {
        if (coords[i]>=GRIDXSIZE)
            return 0;
        if (coords[i+1]>=GRIDYSIZE)
            return 0;
        if (coords[i]<0)
            return 0;
        if (grid->grid[coords[i]][coords[i+1]].position.x>=0)
            return 0;
    }
    piece->y=collisionTest.y;
    return 1;
}

int dropPiece(Piece* piece, Grid* grid,SDL_Surface *screen,int *score,int level)
{
    Block block[4];
    Uint8 coords[8];
    enum Color dropColor;

    //2 points per cell for a hard drop
    while (movePieceDown(piece,grid))*score+=2;

    //Write blocks into grid
    getPieceCollision(piece,coords);

    switch (piece->type)
    {
    case Z:
        dropColor=RED;
        break;
    case L:
        dropColor=BLUE;
        break;
    case I:
        dropColor=CYAN;
        break;
    case O:
        dropColor=YELLOW;
        break;
    case J:
        dropColor=ORANGE;
        break;
    case T:
        dropColor=PURPLE;
        break;
    default:
    case S:
        dropColor=GREEN;
        break;
    }

    setBlock(&block[0],coords[0]*BLOCKSIZE,coords[1]*BLOCKSIZE,screen->format,dropColor);
    setBlock(&block[1],coords[2]*BLOCKSIZE,coords[3]*BLOCKSIZE,screen->format,dropColor);
    setBlock(&block[2],coords[4]*BLOCKSIZE,coords[5]*BLOCKSIZE,screen->format,dropColor);
    setBlock(&block[3],coords[6]*BLOCKSIZE,coords[7]*BLOCKSIZE,screen->format,dropColor);

    grid->grid[coords[0]][coords[1]]=block[0];
    grid->grid[coords[2]][coords[3]]=block[1];
    grid->grid[coords[4]][coords[5]]=block[2];
    grid->grid[coords[6]][coords[7]]=block[3];

    //SCORING
    int lines;

    level++;

    switch (lines=scoreLines(grid))
    {
    case 1:
        *score+=100*level;
        break;
    case 2:
        *score+=300*level;
        break;
    case 3:
        *score+=500*level;
        break;
    case 4:
        *score+=800*level;
        break;
    }

    if (piece->y/BLOCKSIZE>=2)
        return lines;
    else
        return -1;
}

void drawPiece(Piece* const piece, SDL_Surface* screen)
{
    Block a,b,c,d;
    switch (piece->type)
    {
    case Z:
        switch (piece->direction)
        {
        case UP:
            setBlock(&a,piece->x-BLOCKSIZE,piece->y-BLOCKSIZE,screen->format,RED);
            setBlock(&b,piece->x,piece->y-BLOCKSIZE,screen->format,RED);
            setBlock(&c,piece->x,piece->y,screen->format,RED);
            setBlock(&d,piece->x+BLOCKSIZE,piece->y,screen->format,RED);
            break;
        case RIGHT:
            setBlock(&a,piece->x+BLOCKSIZE,piece->y-BLOCKSIZE,screen->format,RED);
            setBlock(&b,piece->x,piece->y,screen->format,RED);
            setBlock(&c,piece->x+BLOCKSIZE,piece->y,screen->format,RED);
            setBlock(&d,piece->x,piece->y+BLOCKSIZE,screen->format,RED);
            break;
        case DOWN:
            setBlock(&a,piece->x-BLOCKSIZE,piece->y,screen->format,RED);
            setBlock(&b,piece->x,piece->y,screen->format,RED);
            setBlock(&c,piece->x,piece->y+BLOCKSIZE,screen->format,RED);
            setBlock(&d,piece->x+BLOCKSIZE,piece->y+BLOCKSIZE,screen->format,RED);
            break;
        case LEFT:
            setBlock(&a,piece->x,piece->y-BLOCKSIZE,screen->format,RED);
            setBlock(&b,piece->x-BLOCKSIZE,piece->y,screen->format,RED);
            setBlock(&c,piece->x,piece->y,screen->format,RED);
            setBlock(&d,piece->x-BLOCKSIZE,piece->y+BLOCKSIZE,screen->format,RED);
            break;
        }
        break;
    case L:
        switch (piece->direction)
        {
        case UP:
            setBlock(&a,piece->x,piece->y-BLOCKSIZE,screen->format,BLUE);
            setBlock(&b,piece->x,piece->y,screen->format,BLUE);
            setBlock(&c,piece->x,piece->y+BLOCKSIZE,screen->format,BLUE);
            setBlock(&d,piece->x+BLOCKSIZE,piece->y+BLOCKSIZE,screen->format,BLUE);
            break;
        case RIGHT:
            setBlock(&a,piece->x-BLOCKSIZE,piece->y,screen->format,BLUE);
            setBlock(&b,piece->x,piece->y,screen->format,BLUE);
            setBlock(&c,piece->x+BLOCKSIZE,piece->y,screen->format,BLUE);
            setBlock(&d,piece->x-BLOCKSIZE,piece->y+BLOCKSIZE,screen->format,BLUE);
            break;
        case DOWN:
            setBlock(&a,piece->x-BLOCKSIZE,piece->y-BLOCKSIZE,screen->format,BLUE);
            setBlock(&b,piece->x,piece->y-BLOCKSIZE,screen->format,BLUE);
            setBlock(&c,piece->x,piece->y,screen->format,BLUE);
            setBlock(&d,piece->x,piece->y+BLOCKSIZE,screen->format,BLUE);
            break;
        case LEFT:
            setBlock(&a,piece->x+BLOCKSIZE,piece->y-BLOCKSIZE,screen->format,BLUE);
            setBlock(&b,piece->x-BLOCKSIZE,piece->y,screen->format,BLUE);
            setBlock(&c,piece->x,piece->y,screen->format,BLUE);
            setBlock(&d,piece->x+BLOCKSIZE,piece->y,screen->format,BLUE);
            break;
        }
        break;
    case I:
        switch (piece->direction)
        {
        case UP:
            setBlock(&a,piece->x-BLOCKSIZE*2,piece->y-BLOCKSIZE,screen->format,CYAN);
            setBlock(&b,piece->x-BLOCKSIZE,piece->y-BLOCKSIZE,screen->format,CYAN);
            setBlock(&c,piece->x,piece->y-BLOCKSIZE,screen->format,CYAN);
            setBlock(&d,piece->x+BLOCKSIZE,piece->y-BLOCKSIZE,screen->format,CYAN);
            break;
        case RIGHT:
            setBlock(&a,piece->x,piece->y-BLOCKSIZE*2,screen->format,CYAN);
            setBlock(&b,piece->x,piece->y-BLOCKSIZE,screen->format,CYAN);
            setBlock(&c,piece->x,piece->y,screen->format,CYAN);
            setBlock(&d,piece->x,piece->y+BLOCKSIZE,screen->format,CYAN);
            break;
        case DOWN:
            setBlock(&a,piece->x-BLOCKSIZE*2,piece->y,screen->format,CYAN);
            setBlock(&b,piece->x-BLOCKSIZE,piece->y,screen->format,CYAN);
            setBlock(&c,piece->x,piece->y,screen->format,CYAN);
            setBlock(&d,piece->x+BLOCKSIZE,piece->y,screen->format,CYAN);
            break;
        case LEFT:
            setBlock(&a,piece->x-BLOCKSIZE,piece->y-BLOCKSIZE*2,screen->format,CYAN);
            setBlock(&b,piece->x-BLOCKSIZE,piece->y-BLOCKSIZE,screen->format,CYAN);
            setBlock(&c,piece->x-BLOCKSIZE,piece->y,screen->format,CYAN);
            setBlock(&d,piece->x-BLOCKSIZE,piece->y+BLOCKSIZE,screen->format,CYAN);
            break;
        }
        break;
    case T:
        switch (piece->direction)
        {
        case UP:
            setBlock(&a,piece->x,piece->y-BLOCKSIZE,screen->format,PURPLE);
            setBlock(&b,piece->x-BLOCKSIZE,piece->y,screen->format,PURPLE);
            setBlock(&c,piece->x,piece->y,screen->format,PURPLE);
            setBlock(&d,piece->x+BLOCKSIZE,piece->y,screen->format,PURPLE);
            break;
        case RIGHT:
            setBlock(&a,piece->x,piece->y-BLOCKSIZE,screen->format,PURPLE);
            setBlock(&b,piece->x,piece->y+BLOCKSIZE,screen->format,PURPLE);
            setBlock(&c,piece->x,piece->y,screen->format,PURPLE);
            setBlock(&d,piece->x+BLOCKSIZE,piece->y,screen->format,PURPLE);
            break;
        case DOWN:
            setBlock(&a,piece->x,piece->y+BLOCKSIZE,screen->format,PURPLE);
            setBlock(&b,piece->x-BLOCKSIZE,piece->y,screen->format,PURPLE);
            setBlock(&c,piece->x,piece->y,screen->format,PURPLE);
            setBlock(&d,piece->x+BLOCKSIZE,piece->y,screen->format,PURPLE);
            break;
        case LEFT:
            setBlock(&a,piece->x,piece->y-BLOCKSIZE,screen->format,PURPLE);
            setBlock(&b,piece->x,piece->y+BLOCKSIZE,screen->format,PURPLE);
            setBlock(&c,piece->x,piece->y,screen->format,PURPLE);
            setBlock(&d,piece->x-BLOCKSIZE,piece->y,screen->format,PURPLE);
            break;
        }
        break;
    case O:
        setBlock(&a,piece->x,piece->y-BLOCKSIZE,screen->format,YELLOW);
        setBlock(&b,piece->x-BLOCKSIZE,piece->y-BLOCKSIZE,screen->format,YELLOW);
        setBlock(&c,piece->x,piece->y,screen->format,YELLOW);
        setBlock(&d,piece->x-BLOCKSIZE,piece->y,screen->format,YELLOW);
        break;
    case J:
        switch (piece->direction)
        {
        case UP:
            setBlock(&a,piece->x,piece->y-BLOCKSIZE,screen->format,ORANGE);
            setBlock(&b,piece->x,piece->y+BLOCKSIZE,screen->format,ORANGE);
            setBlock(&c,piece->x,piece->y,screen->format,ORANGE);
            setBlock(&d,piece->x-BLOCKSIZE,piece->y+BLOCKSIZE,screen->format,ORANGE);
            break;
        case RIGHT:
            setBlock(&a,piece->x+BLOCKSIZE,piece->y,screen->format,ORANGE);
            setBlock(&b,piece->x-BLOCKSIZE,piece->y,screen->format,ORANGE);
            setBlock(&c,piece->x,piece->y,screen->format,ORANGE);
            setBlock(&d,piece->x-BLOCKSIZE,piece->y-BLOCKSIZE,screen->format,ORANGE);
            break;
        case DOWN:
            setBlock(&a,piece->x,piece->y-BLOCKSIZE,screen->format,ORANGE);
            setBlock(&b,piece->x,piece->y+BLOCKSIZE,screen->format,ORANGE);
            setBlock(&c,piece->x,piece->y,screen->format,ORANGE);
            setBlock(&d,piece->x+BLOCKSIZE,piece->y-BLOCKSIZE,screen->format,ORANGE);
            break;
        case LEFT:
            setBlock(&a,piece->x+BLOCKSIZE,piece->y,screen->format,ORANGE);
            setBlock(&b,piece->x-BLOCKSIZE,piece->y,screen->format,ORANGE);
            setBlock(&c,piece->x,piece->y,screen->format,ORANGE);
            setBlock(&d,piece->x+BLOCKSIZE,piece->y+BLOCKSIZE,screen->format,ORANGE);
            break;
        }
        break;
    case S:
        switch (piece->direction)
        {
        case UP:
            setBlock(&a,piece->x,piece->y-BLOCKSIZE,screen->format,GREEN);
            setBlock(&b,piece->x-BLOCKSIZE,piece->y,screen->format,GREEN);
            setBlock(&c,piece->x,piece->y,screen->format,GREEN);
            setBlock(&d,piece->x+BLOCKSIZE,piece->y-BLOCKSIZE,screen->format,GREEN);
            break;
        case RIGHT:
            setBlock(&a,piece->x,piece->y-BLOCKSIZE,screen->format,GREEN);
            setBlock(&b,piece->x+BLOCKSIZE,piece->y,screen->format,GREEN);
            setBlock(&c,piece->x,piece->y,screen->format,GREEN);
            setBlock(&d,piece->x+BLOCKSIZE,piece->y+BLOCKSIZE,screen->format,GREEN);
            break;
        case DOWN:
            setBlock(&a,piece->x,piece->y,screen->format,GREEN);
            setBlock(&b,piece->x-BLOCKSIZE,piece->y+BLOCKSIZE,screen->format,GREEN);
            setBlock(&c,piece->x,piece->y+BLOCKSIZE,screen->format,GREEN);
            setBlock(&d,piece->x+BLOCKSIZE,piece->y,screen->format,GREEN);
            break;
        case LEFT:
            setBlock(&a,piece->x-BLOCKSIZE,piece->y,screen->format,GREEN);
            setBlock(&b,piece->x,piece->y+BLOCKSIZE,screen->format,GREEN);
            setBlock(&c,piece->x,piece->y,screen->format,GREEN);
            setBlock(&d,piece->x-BLOCKSIZE,piece->y-BLOCKSIZE,screen->format,GREEN);
            break;
        }
        break;
    }
    drawBlock(&a,screen);
    drawBlock(&b,screen);
    drawBlock(&c,screen);
    drawBlock(&d,screen);
}

////////////////////////////////////////////////////////////////////////////////
//  Function: getPieceCollision
//
//  Description: Get collision information regarding a particular Piece
//
//  Parameters:
//      piece : The tetromino to measure
//      coordinates : An array to hold the x y coordinates this tetromino occupies
////////////////////////////////////////////////////////////////////////////////
void getPieceCollision(Piece* const piece, Uint8 coordinates[8])
{
    switch (piece->type)
    {
    case Z:
        switch (piece->direction)
        {
        case UP:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE-1;
            coordinates[3]=piece->y/BLOCKSIZE-1;
            coordinates[4]=piece->x/BLOCKSIZE;
            coordinates[5]=piece->y/BLOCKSIZE-1;
            coordinates[6]=piece->x/BLOCKSIZE+1;
            coordinates[7]=piece->y/BLOCKSIZE;
            break;
        case RIGHT:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE;
            coordinates[3]=piece->y/BLOCKSIZE+1;
            coordinates[4]=piece->x/BLOCKSIZE+1;
            coordinates[5]=piece->y/BLOCKSIZE-1;
            coordinates[6]=piece->x/BLOCKSIZE+1;
            coordinates[7]=piece->y/BLOCKSIZE;
            break;
        case DOWN:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE-1;
            coordinates[3]=piece->y/BLOCKSIZE;
            coordinates[4]=piece->x/BLOCKSIZE+1;
            coordinates[5]=piece->y/BLOCKSIZE+1;
            coordinates[6]=piece->x/BLOCKSIZE;
            coordinates[7]=piece->y/BLOCKSIZE+1;
            break;
        case LEFT:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE;
            coordinates[3]=piece->y/BLOCKSIZE-1;
            coordinates[4]=piece->x/BLOCKSIZE-1;
            coordinates[5]=piece->y/BLOCKSIZE;
            coordinates[6]=piece->x/BLOCKSIZE-1;
            coordinates[7]=piece->y/BLOCKSIZE+1;
            break;
        }
        break;
    case L:
        switch (piece->direction)
        {
        case UP:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE;
            coordinates[3]=piece->y/BLOCKSIZE-1;
            coordinates[4]=piece->x/BLOCKSIZE;
            coordinates[5]=piece->y/BLOCKSIZE+1;
            coordinates[6]=piece->x/BLOCKSIZE+1;
            coordinates[7]=piece->y/BLOCKSIZE+1;
            break;
        case RIGHT:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE-1;
            coordinates[3]=piece->y/BLOCKSIZE+1;
            coordinates[4]=piece->x/BLOCKSIZE-1;
            coordinates[5]=piece->y/BLOCKSIZE;
            coordinates[6]=piece->x/BLOCKSIZE+1;
            coordinates[7]=piece->y/BLOCKSIZE;
            break;
        case DOWN:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE-1;
            coordinates[3]=piece->y/BLOCKSIZE-1;
            coordinates[4]=piece->x/BLOCKSIZE;
            coordinates[5]=piece->y/BLOCKSIZE-1;
            coordinates[6]=piece->x/BLOCKSIZE;
            coordinates[7]=piece->y/BLOCKSIZE+1;
            break;
        case LEFT:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE-1;
            coordinates[3]=piece->y/BLOCKSIZE;
            coordinates[4]=piece->x/BLOCKSIZE+1;
            coordinates[5]=piece->y/BLOCKSIZE;
            coordinates[6]=piece->x/BLOCKSIZE+1;
            coordinates[7]=piece->y/BLOCKSIZE-1;
            break;
        }
        break;
    case I:
        switch (piece->direction)
        {
        case UP:
            coordinates[0]=piece->x/BLOCKSIZE-2;
            coordinates[1]=piece->y/BLOCKSIZE-1;
            coordinates[2]=piece->x/BLOCKSIZE-1;
            coordinates[3]=piece->y/BLOCKSIZE-1;
            coordinates[4]=piece->x/BLOCKSIZE;
            coordinates[5]=piece->y/BLOCKSIZE-1;
            coordinates[6]=piece->x/BLOCKSIZE+1;
            coordinates[7]=piece->y/BLOCKSIZE-1;
            break;
        case RIGHT:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE-2;
            coordinates[2]=piece->x/BLOCKSIZE;
            coordinates[3]=piece->y/BLOCKSIZE-1;
            coordinates[4]=piece->x/BLOCKSIZE;
            coordinates[5]=piece->y/BLOCKSIZE;
            coordinates[6]=piece->x/BLOCKSIZE;
            coordinates[7]=piece->y/BLOCKSIZE+1;
            break;
        case DOWN:
            coordinates[0]=piece->x/BLOCKSIZE-2;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE-1;
            coordinates[3]=piece->y/BLOCKSIZE;
            coordinates[4]=piece->x/BLOCKSIZE;
            coordinates[5]=piece->y/BLOCKSIZE;
            coordinates[6]=piece->x/BLOCKSIZE+1;
            coordinates[7]=piece->y/BLOCKSIZE;
            break;
        case LEFT:
            coordinates[0]=piece->x/BLOCKSIZE-1;
            coordinates[1]=piece->y/BLOCKSIZE-2;
            coordinates[2]=piece->x/BLOCKSIZE-1;
            coordinates[3]=piece->y/BLOCKSIZE-1;
            coordinates[4]=piece->x/BLOCKSIZE-1;
            coordinates[5]=piece->y/BLOCKSIZE;
            coordinates[6]=piece->x/BLOCKSIZE-1;
            coordinates[7]=piece->y/BLOCKSIZE+1;
            break;
        }
        break;
    case T:
        switch (piece->direction)
        {
        case UP:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE;
            coordinates[3]=piece->y/BLOCKSIZE-1;
            coordinates[4]=piece->x/BLOCKSIZE-1;
            coordinates[5]=piece->y/BLOCKSIZE;
            coordinates[6]=piece->x/BLOCKSIZE+1;
            coordinates[7]=piece->y/BLOCKSIZE;
            break;
        case RIGHT:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE;
            coordinates[3]=piece->y/BLOCKSIZE-1;
            coordinates[4]=piece->x/BLOCKSIZE;
            coordinates[5]=piece->y/BLOCKSIZE+1;
            coordinates[6]=piece->x/BLOCKSIZE+1;
            coordinates[7]=piece->y/BLOCKSIZE;
            break;
        case DOWN:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE;
            coordinates[3]=piece->y/BLOCKSIZE+1;
            coordinates[4]=piece->x/BLOCKSIZE-1;
            coordinates[5]=piece->y/BLOCKSIZE;
            coordinates[6]=piece->x/BLOCKSIZE+1;
            coordinates[7]=piece->y/BLOCKSIZE;
            break;
        case LEFT:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE;
            coordinates[3]=piece->y/BLOCKSIZE-1;
            coordinates[4]=piece->x/BLOCKSIZE;
            coordinates[5]=piece->y/BLOCKSIZE+1;
            coordinates[6]=piece->x/BLOCKSIZE-1;
            coordinates[7]=piece->y/BLOCKSIZE;
            break;
        }
        break;
    case O:
        coordinates[0]=piece->x/BLOCKSIZE;
        coordinates[1]=piece->y/BLOCKSIZE;
        coordinates[2]=piece->x/BLOCKSIZE-1;
        coordinates[3]=piece->y/BLOCKSIZE;
        coordinates[4]=piece->x/BLOCKSIZE;
        coordinates[5]=piece->y/BLOCKSIZE-1;
        coordinates[6]=piece->x/BLOCKSIZE-1;
        coordinates[7]=piece->y/BLOCKSIZE-1;
        break;
    case J:
        switch (piece->direction)
        {
        case UP:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE;
            coordinates[3]=piece->y/BLOCKSIZE-1;
            coordinates[4]=piece->x/BLOCKSIZE;
            coordinates[5]=piece->y/BLOCKSIZE+1;
            coordinates[6]=piece->x/BLOCKSIZE-1;
            coordinates[7]=piece->y/BLOCKSIZE+1;
            break;
        case RIGHT:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE-1;
            coordinates[3]=piece->y/BLOCKSIZE;
            coordinates[4]=piece->x/BLOCKSIZE+1;
            coordinates[5]=piece->y/BLOCKSIZE;
            coordinates[6]=piece->x/BLOCKSIZE-1;
            coordinates[7]=piece->y/BLOCKSIZE-1;
            break;
        case DOWN:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE;
            coordinates[3]=piece->y/BLOCKSIZE-1;
            coordinates[4]=piece->x/BLOCKSIZE;
            coordinates[5]=piece->y/BLOCKSIZE+1;
            coordinates[6]=piece->x/BLOCKSIZE+1;
            coordinates[7]=piece->y/BLOCKSIZE-1;
            break;
        case LEFT:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE-1;
            coordinates[3]=piece->y/BLOCKSIZE;
            coordinates[4]=piece->x/BLOCKSIZE+1;
            coordinates[5]=piece->y/BLOCKSIZE;
            coordinates[6]=piece->x/BLOCKSIZE+1;
            coordinates[7]=piece->y/BLOCKSIZE+1;
            break;
        }
        break;
    case S:
        switch (piece->direction)
        {
        case UP:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE-1;
            coordinates[3]=piece->y/BLOCKSIZE;
            coordinates[4]=piece->x/BLOCKSIZE;
            coordinates[5]=piece->y/BLOCKSIZE-1;
            coordinates[6]=piece->x/BLOCKSIZE+1;
            coordinates[7]=piece->y/BLOCKSIZE-1;
            break;
        case RIGHT:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE;
            coordinates[3]=piece->y/BLOCKSIZE-1;
            coordinates[4]=piece->x/BLOCKSIZE+1;
            coordinates[5]=piece->y/BLOCKSIZE;
            coordinates[6]=piece->x/BLOCKSIZE+1;
            coordinates[7]=piece->y/BLOCKSIZE+1;
            break;
        case DOWN:
            coordinates[0]=piece->x/BLOCKSIZE;
            coordinates[1]=piece->y/BLOCKSIZE+1;
            coordinates[2]=piece->x/BLOCKSIZE-1;
            coordinates[3]=piece->y/BLOCKSIZE+1;
            coordinates[4]=piece->x/BLOCKSIZE;
            coordinates[5]=piece->y/BLOCKSIZE;
            coordinates[6]=piece->x/BLOCKSIZE+1;
            coordinates[7]=piece->y/BLOCKSIZE;
            break;
        case LEFT:
            coordinates[0]=piece->x/BLOCKSIZE-1;
            coordinates[1]=piece->y/BLOCKSIZE;
            coordinates[2]=piece->x/BLOCKSIZE-1;
            coordinates[3]=piece->y/BLOCKSIZE-1;
            coordinates[4]=piece->x/BLOCKSIZE;
            coordinates[5]=piece->y/BLOCKSIZE;
            coordinates[6]=piece->x/BLOCKSIZE;
            coordinates[7]=piece->y/BLOCKSIZE+1;
            break;
        }
        break;
    }
}


