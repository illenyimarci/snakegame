
#ifndef INC_GAME_H_
#define INC_GAME_H_

#include "main.h"

#define onSnake (xrand2 >= snake[i].x1 && xrand2 <= snake[i].x2+7)

void IDLE_State();
void gameStart();
void gameOver();
void Food(uint8_t fpg, uint8_t lpg, uint8_t x1, uint8_t x2, uint8_t x4);
void incScore(uint8_t fp, uint8_t xcf1, uint8_t xcf2);
void checkCoords(uint8_t* coord1, uint8_t* coord2);
#endif /* INC_GAME_H_ */
