#include <iostream>
#include "minesweeper.h"

using namespace std;

int main(){
    int height, width, bombs;
    cout << "Choose grid height: ";
    cin >> height;
    cout << "Choose grid width: ";
    cin >> width;
    cout << "How many bombs? ";
    cin >> bombs;

    char playerInput = 'A';
    Minesweeper game(height, width, bombs);

    while(playerInput != 'q'){
        //tracks move validity
        int moveResult = 0;

        //print boards
        //cout << "Solution Board:" << endl;
        //game.printSol();
        cout << "Game Board:" << endl;
        game.print();

        //get move type
        cout << "Enter a move (1 - Explore, 2 - Flag, q - quit): ";
        cin >> playerInput;

        if(playerInput == 'q'){
            break;
        }

        //get move position
        int row, col;
        cout << "Where? (Row Col): ";
        cin >> row >> col;
        //make move
        if(playerInput == '1'){
            moveResult = game.move(row, col);
        }
        //flag
        if(playerInput == '2'){
            moveResult = game.flag(row, col);
        }
        if(moveResult == -3){
            cout << "Error idk" << endl;
            continue;
        }
        if(moveResult == -2){
            cout << "Sorry, but that is not a bomb!" << endl;
            continue;
        }
        if(moveResult == -1){
            cout << "Error: Out of Bounds!" << endl;
            continue;
        }
        if(moveResult == 0){
            cout << "Already Visited. Try Again." << endl;
            continue;
        }
        if(moveResult == 1){
            cout << "Great Success!" << endl;
            continue;
        }
        if(moveResult == 2){
            cout << "KABOOM! You Lost!" << endl;
            string answer = "";
            while(answer != "y" && answer != "n"){
                cout << "Want to play again? (Y/N)" << endl;
                cin >> answer;
            }
            if(answer == "N"){
                break;
            }
            else{
                game.restart();
                continue;
            }
        }
        if(moveResult == 3){
            cout << "Flag Placed." << endl;
            continue;
        }
        if(moveResult == 4){
            cout << "You win!" << endl;
            continue;
        }
    }
   return 0;
}
