
#include "place_ships.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>



int isPlacementInBounderies(uint8_t board[ROWS][COLS], int row, int col, int length, char direction) {

    int sizeofcolumn = col + length;
    int sizeofrow    = row + length;

    if (direction == HORIZONTAL) { 

        if (sizeofcolumn> COLS){
            return false;
        } // if ship exceeding boundaries or not
            

        for (int i = col; i < sizeofcolumn; i++) {

            if (board[row][i] != 0 ||
                (row > 0 && board[row - 1][i] != 0) ||
                (row < ROWS - 1 && board[row + 1][i] != 0) || (i > 0 && board[row][i - 1] != 0) ||
                (i < COLS - 1 && board[row][i + 1] != 0) ||  ((row > 0 && i > 0) && board[row - 1][i - 1] != 0) || 
                ((row > 0 && i < COLS - 1) && board[row - 1][i + 1] != 0) ||  ((row < ROWS - 1 && i > 0) && board[row + 1][i - 1] != 0) || 
                ((row < ROWS - 1 && i < COLS - 1) && board[row + 1][i + 1] != 0)) {

                    return false; 
                }

        }
    } else { 
        // same here
        if (sizeofrow > ROWS) { 
                return false;
        }
            

        for (int i = row; i < sizeofrow; i++) {

            if (board[i][col] != 0 || (col > 0 && board[i][col - 1] != 0) ||
                (col < COLS - 1 && board[i][col + 1] != 0) ||  (i > 0 && board[i - 1][col] != 0) || 
                (i < ROWS - 1 && board[i + 1][col] != 0) ||  ((i > 0 && col > 0) && board[i - 1][col - 1] != 0) || 
                ((i > 0 && col < COLS - 1) && board[i - 1][col + 1] != 0) || ((i < ROWS - 1 && col > 0) && board[i + 1][col - 1] != 0) || 
                ((i < ROWS - 1 && col < COLS - 1) && board[i + 1][col + 1] != 0)) {

                    return false; // a ship either neighboring ship, or  corners ?

                }
        }
    }

    return true;
}

int numberOfShips(uint8_t board[ROWS][COLS], int col, uint8_t ship) {

    int hasShip = 0;

    for(int i = 0; i < ROWS; i++) {
                
                // iterating the each row of the field 
        if (board[i][col] != 0) {
            hasShip++; 
        }
    }
    // the purpose is to make sure that there is no 9 length of ships in total or, just one full cell or nothing ,
    // since then placement of ships as a whole in the field is not gonna work!!!
    if (hasShip == ship) {
        return true;

    }else{
        return false;
    }
    
}

void forcopyingelements(uint8_t *element_copying_from, size_t length, uint8_t *element_copying_in) {

    for (size_t i = 0; i < length; i++) {

        element_copying_in[i] = element_copying_from[i];
    }
} 

void copyingfromboard(uint8_t to_be_copied_from[ROWS][COLS], uint8_t to_be_copied_in[ROWS][COLS]){

    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){

            to_be_copied_in[i][j] = to_be_copied_from[i][j];
        }
    }
}
int radndGenerator_col(int sizeofShips){
    int rnd_val_col = rand() % (COLS - sizeofShips + 1);

    return rnd_val_col;

}
int radndGenerator_row(int sizeofShips){
    int rnd_val_row = rand() % (ROWS - sizeofShips + 1);

    return rnd_val_row;
}
int Generate_rnd_rows(){

    return rand() % ROWS;
    
}
int Generate_rnd_cols(){

    return rand() % COLS;

}
int rand2num(){

    return rand() % 2;
}
void placeShip(uint8_t board[ROWS][COLS], const int sizeofAship[NUM_SHIPS], uint16_t rand_value) {
       int allShipsPlaced = 0;

        srand((unsigned int)rand_value);

        while (!allShipsPlaced) {
            Boardinitialiser(board, 0); // making  board empty

            for (int i = 0; i < NUM_SHIPS; i++) {
                int sizeofShips = sizeofAship[i];

                char direction;
                int row, col;
                int sizeofcolumn = col + sizeofShips;
                int sizeofrow = row;

                if (sizeofShips == 5) { // Placing the 5 cells long ship parallel to the board edge
                    // Determine direction
                    if (rand2num() == 0) {
                        direction = HORIZONTAL;
                    } else {
                        direction = VERTICAL;
                    }

                    // Determine starting position based on direction
                    if (direction == HORIZONTAL) {
                        if (rand2num() == 0) {
                            row = 0;
                        } else {
                            row = ROWS - 1;
                        }
                        col = radndGenerator_col(sizeofShips); 

                    } else {
                        row = radndGenerator_row(sizeofShips);

                        if (rand2num() == 0) {
                            col = 0;
                        } else {
                            col = COLS - 1;
                        }
                    }
                } else { // For other ship lengths
                    // Determine direction
                    if (rand2num()== 0) {
                        direction = HORIZONTAL;
                    } else {
                        direction = VERTICAL;
                    }

                    // Determine random starting position
                    row = Generate_rnd_rows;
                    col = Generate_rnd_cols;
                }

                // Validate placement
                int attempts = 0;

                while (!isPlacementInBounderies(board, row, col, sizeofShips, direction)
                                                    && (attempts < MAX_ATTEMPTS)) {
                                                        
                    // Re-determining direction
                    if (rand2num == 0) {
                        direction = HORIZONTAL;
                    } else {
                        direction = VERTICAL;
                    }

                    if (sizeofShips == SIZEOF5SHIP) {
                        if (direction == HORIZONTAL) {
                            if (rand2num == 0) {
                                row = 0;
                            } else {
                                row = ROWS - 1;
                            }
                            col = radndGenerator_col(sizeofShips);

                        } else {
                            row = radndGenerator_row(sizeofShips);

                            if (rand2num == 0) {
                                col = 0;
                            } else {
                                col = COLS - 1;
                            }
                        }
                    } else {
                        row = Generate_rnd_rows();
                        col = Generate_rnd_cols();
                    }
                    attempts++;
                }

                // If 50 attempts reached, reset and try again
                if (attempts == MAX_ATTEMPTS) {

                    allShipsPlaced = 0;
                    break;
                }

                // Place the ship on the board
                if (direction == HORIZONTAL) {
                    for (int j = col; j < col + sizeofShips; j++) {
                        board[row][j] = sizeofShips;
                    }
                } else { // Vertical placement
                    for (int j = row; j < row + sizeofShips; j++) {
                        board[j][col] = sizeofShips;
                    }
                }
        }

        allShipsPlaced = 1; // for successful placement initially


        for (int col = 0; col < COLS; col++) { // if a column is empty, or has only 1 or 9 ships
            
            // if so then ships are not properly placed !!!! so another try!!!
            if (numberOfShips(board, col, 0) || numberOfShips(board, col, 1) || numberOfShips(board, col, 9)) {

                allShipsPlaced = 0;
                break;
            }
        }
    }
}

void Boardinitialiser(uint8_t board[ROWS][COLS], uint8_t inserting_element) {

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {

            board[i][j] = inserting_element;
        }
    }
}

void GET_CHECKSUM(uint8_t board[ROWS][COLS], uint8_t *checksum) {
    for(int i = 0; i < COLS; i++) {

        int counter4ships = 0;

        for(int j = 0; j < ROWS; j++) {
            if(board[j][i] != 0){
                counter4ships++;
            }
        }

        checksum[i + 2] = counter4ships + '0'; 
        // starting from the second element bcs  0 and 1st are CS ....
        // then addid the number of ships and 0 at the end of the string 
    }
}


void priority_target(int board[ROWS][COLS], int *rowIndex, int *colIndex, int rnd_val) {

    int max_val = board[0][0];
    int cntr = 0;
    int arrayOfIndexes[ROWS * COLS][2];

    // iterating to find the greatest number

    
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {

            if (board[row][col] > max_val) {
                // the first max value will be taken from the first two elements of the matrics starting from 0
                // then the counter will be set to 0
                // the array of the index 
                max_val = board[row][col];
                
               // Reseting the counter to 0 because a new highest value was found

                cntr = 0;

               // Storing the row and column indices of the current cell in the array at position 0

                arrayOfIndexes[cntr][0] = row;
                arrayOfIndexes[cntr][1] = col;

                // for storing the row and column indices of the cells that have the highest value.
                //  in the 2D array in the x is going to be stored the index of the highest value, 
                // whereby it is done by appropriating the number of the row
                
                cntr++;
                // in case values match after incrementation, the new indexes will be saved on the next X coordinate of the matrix
            } else if (board[row][col] == max_val) {
                
                // here we check if values match bzw. are the same
                // this iterating erfolgt ueber den counter - cntr 
                // here a new row and column will be set...accordingly the new index coordinates in x and y will change.
                //  the counter - cntr will increase 

                 // If the current cell's value equals the current maximum value, store its indices


                arrayOfIndexes[cntr][0] = row;
                arrayOfIndexes[cntr][1] = col;

               // Increment the counter as we've added another cell with the max value to our list

                cntr++;
            }
        }
    }


    srand(rnd_val);
    int rnd_indx = rand() % cntr;

    // picking randomly cell among those with the highest value and
    //  sets the target coordinates to that cell.

    *rowIndex = arrayOfIndexes[rnd_indx][0];
    *colIndex = arrayOfIndexes[rnd_indx][1];

}


bool hasPlayerWon(uint8_t enemy_board[ROWS][COLS]){
    int boom_counter = 0;
// here iterating to find all the X and announce a BOOM if such
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){

            if(enemy_board[i][j] == 'X'){
                boom_counter++;
            }
        }
    }
    if(boom_counter == BOOM_MAX){
        return true;
    }else{
        return false;
    }
}

uint8_t HasPlayerLost(uint8_t BOARD_IF_PLAYER_LOST[ROWS][COLS]){

    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){

            if(BOARD_IF_PLAYER_LOST[i][j] != 0){

                return false;
            }
        }
    }
    return true;
}

