#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

class cell {
private:
	double input;
	vector<pair<cell*, double>> synapses;

	double normalization(const double& temp);
	int find_synapse_with(cell* const a);
public:
	double output;

	cell();
	bool attach(cell*const a, const double& weight);
	bool detach(cell* const a);
	void clear_synapses();
	vector<pair<cell*, double>> get_synapses();
	bool refresh();
};