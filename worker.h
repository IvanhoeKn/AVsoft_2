#ifndef _WORKER_H_
#define _WORKER_H_

#include <string>
#include <iostream>

struct worker {
	//���� ���������
	std::string surname;
	std::string name;
	std::string patronymic;
	std::string post;
	int salary;
	//����������� �� ���������
	worker() : surname("Default"), name("Default"), patronymic("Default"), post("None"), salary(0) {};
	//���������������� �����������
	worker(std::string surname_tmp, std::string name_tmp, std::string patronymic_tmp, std::string post_tmp, int salary_tmp);
	//���������� �����������
	worker(const worker& worker_tmp);
	//����������
	~worker() {
		surname.clear();
		name.clear();
		patronymic.clear();
		post.clear();
	};
	//��������� ����/�����
	friend std::istream& operator >> (std::istream& input, worker& worker_input);
	friend std::ostream& operator << (std::ostream& output, worker& worker_output);
	//���������� ����������
	worker& operator = (const worker& worker_tmp);
	worker& operator = (worker&& worker_tmp);
	friend bool operator < (worker const& first, worker const& second);
	friend bool operator > (worker const& first, worker const& second);
	friend bool operator == (worker const& first, worker const& second);
	friend bool operator != (worker const& first, worker const& second);
};

#endif // !_WORKER_H_