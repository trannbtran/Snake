#include<iostream>
#include<time.h>
#include"console.h"
#include<conio.h>
#include<fstream>
#include<string>
#include<vector>
using namespace std;
#define BOARD_DAI 28
#define BOARD_NGANG 80//+38
#define MAX_SIZE_FOOD 4
#define LENGTH_SNAKE_STAR_GAME 6
#define MAX_SPEED 40//?
#define MAX_SIZE_SNAKE 24
#define LEVEL1 200

enum  STATUS { UP, DOWN, LEFT, RIGHT, PAUSE, EXIT, ENTER };
int Teleport = 0;
int length_when_into_the_door = 0;
struct Position {
	int x, y;
	int ox, oy; //bien tam luu x, y
	STATUS status;
	int lenght;
	int mssv;
	bool Die = false;
	int score;
	int stage;
	bool createGateCheck = false;
	string name = "Player 01";
};

struct _ToaDo
{
	int x, y;
};
Position Food[4];
Position Gate[3][2];

Position Snake[MAX_SIZE_SNAKE];
int MSSV[MAX_SIZE_SNAKE] = { 1,8,1,2,7,2,6,4,1,8,2,7,0,6,5,1,8,1,2,7,2,3,4 };//18127264, 18127065, 18127234
_ToaDo cong;
_ToaDo food;
STATUS pre_status = DOWN;

void FixConsoleWindow();
void _Snake_Init(Position Snake[]);
bool _checkFood_Snake(Position Snake[], _ToaDo food);
bool _checkGate_Snake(Position snake[], _ToaDo cong);//không cần food và gate , do có gate thì không có food
void _create_and_draw_Food(Position snake[]);
void _create_and_draw_Gate_in(Position snake[]);
bool _Snake_into_Door(Position snake[]);
void _CrashSnake(Position snake[], _ToaDo cong);
bool _Eat(Position Snake[]); // Xu lý ran an thuc an;
void GiamDoDai(Position Snake[]);//trở về ban đầu chứ;

void Move(Position Snake[]);
void Control(Position Snake[]);
void Process(Position Snake[]);
void Show_Snake(Position Snake[]);

void Reset_1(Position snake[], int length);
void SnakeOutGate(Position Snake[], int tempLength1);//#####
void TeleportSnake(Position Snake[]);

void Reset(Position snake[]);
void Interact_Snake(Position Snake[]);
void DrawBoard();
void DrawScore_(Position Snake[]);
//DRAW PAUSE, INPUT NAME, BẢNG XẾP HẠNG, LOAD GAME
void DrawSavingLoading(string s_l);
void DrawSmall(string a);
void DeleteDrawSmall();

void GamePlay(Position Snake[]);

void MENU_new_game();
void MENU_save_game(STATUS pre_sta);
void SaveGame(Position Snake[]);
void NewGame(Position Snake[]);
void LoadGame(Position Snake[]);
void Draw_Menu_group();
void Exit();
//new game
//load game
//exit

//advanced ++
	//difficult :easy, normal,hard
	//music
	//color
#define MAUNEN 176
#define MAUCHU 7

typedef char str[31];
str thaotac[3] = { "NEW GAME ","LOAD GAME", "EXIT     " };
str thaotac_save[4] = { "NEW GAME ","SAVE GAME","LOAD GAME", "EXIT     " };
STATUS Key(int z);
int menu(str thaotac[4], int n);
//menu(thaotac, 5);

vector<string> tokenizer(string line_info);
int string_num_to_int(string num);

