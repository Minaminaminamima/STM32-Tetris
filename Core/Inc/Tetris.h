#include <stdbool.h>
#include "ssd1306.h"
#include "fonts.h"



void PrintScreen();
void DrawMap() ;
void DrawBlock();
void CreateRandomForm();
void DropBlock();
bool CheckCrash(int x, int y); 
void BlockToGround();
void RemoveLine();
void Game_init();
bool Gameover(); 
void keyProcess();