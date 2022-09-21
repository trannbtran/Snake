#include"Header.h"
//resizeConsole
void FixConsoleWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}
void Nocursortype()
{
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = FALSE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}
vector<string> tokenizer(string line_info)
{
	vector<string> tokens;
	int startPos = 0;
	string seperator = " ";
	size_t foundPos = line_info.find(seperator, startPos);
	while (foundPos != string::npos)
	{
		int count = foundPos - startPos;
		string token = line_info.substr(startPos, count);
		tokens.push_back(token);
		startPos = foundPos + seperator.length();
		foundPos = line_info.find(seperator, startPos);
	}
	int count = line_info.length() - startPos;
	string token = line_info.substr(startPos, count);
	tokens.push_back(token);
	return tokens;
}

int string_num_to_int(string num) {
	int a = stoi(num);
	return a;
}

void _Snake_Init(Position Snake[]) //Khoi tao ran
{
	Snake->score = 0;
	Snake->stage = 1;

	for (int i = 0;i < MAX_SIZE_SNAKE;i++) {
		Snake[i].mssv = MSSV[i];
	}// con rắn mang theo dữ liệu 'body' là mssvư
	//Snake[0] la dau cua con ran
	Snake->lenght = LENGTH_SNAKE_STAR_GAME;//=6
	Snake[0].status = RIGHT; //Trang thai ban dau cua ran di chuyen ve phai

	for (int i = 0;i < Snake->lenght;i++) {
		Snake[i].x = 13 - i;
	}
	for (int i = 0;i < Snake->lenght;i++) {
		Snake[i].y = 5;
	}
}
bool _checkFood_Snake(Position Snake[], _ToaDo food) {
	for (int i = 0; i < Snake->lenght; i++)
	{
		if (Snake[i].x == food.x && Snake[i].y == food.y)
		{
			return false;
		}
	}
	return true;
}
bool _checkGate_Snake(Position snake[], _ToaDo cong) {
	for (int i = 0; i < snake->lenght; i++)//tạo cổng sao cho không trùng với rắn
	{
		if (snake[i].x == cong.x - 1 && snake[i].y == cong.y - 1//va vào tường
			|| snake[i].x == cong.x && snake[i].y == cong.y - 1
			|| snake[i].x == cong.x + 1 && snake[i].y == cong.y - 1
			|| snake[i].x == cong.x - 1 && snake[i].y == cong.y
			|| snake[i].x == cong.x && snake[i].y == cong.y//-->đây là cửa đúng
			|| snake[i].x == cong.x + 1 && snake[i].y == cong.y)
			return false;
	}
	return true;
}
//không cần food và gate , do có gate thì không có food
void _create_and_draw_Food(Position snake[]) {
	do {
		food.x = (rand() % (BOARD_NGANG - 1)) + 1;
		food.y = (rand() % (BOARD_DAI - 1)) + 1;
	} while (_checkFood_Snake(snake, food) == false);////hợp lệ hay không	
	gotoXY(food.x, food.y);
	printf_s("%c", 235);
}
void _create_and_draw_Gate_in(Position snake[]) {
	do {
		cong.x = (rand() % (BOARD_NGANG - 3) + 1);//không được làm trùng tường)****************
		cong.y = (rand() % (BOARD_DAI - 3) + 1);
	} while (_checkGate_Snake(snake, cong) == false);//hợp lệ hay không

	int X_ = cong.x, Y_ = cong.y;
	gotoXY(X_ - 1, Y_ - 1);		// O
	printf_s("%c", 220);		//

	gotoXY(X_, Y_ - 1);	        // OO
	printf_s("%c", 220);		//

	gotoXY(X_ + 1, Y_ - 1);  	// OOO
	printf_s("%c", 220);		//

	gotoXY(X_ - 1, Y_);	        // OOO
	printf_s("%c", 219);		// O

	gotoXY(X_ + 1, Y_);		    // OOO   --->hình dáng cái cổng
	printf_s("%c", 219);		// O O
	snake->createGateCheck = true;
}
void Reset_1(Position snake[], int length)//làm mới food, gate
{//VÌ LÍ DO GÌ ĐÓ MÀ PHẢI ĐỂ NÓ Ở ĐÂY MỚI CHẠY ĐƯỢC
	DrawScore_(snake);

	snake->lenght = length;
	/*cong.x = 0;cong.y = 0;
	if (snake->lenght == DO_DAI_LEVEL_1 || snake->lenght == DO_DAI_LEVEL_2 || snake->lenght == DO_DAI_LEVEL_3) {
		_create_and_draw_Gate_in(snake);
	}
	else {
		_create_and_draw_Food(snake);
	}*/
	return;
}
bool _Snake_into_Door(Position snake[]) {	//vào cổng, quay về dộ dài cũ,xóa cổng
	if (snake[0].x == cong.x && snake[0].y == cong.y) {
		if (snake->lenght == MAX_SIZE_SNAKE)
			snake->lenght = LENGTH_SNAKE_STAR_GAME;
		Reset(snake);
		return true;
	}
	return false;
}
void _CrashSnake(Position snake[], _ToaDo cong)//va chạm: thân, tường, cổng(mà không phải cửa)--->dead
{
	if (snake[0].x == BOARD_NGANG || snake[0].x == 0 || snake[0].y == BOARD_DAI || snake[0].y == 0)//chạm tường//**********************
		snake->Die = true;

	for (int i = 1; i < snake->lenght; i++) //chạm thân
	{
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
			snake->Die = true;
	}

	if (snake[0].x == cong.x - 1 && snake[0].y == cong.y - 1//va vào tường
		|| snake[0].x == cong.x && snake[0].y == cong.y - 1
		|| snake[0].x == cong.x + 1 && snake[0].y == cong.y - 1
		|| snake[0].x == cong.x - 1 && snake[0].y == cong.y
		|| snake[0].x == cong.x + 1 && snake[0].y == cong.y)
		snake->Die = true;
}

bool _Eat(Position Snake[]) // Xu lý ran an thuc an
{
	if (Snake[0].x == food.x && Snake[0].y == food.y)
	{
		Snake[Snake->lenght].x = Snake[Snake->lenght - 1].ox;
		Snake[Snake->lenght].y = Snake[Snake->lenght - 1].oy;
		Snake->lenght += 1;
		return true;
	}
	return false;
}
void GiamDoDai(Position Snake[])//trở về ban đầu chứ
{

	Snake[Snake->lenght].x = Snake[Snake->lenght].ox + 1;
	Snake[Snake->lenght].y = Snake[Snake->lenght].oy + 1;
	Snake->lenght -= 1;
	gotoXY(Snake[Snake->lenght].x, Snake[Snake->lenght].y);
	cout << " ";
}

void Move(Position Snake[])
{
	switch (Snake[0].status)
	{
	case(RIGHT):
		Snake[0].x++; break;
	case(LEFT):
		Snake[0].x--; break;
	case(UP):
		Snake[0].y--; break;
	case(DOWN):
		Snake[0].y++; break;
	}
}

void Control(Position Snake[])
{
	if (_kbhit())
	{
		int key = _getch();

		if (key == 'W' || key == 'w' && Snake->status != DOWN)
			Snake->status = UP;
		else if (key == 'S' || key == 's' && Snake->status != UP)
			Snake->status = DOWN;
		else if (key == 'A' || key == 'a' && Snake->status != RIGHT)
			Snake->status = LEFT;
		else if (key == 'D' || key == 'd' && Snake->status != LEFT)
			Snake->status = RIGHT;
		else if (key == 'P' || key == 'p')
			Snake->status = PAUSE;
		else if (key == 'E' || key == 'e')
			Snake->status = EXIT;
		//else if()
		//********************************************** dừng con rắn;hoặc thoát game
	}
}

void Process(Position Snake[]) //Ham xu ly hinh anh con ran khi di chuyen
{
	for (int i = 0; i < Snake->lenght; i++)
	{
		if (i == 0)
		{
			Snake[i].ox = Snake[i].x;
			Snake[i].oy = Snake[i].y;
		}
		else
		{
			Snake[i].ox = Snake[i].x;
			Snake[i].oy = Snake[i].y;

			Snake[i].x = Snake[i - 1].ox;
			Snake[i].y = Snake[i - 1].oy;
		}
	}
}

void Show_Snake(Position Snake[])
{
	pre_status = Snake->status;

	Control(Snake);
	if (pre_status == PAUSE && Snake->status != pre_status) {
		DeleteDrawSmall();
	}
	if (Snake->status != PAUSE) {
		Process(Snake);//pause, nhấn 'W', 'D'
		Move(Snake);//x++,y++
	}
	if (Snake->status == PAUSE) {
		DrawSmall("    PAUSE");
	}
	if (Snake->status == EXIT) {
		//Save game//************************************
		MENU_save_game(pre_status);
		return;
	}

	gotoXY(Snake[Snake->lenght - 1].ox, Snake[Snake->lenght - 1].oy);
	cout << " ";
	for (int i = 0; i < Snake->lenght; i++)
	{
		gotoXY(Snake[i].x, Snake[i].y);
		cout << Snake[i].mssv;
	}

	Sleep(130 - Snake->stage * 20);// length càng tăng thì level càng cao, rắn chạy nhanh hơn khi độ trễ(sleep) ít hơn
	//Level cao nhất là lv 12
}

void Reset(Position snake[])//làm mới food, gate
{//VÌ LÍ DO GÌ ĐÓ MÀ PHẢI ĐỂ NÓ Ở ĐÂY MỚI CHẠY ĐƯỢC
	DrawScore_(snake);
	cong.x = 0;cong.y = 0;
	if (snake->lenght == LENGTH_SNAKE_STAR_GAME) {
		_create_and_draw_Food(Snake);return;
	}
	if ((snake->lenght - LENGTH_SNAKE_STAR_GAME) % 4 == 0 && snake->createGateCheck == false) {//6 food thì tạo 1 cổng
		_create_and_draw_Gate_in(snake);
	}
	else {
		_create_and_draw_Food(snake);
	}
	return;
}
void GamePlay(Position Snake[]) {		  //333333333###############################################################
	do {
		Show_Snake(Snake);// --------------->	PAUSE ,EXIT
		Interact_Snake(Snake);
		
	} while (!Snake->Die);//||Snake->status==PAUSE);//Neu ran chet thi dung vong lap   

	int count = 10;
	while (count != 0) {
		gotoXY(Snake[0].x, Snake[0].y);
		cout << " ";
		Sleep(200);
		gotoXY(Snake[0].x, Snake[0].y);
		cout << "@";
		Sleep(200);
		count--;
	}


	DrawSmall("  GAME OVER");
	//***************************************INPUT NAME --->SAVE
	Snake->Die = false;
	Snake->createGateCheck = false;

	MENU_new_game();
	return;//->RIP rắn, dừng game
}

void Interact_Snake(Position Snake[]) {//tương tác ăn, qua cổng, đụng tường######################################
	_CrashSnake(Snake, cong);//
	if (_Eat(Snake)) {
		Snake->score++;
		Snake->createGateCheck = false;
		Reset(Snake);
	}
	if (_Snake_into_Door(Snake)) {// có hàm xóa cửa
		DrawBoard();
		length_when_into_the_door = Snake->lenght;
		//Snake[0].status = UP;//*************************************************
		//Snake[0].x = 20;
		//Snake[0].y = 20;
		//BOARD_DAI 28
		//BOARD_NGANG 80
		//if (Snake[0].x == 35 && Snake[0].y == 85) {
		//	Snake[0].status = UP;//*************************************************
			//Snake[0].x = 30;
			//Snake[0].y = 20;
		//}
		/**/
		
		//Snake[0].x = cong.x + 1;
		//Snake[0].y = cong.y + 1;
		_create_and_draw_Gate_in(Snake);
		SnakeOutGate(Snake, length_when_into_the_door);
		DrawBoard();
		Reset(Snake);
		Snake->stage++;
		Snake->createGateCheck = true;
		
	}
	//****************************************F********* thiết kế cổng đi
	/*if (Snake[0].x == 10 && Snake[0].y == 10) {
		Snake[0].x = 20;
		Snake[0].y = 20;

		Snake[0].status = LEFT; //Trang thai ban dau cua ran di chuyen xuong duoi

	}*/
	return;

}
void SnakeOutGate(Position Snake[], int tempLength1)
{
	
	
	Snake[0].x = cong.x;
	Snake[0].y = cong.y;

	Snake[0].status = DOWN; //Trang thai ban dau cua ran di chuyen xuong duoi

	Snake->lenght = tempLength1;

	Snake->Die = false;
	//Reset(Snake);
	//Snake.createGateCheck = false;
	
}


void DrawBoard() {
	//vẽ bốn góc
	system("cls");
	TextColor(11);
	gotoXY(0, 0);					printf_s("%c", 201);
	gotoXY(BOARD_NGANG, 0);			printf_s("%c", 187);
	gotoXY(BOARD_NGANG, BOARD_DAI);	printf_s("%c", 188);
	gotoXY(0, BOARD_DAI);			printf_s("%c", 200);
	for (int i = 1; i <= BOARD_NGANG - 1; i++) {//thanh tren
		gotoXY(i, 0);
		printf_s("%c", 205);
	}//ve thanh duoi	
	for (int i = 1; i <= BOARD_NGANG - 1; i++) {
		gotoXY(i, BOARD_DAI);
		printf_s("%c", 205);
	}
	for (int i = 1; i <= BOARD_DAI - 1; i++) {//ve cot
		gotoXY(0, i);
		printf_s("%c", 186);
		gotoXY(BOARD_NGANG, i);
		printf_s("%c", 186);
		gotoXY(BOARD_NGANG + 1, i);
		printf_s("%c", 186);
		gotoXY(BOARD_NGANG + 38, i);
		printf_s("%c", 186);
	}
	//VẼ Ô NHỎ
	gotoXY(BOARD_NGANG + 1, 0);					printf_s("%c", 201);
	gotoXY(BOARD_NGANG + 38, 0);				printf_s("%c", 187);
	gotoXY(BOARD_NGANG + 1, BOARD_DAI);			printf_s("%c", 200);
	gotoXY(BOARD_NGANG + 38, BOARD_DAI);		printf_s("%c", 188);
	for (int i = BOARD_NGANG + 2; i <= BOARD_NGANG + 38 - 1; i++) {
		gotoXY(i, 0);
		printf_s("%c", 205);
	}
	for (int i = BOARD_NGANG + 2; i <= BOARD_NGANG + 38 - 1; i++) {
		gotoXY(i, BOARD_DAI);
		printf_s("%c", 205);
	}
	//VE O NHO HOn NUAX
	for (int i = BOARD_NGANG + 2; i <= BOARD_NGANG + 38 - 1; i++) {
		gotoXY(i, BOARD_DAI / 2 - 5);
		printf_s("%c", 205);
	}
	gotoXY(BOARD_NGANG + 1, BOARD_DAI / 2 - 5);					printf_s("%c", 204);
	gotoXY(BOARD_NGANG + 38, BOARD_DAI / 2 - 5);				printf_s("%c", 185);

	//SCORE
	TextColor(5);
	gotoXY(BOARD_NGANG + 1 + 14, 3);
	cout << "STAGE ";
	gotoXY(BOARD_NGANG + 1 + 14, BOARD_DAI / 2 - 9);//14					
	cout << "Score: ";
	TextColor(5);
	gotoXY(BOARD_NGANG + 1 + 12, 13);
	cout << "Huong dan choi ";	TextColor(13);
	gotoXY(BOARD_NGANG + 1 + 10, 14);//14					
	cout << "W: Len tren";
	gotoXY(BOARD_NGANG + 1 + 10, 15);//14					
	cout << "S: Xuong duoi";
	gotoXY(BOARD_NGANG + 1 + 10, 16);//14					
	cout << "A: Sang trai";
	gotoXY(BOARD_NGANG + 1 + 10, 17);//14					
	cout << "D: Sang phai";
	gotoXY(BOARD_NGANG + 1 + 10, 18);//14					
	cout << "P: Dung choi";
	gotoXY(BOARD_NGANG + 1 + 12, 21);//14
	TextColor(5);
	cout << "Thanh vien: ";TextColor(13);
	gotoXY(BOARD_NGANG - 10 + 16, 22);//14		
	cout << "18127065 - Tran Ha Long";
	gotoXY(BOARD_NGANG - 10 + 16, 23);//14					
	cout << "18127234 - Tran Ngoc Bao Tran";
	gotoXY(BOARD_NGANG - 10 + 16, 24);//14					
	cout << "18127264 - Pham Thi Hong Anh";

	//*********************************
	//gotoXY(10, 10);
	//cout << "A";

}
void DrawDungeon_test() {
	int X_ = 10, Y_ = 10;
	gotoXY(X_ - 1, Y_ - 1);		// O
	printf_s("%c", 220);		//

	gotoXY(X_, Y_ - 1);	        // OO
	printf_s("%c", 220);		//

	gotoXY(X_ + 1, Y_ - 1);  	// OOO
	printf_s("%c", 220);		//

	gotoXY(X_ - 1, Y_);	        // OOO
	printf_s("%c", 219);		// O

	gotoXY(X_ + 1, Y_);		    // OOO   --->hình dáng cái cổng
	printf_s("%c", 219);		// O O
}
void DrawScore_(Position Snake[]) {
	TextColor(13);
	gotoXY(BOARD_NGANG + 1 + 14 + 6, 3);
	cout << Snake->stage;
	gotoXY(BOARD_NGANG + 1 + 14 + 7, BOARD_DAI / 2 - 9);//14					
	cout << Snake->score;
	//gotoXY(BOARD_NGANG + 1 + 10+7, BOARD_DAI / 2 - 7);//14					
	//cout << Snake->speed;
	return;
}
void DrawSmall(string a) {//4 gocDrawBoard();
	int x_left = BOARD_NGANG / 2 - 10;
	int x_right = BOARD_NGANG / 2 + 10;
	int y_up = BOARD_DAI / 2 - 1;
	int y_down = BOARD_DAI / 2 - 1 + 5 + 1;
	gotoXY(x_left, y_up);printf_s("%c", 201);
	gotoXY(x_right, y_up);			printf_s("%c", 187);
	gotoXY(x_left, y_down);	printf_s("%c", 200);
	gotoXY(x_right, y_down);			printf_s("%c", 188);

	for (int i = x_left + 1; i <= x_right - 1; i++) {//thanh tren
		gotoXY(i, y_up);	printf_s("%c", 205);
	}//ve thanh duoi
	for (int i = x_left + 1; i <= x_right - 1; i++) {
		gotoXY(i, y_down);		printf_s("%c", 205);
	}
	for (int i = y_up + 1; i <= y_down - 1; i++) {//ve cot
		gotoXY(x_left, i);
		printf_s("%c", 186);
		gotoXY(x_right, i);
		printf_s("%c", 186);
	}
	gotoXY(x_left + 4, y_up + 3);
	cout << a;
	Sleep(1000);
}
void DrawSavingLoading(string s_l) {
	system("cls");
	int x_left = BOARD_NGANG / 2 - 10;
	int x_right = BOARD_NGANG / 2 + 10 + 20;
	int y_up = BOARD_DAI / 2 - 1;
	int y_down = BOARD_DAI / 2 - 1 + 5 + 1;
	gotoXY(x_left, y_up);printf_s("%c", 201);
	gotoXY(x_right, y_up);			printf_s("%c", 187);
	gotoXY(x_left, y_down);	printf_s("%c", 200);
	gotoXY(x_right, y_down);			printf_s("%c", 188);

	for (int i = x_left + 1; i <= x_right - 1; i++) {//thanh tren
		gotoXY(i, y_up);	printf_s("%c", 205);
	}//ve thanh duoi
	for (int i = x_left + 1; i <= x_right - 1; i++) {
		gotoXY(i, y_down);		printf_s("%c", 205);
	}
	for (int i = y_up + 1; i <= y_down - 1; i++) {//ve cot
		gotoXY(x_left, i);
		printf_s("%c", 186);
		gotoXY(x_right, i);
		printf_s("%c", 186);
	}
	gotoXY(x_left + 4, y_up + 3);
	cout << s_l << " ";
	Sleep(200);
	for (int i = 0;i < 5;i++) {
		Sleep(200);
		gotoXY(x_left + 4 + s_l.size(), y_up + 3);
		cout << "     ";
		gotoXY(x_left + 4 + s_l.size(), y_up + 3);
		switch (i % 3)
		{
		case 0:
			cout << " .";
			break;
		case 1:
			cout << " ..";
			break;
		case 2:
			cout << " ...";
			break;
		default:
			break;
		}
	}
	gotoXY(x_left + 4, y_up + 3);
	cout << "Completed !" << " ";
	Sleep(1000);
}
void DeleteDrawSmall() {
	int x_left = BOARD_NGANG / 2 - 10;
	int x_right = BOARD_NGANG / 2 + 10;
	int y_up = BOARD_DAI / 2 - 1;
	int y_down = BOARD_DAI / 2 - 1 + 5 + 1;
	gotoXY(x_left, y_up);cout << " ";
	gotoXY(x_right, y_up);			cout << " ";
	gotoXY(x_left, y_down);cout << " ";
	gotoXY(x_right, y_down);			cout << " ";

	for (int i = x_left + 1; i <= x_right - 1; i++) {//thanh tren
		gotoXY(i, y_up);	cout << " ";
	}//ve thanh duoi
	for (int i = x_left + 1; i <= x_right - 1; i++) {
		gotoXY(i, y_down);		cout << " ";
	}
	for (int i = y_up + 1; i <= y_down - 1; i++) {//ve cot
		gotoXY(x_left, i);
		cout << " ";
		gotoXY(x_right, i);
		cout << " ";
	}
	gotoXY(x_left + 8, y_up + 3);
	cout << "                 ";
}

STATUS Key(int z) {
	if (z == 224) {
		char c;
		c = _getch();
		if (c == 72)//||c=='W'||c=='w')
			return UP;
		if (c == 80)//||c=='S'||c=='s')
			return DOWN;
		if (c == 77)//||c=='D'||c=='d')
			return RIGHT;
		if (c == 75)//||c=='A'||c=='a')
			return LEFT;
	}
	else if (z == 13)
		return ENTER;
}

int menu(str thaotac[4], int n) {
	int tt = 0;
	int* mau = new int[n];
	for (int i = 0;i < n;i++)
		mau[i] = MAUCHU;
	mau[0] = MAUNEN;

	while (1) {
		clrscr();
		//ve khung
		int ngang = BOARD_NGANG + 37;
		int doc = BOARD_DAI;
		gotoXY(0, 0);					printf_s("%c", 201);
		gotoXY(ngang, 0);			printf_s("%c", 187);
		gotoXY(ngang, doc);	printf_s("%c", 188);
		gotoXY(0, doc);			printf_s("%c", 200);
		for (int i = 1; i <= ngang - 1; i++) {//thanh tren
			gotoXY(i, 0);
			printf_s("%c", 205);
		}//ve thanh duoi	
		for (int i = 1; i <= ngang - 1; i++) {
			gotoXY(i, doc);
			printf_s("%c", 205);
		}
		for (int i = 1; i <= doc - 1; i++) {//ve cot
			gotoXY(0, i);
			printf_s("%c", 186);
			gotoXY(ngang, i);
			printf_s("%c", 186);
		}
		int xxx = 35;
		int yyy = 3;
		gotoXY(xxx, yyy);
		cout << "  HHHH    HH    HH    HH       HH   H   HHHHH";gotoXY(xxx, yyy + 1);
		cout << "HH        HHHH  HH   H HH      HH H     HH   ";gotoXY(xxx, yyy + 2);
		cout << "HHHHHH    HH HH HH  HH  HHH    HHHHH    HHHHH";gotoXY(xxx, yyy + 3);
		cout << "   HHH    HH HH HH HHHHHHHHH   HH  H    HH   ";gotoXY(xxx, yyy + 4);
		cout << "HHHHH     HH   HHH H     HHHH  HH   HH  HHHHH";gotoXY(xxx, yyy + 5);

		for (int i = 0;i < n;i++)
		{
			TextColor(mau[i]);
			//GO TO xy  ***************************************
			gotoXY(BOARD_NGANG / 2 + 10, BOARD_DAI / 2 + i * 2);
			cout << "  " << thaotac[i] << "  " << endl;
		}

		/*int color = rand()%15+1;
			TextColor(color);
			gotoXY(BOARD_NGANG / 2 + 17, BOARD_DAI / 2 - 5);
			cout << "SNAKE";*/

		int z = _getch();
		STATUS trangthai_ = Key(z);
		switch (trangthai_)
		{
		case UP:
			if (tt == 0) {
				tt = n - 1;
			}
			else
				tt--;
			break;
		case DOWN:
			if (tt == n - 1) {
				tt = 0;
				break;
			}
			tt++;
			break;
		case ENTER:
			return tt;
		}
		for (int i = 0;i < n;i++)
			mau[i] = MAUCHU;
		mau[tt] = MAUNEN;
	}
}
void MENU_new_game() {
	system("cls");
	switch (menu(thaotac, 3))
	{
	case 0:
		NewGame(Snake);
		//DrawSavingLoading("Saving");
		break;
	case 1:
		DrawSavingLoading("Loading");
		LoadGame(Snake);
		break;
	case 2:
		Exit();
		DrawSavingLoading("Exiting");
		break;
	}
	return;
}
void MENU_save_game(STATUS pre_sta) {
	system("cls");
	switch (menu(thaotac_save, 4))
	{
	case 0:
		NewGame(Snake);
		break;
	case 1:
		SaveGame(Snake);
		DrawSavingLoading("Saving");
		MENU_save_game(pre_status);
		break;
	case 2:
		DrawSavingLoading("Loading");
		LoadGame(Snake);

		break;
	case 3:
		Exit();
		DrawSavingLoading("Exiting");
		break;
	}
	return;
}

void NewGame(Position Snake[]) {
	system("cls");
	DrawBoard();//NEW GAME

	Snake->createGateCheck = false;
	_Snake_Init(Snake);// khởi tạo dữ liệu con rắn ban đầu	
	Reset(Snake);

	DrawScore_(Snake);
	GamePlay(Snake);
}

void Exit() {
	return;
}

void SaveGame(Position Snake[]) {
	ofstream SaveGame;
	SaveGame.open("SaveFile.txt");
	if (SaveGame.is_open() == false) {
		cout << "Error... Can not open file..." << endl;
		return;
	}
	SaveGame << Snake->lenght << " " << Snake->stage << " " << Snake->score << " " << pre_status << " ";
	for (int i = 0;i < Snake->lenght - 1;i++) {
		SaveGame << Snake[i].x << " " << Snake[i].y << " ";
	}
	SaveGame << Snake[Snake->lenght - 1].x << " " << Snake[Snake->lenght - 1].y;//<<" ";
	//SaveGame << Snake->name;
	SaveGame.close();
	system("cls");

}

void LoadGame(Position Snake[]) {
	ifstream LoadGame;
	LoadGame.open("SaveFile.txt");
	if (LoadGame.is_open() == false) {
		cout << "Error... Can not open file..." << endl;
		system("pause");
		return;
	}
	string line;
	vector<string> info_snake;
	while (!LoadGame.eof()) {
		getline(LoadGame, line);
		info_snake = tokenizer(line);
	}
	LoadGame.close();

	Snake->lenght = string_num_to_int(info_snake[0]);//=6
	Snake->stage = string_num_to_int(info_snake[1]);
	Snake->score = string_num_to_int(info_snake[2]);
	STATUS j;
	switch (string_num_to_int(info_snake[3]))
	{
	case 0:
		j = UP;
		break;
	case 1:
		j = DOWN;
		break;
	case 2:
		j = LEFT;
		break;
	case 3:
		j = RIGHT;
		break;
	default:
		j = DOWN;
		break;
	}
	Snake[0].status = j; //Trang thai ban dau cua ran di chuyen ve phai//++++++++++++++++++
	for (int i = 0;i < MAX_SIZE_SNAKE;i++) {
		Snake[i].mssv = MSSV[i];
	}
	for (int i = 4;i < Snake->lenght + 4;i = i + 2) {
		Snake[i / 2 - 2].x = string_num_to_int(info_snake[i]);
		Snake[i / 2 - 2].y = string_num_to_int(info_snake[i + 1]);
	}
	//Snake->name = info_snake[info_snake.size() - 1];
	system("cls");
	DrawBoard();
	Reset(Snake);
	DrawScore_(Snake);
	GamePlay(Snake);
}

void Draw_Menu_group() {

	ifstream File;
	File.open("nhom5.txt", ios::in);
	TextColor(13);
	char c;
	gotoXY(5, 10);
	while (!File.eof())
	{
		File.get(c);
		cout << c;
	}
	File.close();

	int i;
	TextColor(11);
	for (i = 18; i < 90; i++) // tao dong ngang
	{
		gotoXY(i, 5); cout << "*"; // dong ngang tren
		Sleep(10);
	}
	for (i = 5; i <= 20; i++)  // dong ke thang dung
	{
		gotoXY(90, i); cout << "*"; // dong thang ben phai
		Sleep(10);
	}
	for (i = 90; i > 18; i--) // tao dong ngang
	{
		gotoXY(i, 20); cout << "*";  // dong ngang duoi
		Sleep(10);
	}
	for (i = 20; i > 5; i--)  // dong ke thang dung
	{
		gotoXY(17, i); cout << "*"; // dong thang ben trai
		Sleep(10);
	}
	getchar();
}
int main() {
	srand(time_t(NULL));
	Nocursortype();
	//FixConsoleWindow();
	Draw_Menu_group();
	MENU_new_game();

	//	Position Snake[MAX_SIZE_SNAKE];
	//_Snake_Init(Snake);// khởi tạo dữ liệu con rắn ban đầu
	//SaveGame(Snake);
	system("pause>n");

}
