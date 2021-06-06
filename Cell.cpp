#include "Cell.h"

double round105(const double& temp) {
	return (round(temp * pow(10, 5))) / pow(10, 5);
}

cell::cell() : input(0), output(0), synapses(0) {}

//нормализация значения input в output
double cell::normalization(const double& temp) {
	return round105(1 / (1 + pow(M_E, (temp*-1))));
}

//проверяет, существует ли у нашей клетки this такая связь с клеткой a
//если да - возвращает индекс из списка связей synapses
//если нет - возвращает -1
int cell::find_synapse_with(cell* const a) {
	int found = -1;

	for (int i = 0; i < this->synapses.size(); ++i)
		if (a == synapses[i].first) {
			found = i;
			break;
		}

	return found;
}

//добавляет принимающую связь с клеткой а, если всё прошло успешно возвращает true, иначе - false
bool cell::attach(cell*const a, const double& weight) {
	if (a != nullptr) {
		int already_exist = this->find_synapse_with(a);

		if (already_exist == -1)
			this->synapses.push_back({ a, weight });
		else
			this->synapses[already_exist].second = weight;

		return true;
	}
	else
		return false;
}

//удаляет принимающую связь с клеткой а, если всё прошло успешно возвращает true, иначе - false
bool cell::detach(cell* const a) {
	if (a != nullptr) {
		int already_exist = this->find_synapse_with(a);

		if (already_exist > -1)
			synapses.erase(synapses.begin() + already_exist);

		return true;
	}
	else
		return false;
}

//отчищает все оставшиеся данные из клетки и её связи
void cell::clear_synapses() {
	this->synapses.clear();
	this->synapses.shrink_to_fit();
}

//возвращает список всех связей и весов с иными клетками
vector<pair<cell*, double>> cell::get_synapses() {
	return synapses;
}

//обычный шаг принятия информации и подготовка output, если произошёл сбой, то возвращается false
bool cell::refresh() {
	if (this->synapses.size() > 0) {
		this->input = 0;
		for (int i = synapses.size() - 1; i > -1; --i) {
			if (synapses[i].first != nullptr)
				this->input += round105((*synapses[i].first).output * synapses[i].second);
			else
				synapses.erase(synapses.begin() + i);
		}

		this->output = this->normalization(this->input);
	}

	return true;
}