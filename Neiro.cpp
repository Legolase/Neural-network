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

//������ ���� ������� ������, �� ������� ��������� ��������
neiro::neiro(): site(1, vector<cell> (1)), out() {
	out.attach(&site[0][0], rand(-300000, 300000)/100000.0);
}

//������ input_cells ������� �������� �� ������ ����
neiro::neiro(const size_t& input_cells) : site(0) {
	site.resize(1);
	for (int i = 0; i < input_cells; ++i)
		site[0].push_back(cell());
	
	for (int i = 0; i < site[0].size(); ++i)
		out.attach(&site[0][i], rand(-300000, 300000) / 100000.0);
}

//������ ���� �������� ����� ��������� �������� �� ������� cells ����������� � ����������,
// � bias_cells ������� ��������� 1 � output.
//���� cells ����� ����, �� ���� �� ��������, ��� ��� �� ���� �� ����� ������ ����������,
//������� ������������ false
bool neiro::add_layer(const size_t& cells, const size_t& bias_cells) {
	if (cells > 0) {
		site.push_back(vector<cell>(0));
		int last = site.size() - 1;

		//��������� ������������� �������
		for (int i = 0; i < cells; ++i) {
			site[last].push_back(cell());

			for (int j = 0; j < site[last - 1].size(); ++j)
				site[last][i].attach(&site[last - 1][j], rand(-300000, 300000) / 100000.0);
		}

		//��������� ������� ��������
		for (int i = 0; i < bias_cells; ++i) {
			site[last].push_back(cell());
			site[last][i + cells].output = 2;
		}

		//���������� ����� ������ ������������ �������� �� ����� ������������
		out.clear_synapses();
		for (int i = 0; i < site[last].size(); ++i)
			out.attach(&site[last][i], rand(-300000, 300000) / 100000.0);

		return true;
	}
	else
		return false;
}

//��������� ������������� ���� (����� �������� ��������)
//���� ����� �������� �������� ����� ������� ���� ��������,
//�� ������������ false,
//����� ���������� ��������, �� ������ ������ ��������� �������
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

//���������� �������� � ������� �������
//���� ������ ������������ ������� �� ��������� � �����������
//������� ��������, �� ���������� false, ��� ���������
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

//������� ����� ��������� � ���������� ����������
void neiro::update() {
	for (int i = 1; i < site.size(); ++i) {
		int quan_cell_level = site[i].size();
		for (int j = 0; j < quan_cell_level; ++j)
			site[i][j].refresh();
	}

	this->out.refresh();
}

//���������� ���������� ����� ������ �������� ��������� �������
double neiro::result() {
	return out.output;
}

//������������ ���������� ������� ��������
size_t neiro::input_size() {
	return this->site[0].size();
}

//���������� ��������. ������������ ����� ��������� ���������������
//void neiro::learn(const double& ideal, const double& E, const double& alpha) {
//	double total_delta = round105((ideal - out.output) * ((1 - out.output) * out.output));
//
//}