#include "DxLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>

#define ROW 15
#define COL 6
#define PUYO (((ROW - 2) * COL) / 2) - 1

using namespace std;

void initialize(void);
int CheckCombine(int x, int y, int pno, int cno);
bool Erase();
bool Fall();
bool GameoverCheck();
void GenerateP();
void Print(void);
void Position(int i, vector<vector<int>> gene);
void EraseTop(void);
int Eval(vector<vector<int>> gene);

bool elist[30];

int puyo[PUYO][2] =
{
	{3,1},{2,3},{1,2},
	{3,1},{2,3},{1,2},
	{3,1},{2,3},{1,2},
	{1,2},{3,1},{3,3},
	{2,1},{1,3},{2,3},
	{2,3},{2,1},{3,2},
	{2,3},{2,1},{3,2},
	{3,2},{1,3},{2,1},
	{2,3},{2,1},{3,1},
	{1,2},{3,3},{2,1},
	{3,1},{2,2},{3,2},
	{1,1},{1,3},{2,1},
	{1,2},{2,2}
};

int cmb[ROW][COL];
int field[ROW][COL];
vector<vector<int>> gene2 = vector<vector<int>>(PUYO, vector<int>(2, 0));

int Handle[5];
void setting(void);
void draw(void);

int ren = 0;
int max = 0;
/*
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	setting();

	GenerateP();
	printf("%d\n", Eval(gene2));

	WaitKey();     // キー入力があるまで待機
	DxLib_End();   // DXライブラリ終了処理
	return 0;
}
*/

void draw(void)
{
	int Green = GetColor(0, 255, 0);
	ClearDrawScreen();

	for (int i = 0; i <= ROW; i++) {
		for (int j = -1; j <= COL; j++) {
			if (j == -1 || j == COL || i == ROW) {
				DrawGraph((j + 1) * 30, i * 30, Handle[4], TRUE);
			}
			else {
				DrawGraph((j + 1) * 30, i * 30, Handle[field[i][j]], TRUE);
			}
		}
	}
	DrawFormatString(250, 60, Green, "連鎖数：%d", ren);
	DrawFormatString(250, 100, Green, "最大連鎖数：%d", max);
	ScreenFlip();
	WaitTimer(100);
}

void setting(void)
{
	ChangeWindowMode(TRUE); // ウィンドウモードに設定
	DxLib_Init();   // DXライブラリ初期化処理
	SetGraphMode(400, 480, 32);
	SetDrawScreen(DX_SCREEN_BACK);
	GenerateP();

	Handle[0] = LoadGraph("puyo/yellow.png"); // 画像をロード
	Handle[1] = LoadGraph("puyo/blue.png");
	Handle[2] = LoadGraph("puyo/red.png");
	Handle[3] = LoadGraph("puyo/purple.png");
	Handle[4] = LoadGraph("puyo/wall.png");
}

int Eval(vector<vector<int>> gene)
{
	bool flag;

	for (int y = 0; y < ROW; y++) {
		for (int x = 0; x < COL; x++) {
			field[y][x] = -1;
		}
	}

	for (int i = 0; i < PUYO; i++) {
		initialize();
		flag = true;
		Position(i, gene);
		draw();
		ren = 0;
		while (Erase()) {
			while (flag) {
				flag = Fall();
				draw();
			}
			ren++;
			initialize();
			flag = true;
			draw();
			//WaitKey();
		}
		if (max < ren) {
			max = ren;
		}
		if (!GameoverCheck()) {
			break;
		}
	}

	return (max);
}

void initialize(void)
{
	for (int y = 0; y < ROW; y++) {
		for (int x = 0; x < COL; x++) {
			cmb[y][x] = 0;
		}
	}

	for (int i = 0; i < 30; i++) {
		elist[i] = false;
	}
}

int CheckCombine(int x, int y, int pno, int cno)
{
	if (field[y][x] != pno || cmb[y][x] != 0) return 0;
	int ret = 1;
	cmb[y][x] = cno;
	if (y > 0) ret += CheckCombine(x, y - 1, pno, cno);
	if (x < COL - 1) ret += CheckCombine(x + 1, y, pno, cno);
	if (y < ROW - 1) ret += CheckCombine(x, y + 1, pno, cno);
	if (x > 0) ret += CheckCombine(x - 1, y, pno, cno);
	return ret;
}

bool Erase()
{
	int x, y, i;
	bool flag = false;

	for (i = 0, y = ROW - 1; y >= 0; y--) {
		for (x = 0; x < COL; x++) {
			if (cmb[y][x] == 0 && field[y][x] != -1) {
				i++;
				int ret = CheckCombine(x, y, field[y][x], i);
				if (ret >= 4) {
					flag = true;
					elist[i] = true;
				}
			}
		}
	}

	if (flag) {
		for (y = ROW - 1; y >= 0; y--) {
			for (x = 0; x < COL; x++) {
				if (elist[cmb[y][x]]) field[y][x] = 5;
			}
		}
		for (y = ROW - 1; y >= 0; y--) {
			for (x = 0; x < COL; x++) {
				if (field[y][x] == 5) field[y][x] = -1;
			}
		}
	}

	return (flag);
}

bool Fall()
{
	bool flag = false;
	for (int y = ROW - 2; y >= 0; y--) {
		for (int x = 0; x < COL; x++) {
			if (field[y][x] != -1 && field[y + 1][x] == -1) {
				field[y + 1][x] = field[y][x];
				field[y][x] = -1;
				flag = true;
			}
		}
	}

	return (flag);
}

bool GameoverCheck() {
	bool overFlag = true;
	if (field[3][2] != -1 || field[3][3] != -1) {
		overFlag = false;
	}
	return (overFlag);
}

void Print(void)
{
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			printf("%2d,", field[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void GenerateP()
{
	int i;
	srand((unsigned)time(NULL));
	for (i = 0; i < PUYO; i++) {
		gene2[i][0] = (rand() % 6) + 1;
		gene2[i][1] = (rand() % 4);
	}
}

void Position(int i, vector<vector<int>> gene)
{
	int x = gene[i][0] - 1;
	bool flag = true;

	if (gene[i][1] == 0) {
		field[0][x] = puyo[i][0];
		field[1][x] = puyo[i][1];
	}
	else if (gene[i][1] == 1) {
		if (x == 5) {
			x--;
		}
		field[0][x] = puyo[i][0];
		field[0][x + 1] = puyo[i][1];
	}
	else if (gene[i][1] == 2) {
		field[0][x] = puyo[i][1];
		field[1][x] = puyo[i][0];
	}
	else if (gene[i][1] == 3) {
		if (x == 5) {
			x--;
		}
		field[0][x] = puyo[i][1];
		field[0][x + 1] = puyo[i][0];
	}

	while (flag) {
		flag = Fall();
		draw();
	}

	EraseTop();
}

void EraseTop(void)
{
	for (int i = 0; i < COL; i++) {
		field[0][i] = -1;
		field[1][i] = -1;
	}
}