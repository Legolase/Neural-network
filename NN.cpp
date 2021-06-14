#include "NN.h"
#include <cassert>
#include <random>

int rand(const int& a, const int& b) {
	mt19937 gen{ random_device()() };
	uniform_int_distribution<int> dist(a, b);
	return dist(gen);
}

//обычный прогон полученных данных
void NN::update() {
	const size_t outsize = this->size();

	for (int i = 1; i < outsize; ++i) {
		const size_t insize = (*this)[i].size();

		for (int j = 0; j < insize; ++j)
			(*this)[i][j].refresh();
	}
}

//Создаётся нейросеть с construction.size() слоями в каждом из которых first обычных нейронов
//и second нейронов смещения
NN::NN(const vector<pair<size_t, size_t>>& construction) : speed(0), moment(0) {
	assert(construction.size() > 2);
	this->resize(construction.size());

	for (int i = 0; i < size(); ++i)
		(*this)[i].resize(construction[i].first + construction[i].second);

	for (int i = 1; i < size(); ++i) {
		for (int j = 0; j < construction[i].first; ++j) {
			for (int q = 0; q < (*this)[i - 1].size(); ++q)
				(*this)[i][j].attach(&(*this)[i - 1][q], rand(-2000, 2000) / 1000.0);
		}
	}
}

//копирующий конструктор
NN::NN(const NN& nn): moment(nn.moment), speed(nn.speed) {
	this->resize(nn.size());
	
	for (int i = 0; i < size(); ++i)
		(*this)[i].resize(nn[i].size());

	for (int i = 1; i < size(); ++i) {
		const size_t insize = (*this)[i].size();
		for (int j = 0; j < insize; ++j) {
			if (nn[i][j].ahead_synapses.size() > 0) {
				for (int q = 0; q < (*this)[i - 1].size(); ++q)
					(*this)[i][j].attach(&(*this)[i - 1][q], nn[i][j].ahead_synapses[q].second.first, nn[i][j].ahead_synapses[q].second.second);
			}
			else
				(*this)[i][j].output = nn[i][j].output;
		}
	}
}

//задаёт входные данные
bool NN::push(const vector<double>& data) {
	if ((*this)[0].size() == data.size()) {
		const size_t insize = data.size();

		for (int i = 0; i < insize; ++i)
			(*this)[0][i].output = data[i];

		this->update();

		return true;
	}
	else
		return false;
}

//получение вещественных данных нейросети
vector<double> NN::pull() {
	vector<double> v;
	const size_t outsize = this->size()-1;
	const size_t insize = (*this)[outsize].size();

	for (int i = 0; i < insize; ++i)
		v.push_back((*this)[outsize][i].output);

	return v;
}

//Происходит обучение нейросети
bool NN::teach(const vector<double>& ideal) {
	const size_t outsize = this->size() - 1;

	if (ideal.size() == (*this)[outsize].size()) {
		for (int i = outsize; i > -1; --i) {
			const size_t insize = (*this)[i].size();

			if (i == outsize) {
				for (int j = 0; j < insize; ++j)
					(*this)[i][j].learn(this->moment, this->speed, &ideal[j]);
			}
			else {
				for (int j = 0; j < insize; ++j)
					(*this)[i][j].learn(this->moment, this->speed);
			}
		}

		return true;
	}
	else
		return false;
}
