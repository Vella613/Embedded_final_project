#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define START_NACHRICHT      ('S' << 8) | 'T'
#define CHECK_SUM            ('C' << 8) | 'S'
#define BOOM_MSSG            ('B' << 8) | 'O'
#define WASSER_M             ('W' << 8) | '\n'
#define TREFFER_M            ('T' << 8) | '\n'
#define VICTORY_M            ('V' << 8) | '\n'
#define GAMEFIELD_MSSG       ('S' << 8) | 'F'

//  Die nachrichten werden an der copyingelement uebergeben... die Daten werden Durch COM3 erhalten und ueber den USART bekommen
//  dann werden diese in einem Register RDR gespiechert und das was in der Received Data Register gespeichert wurde wird in saving_read_symbol gespcheirt


// #define START_NACHRICHT 1
// #define CHECK_SUM        2  
// #define BOOM_MSSG         3 
// #define SHOOT_MSSG_1       4
// #define SHOOT_MSSG_2        5
// #define SHOOT_MSSG_3         6
// #define GAMEFIELD_MSSG      7

#define Zero4Board           '0'
#define WASSER               'W'
#define TREFFER              'T'
#define CELL_EMPTY           'N'

#define WAIT                 0
#define TRANSMIT4ECKSUM      1
#define START_MSSG_SEND      2
#define SHOOT_MSSG_TRANSMIT  3
#define IFWINNER             4
#define GAMEFIELD_EVALAUTE   5
#define CS_GNR_SND           6
#define SHOOTPRIORITY        7

#define PLAYER_1             1
#define PLAYER_2             2

#define MAX                  30

#endif 