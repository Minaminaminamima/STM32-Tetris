#include "Tetris.h"


int block[7][4][4][4] = {
	{ // T모양 블럭
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
	{    // 번개 블럭
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
	{   // 번개 블럭 반대
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
	{   // 1자형 블럭
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
	{   // L자형 블럭
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
	{   // L자형 블럭 반대
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
	{   // 네모 블럭
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
const int standardTick = 300; // 게임속도 


//  map[15][7] 에 대한 좌표계로 사용할 위치 배열 

// x_pos >> virtualCoordX
const int virtualCoord_X[15] = {1, 9, 17, 25, 33, 41, 49, 57, 65, 73, 81, 89, 97, 105, 113};  
const int virtualCoord_Y[7] = {4, 12, 20, 28, 36, 44, 52};

// 현재 컨트롤하는 블럭 위치
// x_nowpos >> currentPosition_X
int currentPosition_X=14;
int currentPosition_Y=2;

// game over 판단시 사용할 변수 x_target>>beforePosition_X
int beforePosition_X=0;
int beforePosition_Y=0;
int block_target=0;

// cnt_target >> gameoverCnt
int gameoverCnt=0;

// cnt_block >> blockCnt
int blockCnt=0;

int score=0; // 점수
extern char KeyState;



typedef struct {
   int         x;
   int         y;
   FontDef_t   *pFont;
   char      *pStr;
} TEXTOUT, *PTEXTOUT;



void CreateRandomForm() {
	blockForm = rand() % 7; // 현재 블럭 랜덤 발생 
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

void PrintScreen(int x){ // 시작화면 설정. Tetris : Press the switch to start 나타냄
  
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




void DrawMap() { // 현재 Map 상태 그리는 함수
  
    // 현재 화면 지우기 
    SSD1306_Fill(SSD1306_COLOR_BLACK);
    
    // 현재 점수 표시
    SSD1306_GotoXY(37, 126);
    int ones = score%10;
    int tens = (score/10)%10;
    int thou = score/100;
    
    SSD1306_Screen(thou+'0', &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_Screen(tens+'0', &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_Screen(ones+'0', &Font_7x10, SSD1306_COLOR_WHITE);
  
    // 양쪽 선 그리기
    SSD1306_DrawLine(0, 61, 128, 61, SSD1306_COLOR_WHITE );
    SSD1306_DrawLine(0, 3, 128, 3, SSD1306_COLOR_WHITE ); // 좌측 x 
    SSD1306_DrawLine(0, 3, 0, 61, SSD1306_COLOR_WHITE ); // 맨 끝 짧은거  
    
    // Map[15][7] 에 대해 1이면 검은네모로 채우기 
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 7; j++) {
			if (map[i][j] == 1) {
                          SSD1306_GotoXY(virtualCoord_X[i],virtualCoord_Y[j]);    
                          SSD1306_Putc (0, &Font_8x8, SSD1306_COLOR_WHITE); 
			}
		}
	}
}
 

void DrawBlock() { // 현재 떨어지는(컨트롤하는) 블럭 그리기
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[blockForm][blockRotation][i][j] == 1) {
                          SSD1306_GotoXY(virtualCoord_X[currentPosition_X-i],virtualCoord_Y[currentPosition_Y+j]);    
                          SSD1306_Putc (0, &Font_8x8, SSD1306_COLOR_WHITE); 
			}
		}
	}
}

void DropBlock() { // 현재 블럭 떨어트리기. 
	

        if(HAL_GetTick()-currentTick>=standardTick){
         	 currentPosition_X--;  
                currentTick = HAL_GetTick();
	}
}



bool CheckCrash(int x, int y) { // 충돌 확인하는 함수. 충돌시 true 리턴
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[blockForm][blockRotation][i][j] == 1) {
                              if(x-i<0 || y+j<0 || y+j>6  ) // 벽면을 나가지 않기 위한 조건문
                                     return true;
                              
                              int t =map[x-i][y+j]; // 떨어지려는 위치에 블럭이 있는지 확인하기 위한 조건문 
				if (t==1)   
					return true;

			}
		}
	}
	return false;
}

void BlockToGround() { // CheckCrash함수가 true일 경우 Map에 해당 블럭을 그리는 함수
	if (CheckCrash(currentPosition_X-1, currentPosition_Y) == true) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (block[blockForm][blockRotation][i][j] == 1) {
						map[currentPosition_X-i][currentPosition_Y+j]=1;
					}
				}
			}
                        // Map에 그리고 나서 초기 조건(초기위치, 블럭회전) 초기화 및 새로운 블럭 생성
                        currentPosition_X=14;
                        currentPosition_Y=2;
                        blockRotation=0;
                        CreateRandomForm();
	}
}
 

void RemoveLine() { // Map을 확인하여 채워진 라인이 있으면 지우고 점수를 올리는 함수
  int combo=1;
        // Map[15][7]의 값을 확인하여 모든 y축이 채워져있는 x값에 대한 줄을 제거 
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
    if (KeyState=='d') { // d입력시 우측이동
      if (CheckCrash(currentPosition_X , currentPosition_Y+1) == false) currentPosition_Y++; }
    else if (KeyState=='a') { // a입력시 좌측이동
      if (CheckCrash(currentPosition_X , currentPosition_Y-1) == false) currentPosition_Y--; }
    else if (KeyState=='w') { // w입력시 회전 
      //else { // 양쪽 벽을 넘어서지 않기 위해 예외처리 
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



bool Gameover(){ // 게임 끝을 확인하는 함수. 
  // 블럭이 다른 바뀌지 않은 상태로 일정시간동안 블럭 위치가 변하지 않으면 게임 종료 
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



void Game_init(){ // 게임 동작시키는 변수 초기화

     blockRotation=0;
     currentPosition_X=14;
     currentPosition_Y=2; 
     score=0;
      for ( int i = 0 ;  i < 16; i++ )
      memset( map[i], 0, sizeof(map[i]));

}

