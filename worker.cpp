#include "worker.h"

//Инициализирующий конструктор
worker::worker(std::string surname_tmp, std::string name_tmp, std::string patronymic_tmp, std::string post_tmp, int salary_tmp) {
	surname = surname_tmp;
	name = name_tmp;
	patronymic = patronymic_tmp;
	post = post_tmp;
	salary = salary_tmp;
}

//Копирующий конструктор
worker::worker(const worker& worker_tmp) {
	surname = worker_tmp.surname;
	name = worker_tmp.name;
	patronymic = worker_tmp.patronymic;
	post = worker_tmp.post;
	salary = worker_tmp.salary;
}

std::istream& operator >> (std::istream& input, worker& worker_input) { //добавить обработку коррктности введенных данных
	std::cout << "Enter Surname: ";
	std::cin >> worker_input.surname;
	std::cout << "Enter Name: ";
	std::cin >> worker_input.name;
	std::cout << "Enter Patronymic: ";
	std::cin >> worker_input.patronymic;
	std::cout << "Enter Post: ";
	std::cin >> worker_input.post;
	std::cout << "Enter Salary: ";
	std::cin >> worker_input.salary;
	return input;
}

std::ostream& operator << (std::ostream& output, const worker& worker_output) {
	std::cout << "Surname: \'" << worker_output.surname << "\'" << std::endl;
	std::cout << "Name: \'" << worker_output.name << "\'" << std::endl;
	std::cout << "Patronymic: \'" << worker_output.patronymic << "\'" << std::endl;
	std::cout << "Post: \'" << worker_output.post << "\'" << std::endl;
	std::cout << "Salary: " << worker_output.salary << std::endl;
	return output;
}

worker& worker::operator = (const worker& worker_tmp) {
	surname = worker_tmp.surname;
	name = worker_tmp.name;
	patronymic = worker_tmp.patronymic;
	post = worker_tmp.post;
	salary = worker_tmp.salary;
	return *this;
}

worker& worker::operator = (worker&& worker_tmp) {
	std::string buf;
	int buf_salary;
	buf = surname;
	surname = worker_tmp.surname;
	worker_tmp.surname = buf;
	buf = name;
	name = worker_tmp.name;
	worker_tmp.name = buf;
	buf = patronymic;
	patronymic = worker_tmp.patronymic;
	worker_tmp.patronymic = buf;
	buf = post;
	post = worker_tmp.post;
	worker_tmp.post = buf;
	buf_salary = salary;
	salary = worker_tmp.salary;
	worker_tmp.salary = buf_salary;
	return *this;
}

bool operator < (worker const& first, worker const& second) {
	return first.surname < second.surname && first.name < second.name && first.patronymic < second.patronymic && first.post < second.post;
}

bool operator > (worker const& first, worker const& second) {
	return first.surname > second.surname && first.name > second.name && first.patronymic > second.patronymic && first.post > second.post;
}

bool operator == (worker const& first, worker const& second) {
	return !(first < second) && !(first > second);
}

bool operator != (worker const& first, worker const& second) {
	return !(first == second);
}