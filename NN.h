#pragma once
#include "Cell.h"

class NN : private vector<vector<cell>> {
private:
	void update();
public:
	double speed, moment;

	NN(const vector<pair<size_t, size_t>>& construction);
	NN(const NN& nn);
	bool push(const vector<double>& data);
	vector<double> pull();
	bool teach(const vector<double>& ideal);
};