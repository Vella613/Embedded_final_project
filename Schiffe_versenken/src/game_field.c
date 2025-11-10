// #include "game_field.h"
// #include "epl_clock.h"
// #include "epl_usart.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// #include <stdbool.h>

// // 

// // void GameFieldMessage(uint8_t board[ROWS][COLS]){

// //     for(int i = 0; i < ROWS; i++){
// //         for(int j = 0; j < COLS; j++){

// //             gamefield_mssg[4 + j] = board[i][j] + '0'; 
// //             gamefield_mssg[2] = i + '0';
// //         }
// //         EPL_usart_write_n_bytes(gamefield_mssg, sizeof(gamefield_mssg)-1);
// //     }
// // }
// bool hasPlayerWon(uint8_t enemy_board[ROWS][COLS]){
//     int boom_counter = 0;

//     for(int i = 0; i < ROWS; i++){
//         for(int j = 0; j < COLS; j++){

//             if(enemy_board[i][j] == 'X'){
//                 boom_counter++;
//             }
//         }
//     }
//     if(boom_counter == 30){
//         return true;
//     }else{
//         return false;
//     }
// }
