#include <iostream>
#include <Windows.h>
#include <iostream>
#include <string>
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
	NN brain({{ 3, 0 }, { 3, 1 }, { 3, 1 }, { 2, 0 }});
	brain.moment = 0.3;
	brain.speed = 0.8;
	bool smart = false;
	int count = 0;

	while (!smart || ++count <= 100000) {
		const size_t outsize = v.size();
		int right_answer = 0;

		setcur(0, 0);
		cout << count << endl;
		for (int i = 0; i < outsize; ++i) {
			const size_t insize = v[i].size();
			for (int j = 0; j < insize; ++j) {
				const vector<double> ideal = { v[i][j][2], (double)abs((int)v[i][j][2] - 1) };
				brain.push({ v[i][j][0], v[i][j][1], i / 4.0 });
				vector<double> out = brain.pull();
				brain.teach(ideal);

				string s = to_string((int)v[i][j][0]) + vc[i] + to_string((int)v[i][j][1]) + " == " + to_string(out[0])
					+ '/' + to_string(out[1]) + " (ideal == " + to_string(ideal[0]) + '/'
					+ to_string(ideal[1]) + ")";

				if (((out[0] < out[1]) && ((int)ideal[0] == 1)) || ((out[0] > out[1]) && ((int)ideal[0] == 0)))
					s += " ----";
				else {
					s += "       ";
					++right_answer;
				}

				s += "\n";

				cout << s;
			}
			cout << "====\n";
		}
		if (right_answer == 16)
			smart = true;
		++count;
	}
}