
#include <stm32f0xx.h>
#include "epl_clock.h"
#include "epl_usart.h"
#include "place_ships.h"
#include "registers.h"
#include <stdbool.h>
#include <stddef.h> 
#include "clock_functions.h"
#include "definitions.h"
//#define DEBUG
#ifdef DBG_MSSG
  #define LOG( msg... ) printf( msg );
#else
  #define LOG( msg... ) ;
#endif
// removed #define functions from here since   smth strangely happens ... 
// now and unexpected line comes out ... idk why
// player already wins aber wenn es so wirklch stimmt bin mir nt sicher


#define COLS 10 
#define ROWS 10 

uint8_t cheat_alert[] = "Enemy cheating!\n";
uint8_t gamefield_mssg[] = "SF0D0000000000\n";

const int ships_size[] = SHIP_SIZE;

int State = 0;
 
int WINS = 0;
int LOSES = 0;

int player = 2;

/**
 * @brief Delays the program execution for a specified amount of time.
 * @param time The amount of time to delay in number of cycles.
 * @return 0 when the delay is completed.
 */

int delay(uint32_t time){
    for(uint32_t i = 0; i < time; i++ ){
        asm("nop"); 
    }
    return 0;
}

uint16_t ADC_Read(void) {

    // a loop runing until the end-of-conversion bit (EOC) in the ADC interrupt status register (ISR) is set
    //  of the ADC1 module . This means that the ADC conversion is complete and the value is available.
// gibt danach diesen zurueck
    while (!(ADC1->ISR & ADC_ISR_EOC));
    return ADC1->DR;
}

void validate_Checksum_And_SetShootPriority(int target_priority[ROWS][COLS], uint8_t enemy_Checksum[12]){
    // validates the checksum received from the enemy. 
    // The checksum is provided in the enemy_Checksum array. 
    // The function calculates the total sum of elements in the checksum array
    //  and compares it with maximum. If the total sum doesn't match the maximum,
    //  it indicates a cheating attempt, and an alert message (cheat_alert) is sent via USART.

    //Seting  the shoot priority for each cell on the target grid (target_priority). 
   
    //The priority is determined by the values in the enemy_Checksum array. 
     //Each element of target_priority is set to the corresponding value from the enemy_Checksum.

    int total_num = 0;
    for (int i = 0; i < ROWS; i++){

        total_num = total_num + enemy_Checksum[i + 2] - Zero4Board;

        for (int j = 0; j < COLS; j++){
            target_priority[j][i] = enemy_Checksum[i + 2] - Zero4Board;
        }
    }
    if(total_num != MAX){

        EPL_usart_write_n_bytes(cheat_alert, sizeof(cheat_alert) -1);
    }
}

void GameFieldMessage(uint8_t board[ROWS][COLS]){
    // for sending the game field message via USART. 
    // constructs the message using the current state of the board array, 
    // which/representing the player's game field. 
    //  message is sent out using EPL_usart_write_n_bytes

    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){

            gamefield_mssg[4 + j] = board[i][j] + Zero4Board; 
            gamefield_mssg[2] = i + Zero4Board;
        }
        EPL_usart_write_n_bytes(gamefield_mssg, sizeof(gamefield_mssg)-1);
    }
}

int main(void){
    
    GPIO_Init();
    TIM2_Init(); 
    TIM2->CR1 |= TIM_CR1_CEN;

    init_user_button();
    init_exti();
    ADC_Init();
    uint16_t adc_value;

    epl_usart_t myusart;
    myusart.baudrate = 9600;
 
    // Configure the system clock to 48MHz
    EPL_SystemClock_Config();
    EPL_init_usart(&myusart);

    uint8_t enemyMatrikelnummer[8];
    uint8_t enemy_Checksum[12];
    uint8_t copyOfrecieved_mssg[2];
    
    uint8_t start_mssg[] = "START2206020\n";
    uint8_t target_mssg[] = "BOOM00\n";
    uint8_t checksum[] = "CS0000000000\n"; 
    

    uint8_t board[ROWS][COLS];
    uint8_t IsThereAlose[ROWS][COLS];
    uint8_t enemy_board[ROWS][COLS];
    uint8_t enemy_gamefield_mssg[ROWS][COLS];

    int lastTargetedColumn = 0;
    int lastTargetedRow = 0;

    int p_row = lastTargetedRow+1;
    int p_col = lastTargetedColumn+1;

    
    int n_row = lastTargetedRow-1;
    int n_col = lastTargetedColumn-1;

    bool HasLost = false;

    int target_priority[ROWS][COLS];

    Boardinitialiser(target_priority, 1);
    Boardinitialiser(board,(uint8_t)Zero4Board);
    Boardinitialiser(IsThereAlose,(uint8_t)'.');
    Boardinitialiser(enemy_board,(uint8_t)'O');
    Boardinitialiser(enemy_gamefield_mssg,(uint8_t)'O');

    int row_counter = 0;


    while (1){

        if (button_was_pressed) {

            player = 1;
            button_was_pressed = false;
            EPL_usart_write_n_bytes(start_mssg, sizeof(start_mssg)- 1);
        }
        adc_value = ADC_Read();

        if(newline_rcvd){
            TIM2->CNT = 0;//timer for delay

            // here data will be copied - damit den Wert nicht ueberschrieben wird, sondern wird eine Kopie erstellt 
            forcopyingelements(data,2,copyOfrecieved_mssg);

            // mit der Kopie wirds weitergearbeitet
            // the state will be set via bit operation to determine the State
            // so wird ein hexadecimal zahl bekommen, welcher als decimal von dem PC gelesen wird
            //  so wird zum jeden State einen Zahl ereignet

            uint16_t which_state = (copyOfrecieved_mssg[0] << 8) | copyOfrecieved_mssg[1];
            
            switch (which_state){
                case START_NACHRICHT:
                    
                    if(player == 1){
                        State = SHOOTPRIORITY; 
                    } //Matrikelnummer saving, starting Beschuss - firing 
                    if(player == 2){
                      State = TRANSMIT4ECKSUM;
                    }  // saving Matrikelnummer , generating GM field, transmiting checksum 
                    break;
                
                case CHECK_SUM:
                    
                    if(player == PLAYER_1){
                        State = CS_GNR_SND;      //Checksum checking, generating GM field
                    }
                    if(player == PLAYER_2){
                        State = START_MSSG_SEND;      //Checksumchecking...
                    }
                    break;

                case BOOM_MSSG:
                    State = SHOOT_MSSG_TRANSMIT;      //Transmit shot report , checking for defeat and Shoot

                    // printf("#BOOM_MESSAGE\n");

                    break;

                case WASSER_M: 
                case TREFFER_M:
                case VICTORY_M:
                
                    State = IFWINNER;      
                break;

                case GAMEFIELD_MSSG:
                    
                    State = GAMEFIELD_EVALAUTE;
                  
                    // printf("#GENERATE_GameField\n");

                break;
            
                default:
                    EPL_usart_write_n_bytes("unexpected line recived", 24);
                    resetBuffer();
                    break;
            }
        }

        switch (State){
       
            case WAIT:
            break;

            case TRANSMIT4ECKSUM: 
                // not writen but generating field
                
                for(int i = 5; i < 13; i++){
                    enemyMatrikelnummer[(i-5)] = data[i];
                    // since the enemy#s matricel is not initialiszed but just defined, we dont have only 0 in the first 5 positions
                    // so here it is how we can easly assign the same number  to the gegner like of the other player.
                    // THE DIFFERENCE will be that for the other player -me , the first 5 symbls are START and for him 0

                }
                
                placeShip(board, ships_size,adc_value);
                copyingfromboard(board, IsThereAlose);
                GET_CHECKSUM(board, &checksum);

                EPL_usart_write_n_bytes(checksum, 13);

                row_counter = 0;  
                HasLost = false;
                Boardinitialiser(enemy_board,(uint8_t)'O');

                TIM2->CNT = 0;//REST TIMER
                State = WAIT;
                resetBuffer();
            break;

            case START_MSSG_SEND:

                forcopyingelements(data,sizeof(enemy_Checksum),enemy_Checksum);
                // the info will be copied from data to the enemychecksum....

                validate_Checksum_And_SetShootPriority(target_priority, enemy_Checksum);
                // checking if enemy cheats with a few words...

                EPL_usart_write_n_bytes(start_mssg, sizeof(start_mssg)- 1);
                // sending start message via USART
                
                TIM2->CNT = 0; 
                // here is a delay....
                State = WAIT;
                resetBuffer();
            break;

            case SHOOT_MSSG_TRANSMIT:{

                int enemy_shot_at_col = (data[4] - Zero4Board );
                // the received data - bzw empfangene Daten werden mit data[4] for and 5 for col gegeben.
                // i.e. werden die beide dementsprechend feur indexen  von row and col verwendet

                int enemy_shot_at_row = (data[5] - Zero4Board);

                if(board[enemy_shot_at_row][enemy_shot_at_col] != 0){
                    // checking for lose when in the respective places an index is vorhanden
                    IsThereAlose[enemy_shot_at_row][enemy_shot_at_col] = 0;
                    // setting to zero  bcs player has lost
                    // /so that the func can return true 
                    if(HasPlayerLost(IsThereAlose) == true){

                        LOSES++;
                        HasLost = true;
                        GameFieldMessage(board);
                        // sharing the board with the enough for where it was shoot

                    }else{
                        EPL_usart_write_n_bytes("T\n", 2);
                        // sending info Treffer
                    }

                }else{

                    EPL_usart_write_n_bytes("W\n", 2);
                    // sending info WASSER
                }

                // habs grade erklaert kein Bock dies zu wiederhole fia pr target

                priority_target(target_priority, &lastTargetedRow, &lastTargetedColumn,adc_value);
                target_priority[lastTargetedRow][lastTargetedColumn] = 0; 
                
                // setting target priorty to 0

                target_mssg[4] = lastTargetedColumn + Zero4Board,
                target_mssg[5] = lastTargetedRow + Zero4Board;

                EPL_usart_write_n_bytes(target_mssg, sizeof(target_mssg)-1);

                TIM2->CNT = 0;
                State = WAIT;
                resetBuffer();
            break;}
        
            case IFWINNER:
                
                // wenn ein Schuss auf ein gegnerisches Spielfeld abgefeuert
                //  wird und entweder ein Treffer  oder Wasser  zurückgegeben wird.
                // wenn treffer Verringere die Priorität der Zellen in der Spalte, in der der Treffer stattgefunden hat, um 1.
               // Wenn die benachbaren Zellen gültig sind, markiere sie als leer 
                // und setze ihre Priorität auf 0.

 
              // Wenn es sich um Wasser handelt:

               // Markiere das Feld des Gegners als leer  und setze die Priorität auf 0.
                // target priority - die Priorität jedes Feldes für den nächsten Schuss des Spielers speichert.

                if(data[0] == TREFFER){
                    enemy_board[lastTargetedRow][lastTargetedColumn] = 'X';

                    for(int i = 0; i < ROWS; i++){
                        if(target_priority[i][lastTargetedColumn] >= 0){
                            target_priority[i][lastTargetedColumn] -= 1;
                        }
                    }

                    if(  (p_row< ROWS) 
                       && (p_col < COLS) ){

                        enemy_board[p_col][p_col] = CELL_EMPTY;
                        target_priority[p_row][p_col] = 0;
                    }
                    if(    (p_row < ROWS) 
                        && (n_col >= 0)){

                        enemy_board[p_row][n_row] = CELL_EMPTY;
                        target_priority[p_row][n_row] = 0;
                    }
                    if( (n_row >= 0 
                      && (p_col < COLS))){

                        enemy_board[n_row][p_col] = CELL_EMPTY;
                        target_priority[n_row][p_col] = 0;
                    }
                    if( (n_row>= 0)
                     && (n_col>= 0)){

                        enemy_board[n_row][n_col] = CELL_EMPTY;
                        target_priority[n_row][n_col] = 0;
                    }

                }else if(data[0] == WASSER){

                    enemy_board[lastTargetedRow][lastTargetedColumn] = CELL_EMPTY;
                    target_priority[lastTargetedRow][lastTargetedColumn] = 0;
                }

                // smth going wrong here ????? schiff.py
                if(hasPlayerWon(enemy_board)){

                    EPL_usart_write_n_bytes("GEWONNEN\n", 9);
                    WINS++;
                    
                }
                

                TIM2->CNT = 0;//timer for delay 
                State = WAIT;
                resetBuffer();
            break;

            case GAMEFIELD_EVALAUTE:     
                
                for(int j = 0; j < COLS; j++){
                    enemy_gamefield_mssg[data[2] - Zero4Board][j] = data[j + 4] - Zero4Board;
                }
                row_counter++;

                if((row_counter == ROWS) 
                    && (HasLost == false)){
                        // if end reached and player won , schicken wir win

                    GameFieldMessage(board);
                    WINS++;
                }

                if((player == PLAYER_1) 
                && (row_counter == ROWS)){// new round if player lost

                    EPL_usart_write_n_bytes(start_mssg, sizeof(start_mssg)- 1);
                    row_counter = 0;
                }
    
                
                TIM2->CNT = 0;
                State = WAIT;
                resetBuffer();
            break;

            case CS_GNR_SND:  

                forcopyingelements(data,sizeof(enemy_Checksum),enemy_Checksum);
                validate_Checksum_And_SetShootPriority(target_priority, enemy_Checksum);
                placeShip(board, ships_size,adc_value);
                copyingfromboard(board, IsThereAlose);
                GET_CHECKSUM(board, &checksum);

                EPL_usart_write_n_bytes(checksum, 13);

                row_counter = 0; 
                HasLost = false; 
                Boardinitialiser(enemy_board,(uint8_t)'O');

                TIM2->CNT = 0;
                State = WAIT;
                resetBuffer();
            break;

            case SHOOTPRIORITY:
                for(int i = 5; i < 13; i++){
                    enemyMatrikelnummer[(i-5)] = data[i];
                    // saving the data of  the matrikel nummer ...getting them through the recieved data
                    // starting from index 5 in the received data since there is where the number o
                    // since in the array we start counting from 0,1,2,3,4 ...
                    // in this case 
                    // and the 5th elementh is the first number of the person´s matricel nummer
                }

                priority_target(target_priority, &lastTargetedRow, &lastTargetedColumn,adc_value);
                target_priority[lastTargetedRow][lastTargetedColumn] = 0; 

                target_mssg[4] = lastTargetedColumn + Zero4Board;
                target_mssg[5] = lastTargetedRow + Zero4Board;

                EPL_usart_write_n_bytes(target_mssg, sizeof(target_mssg)-1);
                
                validate_Checksum_And_SetShootPriority(target_priority, enemy_Checksum);
                // simply moved the glbal definitions to the header file and back and smth happened... no more wins coming out
                //  idk why ...HELP.. na gibts trotzdem

                TIM2->CNT = 0;
                State = WAIT;
                resetBuffer();
            break;

            default:
            break;
        }
    } 
}


