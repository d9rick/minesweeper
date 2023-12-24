#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string>
#include <deque>
#include <vector>

//helper struct
struct Location{
    int row;
    int col;
    int type;
    Location(int r, int c){
        row = r;
        col = c;
    }
    Location(int r, int c, int t){
        row = r;
        col = c;
        type = t;
    }
};

//main game object
class Minesweeper{

public:

    //creates board
    Minesweeper(int h, int w, int numBombs);

    //deallocates grids
    ~Minesweeper();

    //print the solved board
    void printSol();

    //print normal board
    void print();

        /*
        Returns:
        0: if square is already explored
        1: if move was successful
        2: if you chose a bomb
        -1: if move is out of bounds
        */
    int move(int row, int col);

    /*
    3: Good Flag
    -2: Not bomb
    */
    int flag(int row, int col);

    //restarts game
    void restart();

    friend class MineSolver;
    
private:
    //converts position to type location
    Location posToLoc(int r, int c);

    //explores open tiles via bfs
    void floodFill(Location start);

    //returns whether position has been uncovered yet
    bool exploredPos(int r, int c);
    bool exploredPos(Location loc);

    //returns true if won, false else
    bool isOver();

    //holds all game tiles
    int** grid;

    //holds whether each point has been visited
    int** explored;

    //rows in grid
    int height;

    //cols in grid
    int width;

    //number of bombs
    int bombCount;
};