#include "DxLib.h"
#include "DrawPuyo.h"
#include "PuyoPuyo.h"
#include <time.h>
#include <vector>

namespace {
	using std::vector;
}  // namespace

vector<vector<int>> gene2 = vector<vector<int>>(38, vector<int>(2, 0));

void GenerateP()
{
	int i;
	srand((unsigned)time(NULL));
	for (i = 0; i < 38; i++) {
		gene2[i][0] = (rand() % 6) + 1;
		gene2[i][1] = (rand() % 4);
	}
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	DrawPuyo drawpuyo;

	GenerateP();
	drawpuyo.drawchain(gene2);
	return 0;
}