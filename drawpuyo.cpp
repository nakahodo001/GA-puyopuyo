#include "DxLib.h"
#include "DrawPuyo.h"

#include <vector>

namespace {
	using std::vector;
}  // namespace

DrawPuyo::DrawPuyo():PuyoPuyo(15, 6){}
void DrawPuyo::draw(void)
{
	int Green = GetColor(0, 255, 0);
	ClearDrawScreen();

	for (int i = 0; i <= m_row; i++) {
		for (int j = -1; j <= m_col; j++) {
			if (j == -1 || j == m_col || i == m_row) {
				DrawGraph((j + 1) * 30, i * 30, Handle[4], TRUE);
			}
			else {
				DrawGraph((j + 1) * 30, i * 30, Handle[m_field[i][j]], TRUE);
			}
		}
	}
	DrawFormatString(250, 60, Green, "連鎖数：%d", ren);
	DrawFormatString(250, 100, Green, "最大連鎖数：%d", max);
	ScreenFlip();
	WaitTimer(100);
}

void DrawPuyo::setting(void)
{
	ChangeWindowMode(TRUE); // ウィンドウモードに設定
	DxLib_Init();   // DXライブラリ初期化処理
	SetGraphMode(400, 480, 32);
	SetDrawScreen(DX_SCREEN_BACK);

	Handle[0] = LoadGraph("puyo/yellow.png"); // 画像をロード
	Handle[1] = LoadGraph("puyo/blue.png");
	Handle[2] = LoadGraph("puyo/red.png");
	Handle[3] = LoadGraph("puyo/purple.png");
	Handle[4] = LoadGraph("puyo/wall.png");
}

void DrawPuyo::drawchain(const vector<vector<int>>& gene)
{
	bool flag;

	setting();

	for (int y = 0; y < m_row; y++) {
		for (int x = 0; x < m_col; x++) {
			m_field[y][x] = -1;
		}
	}

	for (int i = 0; i < m_puyo; i++) {
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
		}
		if (max < ren) {
			max = ren;
		}
		if (!GameoverCheck()) {
			break;
		}
	}

	WaitKey();
	DxLib_End();
}