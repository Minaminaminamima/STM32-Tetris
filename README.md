# STM32-Tetris
- Tetris on STM32G474
- https://www.youtube.com/watch?v=hoAremb6tVk
- 개발기간 : 2021.02~2021.03

</br>

## 개요

SSD1306에서 테트리스를 구현하기 위해 화면상에 좌표계가 있다고 생각하고 코드를 작성하였다. 해당 디스플레이는 1픽셀씩 제어할 수 있었는데 이를 하나의 좌표로 생각하기에는 크기가 너무 작아 8*8을 하나의 좌표로 여기고 코드를 작성하였다.  따라서 이렇게 만든 가상 좌표계를 2차원 배열 map위에 나타내서 게임을 진행하였다. 

## 개발내용

### 게임 플레이를 위한 **라이브러리 수정**

- 블럭 표현을 위한 폰트 수정

기존 폰트 라이브러리는 ASCII code 내 존재하는 문자만 지원했기 때문에 테트리스 블럭 표현을 위한 특수문자(◼)를 나타내기 위해 라이브러리를 수정하였다. 화면에 문자를 나타내는 방법은 OLED에 1과 0을 이용하여 글자가 있는 부분은 1로 없는 부분은 0으로 나타내는 방식이었다. 8*8을 하나의 좌표로 생각하기 위하여 8*8크기로 특수문자 ◼를 작성하여 라이브러리에 추가해 사용하였다.

```c
/*  8 * 8 Block
 000000000000000 //0x000
 011111111111110 //0x3FFE
 011111111111110
 011111111111110
 011111111111110
 011111111111110
 011111111111110
 000000000000000
 */
//6x6
const uint16_t Font8x8 [] = {
      0x0000, 0x3FFE, 0x3FFE, 0x3FFE, 0x3FFE, 0x3FFE, 0x3FFE, 0x0000,// square
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 // blank
};
```

- 세로로 디스플레이 하기 위한 라이브러리 수정

기존 라이브러리는 가로 기준으로 폰트가 작성되도록 함수가 작성되었는데, 세로로 게임을 플레이하기 위하여 폰트가 세로로 표시되게 폰트가 작성되는 위치를 수정하여 사용하였다. 

```c
char SSD1306_Screen(char ch, FontDef_t* Font, SSD1306_COLOR_t color){
	uint32_t i, b, j;
	for (i = 0; i < Font->FontHeight; i++) {
		b = Font->data[(ch - 32) * Font->FontHeight + i];
		for (j = 0; j < Font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				SSD1306_DrawPixel(SSD1306.CurrentY - i, (SSD1306.CurrentX + j), (SSD1306_COLOR_t) color);
			} else { //SSD1306.CurrentX + j, (SSD1306.CurrentY + i),
				SSD1306_DrawPixel(SSD1306.CurrentY - i, (SSD1306.CurrentX + j), (SSD1306_COLOR_t)!color);
			}
		}
	}

	/* Increase pointer */
	SSD1306.CurrentX += Font->FontWidth;

/*
for (i = 0; i < Font->FontHeight; i++) {
		b = Font->data[(ch) * Font->FontHeight + i];
		for (j = 0; j < Font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t) color);
			} else {
				SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)!color);
			}
		}
	}
	SSD1306.CurrentY += Font->FontHeight;
*/
	
	/* Return character written */
	return ch;
}
```

### **프로그램**

해당 프로그램은 main문 내에서 다음과 같이 동작한다.  실질적인 게임은 while(1)문 내에서 진행된다. 

```c
int main(void){
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_LPUART1_UART_Init();

  srand(time(NULL)); // 난수발생 
  
  SSD1306_Init(); // OLED 초기화 
  SSD1306_Fill(0);
   
Restart :
  StartScreen(); // 초기 화면 시작
  Game_init();   // 게임 변수 초기화 
  flag=1;        // 버튼 눌리면 시작하기 위한 변수 / 인터럽트13 발생시 flag=0
  
  while(flag){;} // 버튼 눌릴때까지 대기 
	HAL_Delay(10); 
  CreateRandomForm(); // 랜덤블록 발생

  while (1){
    DrawMap();    // MAP 그리는 함수
    DrawBlock();  // 현재 Block 그리는 함수
    DropBlock();  // 현재 Block 떨어트리는 함수 
    BlockToGround(); // 현재 Block이 control 안될 때 Map에 작성하는 함수
    RemoveLine(); // 한줄 채워질 때 지우는 함수
    if(Gameover()==true) // Game over시 Restart지점으로 이동 
      goto Restart;
    SSD1306_UpdateScreen(); // 현재 화면 update
  }
}
```

### 게임 플레이를 위한 변수

```c
int map[15][7]={0,};  // 현재 좌표에 대한 상태를 나타내는 블럭 
int blockForm;        // 선택된 블럭을 저장하기 위한 변수
int blockRotation=0;  // 블럭 회전 상태를 나타내기 위한 변수 

volatile int count=0; // 블럭이 떨어질때 시간을 확인하기 위한 변수 
// 8*8 기준 좌표값에 대한 상수 정의 
const int x_pos[16] = {1, 9, 17, 25, 33, 41, 49, 57, 65, 73, 81, 89, 
                 97, 105, 113, 121};
const int y_pos[7] = {4, 12, 20, 28, 36, 44, 52};

// 현재 컨트롤되는 블럭 좌표값 (x_pos, y_pos기준)
int x_nowpos=14;
int y_nowpos=2;

// Gameover를 확인하기 위한 변수값 
int x_target=0;
int y_target=0;
int cnt_target=0;

int score=0; // 점수 
```

### **게임 플레이를 위한 함수**

- 1) DrawMap()

앞서 정의한 좌표값 Map과 점수를 업데이트 하는 함수.

```c
void DrawMap() {
    SSD1306_Fill(SSD1306_COLOR_BLACK); // 화면 출력 초기화 
     
    // 점수 화면 출력  
    SSD1306_GotoXY(37, 126);
    int ones = score%10;
    int tens = (score/10)%10;
    int thou = score/100;
    
    SSD1306_Screen(thou+'0', &Font_7x10, 1);
    SSD1306_Screen(tens+'0', &Font_7x10, 1);
    SSD1306_Screen(ones+'0', &Font_7x10, 1);
    
    // 게임 벽 출력 
    SSD1306_DrawLine(0, 61, 128, 61, 1);
    SSD1306_DrawLine(0, 3, 128, 3, 1); 
    SSD1306_DrawLine(0, 3, 0, 61, 1); 
  
    // 현재 Map 상태를 확인하여 출력 
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 7; j++) {
			if (map[i][j] == 1) {
                          SSD1306_GotoXY(x_pos[i],y_pos[j]);    
                          SSD1306_Putc (0, &Font_8x8, 1); 
			}
		}
	}
}
```

- 2) DrawBlock()

현재 컨트롤하는 블럭을 그리는 함수

```c
void DrawBlock() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[blockForm][blockRotation][i][j] == 1) {
                      SSD1306_GotoXY(x_pos[x_nowpos-i],y_pos[y_nowpos+j]); 
                      SSD1306_Putc (0, &Font_8x8, 1); }}}}
```

- 3) DropBlock()

현재 컨트롤하는 블럭에 대해 위를 아래로 떨어트리는 함수이. 변수 cnt가 4번 증가할 때 마다 한번 떨어진다.

```c
void DropBlock() {
	count++;
	if (count==4) {
		 x_nowpos--;  count=0;
	}
}
```

- 4) BlockToGround()

블럭이 땅이나 다른 블럭 위에 닿았을 때 이를 확인하여 map를 set하는 함수이다. 이를 동작시키기 위해 다른 블럭과의 충돌을 확인하는 함수 CheckCrash() 를 별도로 구현하여 CheckCrash의 리턴값이 true일 때만 동작한다. 블럭이 Map에 그려지면 새로 컨트롤할 블럭을 불러야 하므로 현재 블럭을 컨트롤하는 변수를 초기화하고 새로운 블럭을 리턴하는 함수 CreateRandomForm()를 호출한다.

```c
void BlockToGround() {
	if (CheckCrash(x_nowpos-1, y_nowpos) == true) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (block[blockForm][blockRotation][i][j] == 1) {
						map[x_nowpos-i][y_nowpos+j]=1;  }}}
                        x_nowpos=14;
                        y_nowpos=2;
                        blockRotation=0;
                        CreateRandomForm();
	}
}
```

- 5) RemoveLine()

Map의 상태를 확인하여 가로축이 모두 채워졌을 때 그 줄을 지우는 함수. 한번에 여러줄이 지워지면 더 높은 점수를 받을 수 있도록 코드를 작성하였다.

```c
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
}}
```

- 6) Gameover

게임 종료를 확인하는 함수. 일정시간동안 블럭이 다른 블럭으로 변하지 않은 상태로 위치가 변하지 않으면 한 블럭이 계속해서 같은 위치인것으로 판단하여 게임을 종료하게 작성하였다.

```c
bool Gameover(){ 
  cnt_target++;
  if (cnt_target==7){
    if(y_target==y_nowpos && x_target==x_nowpos && cnt_block!=block_target) return true;
    else {
        y_target=y_nowpos;
        x_target=x_nowpos;
        block_target=cnt_block;
       cnt_target=0;
       return false;
    }
  }
  else return false;
}
```

- 7) CheckCrash

블럭의 충돌을 확인하는 함수

```c
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
```

## UART를 통한 Key control
```c
/* USER CODE BEGIN 4 */
void USART1_IRQHandler(void){
  HAL_UART_IRQHandler(&hlpuart1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  if(huart->Instance == LPUART1){
    tx_buff=rx_buff;
    HAL_UART_Transmit(&hlpuart1, &tx_buff, sizeof(tx_buff), 10);
    if (tx_buff=='q') flag=0; // q 입력시 flag=0전환 --> 게임시작 
    if (tx_buff=='d') { // d입력시 우측이동
      if (CheckCrash(x_nowpos , y_nowpos+1) == false) y_nowpos++; }
    if (tx_buff=='a') { // a입력시 좌측이동
      if (CheckCrash(x_nowpos , y_nowpos-1) == false) y_nowpos--; }
    if (tx_buff=='w') { // w입력시 회전 
       int turn=1;
       for (int i = 0; i < 4; i++) {
		        for (int j = 0; j < 4; j++) {
			if (block[blockForm][(blockRotation+1)%4][i][j] == 1) {
                       if (y_nowpos+j>6  || y_nowpos+j<=0) { turn=0; break;}  
			}
		}}
    if (turn) blockRotation=(blockRotation+1)%4;
    }
```
