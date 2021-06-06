#include <iostream>
#include "Neiro.h"

using namespace std;

int main() {
	neiro a(2);
	a.add_layer(3, 0);
	a.add_layer(5, 1);
	a.add_layer(4, 0);
	
	while (true) {
		double x, b, y;
		cin >> x >> b;
		a.set_datas(vector<double>() = { x, b });
		a.update();
		cout << a.result() << endl;
	}
}