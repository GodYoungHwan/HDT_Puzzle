#include "Solver.h"


Solver::Solver(int row, int col)
	:max(0), isEnd(false), zero_count(0), puzzle_row(row), puzzle_col(col), hint_count(0)
{
	wei_puzzle1 = new float*[puzzle_row];
	wei_puzzle2 = new float*[puzzle_row];
	for (int i = 0; i < puzzle_row; i++)
	{
		wei_puzzle1[i] = new float[puzzle_col];
		wei_puzzle2[i] = new float[puzzle_col];
	}
}

Solver::~Solver()
{
	/*
	TO DO
	delete 2d arrays
	*/
}

void Solver::setSolPuzzle(int** gen_puzzle)
{
	puzzle = new int*[puzzle_row];
	for (int i = 0; i < puzzle_row; ++i)
	{
		puzzle[i] = new int[puzzle_col];
		for (int j = 0; j < puzzle_col; ++j)
		{
			puzzle[i][j] = gen_puzzle[i][j];
			if (gen_puzzle[i][j] != -1 && gen_puzzle[i][j] != 0)// table입력이 -1, 0 아니면 hint로 판단하고 위치 저장,start&end는 아직
			{
				hint.push_back(make_pair(puzzle[i][j], INT_PAIR(i, j)));
				hint_count++;
			}
			if (gen_puzzle[i][j] == 1)
			{
				start.first = i, start.second = j;
				
			}
			if (gen_puzzle[i][j] > max)
			{
				max = gen_puzzle[i][j];
				end.first = i, end.second = j;
								
			}
			if (gen_puzzle[i][j] == 0)
			{
				++zero_count;
			}
		}
	}
	sort(hint.begin(), hint.end());
}

// Creating new puzzle tables infinitely.. :<
// This function has to be called only once.
void Solver::setWeightPuzzle(int** gen_puzzle, float** wei_puzzle1)
{
	for (int i = 0; i < puzzle_row; i++)
	{
		for (int j = 0; j < puzzle_col; j++)
		{
			if (gen_puzzle[i][j] == 0)
			{
				float sum = 0;
				int count = 0;
				for (int k = 0; k < 8; k++)
				{
					int lookX = i + dRow[k], lookY = j + dCol[k];
					if (lookX > MAX_SIZE_MAP - 1 || lookX < 0 || lookY > MAX_SIZE_MAP - 1 || lookY < 0)
					{
						continue;
					}
					if (gen_puzzle[lookX][lookY] != 0 && gen_puzzle[lookX][lookY] != -1)
					{
						sum += gen_puzzle[lookX][lookY];
						count++;
					}
				}
				if (sum == 0)
					wei_puzzle1[i][j] = 0;
				else
				{
					wei_puzzle1[i][j] = sum / count;
					zero_count--;
				}
			}
			else
			{
				wei_puzzle1[i][j] = -1;
			}
		}
	}
	if (!zero_count)
	{
		weight_puzzle = wei_puzzle1;
		return;
	}
}

// Overloaded for first parameter
void Solver::setWeightPuzzle(float** wei_puzzle1, float** wei_puzzle2)
{
	for (int i = 0; i < puzzle_row; i++)
	{
		for (int j = 0; j < puzzle_col; j++)
		{
			if (wei_puzzle1[i][j] == 0)
			{
				float sum = 0;
				int count = 0;
				for (int k = 0; k < 8; k++)
				{
					int lookX = i + dRow[k], lookY = j + dCol[k];
					if (lookX > MAX_SIZE_MAP - 1 || lookX < 0 || lookY > MAX_SIZE_MAP - 1 || lookY < 0)
					{
						continue;
					}
					if (wei_puzzle1[lookX][lookY] != 0 && wei_puzzle1[lookX][lookY] != -1)
					{
						sum += wei_puzzle1[lookX][lookY];
						count++;
					}
				}
				if (sum == 0)
					wei_puzzle2[i][j] = 0;
				else
				{
					wei_puzzle2[i][j] = sum / count;
					zero_count--;
				}
			}
			else
			{
				wei_puzzle2[i][j] = wei_puzzle1[i][j];
			}
		}
	}
	if (!zero_count)
	{
		weight_puzzle = wei_puzzle2;
		return;
	}
	//cout << "next!" << endl;
	setWeightPuzzle(wei_puzzle2, wei_puzzle1);
}


void Solver::solveWeightpuzzle(const int& x, const int& y, int step, int hint_idx)
{
	//End. After finding one solve, all Solve will stop.
	if (isEnd)
	{
		return;
	}
	// Success. It checks whether step is same end.
	if (step == puzzle[end.first][end.second])
	{
		cout << "SUCCESS" << endl;
		isEnd = true;
		return;
	}
	puzzle[x][y] = step;
	weight_puzzle[x][y] = -1;
	vector<pair<float, int> > weight;

	//set weight by searching adjacent matrices
	for (int i = 0; i < 8; i++) {
		int lookX = x + dRow[i], lookY = y + dCol[i];
		if (lookX > MAX_SIZE_MAP - 1 || lookX < 0 || lookY > MAX_SIZE_MAP - 1 || lookY < 0)
		{
			continue;
		}
		if (weight_puzzle[lookX][lookY] != -1) {
			weight.push_back(make_pair(weight_puzzle[lookX][lookY], i));
		}
		if (step + 1 == puzzle[lookX][lookY]) {
			float nextWeight = weight_puzzle[lookX][lookY];
			solveWeightpuzzle(lookX, lookY, step + 1, hint_idx + 1);
			weight_puzzle[lookX][lookY] = nextWeight;
		}
	}
	sort(weight.begin(), weight.end());


	int hintValue = puzzle[hint[hint_idx].second.first][hint[hint_idx].second.second];
	// 가중치로 진행.
	if (step + 1 < hintValue)
	{
		for (int i = 0; i < weight.size(); ++i)
		{
			int lookX = x + dRow[weight[i].second], lookY = y + dCol[weight[i].second];
			float nextWeight = weight_puzzle[lookX][lookY]; 
			solveWeightpuzzle(lookX, lookY, step + 1, hint_idx);
			if (!isEnd) { 
				puzzle[lookX][lookY] = 0;
				weight_puzzle[lookX][lookY] = nextWeight;
			}
		}
	}
}


void Solver::ShowPuzzle()
{
	for (int i = 0; i < puzzle_row; ++i)
	{
		for (int j = 0; j < puzzle_col; ++j)
		{
			printf("%3d", puzzle[i][j]);
		}
		cout << endl;
	}
}

//출력시 소수점 1자리만 출력하도록 수정
void Solver::ShowWeightPuzzle()
{
	for (int i = 0; i < puzzle_row; ++i)
	{
		for (int j = 0; j < puzzle_col; ++j)
		{
			printf("%3.0f", weight_puzzle[i][j]);
		}
		cout << endl;
	}
}

void Solver::Solve(const int& x, const int& y, int step, int hint_idx, bool isHint)
{
	//Stop all remaining Solve recursions if finds a solution.
	if (isEnd)
	{
		return;
	}
	if (step == puzzle[end.first][end.second])
	{
		cout << "SUCCESS" << endl;
		isEnd = true;
		return;
	}
	//Stop Solve when current step comes up to hint's number while it's not on the hint.
	if (step == puzzle[hint[hint_idx].second.first][hint[hint_idx].second.second] && (x != hint[hint_idx].second.first || y != hint[hint_idx].second.second))
	{
		return;
	}
	puzzle[x][y] = step;
	int lookX, lookY, stepMax, difX, difY;
	int difStep = (puzzle[hint[hint_idx].second.first][hint[hint_idx].second.second] - step - 1);

	for (int i = 0; i < 9 && !isEnd; ++i)
	{
		lookX = x + dRow[i]; lookY = y + dCol[i];
		difX = abs(lookX - hint[hint_idx].second.first); difY = abs(lookY - hint[hint_idx].second.second);
		if (difX > difY) stepMax = difX;
		else stepMax = difY;

		//길이 막혔을 때 숫자를 채우면서 왔던 길을 다시 0으로 만드는 부분.
		if (i == 8 && !isHint) {
			puzzle[x][y] = 0;
			return;
		}
		//길이 막혀서 왔던 길을 다시 되돌아오려고 하는데 힌트자리라서 0으로 바꾸지 않고 숫자를 그대로 놔두는 경우.
		else if (i == 8 && isHint) {
			return;
		}
		else if (difStep < stepMax) {
			continue;
		}
		//한 칸을 더 가기위해 탐색을 하는데 hidato puzzle판을 넘어가는 경우. 아무일도 안하고 진행한다.
		else if (lookX >= MAX_SIZE_MAP || lookX < 0 || lookY >= MAX_SIZE_MAP || lookY < 0) {
			continue;
		}
		//한 칸 진행하려는 부분이 0이면서, 다음 힌트인 숫자보다 작다면 한 칸 진행한다. 재귀로 호출.
		else if (puzzle[lookX][lookY] == 0 && step + 1 < puzzle[hint[hint_idx].second.first][hint[hint_idx].second.second])// 0을 만났을 경우
		{
			Solve(lookX, lookY, step + 1, hint_idx, false);
		}
		//힌트를 제때에 만난 경우. 힌트 좌표와 일치하면서, 힌트 숫자도 일치하면 진행한다.
		else if ((lookX == hint[hint_idx].second.first && y + dCol[i] == hint[hint_idx].second.second) && (step + 1) == puzzle[hint[hint_idx].second.first][hint[hint_idx].second.second])
		{
			Solve(lookX, lookY, step + 1, hint_idx + 1, true);
		}
	}
}

int Solver::getMax()
{
	return max;
}

INT_PAIR Solver::getStart()
{
	return INT_PAIR(start);
}

INT_PAIR Solver::getEnd()
{
	return INT_PAIR(end);
}

float** Solver::getWeiPuzzle1()
{
	return wei_puzzle1;
}

float** Solver::getWeiPuzzle2()
{
	return wei_puzzle2;
}
int** Solver::getPuzzle()
{
	return puzzle;
}

int Solver::getHintCount()
{
	return hint_count;
}

void Solver::Initiate()
{
	Solve(start.first, start.second, 1, 1, false);
}
void Solver::InitiateW()
{
	solveWeightpuzzle(start.first, start.second, 1, 1);
}
