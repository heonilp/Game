#ifndef __CONSOLE_H__ 
#define __CONSOLE_H__

#define MAX 100
#define BUFFER 30
#define dfSCREEN_WIDTH 81
#define dfSCREEN_HEIGHT 24


void cs_Initial(void);
void cs_MoveCursor(int iPosX, int iPosY);
void cs_ClearScreen(void);
void Buffer_Flip(void);
void Buffer_Clear(void);
void Sprite_Draw(int iPosX, int iPosY, char *sprite);
void Sprite_Draw(int iPosX, int iPosY, char sprite);


#endif




