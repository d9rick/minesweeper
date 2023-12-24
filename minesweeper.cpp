#include "minesweeper.h"
using namespace std;

Minesweeper::Minesweeper(int h, int w, int bombs){
    //init member variables
    height = h;
    width = w;
    bombCount = bombs;
    
    //seed for rand
    srand(time(NULL));

    //allocate board array
    grid = new int*[height];
    for(int i = 0; i < height; i++){
        grid[i] = new int[w]();
    }
    //allocate explored array
    explored = new int*[height];
    for(int i = 0; i < height; i++){
        explored[i] = new int[w]();
    }
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            explored[i][j] = -1;
        }
    }
    
    //add bombs
    for(int i = 0; i < bombCount; i++){
        int bombRow = rand()%height;
        int bombCol = rand()%width;
        //skip if bomb exists already
        if(grid[bombRow][bombCol] == -1){
            i--;
            continue;
        }
        grid[bombRow][bombCol] = -1;
    }

    /*number tiles*/

    //reference array
    int neighborRows[8] = {0,0,1,1,1,-1,-1,-1};
    int neighborCols[8] = {-1,1,0,-1,1,0,-1,1};

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(grid[i][j] == -1){
                continue;
            }
            int bCount = 0;
            //check neighbors
            for(int k = 0; k < 8; k++){
                if(j+neighborCols[k] < 0 || j+neighborCols[k] >= width){
                    continue;
                }
                if(i+neighborRows[k] < 0 || i+neighborRows[k] >= height){
                    continue;
                }
                if(grid[i+neighborRows[k]][j+neighborCols[k]] == -1){
                    bCount++;
                }
            }
            grid[i][j] = bCount;
        }
    }
}

void Minesweeper::restart(){
    for(int i = 0; i < height; i++){
        delete[] grid[i];
    }
    delete[] grid;

    Minesweeper(height, width, bombCount);
}

Minesweeper::~Minesweeper(){
    for(int i = 0; i < height; i++){
        delete[] grid[i];
    }
    delete[] grid;
}

void Minesweeper::printSol(){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width-1; j++){
            cout << grid[i][j] << " ";
        }
        cout << grid[i][width-1] << endl;
    }
}

/*
Move Explanation:
    -2 = Flag
    -1 = Bomb
    0 = Unseen Tile
    1-8 = Visible with X bombs
*/

void Minesweeper::print(){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            cout << "[";
            Location current = posToLoc(i,j);
            //check if explored
            if(!exploredPos(current)){
                cout << 'X' << "]";
                continue;
            }
            //flag
            if(current.type == -2){
                cout << "F" << "]";
                continue;
            }
            cout << current.type << "]";
        }
        cout << endl;
    }
}

/*
Returns:
0: if square is already explored
1: if move was successful
2: if you chose a bomb
-1: if move is out of bounds
4: if game is won
*/
int Minesweeper::move(int row, int col){
    //check if in bounds
    if(row < 0 || row >= height || col < 0 || col >= width){
        return -1;
    }
    Location loc = posToLoc(row,col);
    //check if already explored
    if(exploredPos(loc)){
        return 0;
    }
    //check if unseen && open tiles
    if(loc.type == 0){
        floodFill(loc);
        if(isOver()){
            return 4;
        }
        return 1;
    }
    //check if is bomb
    if(loc.type == -1){
        return 2;
    }
    //if number tile, just reveal
    explored[row][col] = 1;
    return 1;
}

bool Minesweeper::isOver(){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(grid[i][j] == -1){
                return false;
            }
        }
    }
    return true;
}

int Minesweeper::flag(int row, int col){
    int type = grid[row][col];
    //flag bombs
    if(type == -1){
        grid[row][col] = -2;
        explored[row][col] = 1;
        return 3;
    }
    return -2;
}

/*PRIVATE METHODS*/

Location Minesweeper::posToLoc(int r, int c){
    return Location(r,c,grid[r][c]);
}

//returns true if explored, else false
bool Minesweeper::exploredPos(int r, int c){
    if(explored[r][c] == 1){
        return true;
    }
    return false;
}
//alternate input
bool Minesweeper::exploredPos(Location loc){
    if(explored[loc.row][loc.col] == 1){
        return true;
    }
    return false;
}

/*Uses BFS to clear tiles until there is a bomb found*/
void Minesweeper::floodFill(Location start){
    //init queue
    deque<Location> queue;

    //get and add starting position
    Location startPos = start;
    queue.push_front(startPos);

    //neighbor coordinates
    int neighborRows[8] = {0,0,1,1,1,-1,-1,-1};
    int neighborCols[8] = {-1,1,0,-1,1,0,-1,1};

    //queue searcher
    while(!queue.empty()){
        //get current loc and pop front of queue
        Location current = queue.front();
        queue.pop_front();

        //check if bomb or flag or if its been seen before
        if(current.type == -1 || current.type == -2 || exploredPos(current)){
            //dont explore or get neighbors
            continue;
        }

        //reveal open squares
        if(current.type == 0){
            //explore & get neighbors
            explored[current.row][current.col] = 1;
        }

        //check neighbors
        for(int i = 0; i < 8; i++){
            //get neighbor coords
            int nc = current.col + neighborCols[i];
            int nr = current.row + neighborRows[i];

            //check if in bounds
            if(nc < 0 || nc >= width){
                continue;
            }
            if(nr < 0 || nr >= height){
                continue;
            }
            int nt = grid[nr][nc];
            cout << "Checking [" << nr << ", " << nc << "]. " << "Type is: "  << nt << endl;
            //check if explored
            if(explored[nr][nc] != -1){
                continue;
            }
            //if numTile just set to visible
            if(nt > 0 && nt < 9){
                explored[nr][nc] = 1;
                continue;
            }
            //if not a 0 skip
            if(nt != 0){
                continue;
            }
            cout << "Adding [" << nr << ", " << nc << "] to queue. " << "Type is: "  << nt << endl;
            queue.push_back(Location(nr, nc, nt));
        }
    }
}