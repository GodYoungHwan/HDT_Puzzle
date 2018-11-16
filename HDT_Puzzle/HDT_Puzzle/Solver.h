#pragma once
#include "CommonDef.h"
#include <iostream>
#include <stack>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;
int mbb = max(2, 3);

class Solver {
private:

	//�� ���� �� �ϼ� �� �ϵ� �� ����
	int puzzle_row, puzzle_col;
	int** table;
	int max = 0;
	VEC_PAIR hint;
	STK_PAIR path;
	INT_PAIR end;
	ifstream fin;
	stack<int> s_step;
	stack<int**> s_table;
	stack<INT_PAIR> back;


public:

	Solver();
	
	void getPuzzle();
	void ShowPuzzle();
	void ShowHint();
	//void Solve(int** puzzle, const int& x, const int& y, int& step, const VEC_PAIR& hint, int& hint_idx, STK_PAIR& path);
	void Solve(int** puzzle, const int& x, const int& y, int& step, int& hint_idx);
	void Solve2(int** puzzle, const int& x, const int& y, int& step, int& hint_idx, int cnt);
	int getMax();
	INT_PAIR getEnd();
};
