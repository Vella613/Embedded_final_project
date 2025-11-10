#ifndef PLACE_SHIPS_H
#define PLACE_SHIPS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <stdbool.h>

#define COLS 10
#define ROWS 10
#define NUM_SHIPS 10

#define SHIP_SIZE {5, 4, 4, 3, 3, 3, 2, 2, 2, 2}

#define HORIZONTAL 0
#define VERTICAL   1

#define MAX_ATTEMPTS 50
#define BOOM_MAX 30
#define SIZEOF5SHIP  5

int isPlacementInBounderies(uint8_t board[ROWS][COLS], int row, int col, int length, char direction);
int numberOfShips(uint8_t board[ROWS][COLS], int col, uint8_t ship);


void Boardinitialiser(uint8_t board[ROWS][COLS], uint8_t insert_element);
void forcopyingelements(uint8_t *element_copying_from, size_t length, uint8_t *element_copying_in) ;
void copyingfromboard(uint8_t to_be_copied_from[ROWS][COLS], uint8_t to_be_copied_in[ROWS][COLS]);

int radndGenerator_col(int sizeofShips);
int radndGenerator_row(int sizeofShips);

int Generate_rnd_rows();
int Generate_rnd_cols();

int rand2num();

void placeShip(uint8_t board[ROWS][COLS], const int shipLengths[NUM_SHIPS], uint16_t rand_value);
void GET_CHECKSUM(uint8_t board[ROWS][COLS], uint8_t *checksum);
void priority_target(int array[ROWS][COLS], int *max_row, int *max_col, int rand_value);

bool AreShipsWellPositioned(uint8_t board[ROWS][COLS]);
bool hasPlayerWon(uint8_t enemy_board[ROWS][COLS]);

uint8_t HasPlayerLost(uint8_t BOARD_IF_PLAYER_LOST[ROWS][COLS]);


#endif 
