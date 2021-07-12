#ifndef _DEPARTMENT_H_
#define _DEPARTMENT_H_

#include <string>
#include <iostream>
#include "rb_tree.h"
#include "worker.h"

struct department {
	//Поля структуры
	std::string title;
	int amount_wrk;
	double average_slr;
	rb_tree<worker> employment;
	//Конструктор по умолчанию
	department() : title("Default"), amount_wrk(0), average_slr(0) {};
	//Инициализирующий конструктор
	//department(std::string title_tmp) : title(title_tmp) {}
	department(std::string& title_tmp, int amount_wrk_tmp, double average_slt_tmp);
	department(const std::string& title_tmp) : title(title_tmp), amount_wrk(0), average_slr(0) {}
	//Копирующий конструктор
	department(const department& department_tmp);
	//Деструктор
	~department();
	//Потоковый ввод/вывод
	friend std::istream& operator >> (std::istream& input, department& department_input);
	friend std::ostream& operator << (std::ostream& output, department& department_output);
	//Перегрузка операторов
	department& operator = (const department& worker_tmp);
	department& operator = (department&& worker_tmp);
	friend bool operator < (department const& first, department const& second);
	friend bool operator > (department const& first, department const& second);
	friend bool operator == (department const& first, department const& second);
	friend bool operator != (department const& first, department const& second);
};

#endif // !_DEPARTMENT_H