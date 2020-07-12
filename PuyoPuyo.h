#ifndef __PUYOPUYO_H__
#define __PUYOPUYO_H__

#include <vector>

class PuyoPuyo {
   protected:
	int m_row;
	int m_col;
	int m_puyo;
	std::vector<bool> m_elist;
	std::vector<std::vector<int>> m_cmb;
	std::vector<std::vector<int>> m_field;
	static const std::vector<std::vector<int>> puyo;

	int getXpos(int gene);
	int getRotate(int gene);

	void initialize(void);
	int CheckCombine(int x, int y, int pno, int cno);
	bool Erase();
	bool Fall();
	bool GameoverCheck();
	void Print(void);
	void Position(int i, const std::vector<std::vector<int>>& gene);
	void Position(int i, const std::vector<int>& gene);
	void EraseTop(void);

   public:
	PuyoPuyo(int row, int col);
	int Eval(const std::vector<std::vector<int>>& gene);

	int Eval(const std::vector<int>& gene);
};

#endif  // __PUYOPUYO_H__