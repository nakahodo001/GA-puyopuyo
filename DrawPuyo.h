#ifndef __DRAWPUYO_H__
#define __DRAWPUYO_H__

#include <vector>
#include "PuyoPuyo.h"

class DrawPuyo : public PuyoPuyo{
	private:
		int ren = 0;
		int max = 0;
		int Handle[5];
		void setting();
		void draw();
	public:
		DrawPuyo();
		void drawchain(const std::vector<std::vector<int>>& gene);
};

#endif  // __DRAWPUYO_H__