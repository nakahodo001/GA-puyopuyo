#include "puyopuyo.h"

#include <vector>

namespace {
using std::vector;
}  // namespace

const std::vector<std::vector<int>> PuyoPuyo::puyo(
	{{3, 1}, {2, 3}, {1, 2}, {3, 1}, {2, 3}, {1, 2}, {3, 1}, {2, 3},
	 {1, 2}, {1, 2}, {3, 1}, {3, 3}, {2, 1}, {1, 3}, {2, 3}, {2, 3},
	 {2, 1}, {3, 2}, {2, 3}, {2, 1}, {3, 2}, {3, 2}, {1, 3}, {2, 1},
	 {2, 3}, {2, 1}, {3, 1}, {1, 2}, {3, 3}, {2, 1}, {3, 1}, {2, 2},
	 {3, 2}, {1, 1}, {1, 3}, {2, 1}, {1, 2}, {2, 2}});

int PuyoPuyo::getXpos(int gene) { return gene >> 2; }

int PuyoPuyo::getRotate(int gene) { return gene & 3; }

PuyoPuyo::PuyoPuyo(int row, int col)
	: m_row{row},
	  m_col{col},
	  m_puyo{(((row - 2) * col) / 2) - 1},
	  m_elist{vector<bool>(row * col, false)},
	  m_cmb{vector<vector<int>>(row, vector<int>(col, 0))},
	  m_field{vector<vector<int>>(row, vector<int>(col, -1))} {}

int PuyoPuyo::Eval(const vector<vector<int>>& gene) {
	int ren = 0;
	int max = 0;
	bool flag;

	for (int y = 0; y < m_row; y++) {
		for (int x = 0; x < m_col; x++) {
			m_field[y][x] = -1;
		}
	}

	for (int i = 0; i < m_puyo; i++) {
		initialize();
		flag = true;
		Position(i, gene);
		ren = 0;
		while (Erase()) {
			while (flag) {
				flag = Fall();
			}
			ren++;
			initialize();
			flag = true;
		}
		if (max < ren) {
			max = ren;
		}
		if (!GameoverCheck()) {
			break;
		}
		// Print();
		// system("pause");
	}

	return (max);
}

int PuyoPuyo::Eval(const vector<int>& gene) {
	int ren = 0;
	int max = 0;
	bool flag;

	for (int y = 0; y < m_row; y++) {
		for (int x = 0; x < m_col; x++) {
			m_field[y][x] = -1;
		}
	}

	for (int i = 0; i < m_puyo; i++) {
		initialize();
		flag = true;
		Position(i, gene);
		ren = 0;
		while (Erase()) {
			while (flag) {
				flag = Fall();
			}
			ren++;
			initialize();
			flag = true;
		}
		if (max < ren) {
			max = ren;
		}
		if (!GameoverCheck()) {
			break;
		}
		// Print();
		// system("pause");
	}

	return (max);
}

void PuyoPuyo::initialize(void) {
	for (int y = 0; y < m_row; y++) {
		for (int x = 0; x < m_col; x++) {
			m_cmb[y][x] = 0;
		}
	}

	for (int i = 0; i < m_row*m_col; i++) {
		m_elist[i] = false;
	}
}

int PuyoPuyo::CheckCombine(int x, int y, int pno, int cno) {
	if (m_field[y][x] != pno || m_cmb[y][x] != 0) return 0;
	int ret = 1;
	m_cmb[y][x] = cno;
	if (y > 0) ret += CheckCombine(x, y - 1, pno, cno);
	if (x < m_col - 1) ret += CheckCombine(x + 1, y, pno, cno);
	if (y < m_row - 1) ret += CheckCombine(x, y + 1, pno, cno);
	if (x > 0) ret += CheckCombine(x - 1, y, pno, cno);
	return ret;
}

bool PuyoPuyo::Erase() {
	bool flag = false;

	int i = 0;
	for (int y = m_row - 1; y >= 0; y--) {
		for (int x = 0; x < m_col; x++) {
			if (m_cmb[y][x] == 0 && m_field[y][x] != -1) {
				i++;
				int ret = CheckCombine(x, y, m_field[y][x], i);
				if (ret >= 4) {
					flag = true;
					m_elist[i] = true;
				}
			}
		}
	}

	if (flag) {
		for (int y = m_row - 1; y >= 0; y--) {
			for (int x = 0; x < m_col; x++) {
				if (m_elist[m_cmb[y][x]]) m_field[y][x] = 5;
			}
		}
		for (int y = m_row - 1; y >= 0; y--) {
			for (int x = 0; x < m_col; x++) {
				if (m_field[y][x] == 5) m_field[y][x] = -1;
			}
		}
	}

	return (flag);
}

bool PuyoPuyo::Fall() {
	bool flag = false;
	for (int y = m_row - 2; y >= 0; y--) {
		for (int x = 0; x < m_col; x++) {
			if (m_field[y][x] != -1 && m_field[y + 1][x] == -1) {
				m_field[y + 1][x] = m_field[y][x];
				m_field[y][x] = -1;
				flag = true;
			}
		}
	}

	return (flag);
}

bool PuyoPuyo::GameoverCheck() {
	bool overFlag = true;
	if (m_field[3][2] != -1 || m_field[3][3] != -1) {
		overFlag = false;
	}
	return (overFlag);
}

void PuyoPuyo::Print(void) {
	for (int i = 0; i < m_row; i++) {
		for (int j = 0; j < m_col; j++) {
			printf("%2d,", m_field[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void PuyoPuyo::Position(int i, const vector<vector<int>>& gene) {
	int x = gene[i][0] - 1;

	if (gene[i][1] == 0) {
		m_field[0][x] = puyo[i][0];
		m_field[1][x] = puyo[i][1];
	} else if (gene[i][1] == 1) {
		if (x == 5) {
			x--;
		}
		m_field[0][x] = puyo[i][0];
		m_field[0][x + 1] = puyo[i][1];
	} else if (gene[i][1] == 2) {
		m_field[0][x] = puyo[i][1];
		m_field[1][x] = puyo[i][0];
	} else if (gene[i][1] == 3) {
		if (x == 5) {
			x--;
		}
		m_field[0][x] = puyo[i][1];
		m_field[0][x + 1] = puyo[i][0];
	}

	bool flag = true;
	while (flag) {
		flag = Fall();
	}

	EraseTop();
}

void PuyoPuyo::Position(int i, const vector<int>& gene) {
	int x = getXpos(gene[i]);
	int rotate = getRotate(gene[i]);

	if (rotate == 0) {
		m_field[0][x] = puyo[i][0];
		m_field[1][x] = puyo[i][1];
	} else if (rotate == 1) {
		if (x == 5) {
			x--;
		}
		m_field[0][x] = puyo[i][0];
		m_field[0][x + 1] = puyo[i][1];
	} else if (rotate == 2) {
		m_field[0][x] = puyo[i][1];
		m_field[1][x] = puyo[i][0];
	} else if (rotate == 3) {
		if (x == 5) {
			x--;
		}
		m_field[0][x] = puyo[i][1];
		m_field[0][x + 1] = puyo[i][0];
	}

	bool flag = true;
	while (flag) {
		flag = Fall();
	}

	EraseTop();
}

void PuyoPuyo::EraseTop(void) {
	for (int i = 0; i < m_col; i++) {
		m_field[0][i] = -1;
		m_field[1][i] = -1;
	}
}