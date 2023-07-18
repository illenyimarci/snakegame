/*
 * glcd.h
 *
 *  Created on: Jun 1, 2023
 *      Author: illen
 */

#ifndef INC_GLCD_H_
#define INC_GLCD_H_

#define GLCD_ON (0x3F)
#define GLCD_OFF (0x3E)
#define GLCD_START_LINE (0xC0)
#define GLCD_SET_Y (0x40)
#define GLCD_SET_X (0xB8)

void GLCD_Init(void);
void GLCD_Write(char cs_s,char d_i,char g_data);
void GLCD_Clear(void);
void GLCD_WriteBlock(char m_data,char cX,char cY);
void GLCD_DrawSnake(uint8_t frontPg, uint8_t backPg, uint16_t xcoord1, uint16_t xcoord2);
void GLCD_EraseBlock(uint8_t Page, uint16_t xcoord1, uint16_t xcoord2);
void GLCD_DrawBlock(uint8_t Pg, uint16_t coord1, uint16_t coord2);
void GLCD_WriteString(const char* string,char Y, char X);
void GLCD_Write_Char(char cPlace,char cX,char cY);
void GLCD_DataWrite(char g_data);

#endif /* INC_GLCD_H_ */
