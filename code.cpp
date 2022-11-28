	#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <deque>
#include <set>
#include <map>
#include <deque>
#include <queue>
#include <ctime>
#include <Windows.h>


using namespace std;

ifstream in("input.txt");
/*ofstream cout("output.txt");*/

int n;
int k = 0;
int white, blue, red;
WORD Attributes = 0;
vector <vector <char>> all;
struct st {
	int i;
	int j;
};
queue <st> q;

void SetConsoleColour(WORD* Attributes, DWORD Colour)
{
	CONSOLE_SCREEN_BUFFER_INFO Info;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdout, &Info);
	*Attributes = Info.wAttributes;
	SetConsoleTextAttribute(hStdout, Colour);
}
void ResetConsoleColour(WORD Attributes)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attributes);
}

char getColor() {
	int r = rand() % 3;
	if (r == 0)
		if (white > 0) {
			white--;
			return 'W';
		}
		else
			return getColor();
	else if (r == 1)
		if (blue > 0) {
			blue--;
			return 'B';
		}
		else
			return getColor();
	else
		if (red > 0) {
			red--;
			return 'R';
		}
		else
			return getColor();
}

int check_pair(int i1, int j1, int i2, int j2) {
	if (i2 >= 0 && i2 < n && j2 >= 0 && j2 < n)
		return all[i1][j1] == all[i2][j2];
	else
		return 0;
}

bool unhappy(int i, int j) {
	int same = 0;
	same += check_pair(i, j, i - 1, j - 1);
	same += check_pair(i, j, i, j - 1);
	same += check_pair(i, j, i + 1, j - 1);
	same += check_pair(i, j, i - 1, j);
	same += check_pair(i, j, i + 1, j);
	same += check_pair(i, j, i - 1, j + 1);
	same += check_pair(i, j, i, j + 1);
	same += check_pair(i, j, i + 1, j + 1);
	return same < 2;
}

st find_unhappy(int I, int J) {
	bool passed = false;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (i == I && j == J)
				passed = true;
			if (passed && all[i][j] != 'W' && unhappy(i, j))
				return { i,j };
			
		}
	}
	return { -1,-1 };
}

void printM() {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (all[i][j] == 'W') {
				SetConsoleColour(&Attributes, BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN /*| FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE*/);
			}
			else if (all[i][j] == 'R') {
				SetConsoleColour(&Attributes, BACKGROUND_INTENSITY | BACKGROUND_RED | FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else {
				SetConsoleColour(&Attributes, BACKGROUND_INTENSITY | BACKGROUND_BLUE | FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			cout << all[i][j];
			ResetConsoleColour(Attributes);
		}
		cout << "\n";
	}
}

void Move(st notHappy) {
	//If we checked all unhappy nodes but didn't find place where they happy
	if (notHappy.i == -1) {
		cout << "We needed " << k << " moves to make good rezult";
		cout << "\n" << "Final matrix: " << "\n";
		printM();
		exit(0);
	}
	//Seek new place where node will be happy
	for (int i = 0; i < q.size(); ++i) {
		st tmp = q.front();
		q.pop();
		char color = all[notHappy.i][notHappy.j];
		all[tmp.i][tmp.j] = all[notHappy.i][notHappy.j];
		all[notHappy.i][notHappy.j] = 'W';
		if (!unhappy(tmp.i, tmp.j))
		{
			q.push(notHappy);
			return;
		}
		else {
			all[tmp.i][tmp.j] = 'W';
			all[notHappy.i][notHappy.j] = color;
			q.push(tmp);
		}
	}
	//if we didn't find good place for our node we try to find place for next unhappy node
	if(notHappy.j + 1 < n)
		Move(find_unhappy(notHappy.i, notHappy.j + 1));
	else
		Move(find_unhappy(notHappy.i + 1, 0));
	return;
}

int main() {
	srand(time(0));
	cin >> n;
	white = round(0.1 * (n * n));
	blue = round(0.45 * (n * n)), red = round(0.45 * (n * n));
	//0 - white, 1 - blue, 2 - red
	all.resize(n);
	for (int i = 0; i < n; ++i)
		all[i].resize(n);
	//fill the matrix from file
	/*for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j) {
			in >> all[i][j];
			if (all[i][j] == 'W')
				q.push({ i,j });
		}*/
	//fill the matrix with rand
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			all[i][j] = getColor();
			if (all[i][j] == 'W')
				q.push({ i,j });
		}
	}
	//main part
	printM();
	cout << "\n";
	//O(1000) * (O(n*n) + O(2*n*n) + O(n*n)) = O(4000*n*n)
	for (; k < 1000; k++) {
		st notHappy = find_unhappy(0, 0);
		if (notHappy.i == -1)
			break;
		Move(notHappy);
		printM();
		cout << "\n";
	}
	cout << "We needed " << k << " moves to make all of them happy";
	cout << "\n" << "Final matrix: " << "\n";
	printM();
	return 0;
}
