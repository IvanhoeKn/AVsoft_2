#ifndef _WORKER_H_
#define _WORKER_H_

#include <string>
#include <iostream>

struct worker {
	//Поля структуры
	std::string surname;
	std::string name;
	std::string patronymic;
	std::string post;
	int salary;
	//Конструктор по умолчанию
	worker() : surname("Default"), name("Default"), patronymic("Default"), post("None"), salary(0) {};
	//Инициализирующий конструктор
	worker(std::string surname_tmp, std::string name_tmp, std::string patronymic_tmp, std::string post_tmp, int salary_tmp);
	//Копирующий конструктор
	worker(const worker& worker_tmp);
	//Деструктор
	~worker() {
		surname.clear();
		name.clear();
		patronymic.clear();
		post.clear();
	};
	//Потоковый ввод/вывод
	friend std::istream& operator >> (std::istream& input, worker& worker_input);
	friend std::ostream& operator << (std::ostream& output, worker& worker_output);
	//Перегрузка операторов
	worker& operator = (const worker& worker_tmp);
	worker& operator = (worker&& worker_tmp);
	friend bool operator < (worker const& first, worker const& second);
	friend bool operator > (worker const& first, worker const& second);
	friend bool operator == (worker const& first, worker const& second);
	friend bool operator != (worker const& first, worker const& second);
};

#endif // !_WORKER_H_