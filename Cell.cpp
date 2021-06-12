#include "Cell.h"

cell::cell() : input(0), output(1), ahead_synapses(0), backward_synapses(0), delta_input(0) {}

//����������� ��� ���������� ������ � ��������
double cell::fin() {
	return (1 - this->output) * this->output;
}

//������������ �������� input � output
double cell::normalization(const double& temp) {
	return 1 / (1 + pow(M_E, (temp*-1)));
}

//���������, ���������� �� � ����� ������ this ����� ����� � ������� a
//���� �� - ���������� ������ �� ������ ������ synapses
//���� ��� - ���������� -1
//����������, ��� ������ ���������� ����������� ����� (�� backward_synapses)
int cell::find_ahead_synapse_with(cell* const a) {
	int found = -1;

	for (int i = 0; i < this->ahead_synapses.size(); ++i)
		if (a == ahead_synapses[i].first) {
			found = i;
			break;
		}

	return found;
}

//�� �� ����� ��� � find_ahead_synapse_with, ������ ������� �������� �������
int cell::find_backward_synapse_with(cell* const a) {
	int found = -1;

	for (int i = 0; i < this->backward_synapses.size(); ++i)
		if (a == backward_synapses[i].first) {
			found = i;
			break;
		}

	return found;
}

//��������� ����������� ����� � ������� �, � ������� ����� ����������� �������� ����� � ������, ���� �� ������ ������� ���������� true, ����� - false
bool cell::attach(cell*const a, const double& weight) {
	if (a != nullptr) {
		int already_exist = this->find_ahead_synapse_with(a);

		if (already_exist == -1) {
			this->ahead_synapses.push_back({ a, {weight, 0} });
			a->backward_synapses.push_back({this, ahead_synapses.size() - 1});
		}
		else
			this->ahead_synapses[already_exist].second = { weight, 0 };

		return true;
	}
	else
		return false;
}

//������� ��� �������� ���������� � ���������� output, ���� ��������� ����, �� ������������ false
bool cell::refresh() {
	if (this->ahead_synapses.size() > 0) {
		this->input = 0;
		for (int i = ahead_synapses.size() - 1; i > -1; --i) {
			if (ahead_synapses[i].first != nullptr)
				this->input += ahead_synapses[i].first->output * ahead_synapses[i].second.first;
			else
				ahead_synapses.erase(ahead_synapses.begin() + i);
		}

		this->output = this->normalization(this->input);

		return true;
	}

	return false;
}

//����� ��������� ���������������. ���� ������ ��������� ����, �� ��������� �� double ������ ���� ���������.
//���� ������ �������� ���� �������� ����, ideal �� ����������� � ���� ���������� ��������� ���.
void cell::learn(double& moment, double& speed, const double* ideal) {
	if (this->backward_synapses.size() == 0 && ideal != nullptr) {
		this->delta_input = (*ideal - this->output) * this->fin();
	}
	else if (this->backward_synapses.size() > 0) {
		double sumwidi = 0;

		for (int i = 0; i < this->backward_synapses.size(); ++i) {
			int index = this->backward_synapses[i].second;
			sumwidi += this->backward_synapses[i].first->ahead_synapses[index].second.first
				* this->backward_synapses[i].first->delta_input;
			double grad = this->output * this->backward_synapses[i].first->delta_input;
			this->backward_synapses[i].first->ahead_synapses[index].second.second =
				(moment * grad)
				+ (speed * this->backward_synapses[i].first->ahead_synapses[index].second.second);
			this->backward_synapses[i].first->ahead_synapses[index].second.first +=
				this->backward_synapses[i].first->ahead_synapses[index].second.second;
		}
		this->delta_input = fin() * sumwidi;
	}
}