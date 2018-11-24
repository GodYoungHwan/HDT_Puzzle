#include "Solver.h"


Solver::Solver(int row, int col)
	:max(0), isEnd(false), zero_count(0), puzzle_row(row), puzzle_col(col)
{
	wei_puzzle1 = new float*[puzzle_row];
	wei_puzzle2 = new float*[puzzle_row];
	for (int i = 0; i < puzzle_row; i++)
	{
		wei_puzzle1[i] = new float[puzzle_col];
		wei_puzzle2[i] = new float[puzzle_col];
	}
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
					if (!zero_count)
					{ 
						weight_puzzle = wei_puzzle1;
						return;
					}
				}
			}
			else
			{
				wei_puzzle1[i][j] = -1;
			}
		}
	}
}

// Overloaded for first parameter
// There would be a better way :(
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
					if (!zero_count) 
					{ 
						weight_puzzle = wei_puzzle2;
						return; 
					}
				}
			}
			else
			{
				wei_puzzle2[i][j] = wei_puzzle1[i][j];
			}
		}
	}
	cout << "next!" << endl;
	setWeightPuzzle(wei_puzzle2, wei_puzzle1);
}


// CURRENTLY WORKING !!
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
		cout << "****************************************SUCCESS" << endl;
		isEnd = true;
		return;
	}

	puzzle[x][y] = step;
	weight_puzzle[x][y] = -1; ////////////////////////////////
	//cout << step << " , visiting: " << "(" << x << "," << y << ") ::" << puzzle[x][y] << "------" << endl;
	

	vector<pair<float, int> > weight;

	//가중치 저장.
	for (int i = 0; i < 8; i++) {
		int lookX = x + dRow[i], lookY = y + dCol[i];
		if (lookX > MAX_SIZE_MAP - 1 || lookX < 0 || lookY > MAX_SIZE_MAP - 1 || lookY < 0)
		{
			continue;
		}
		if (weight_puzzle[lookX][lookY] != -1) {///////////////////////////////////
			weight.push_back(make_pair(weight_puzzle[lookX][lookY], i));
		}
		//아래는 정상 작동.
		if (step + 1 == puzzle[lookX][lookY]) {
			float nextWeight = weight_puzzle[lookX][lookY]; /////////////////////////
			solveWeightpuzzle(lookX, lookY, step + 1, hint_idx + 1);
			weight_puzzle[lookX][lookY] = nextWeight; //////////////////////////
		}
	}
	sort(weight.begin(), weight.end());////////////////////////////


	int hintValue = puzzle[hint[hint_idx].second.first][hint[hint_idx].second.second];
	// 가중치로 진행.
	if (step + 1 < hintValue)
	{
		for (int i = 0; i < weight.size(); ++i)
		{
			int lookX = x + dRow[weight[i].second], lookY = y + dCol[weight[i].second];
			float nextWeight = weight_puzzle[lookX][lookY]; //////////////////////////
			solveWeightpuzzle(lookX, lookY, step + 1, hint_idx);
			if (!isEnd) { ////////////////////////////////
				puzzle[lookX][lookY] = 0;
				weight_puzzle[lookX][lookY] = nextWeight;
			}
		}
	}
	// 백트래킹
	/*
	else if (!isHint)
	{
		cout << "???" << endl;
		puzzle[x][y] = 0;
		return;
	}
	else if (isHint)
	{
		cout << "???" << endl;
		return;
	}
	*/
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

void Solver::ShowHint()
{
	vector<pair<int, pair<int, int> > >::iterator it;
	for (it = hint.begin(); it != hint.end(); ++it)
	{
		//cout << puzzle[(*it).first][(*it).second]<<" :::: "<<(*it).first << " " << (*it).second << endl;
		//cout << (*it).first << " :::: " << (*it).second.first << " " << (*it).second.second << endl;
	}
}

void Solver::Solve(const int& x, const int& y, int step, int hint_idx, bool isHint)
{
	//After finding one solve, all Solve will stop.
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
	// It will cut unnecessary searching. If step is same
	if (step == puzzle[hint[hint_idx].second.first][hint[hint_idx].second.second] && (x != hint[hint_idx].second.first || y != hint[hint_idx].second.second))
	{
		return;
	}
	puzzle[x][y] = step;
	//cout << "step: " << step << " , visiting: " << "(" << x << "," << y << ") ::" << puzzle[x][y] << "---------------------------------------------------" << endl;
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
		else if (x + dRow[i] > MAX_SIZE_MAP - 1 || x + dRow[i] < 0 || y + dCol[i] > MAX_SIZE_MAP - 1 || y + dCol[i] < 0) {
			//cout << "***continue***" << endl;
			continue;
		}
		//한 칸 진행하려는 부분이 0이면서, 다음 힌트인 숫자보다 작다면 한 칸 진행한다. 재귀로 호출.
		else if (puzzle[x + dRow[i]][y + dCol[i]] == 0 && step + 1 < puzzle[hint[hint_idx].second.first][hint[hint_idx].second.second])// 0을 만났을 경우
		{
			Solve(x + dRow[i], y + dCol[i], step + 1, hint_idx, false);
		}
		//힌트를 제때에 만난 경우. 힌트 좌표와 일치하면서, 힌트 숫자도 일치하면 진행한다.
		else if ((x + dRow[i] == hint[hint_idx].second.first && y + dCol[i] == hint[hint_idx].second.second) && (step + 1) == puzzle[hint[hint_idx].second.first][hint[hint_idx].second.second])
		{
			Solve(x + dRow[i], y + dCol[i], step + 1, hint_idx + 1, true);
		}

		/*cout << "i= " << i << " : step= " << step << " : hint_idx= " << hint_idx << endl;
		cout << puzzle[x][y] << " : " << puzzle[hint_temp[hint_idx].second.first][hint_temp[hint_idx].second.second] << " : look= " << puzzle[x + dRow[i]][y + dCol[i]] << " : lookX= " << x + dRow[i] << " : lookY= " << y + dCol[i] << endl;
  */
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

void Solver::Initiate()
{
	//Solve(start.first, start.second, 1, 1, false);
	solveWeightpuzzle(start.first, start.second, 1, 1);
}

/*
Add Weight
가중치 추가

단계가 존재하며 몇 차례 단계를 거져처 가중치를 구해야 한다.
아래부터는 예시코드로 설명을 하겠다.
첨부되는 엑셀파일을 보면 좀 더 이해가 쉽다.

 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1  0 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1  0  0 25 -1 -1 -1 -1 -1 -1 -1 -1 -1
  0  0  0  0  0 33 26 -1 -1 -1 -1 -1 -1 -1 -1
 -1  0  0  0 32  0 28  1 -1 -1 -1 -1 -1 -1 -1
 -1 15  0 36  0  0  0  3  2 -1 -1 -1 -1 -1 -1
 -1 -1 13  0  0  0  0 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1  0  0  0 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1  0  0 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 위의 판이 우리가 풀어야 할 퍼즐이다.

 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 25 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1  0 29  X -1 -1 -1 -1 -1 -1 -1 -1 -1
  0  0  0 32 30  X  X -1 -1 -1 -1 -1 -1 -1 -1
 -1 15 25 34  X 29  X  1 -1 -1 -1 -1 -1 -1 -1
 -1  X 21  X 34 30 16  X  X -1 -1 -1 -1 -1 -1
 -1 -1  X 24 36  0  3 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 13 13  0 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1  0  0 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 우선 헷갈리니까 힌트를 X로 놓겠다.
 0이면서 8방향탐색을 했을 때, 힌트가 있는 경우만 가중치를 부여하겠다.
 그러므로 0과 -1은 제외 된다.
 가중치 부여는 8방향 탐색 시에 나온 힌트들의 합을 힌트의 수로 나눈 수를 가중치로 보고 부여한다.
 ex)
 15  0  25
 -1  ?   0
  0 -1  -1
  위 의 경우에는 (15 + 25) / 2 = 20 이므로 ?에 20의 가중치가 부여된다.

  이 를 반복하면 위의 판이 만들어 진다.

  여기서 남는 0은 8방향 탐색을 했을 때 힌트가 없던 경우이다.
  아직 0이 존재하므로 다음 단계로 넘어간다.

  덧, 소수점 걍 귀찮아서 int형으로 받고 소수점 버림.

 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 25 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 29 29  X -1 -1 -1 -1 -1 -1 -1 -1 -1
 15 20 26 32 30  X  X -1 -1 -1 -1 -1 -1 -1 -1
 -1 15 25 34  X 29  X  1 -1 -1 -1 -1 -1 -1 -1
 -1  X 21  X 34 30 16  X  X -1 -1 -1 -1 -1 -1
 -1 -1  X 24 36 23  3 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 13 13 24 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 13 13 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
위의 단계에서는 앞에서 작성된 가중치를 기준으로 채운다.
가중치를 채우는 방식은 똑같다. 합을 개수로 나눈다.

위와 같이 단계별로 참조판과 작성판을 따로 놓지 않으면 원하는 가중치가 안나온다.
그에 대한 설명은 나한테서.... (써서 설명하기 힘들다.)



*/