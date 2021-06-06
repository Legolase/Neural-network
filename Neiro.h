#pragma once
#include "Cell.h"

class neiro {
private:
	vector<vector<cell>> site;
	cell out;

public:

	neiro();
	neiro(const size_t& input_cells);
	bool add_layer(const size_t& cells, const size_t& bias_cells);
	bool del_layer();
	size_t input_size();
	bool set_datas(vector<double>& values);
	void update();
	double result();
	/*void learn(const double& ideal, const double& E, const double& alpha);*/
};