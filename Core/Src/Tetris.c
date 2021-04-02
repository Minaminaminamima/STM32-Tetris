#include "Tetris.h"


int block[7][4][4][4] = {
	{ // T��� ��
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{0,1,1,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{0,1,0,0}
		}
	},
	{    // ���� ��
		{
			{0,0,0,0},
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,0,0,0},
			{1,1,0,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,0,0,0},
			{1,1,0,0},
			{0,1,0,0}
		}
	},
	{   // ���� �� �ݴ�
		{   
			{0,0,0,0},
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{1,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{1,0,0,0}
		}
	},
	{   // 1���� ��
		{
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0}
		},
		{
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0}
		}
	},
	{   // L���� ��
		{
			{0,0,0,0},
			{1,0,0,0},
			{1,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0},
			{1,0,0,0},
			{1,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,1,0},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,1,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{0,0,0,0}
		}
	},
	{   // L���� �� �ݴ�
		{
			{0,0,0,0},
			{0,0,1,0},
			{1,1,1,0},
			{0,0,0,0}
		},
		{
			{1,0,0,0},
			{1,0,0,0},
			{1,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,1,0},
			{1,0,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		}
	},
	{   // �׸� ��
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		}
	}
};

#define   _COUNTOF(a_)   (sizeof(a_) / sizeof(a_[0]))

int map[15][7]={0,};      
int blockForm;
int blockRotation=0;
uint32_t  currentTick;
const int standardTick = 300; // ���Ӽӵ� 


//  map[15][7] �� ���� ��ǥ��� ����� ��ġ �迭 

// x_pos >> virtualCoordX
const int virtualCoord_X[15] = {1, 9, 17, 25, 33, 41, 49, 57, 65, 73, 81, 89, 97, 105, 113};  
const int virtualCoord_Y[7] = {4, 12, 20, 28, 36, 44, 52};

// ���� ��Ʈ���ϴ� �� ��ġ
// x_nowpos >> currentPosition_X
int currentPosition_X=14;
int currentPosition_Y=2;

// game over �Ǵܽ� ����� ���� x_target>>beforePosition_X
int beforePosition_X=0;
int beforePosition_Y=0;
int block_target=0;

// cnt_target >> gameoverCnt
int gameoverCnt=0;

// cnt_block >> blockCnt
int blockCnt=0;

int score=0; // ����
extern char KeyState;



typedef struct {
   int         x;
   int         y;
   FontDef_t   *pFont;
   char      *pStr;
} TEXTOUT, *PTEXTOUT;



void CreateRandomForm() {
	blockForm = rand() % 7; // ���� �� ���� �߻� 
        blockCnt++;
}



TEXTOUT textOut[] = {
  {0, 100, &Font_11x18, "Tetris"},
  {0, 70, &Font_7x10, "type 'q'"},
  {0, 55, &Font_7x10, "to play"},
  {0, 40, &Font_7x10, "tetris :p"}
   };

TEXTOUT endTextOut[] = {
  {0, 70, &Font_7x10, "score is"}
   };

void PrintScreen(int x){ // ����ȭ�� ����. Tetris : Press the switch to start ��Ÿ��
  
  if (x==1){
   SSD1306_Fill(SSD1306_COLOR_BLACK);
    

      for (int c=0; c<_COUNTOF(textOut); c++) {
      SSD1306_GotoXY(textOut[c].x, textOut[c].y);
      for (int i=0; i<strlen(textOut[c].pStr); i++){
          SSD1306_Screen(textOut[c].pStr[i], textOut[c].pFont, SSD1306_COLOR_WHITE);
       }
   }

       SSD1306_UpdateScreen(); 
}
  else if (x==0){
      HAL_Delay(1500);
  SSD1306_Fill(SSD1306_COLOR_BLACK);
    
      for (int c=0; c<_COUNTOF(endTextOut); c++) {
      SSD1306_GotoXY(endTextOut[c].x, endTextOut[c].y);
      for (int i=0; i<strlen(endTextOut[c].pStr); i++){
          SSD1306_Screen(endTextOut[c].pStr[i], endTextOut[c].pFont, SSD1306_COLOR_WHITE);
       }
   }
  
  SSD1306_GotoXY(0, 55);
  int ones = score%10;
  int tens = (score/10)%10;
  int thou = score/100;
    
  SSD1306_Screen(thou+'0', &Font_7x10, SSD1306_COLOR_WHITE);
  SSD1306_Screen(tens+'0', &Font_7x10,  SSD1306_COLOR_WHITE);
  SSD1306_Screen(ones+'0', &Font_7x10, SSD1306_COLOR_WHITE);
    
  SSD1306_UpdateScreen();    
  HAL_Delay(3000);}
}




void DrawMap() { // ���� Map ���� �׸��� �Լ�
  
    // ���� ȭ�� ����� 
    SSD1306_Fill(SSD1306_COLOR_BLACK);
    
    // ���� ���� ǥ��
    SSD1306_GotoXY(37, 126);
    int ones = score%10;
    int tens = (score/10)%10;
    int thou = score/100;
    
    SSD1306_Screen(thou+'0', &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_Screen(tens+'0', &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_Screen(ones+'0', &Font_7x10, SSD1306_COLOR_WHITE);
  
    // ���� �� �׸���
    SSD1306_DrawLine(0, 61, 128, 61, SSD1306_COLOR_WHITE );
    SSD1306_DrawLine(0, 3, 128, 3, SSD1306_COLOR_WHITE ); // ���� x 
    SSD1306_DrawLine(0, 3, 0, 61, SSD1306_COLOR_WHITE ); // �� �� ª����  
    
    // Map[15][7] �� ���� 1�̸� �����׸�� ä��� 
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 7; j++) {
			if (map[i][j] == 1) {
                          SSD1306_GotoXY(virtualCoord_X[i],virtualCoord_Y[j]);    
                          SSD1306_Putc (0, &Font_8x8, SSD1306_COLOR_WHITE); 
			}
		}
	}
}
 

void DrawBlock() { // ���� ��������(��Ʈ���ϴ�) �� �׸���
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[blockForm][blockRotation][i][j] == 1) {
                          SSD1306_GotoXY(virtualCoord_X[currentPosition_X-i],virtualCoord_Y[currentPosition_Y+j]);    
                          SSD1306_Putc (0, &Font_8x8, SSD1306_COLOR_WHITE); 
			}
		}
	}
}

void DropBlock() { // ���� �� ����Ʈ����. 
	

        if(HAL_GetTick()-currentTick>=standardTick){
         	 currentPosition_X--;  
                currentTick = HAL_GetTick();
	}
}



bool CheckCrash(int x, int y) { // �浹 Ȯ���ϴ� �Լ�. �浹�� true ����
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[blockForm][blockRotation][i][j] == 1) {
                              if(x-i<0 || y+j<0 || y+j>6  ) // ������ ������ �ʱ� ���� ���ǹ�
                                     return true;
                              
                              int t =map[x-i][y+j]; // ���������� ��ġ�� ���� �ִ��� Ȯ���ϱ� ���� ���ǹ� 
				if (t==1)   
					return true;

			}
		}
	}
	return false;
}

void BlockToGround() { // CheckCrash�Լ��� true�� ��� Map�� �ش� ���� �׸��� �Լ�
	if (CheckCrash(currentPosition_X-1, currentPosition_Y) == true) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (block[blockForm][blockRotation][i][j] == 1) {
						map[currentPosition_X-i][currentPosition_Y+j]=1;
					}
				}
			}
                        // Map�� �׸��� ���� �ʱ� ����(�ʱ���ġ, ��ȸ��) �ʱ�ȭ �� ���ο� �� ����
                        currentPosition_X=14;
                        currentPosition_Y=2;
                        blockRotation=0;
                        CreateRandomForm();
	}
}
 

void RemoveLine() { // Map�� Ȯ���Ͽ� ä���� ������ ������ ����� ������ �ø��� �Լ�
  int combo=1;
        // Map[15][7]�� ���� Ȯ���Ͽ� ��� y���� ä�����ִ� x���� ���� ���� ���� 
	for (int i = 14; i >= 0; i--) { 
		int cnt = 0;
		for (int j = 0; j < 7; j++) { // 
			if (map[i][j] == 1) 
                          cnt++;}
                if(cnt==7){
                  memset(map[i], 0, sizeof(map[i]));
                  for (int k=i; k<=13; k++){
                  memcpy(map[k], map[k+1], sizeof(map[k]));
                  memset(map[k+1], 0, sizeof(map[k+1]));
		}
                score+=10*(combo++);
	}
}
}

void keyProcess(){
    if (KeyState=='d') { // d�Է½� �����̵�
      if (CheckCrash(currentPosition_X , currentPosition_Y+1) == false) currentPosition_Y++; }
    else if (KeyState=='a') { // a�Է½� �����̵�
      if (CheckCrash(currentPosition_X , currentPosition_Y-1) == false) currentPosition_Y--; }
    else if (KeyState=='w') { // w�Է½� ȸ�� 
      //else { // ���� ���� �Ѿ�� �ʱ� ���� ����ó�� 
          int turn=1;
         for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[blockForm][(blockRotation+1)%4][i][j] == 1) {
                          if (currentPosition_Y+j>6  || currentPosition_Y+j<=0) { turn=0; break;}  
			}
		}}
    if (turn) blockRotation=(blockRotation+1)%4;
    }
    
    KeyState='n';
}



bool Gameover(){ // ���� ���� Ȯ���ϴ� �Լ�. 
  // ���� �ٸ� �ٲ��� ���� ���·� �����ð����� �� ��ġ�� ������ ������ ���� ���� 
  gameoverCnt++;
  if (gameoverCnt==7){
    if(beforePosition_Y==currentPosition_Y && beforePosition_X==currentPosition_X && blockCnt!=block_target) return true;
    else {
        beforePosition_Y=currentPosition_Y;
        beforePosition_X=currentPosition_X;
        block_target=blockCnt;
       gameoverCnt=0;
       return false;
    }
  }
  else return false;
}



void Game_init(){ // ���� ���۽�Ű�� ���� �ʱ�ȭ

     blockRotation=0;
     currentPosition_X=14;
     currentPosition_Y=2; 
     score=0;
      for ( int i = 0 ;  i < 16; i++ )
      memset( map[i], 0, sizeof(map[i]));

}

