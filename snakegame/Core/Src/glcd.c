
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"

#include "main.h"
#include "glcd.h"
#include "font.h"
#include "pwm.h"
#include "delay.h"

extern enum direction currdirect;

void GLCD_Init(void)
{
	pwmInit(); 													//háttérvilágítás beállítás
	// Minden alapállapotba
	HAL_GPIO_WritePin(RW_GPIO_Port, RW_Pin, GPIO_PIN_RESET);	// GLCD_RW = 0
	HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_SET);		// GLCD_E = 1
	HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);	// GLCD_RESET = 0 (alacsony aktív)
	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);	// #GLCDEN = 0



	// Init GLCD functions
	HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_RESET);		// GLCD_E = 0
	HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);	// GLCD_RESET = 1

	GLCD_Write(3,0,GLCD_ON); 			//GLCD bekapcsolása
	GLCD_Write(3,0,GLCD_START_LINE);	//GLCD Start Line
	GLCD_Clear();						//Kijelzõ törlése
}

void GLCD_Clear(void)
{
	char x,y;
	for(x=0;x<8;x++)
	{
		GLCD_Write(3,0,0x40);
		GLCD_Write(3,0,(0xB8|x));
		for(y=0;y<64;y++)
		{
			GLCD_Write(3,1,0x00);
		}
	}
}


void GLCD_Write(char cs_s,char d_i,char g_data)
{
	//Kijelző vezérlő kiválasztás
	switch(cs_s)
	{
	case 1:
		HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(CS2_GPIO_Port, CS2_Pin, GPIO_PIN_RESET);
		break;
	case 3:
		HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CS2_GPIO_Port, CS2_Pin, GPIO_PIN_RESET);
		break;
	}
	//Adat/Utasítás beállítása
	switch(d_i)
	{
	case 0:
		HAL_GPIO_WritePin(DI_GPIO_Port, DI_Pin, GPIO_PIN_RESET);	//instruction
		break;
	case 1:
		HAL_GPIO_WritePin(DI_GPIO_Port, DI_Pin, GPIO_PIN_SET); 		//data
		break;
	}

	//Adat kirakása a Portra
	GLCD_DataWrite(g_data);
	//Adat kiadása a kijelzőnek
	delay_us(1);

	HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_SET); //E felfutó él
	delay_us(1);

	HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_RESET); //E lefutó él
	delay_us(1);
	//CSS vissza
	HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS2_GPIO_Port, CS2_Pin, GPIO_PIN_SET);
}

void GLCD_WriteBlock(char m_data,char cPage,char cX)
{
	char chip=1;
	if(cX>=64)
	{
		chip=2;
		cX-=64;
	}
	GLCD_Write(chip,0,(0x40|cX));
	GLCD_Write(chip,0,(0xB8|cPage));
	GLCD_Write(chip,1,m_data);
}

/*kígyó megjelenítésének megvalósítása*/
void GLCD_DrawSnake(uint8_t frontPg, uint8_t backPg, uint16_t xcoord1, uint16_t xcoord2)
{
	for(uint8_t page = backPg; page <= frontPg; page++)
	{
		for(uint16_t x = xcoord2; x <= xcoord1; x++)
		{
			GLCD_WriteBlock(0xFE, page, x);
		}
	}
}

/*egyetlen blokkot 2 koordináta között kitöltő függvény*/
void GLCD_DrawBlock(uint8_t Pg, uint16_t coord1, uint16_t coord2)
{
	if(coord1 > coord2)
	{
		for(uint16_t c = coord2; c <= coord1; c++)
		{
			if(c == 128)
			{
				c = 0;
			}
			GLCD_WriteBlock(0xFE, Pg, c);
		}
	}
	else if(coord2 > coord1)
	{
		for(uint16_t c = coord1; c <= coord2; c++)
		{
			if(c == 128)
			{
				c = 0;
			}
			GLCD_WriteBlock(0xFE, Pg, c);
		}
	}
}

/*adott page-en két x koordináta közt levő pixelek törlése*/
void GLCD_EraseBlock(uint8_t Page, uint16_t coord1, uint16_t coord2)
{
	if(coord1 > coord2)
		{
			for(uint16_t c = coord2; c <= coord1; c++)
			{
				GLCD_WriteBlock(0x00, Page, c);
			}
		}
		else if(coord2 > coord1)
		{
			for(uint16_t c = coord1; c <= coord2; c++)
			{
				GLCD_WriteBlock(0x00, Page, c);
			}
		}
}

void GLCD_WriteString(const char* string,char Y, char X)
{
	char temp = 0;
	int i=0;
	while(string[i]!='\0')
	{
		temp = string[i];
		GLCD_Write_Char(temp-32,X+6*i,Y);
		i++;
	}
}

void GLCD_Write_Char(char cPlace,char cX,char cY)
{
	char i=0;
	char chip=1;
	if(cX>=64)
	{
		chip=2;
		cX-=64;
	}
	GLCD_Write(chip,0,(0x40|cX));
	GLCD_Write(chip,0,(0xB8|cY));
	/*minden betű 5 pixel széles*/
	for (i=0;i<5;i++)
	{
	  if (cX+i >= 64)
	  {
		  chip=2;
		  GLCD_Write(chip,0,(0x40|(cX+i-64)));
		  GLCD_Write(chip,0,(0xB8|cY));
	  }
	  GLCD_Write(chip,1,fontdata[cPlace*5+i]);
	}
}

void GLCD_DataWrite(char g_data)
{
	//DB0
	HAL_GPIO_WritePin(DB0_GPIO_Port, DB0_Pin, g_data & 0x01);
	//DB1
	HAL_GPIO_WritePin(DB1_GPIO_Port, DB1_Pin, g_data & 0x02);
	//DB2
	HAL_GPIO_WritePin(DB2_GPIO_Port, DB2_Pin, g_data & 0x04);
	//DB3
	HAL_GPIO_WritePin(DB3_GPIO_Port, DB3_Pin, g_data & 0x08);
	//DB4
	HAL_GPIO_WritePin(DB4_GPIO_Port, DB4_Pin, g_data & 0x10);
	//DB5
	HAL_GPIO_WritePin(DB5_GPIO_Port, DB5_Pin, g_data & 0x20);
	//DB0
	HAL_GPIO_WritePin(DB6_GPIO_Port, DB6_Pin, g_data & 0x40);
	//DB0
	HAL_GPIO_WritePin(DB7_GPIO_Port, DB7_Pin, g_data & 0x80);
}
