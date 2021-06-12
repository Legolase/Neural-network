#include <iostream>
#include <Windows.h>
#include <iostream>
#include <vector>
#include "NN.h"

using namespace std;

void setcur(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

const vector<vector<vector<double>>> v = { 
	{{0, 0, 0}, {0, 1, 0}, {1, 0, 0}, {1, 1, 1}}, // конъюнкция
	{{0, 0, 0}, {0, 1, 1}, {1, 0, 1}, {1, 1, 1}}, // дизъюнкция
	{{0, 0, 1}, {0, 1, 1}, {1, 0, 0}, {1, 1, 1}}, // импликация
	{{0, 0, 1}, {0, 1, 0}, {1, 0, 0}, {1, 1, 1}}, // эквиваленция
};

const char vc[] = { '*', '+', '>', '=' };

int main() {
	const int line = 2;
	NN brain({ {2,0}, {2,1}, {1, 0} });
	vector<double> out;
	bool reach = false;
	int count = 0;
	
	brain.speed = 0.7;
	brain.moment = 0.3;
	while (!reach) {
		++count;
		short right = 0;
		setcur(0, 0);
		cout << count << endl;
		for (int i = 0; i < v[line].size(); ++i) {
			brain.push({ v[line][i][0], v[line][i][1] });
			double real = (brain.pull())[0];
			double ideal = v[line][i][2];

			
			cout << v[line][i][0] << '*' << v[line][i][1] << " == " << real
				<< " (ideal == " << v[line][i][2] << ')';

			if (round(real) == round(ideal)) {
				++right;
				cout << "                     ";
			}
			else
				cout << "-----";

			cout << endl;
			brain.teach({ ideal });
		}

		cout << "====================================\n";

		if (right == v[line].size() && count > 100000)
			reach = true;
	}

	double a, b;
	while (cin >> a >> b) {
		brain.push({a, b});
		cout << (brain.pull())[0] << endl;
	}
}