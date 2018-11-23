#include <iostream>
#include <ctime>
#include <cstdlib>
#include "Solver.h"
#include "GeneratorC.h"

using namespace std;

int main(void)
{
	int tc = 100;
	clock_t total = 0;
	while (tc--) {
		srand((unsigned int)time(NULL));
		Generator Gen;
		int startX = Gen.getStartRow();
		int startY = Gen.getStartCol();
		Gen.GeneratorPuzzle(startX, startY, START_NUM);
		//Gen.getEndNumber();
		//Gen.ShowPuzzle();
		//Gen.ShowCompletePuzzle();
		//Gen.ShowHint();

		Solver sol;
		//sol.setPuzzlePtr(Gen.getPuzzle());
		sol.setSolPuzzle(Gen.getPuzzle(), 15, 15); // row, col size ���� �����ؾ���
		cout << sol.getMax() << endl;
		//cout << sol.getEnd().first << " " << sol.getEnd().second << endl;
		////sol.ShowPuzzle();
		clock_t begin, end;
		begin = clock();
		sol.Initiate();
		end = clock();
		////sol.ShowPuzzle();
		cout << endl << "Time: " << end - begin << "ms" << endl;
		total += (end - begin);
	}
	cout << "average:: " << total / tc << endl;
	return 0;
}