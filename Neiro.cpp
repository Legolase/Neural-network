#include "Neiro.h"
#include <random>
#include <iostream>

int rand(const int& a, const int& b) {
	mt19937 gen{ random_device()() };
	uniform_int_distribution<int> dist(a, b);
	return dist(gen);
}

//double round105(const double& temp) {
//	return (round(temp * pow(10, 5))) / pow(10, 5);
//}

//создаёт один входной нейрон, на который ссылается выходной
neiro::neiro(): site(1, vector<cell> (1)), out() {
	out.attach(&site[0][0], rand(-300000, 300000)/100000.0);
}

//создаёт input_cells входных нейронов на первом слое
neiro::neiro(const size_t& input_cells) : site(0) {
	site.resize(1);
	for (int i = 0; i < input_cells; ++i)
		site[0].push_back(cell());
	
	for (int i = 0; i < site[0].size(); ++i)
		out.attach(&site[0][i], rand(-300000, 300000) / 100000.0);
}

//создаёт слой нейронов перед последним выходным из которых cells связываются с предыдущим,
// а bias_cells задаётся константа 1 в output.
//Если cells равно нулю, то слой не создаётся, так как на него не будут влиять предыдущие,
//поэтому возвращается false
bool neiro::add_layer(const size_t& cells, const size_t& bias_cells) {
	if (cells > 0) {
		site.push_back(vector<cell>(0));
		int last = site.size() - 1;

		//создаются связывающиеся нейроны
		for (int i = 0; i < cells; ++i) {
			site[last].push_back(cell());

			for (int j = 0; j < site[last - 1].size(); ++j)
				site[last][i].attach(&site[last - 1][j], rand(-300000, 300000) / 100000.0);
		}

		//создаются нейроны смещения
		for (int i = 0; i < bias_cells; ++i) {
			site[last].push_back(cell());
			site[last][i + cells].output = 2;
		}

		//заменяются связи старых предвыходных нейронов на новые предвыходные
		out.clear_synapses();
		for (int i = 0; i < site[last].size(); ++i)
			out.attach(&site[last][i], rand(-300000, 300000) / 100000.0);

		return true;
	}
	else
		return false;
}

//удаляется предпоследний слой (перед выходным нейроном)
//если перед выходным нейроном лежит входной слой нейронов,
//то возвращается false,
//иначе происходит удаление, со сменой связей выходного нейрона
bool neiro::del_layer() {
	if (site.size() > 1) {
		site.resize(site.size() - 1);

		out.clear_synapses();
		for (int i = 0; i < site[site.size() - 1].size(); ++i)
			out.attach(&site[site.size() - 1][i], 1);

		return true;
	}
	else
		return false;
}

//записывает значения в входные нейроны
//если размер вписываемого вектора не совпадает с количеством
//входных нейронов, то возвращает false, без изменений
bool neiro::set_datas(vector<double>& values) {
	int inptsz = this->input_size();
	if (values.size() == inptsz) {
		for (int i = 0; i < inptsz; ++i)
			site[0][i].output = values[i];
		return true;
	}
	else
		return false;
}

//обычный обход неиросети с получением результата
void neiro::update() {
	for (int i = 1; i < site.size(); ++i) {
		int quan_cell_level = site[i].size();
		for (int j = 0; j < quan_cell_level; ++j)
			site[i][j].refresh();
	}

	this->out.refresh();
}

//возвращает полученное после обхода значение выходного нейрона
double neiro::result() {
	return out.output;
}

//возвращается количество входных нейронов
size_t neiro::input_size() {
	return this->site[0].size();
}

//происходит обучение. используется метод обратного распространения
//void neiro::learn(const double& ideal, const double& E, const double& alpha) {
//	double total_delta = round105((ideal - out.output) * ((1 - out.output) * out.output));
//
//}