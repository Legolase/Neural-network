#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

class cell {
protected:
	vector<pair<cell*, int>> backward_synapses;
	double input;
	double delta_input;

	double normalization(const double& temp);
	int find_ahead_synapse_with(cell* const a);
	int find_backward_synapse_with(cell* const a);
	double fin();
public:
	vector<pair<cell*, pair<double, double>>> ahead_synapses;
	double output;

	cell();
	bool attach(cell*const a, const double& weight);
	bool attach(cell* const a, const double& weight, const double& delta_weight);
	bool refresh();
	void learn(double& moment, double& speed, const double* ideal = nullptr);
};